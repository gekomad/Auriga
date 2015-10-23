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

#include "Engine.h"


void Engine::readStdin() {
    int bytes_read;
    char readbuffer[1024];
    while (reading) {
        bytes_read = read(fd_c2p[0], readbuffer, sizeof(readbuffer) - 1);
        if (!reading || bytes_read <= 0) {
            reading = false;
            break;
        }
        readbuffer[bytes_read] = 0;
        receiveOutput.append(readbuffer);
//        log(name, " id:", getId(), " Reading from engine stdout: |" + receiveOutput + "|");
        std::smatch match;
        if (regex_heartbeat.size() && regex_search(((const string) receiveOutput).begin(), ((const string) receiveOutput).end(), match, rgxPartial)) {
            debug(engineName, " stdin match partial: ", match[1].str());
            notifyPartialResult(stoull(match[1].str()), fen, engineName);
        }

        if (regex_search(((const string) receiveOutput).begin(), ((const string) receiveOutput).end(), match, rgxTot)) {
            reading = false;
            debug(engineName, " stdin match tot: ", match[1].str());
            result = stoull(match[1].str());
            ASSERT(result != NO_RESULT);
        }
        if (result != NO_RESULT) {
            log(engineName, " id:", getId(), " Reading from engine stdout: |" + receiveOutput + "|");
            cv.notify_all();
        }
    }
}

void Engine::notifyTotResult(const u64 i, const string &fen, const string &engineName1) {
    if (observer != nullptr) {
        assert(i != NO_RESULT);
        observer->observerTotResult(i, fen, engineName1);
    }
}

void Engine::notifyPartialResult(const u64 i, const string &fen, const string &engineName1) {
    if (observer != nullptr) {
        observer->observerPartialResult(i, fen, engineName1);
    }
}

void Engine::readStderr() {
    int bytes_read_err;
    char readStderrBuffer[1024];
    while (reading) {
        bytes_read_err = read(stdErr[0], readStderrBuffer, sizeof(readStderrBuffer) - 1);

        if (!reading || bytes_read_err <= 0) {
            reading = false;
            break;
        }
        readStderrBuffer[bytes_read_err] = 0;
        receiveStdErr.append(readStderrBuffer);

//        log(name, " id:", getId(), " Reading from engine stderr: |" + receiveStdErr + "|");
        std::smatch match;

        if (regex_heartbeat.size() && regex_search(((const string) receiveStdErr).begin(), ((const string) receiveStdErr).end(), match, rgxPartial)) {
            debug(engineName, " stderr match partial: ", match[1].str());
            notifyPartialResult(stoull(match[1].str()), fen, engineName);
        }


        if (regex_search(((const string) receiveStdErr).begin(), ((const string) receiveStdErr).end(), match, rgxTot)) {
            reading = false;
            result = stoull(match[1].str());
            ASSERT(result != NO_RESULT);
            debug(engineName, " err match tot: ", match[1].str(), "=", result, " id:", getId());
        }
        if (result != NO_RESULT) {
            log(engineName, " id:", getId(), " Reading from engine stderr: |" + receiveStdErr + "|");
            cv.notify_all();
        }
    }
}

void Engine::run() {
    reading = true;
    result = NO_RESULT;
    std::thread in(&Engine::readStdin, this);
    std::thread err(&Engine::readStderr, this);
    mutex mtx;
    unique_lock<mutex> lck(mtx);
    cv.wait(lck);
    err.detach();
    in.detach();
}

void Engine::endRun() {
    receiveOutput.clear();
    receiveStdErr.clear();
    info(engineName, " endRun id ", getId(), " result: ", result, " reading: ", (bool) reading);
    assert(result != NO_RESULT);
    notifyTotResult(result, fen, engineName);
}

Engine::~Engine() {
    debug(engineName, " ~Engine");
    if (!initialized)return;
    put("quit");
    initialized = false;
    close(fd_c2p[0]);
    close(fd_p2c[1]);
}

void Engine::put(string command) {
    lock_guard<mutex> lock(putMutex);
    receiveOutput.clear();
    receiveStdErr.clear();
    info(engineName, " Send to engine id ", getId(), " |" + command + "\\n|");
    command.append("\n");
    int nbytes = command.length();
    assert(write(fd_p2c[1], command.c_str(), nbytes) == nbytes);
    usleep(500000);
}

void Engine::setPosition(const string &fen1) {
    put(POSITION_FEN[protocol] + fen1);
    fen = fen1;
}

