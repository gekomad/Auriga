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

Perft::Perft(const string &taskUUID1, const string &masterFile1, const string &engineConfFile) {
    if (!FileUtil::fileExists(masterFile1)) {
        error("File not found ", masterFile1);
        exit(1);
    }
    if (!FileUtil::fileExists(engineConfFile)) {
        error("File not found ", engineConfFile);
        exit(1);
    }
    if (taskUUID1.size() != 36) {
        error("taskUUID malformed ", taskUUID1);
        exit(1);
    }
    engineConf = engineConfFile;
    masterFile = masterFile1;
    taskUUID = taskUUID1;
}

void Perft::observerTotResult(const u64 result, const string &fen) {
    TOT += result;
    info("TOT: ", String::toString(TOT));

    Client::getInstance().post(perftUUID, taskUUID, "0", String::toString(TOT), engineName, author, fen);
}

void Perft::observerPartialResult(const u64 result, const string &fen) {
    info("partial result: ", result);
    Client::getInstance().post(perftUUID, taskUUID, to_string(result), "0", engineName, author, fen);
}

__int128_t Perft::calculate() {
    auto start1 = std::chrono::high_resolution_clock::now();

    ThreadPool<Engine> threadPool;
    IniFile iniFile(engineConf);
    engineName = iniFile.getValue("engine");
    author = iniFile.getValue("author");

    int threads = String::stoi(iniFile.getValue("uci_option_perft_thread_value"));
    if (!threads) {
        int ins = String::stoi(iniFile.getValue("n_instances"));
        threadPool.setNthread(ins == 0 ? 1 : ins);
    } else {
        threadPool.setNthread(1);
    }

    PerftTree perftTreeDao(masterFile);
    perftUUID = perftTreeDao.getPerftEntity()->getUuid();
    const TaskEntity *taskEntity = perftTreeDao.getTaskEntity(taskUUID);
    if (!taskEntity) {
        error("taskUUID ", taskUUID, " not found");
        exit(1);
    }

    for (string fen:taskEntity->getFen()) {
        Engine &e = threadPool.getNextThread();
        e.init(engineConf);
        e.registerObserverEngine(this);
        e.start();
        e.setPosition(fen);
        e.put("perft " + String(taskEntity->getDepth()));
    }
    threadPool.joinAll();
    auto stop1 = std::chrono::high_resolution_clock::now();

    string timetot = Time::diffTimeToString(start1, stop1);

    cout << "Tot Perft moves for TaskUUID " << taskUUID << " :" << String::toString(TOT) << " in " << timetot << endl;
    return TOT;
}