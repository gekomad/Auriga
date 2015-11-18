/*
    https://github.com/gekomad/Auriga
    Copyright (C) Giuseppe Cannella

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Perft.h"

using namespace _perft;

Perft::Perft(const string &taskUUID1, const string &masterFile1, const string &workerIniFile) {
    if (!FileUtil::fileExists(masterFile1)) {
        fatal("File not found ", masterFile1);
        exit(1);
    }
    if (!FileUtil::fileExists(workerIniFile)) {
        fatal("File not found ", workerIniFile);
        exit(1);
    }
    if (taskUUID1.size() != 36) {
        fatal("task_uuid malformed ", taskUUID1);
        exit(1);
    }
    IniFile ini(workerIniFile);
    aurigaHost = ini.getValue("host");
    if (aurigaHost.empty()) {
        warn("auriga host not defined, the results will not be sent to server");
    } else {
        string port = ini.getValue("port");
        if (port.empty()) {
            warn("auriga server port not defined, the results will not be sent to server");
            aurigaHost = "";
        } else {
            aurigaPort = stoi(port);
        }
    }

    Perft::workerIniFile = workerIniFile;
    masterFile = masterFile1;
    taskUUID = taskUUID1;
}

int Perft::getOStype() {
#ifdef _WIN32
    return OS_WIN;
#elif __APPLE__
    return OS_APPLE;
#elif _AURIGA_RASPBERRY
    return OS_RASPBERRY;
#elif _AURIGA_ODROID
    return OS_ODROID;
#elif __linux__
    return OS_LINUX;
#elif __unix__
    return OS_UNIX;
#endif
    return 0;
}

void Perft::observerTotResult(const u64 result, const string &fen, const string &engineName, const int minutes, const int depth) {
    getResultMutex.lock();
    TOT += result;
    getResultMutex.unlock();
    info("TOT: ", String::toString(TOT));
    int type = getOStype();
    HttpPost::getInstance().postThread(aurigaHost, aurigaPort, perftUUID, taskUUID, to_string(type), String::toString(result), engineName, author, fen, to_string(minutes), to_string(depth));
}

void Perft::observerHeartbeat(const string &fen, const string &engineName, const int minutes, const int depth) {
    info("Heartbeat ");
    int type = getOStype();
    type |= HEARTBEAT;
    HttpPost::getInstance().postThread(aurigaHost, aurigaPort, perftUUID, taskUUID, to_string(type), "0", engineName, author, fen, to_string(minutes), to_string(depth));
}

i128 Perft::calculate() {
    auto start1 = std::chrono::high_resolution_clock::now();

    ThreadPool<Engine> threadPool;
    IniFile iniFile(workerIniFile);
    author = iniFile.getValue("public_name");

    int ins = String::stoi(iniFile.getValue("instances"));
    threadPool.setNthread(ins == 0 ? 1 : ins);

    for (Engine *e :threadPool.threadPool) {
#if defined(_WIN32)
        e->setPipe(new PipeWindows());
#else
        e->setPipe(new PipePosix());
#endif
    }

    PerftTree perftTreeDao(masterFile);
    perftUUID = perftTreeDao.getPerftEntity()->getUuid();
    const TaskEntity *taskEntity = perftTreeDao.getTaskEntity(taskUUID);
    if (!taskEntity) {
        fatal("task_uuid ", taskUUID, " not found");
        exit(1);
    }

    vector<string> uniqueListFen;
    for (string fen:taskEntity->getFenList()) {
        if (std::find(uniqueListFen.begin(), uniqueListFen.end(), fen) == uniqueListFen.end()) {
            uniqueListFen.push_back(fen);
        }
    }
    for (string fen:uniqueListFen) {
        Engine &e = threadPool.getNextThread();
        e.init(workerIniFile);
        e.registerObserverEngine(this);
        e.start();
        e.setPosition(fen);
        e.setDepth(taskEntity->getDepth());
        e.runPerft();

    }
    threadPool.joinAll();
    auto stop1 = std::chrono::high_resolution_clock::now();

    string timetot = Time::diffTimeToString(start1, stop1);

    //TODO append in file result.log
    cout << "Tot Perft moves for task_uuid " << taskUUID << ": " << String::toString(TOT) << " in " << timetot << endl;

    return TOT;
}