Engine::Engine() {
    GET_NAME_REGEX[0].assign("id name (.+)");
    GET_NAME_REGEX[1].assign("feature myname=\"(.+)\".*");
}

void Engine::init(const string &confFileName1) {
    result = 0;
    if (!forceRestart && initialized)return;
    WorkerEntityDao workerEntityDao(confFileName1);
    string enginePath = workerEntityDao.getWorkerEntity().getEnginePath();
    forceRestart = workerEntityDao.getWorkerEntity().getForce_restart();
    if (!FileUtil::fileExists(enginePath)) {
        fatal("engine not found", enginePath);
        exit(1);
    }
    rgxPartial.assign(workerEntityDao.getWorkerEntity().getRegex_heartbeat());
    rgxTot.assign(workerEntityDao.getWorkerEntity().getRegex_perft_moves());
    regex_heartbeat = workerEntityDao.getWorkerEntity().getRegex_heartbeat();

    info(" load engine ", enginePath);
    pid_t childpid;
    assert(!pipe(fd_p2c) && !pipe(fd_c2p) && !pipe(stdErr));

    childpid = fork();

    assert(childpid >= 0);
    if (childpid == 0) {
        assert(dup2(fd_p2c[0], 0) == 0 && close(fd_p2c[0]) == 0 && close(fd_p2c[1]) == 0);
        assert(dup2(fd_c2p[1], 1) == 1 && close(fd_c2p[1]) == 0 && close(fd_c2p[0]) == 0);
        assert(dup2(stdErr[1], 2) >= 0);
        execl(enginePath.c_str(), enginePath.c_str(), (char *) 0);
        fatal(" Failed to execute ", enginePath);
        exit(1);
    }
    close(fd_p2c[0]);
    close(fd_c2p[1]);
    close(stdErr[1]);

    char readbuffer[2048];
    bool detected = false;
    for (unsigned i = 0; i < SEND_INIT_STRING->size(); i++) {
        put(SEND_INIT_STRING[i]);

        int bytes_read = read(fd_c2p[0], readbuffer, sizeof(readbuffer) - 1);
        if (bytes_read <= 0) {
            fatal("engine not responding");
            exit(1);
        };
        readbuffer[bytes_read] = 0;
        receiveOutput = readbuffer;
        debug("read from engine: ", receiveOutput);
        std::smatch match;
        if (receiveOutput.find(RECEIVE_INIT_STRING[i]) != string::npos) {
            protocol = static_cast<PROTOCOL_TYPE>(i);
            detected = true;
            break;
        }
    }
    if (!detected) {
        fatal("protocol not detected");
        exit(1);
    };
    ///////
    receiveOutput.clear();
    initialized = false;
    int count = 0;
    put(SEND_INIT_STRING[protocol]);
    if (protocol == UCI) {
        while ((count++) < 100) {
            int bytes_read = read(fd_c2p[0], readbuffer, sizeof(readbuffer) - 1);
            if (bytes_read <= 0)break;
            readbuffer[bytes_read] = 0;
            receiveOutput.append(readbuffer);
            log(receiveOutput);
            std::smatch match;

            if (regex_search(((const string) receiveOutput).begin(), ((const string) receiveOutput).end(), match, GET_NAME_REGEX[protocol])) {
                engineName = match[1].str();
            }

            if (receiveOutput.find(RECEIVE_INIT_STRING[protocol]) != string::npos) {
                initialized = true;

                //setoption
                for (pair<string, string> option:workerEntityDao.getWorkerEntity().getOptions()) {
                    put("setoption name " + option.first + string(" value ") + option.second);
                }
                break;
            }
        }
    } else {
        while ((count++) < 100) {
            int bytes_read = read(fd_c2p[0], readbuffer, sizeof(readbuffer) - 1);
            if (bytes_read <= 0)break;
            readbuffer[bytes_read] = 0;
            receiveOutput.append(readbuffer);
            log(receiveOutput);
            std::smatch match;

            if (regex_search(((const string) receiveOutput).begin(), ((const string) receiveOutput).end(), match, GET_NAME_REGEX[protocol])) {
                engineName = match[1].str();
            }
            if (initialized)break;
            if (receiveOutput.find(RECEIVE_INIT_STRING[protocol]) != string::npos) {
                put(XBOARD_REQUEST_NAME_STRING);
                usleep(100000);
                initialized = true;
            }
        }
    }
    debug(engineName, " id: ", getId(), receiveOutput);
    assert(initialized);

}

