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
            debug(name, " stdin match partial: ", match[1].str());
            notifyPartialResult(stoull(match[1].str()), fen);
        }

        if (regex_search(((const string) receiveOutput).begin(), ((const string) receiveOutput).end(), match, rgxTot)) {
            reading = false;
            debug(name, " stdin match tot: ", match[1].str());
            result = stoull(match[1].str());
            ASSERT(result != NO_RESULT);
        }
        if (result != NO_RESULT) {
            log(name, " id:", getId(), " Reading from engine stdout: |" + receiveOutput + "|");
            cv.notify_all();
        }
    }
}

void Engine::notifyTotResult(const u64 i, const string &fen) {
    if (observer != nullptr) {
        assert(i != NO_RESULT);
        observer->observerTotResult(i, fen);
    }
}

void Engine::notifyPartialResult(const u64 i, const string &fen) {
    if (observer != nullptr) {
        observer->observerPartialResult(i, fen);
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
            debug(name, " stderr match partial: ", match[1].str());
            notifyPartialResult(stoull(match[1].str()), fen);
        }


        if (regex_search(((const string) receiveStdErr).begin(), ((const string) receiveStdErr).end(), match, rgxTot)) {
            reading = false;
            result = stoull(match[1].str());
            ASSERT(result != NO_RESULT);
            debug(name, " err match tot: ", match[1].str(), "=", result, " id:", getId());
        }
        if (result != NO_RESULT) {
            log(name, " id:", getId(), " Reading from engine stderr: |" + receiveStdErr + "|");
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
    info(name, " endRun id ", getId(), " result: ", result, " reading: ", (bool) reading);
    assert(result != NO_RESULT);
    notifyTotResult(result, fen);
}

Engine::~Engine() {
    debug(name, " ~Engine");
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
    info(name, " Send to engine id ", getId(), " |" + command + "\\n|");
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

void Engine::init(const string &confFileName) {
    result = 0;
    if (!forceRestart && initialized)return;
    IniFile iniFile(confFileName);
    forceRestart = iniFile.getValue("force_restart") == "true";

    while (true) {
        pair<string, string> *parameters = iniFile.get();
        if (!parameters)break;
        if (parameters->first == "path") {
            enginePath = parameters->second;
            if (!FileUtil::fileExists(enginePath)) {
                fatal("engine not found", enginePath);
                exit(1);
            }
        }
        else if (parameters->first == "regex_heartbeat") {
            regex_heartbeat = parameters->second;
            rgxPartial.assign(regex_heartbeat);
        } else if (parameters->first == "regex_perft_moves") {
            regex_perft_moves = parameters->second;
            if (!regex_perft_moves.size()) {
                fatal("regex_perft_moves in ", confFileName, " is mandatory");
                exit(1);
            }
            if (regex_perft_moves.find("(\\d+)") == string::npos) {
                fatal("regex_perft_moves in ", confFileName, " is malformed");
                exit(1);
            }
            rgxTot.assign(regex_perft_moves);
        }
    }
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
                name = match[1].str();
            }

            if (receiveOutput.find(RECEIVE_INIT_STRING[protocol]) != string::npos) {
                initialized = true;

                //setoption
                vector<pair<string, string>> options = getOptions(confFileName);

                for (pair<string, string> option:options) {
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
                name = match[1].str();
            }
            if (initialized)break;
            if (receiveOutput.find(RECEIVE_INIT_STRING[protocol]) != string::npos) {
                put(XBOARD_REQUEST_NAME_STRING);
                usleep(100000);
                initialized = true;
            }
        }
    }
    debug(name, " id: ", getId(), receiveOutput);
    assert(initialized);

}

vector<pair<string, string>> Engine::getOptions(const string &confFileName) {
    bool opt = false;
    IniFile iniFile(confFileName);
    vector<pair<string, string>> options;
    pair<string, string> *parameters;
    while (true) {
        parameters = iniFile.get();
        if (!parameters)break;
        if (opt) {
            if (String(parameters->first).trimRight().endsWith("]"))break;
            pair<string, string> a(parameters->first, parameters->second);
            options.push_back(a);
        } else if (parameters->first == "[setoption_name_value]") {
            opt = true;
        }
    }
    return options;
}