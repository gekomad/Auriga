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

#include <unistd.h>
#include "Engine.h"


void Engine::readStdin() {
    while (reading) {

        const string readbuffer = pipe->readStdin();
        if (!reading || readbuffer.empty()) {
            reading = false;
            break;
        };

        receiveOutput.append(readbuffer);
        std::smatch match;
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

void Engine::notifyTotResult(const u64 i, const string &fen, const string &engineName1, const int minutes, const int depth) {
    if (observer != nullptr) {
        assert(i != NO_RESULT);
        observer->observerTotResult(i, fen, engineName1, minutes, depth);
    }
}

void Engine::notifyHearthbeat(const string &fen, const string &engineName1, const int minutes, const int depth) {
    if (observer != nullptr) {
        observer->observerHearthbeat(fen, engineName1, minutes, depth);
    }
}

void Engine::readStderr() {
    while (reading) {
        const string &readStderrBuffer = pipe->readStderr();
        if (!reading || readStderrBuffer.empty()) {
            reading = false;
            break;
        };

        receiveStdErr.append(readStderrBuffer);
        std::smatch match;

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
    timeStart = std::chrono::high_resolution_clock::now();
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
    high_resolution_clock::time_point timeEnd = std::chrono::high_resolution_clock::now();
    receiveOutput.clear();
    receiveStdErr.clear();
    info(engineName, " endRun id ", getId(), " result: ", result, " reading: ", (bool) reading);
    assert(result != NO_RESULT);
    int minutes = Time::diffTime(timeEnd, timeStart) / 1000 / 60;
    notifyTotResult(result, fen, engineName, minutes, depth);
}

Engine::~Engine() {
    debug(engineName, " ~Engine");
    if (!initialized)return;
    put("quit");
    initialized = false;
    if (pipe)delete pipe;
    if (timerHearbeat) {
        delete timerHearbeat;
    }

}

void Engine::put(string command) {
    lock_guard<mutex> lock(putMutex);
    receiveOutput.clear();
    receiveStdErr.clear();
    info(engineName, " Send to engine id ", getId(), " |" + command + "\\n|");
    command.append("\n");
    int nbytes = command.size();
    assert(pipe->writeStdout(command) == nbytes);
//    usleep(500000);
    sleep(1);
}

void Engine::setPosition(const string &fen1) {
    put(POSITION_FEN[protocol] + fen1);
    fen = fen1;
}

void Engine::init(const string &confFileName1) {
    result = 0;
    if (!forceRestart && initialized)return;
    WorkerEntityDao workerEntityDao(confFileName1);
    string enginePath = workerEntityDao.getWorkerEntity().getEnginePath();
    forceRestart = workerEntityDao.getWorkerEntity().getForce_restart();

    rgxTot.assign(workerEntityDao.getWorkerEntity().getRegex_perft_moves());

    info(" load engine ", enginePath);
    if (!pipe->init(enginePath)) {
        fatal(" Failed to execute ", enginePath);
        exit(1);
    };

    bool detected = false;
    for (unsigned ii = 0; ii < SEND_INIT_STRING->size(); ii++) {
        put(SEND_INIT_STRING[ii]);
        const string &readbuffer = pipe->readStdin();
        if (readbuffer.empty()) {
            fatal("engine not responding");
            exit(1);
        };
        receiveOutput = readbuffer;
        debug("read from engine: ", receiveOutput);
        std::smatch match;
        for (int i = 0; i < 2; i++)//TODO calcolare 2 a runtime è la size dell'enum
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
    //detect name
    put(SEND_GET_NAME_STRING[protocol]);
    if (protocol == UCI) {
        while ((count++) < 100) {
            const string &readbuffer = pipe->readStdin();
            if (readbuffer.empty()) {
                break;
            };
            receiveOutput.append(readbuffer);
            log(receiveOutput);
            std::smatch match;

            if (regex_search(((const string) receiveOutput).begin(), ((const string) receiveOutput).end(), match, GET_NAME_REGEX[protocol])) {
                engineName = match[1].str();
                info("engine name is ", engineName)
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
            const string &readbuffer = pipe->readStdin();
            if (readbuffer.empty()) {
                break;
            };
            receiveOutput.append(readbuffer);
            log(receiveOutput);
            std::smatch match;

            if (regex_search(((const string) receiveOutput).begin(), ((const string) receiveOutput).end(), match, GET_NAME_REGEX[protocol])) {
                engineName = match[1].str();
                info("engine name is ", engineName)
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

void Engine::runPerft() {
    timerHearbeat = new Timer(Time::HOUR_IN_SECONDS * 6);
    timerHearbeat->registerObservers([this]() {
        high_resolution_clock::time_point timeEnd = std::chrono::high_resolution_clock::now();
        int minutes = Time::diffTime(timeEnd, timeStart) / 1000 / 60;
        notifyHearthbeat(fen, engineName, minutes, depth);
    });
    timerHearbeat->start();
    put("perft " + to_string(depth));
}
