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
    bool reading = true;
    while (reading) {
        bytes_read = read(fd_c2p[0], readbuffer, sizeof(readbuffer) - 1);
        if (!reading || bytes_read <= 0) {
            break;
        }
        readbuffer[bytes_read] = 0;
        receiveOutput.append(readbuffer);
        log("id:", getId(), " Reading from engine stdout: |" + receiveOutput + "|");
        std::smatch match;
        if (regex_heartbeat.size() && regex_search(((const string) receiveOutput).begin(), ((const string) receiveOutput).end(), match, rgxPartial)) {
            debug("match[1].str(): ", match[1].str());
            notifyPartialResult(stoull(match[1].str()), fen);
        }

        result = NO_RESULT;
        if (regex_search(((const string) receiveOutput).begin(), ((const string) receiveOutput).end(), match, rgxTot)) {
            debug("match[1].str(): ", match[1].str());
            result = stoull(match[1].str());
        }
        if (result != NO_RESULT) {
            reading = false;
            cv.notify_all();
        }
    }
}

void Engine::notifyTotResult(const u64 i, const string &fen) {
    if (observer != nullptr) {
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
    bool reading = true;
    while (reading) {
        bytes_read_err = read(stdErr[0], readStderrBuffer, sizeof(readStderrBuffer) - 1);

        if (!reading || bytes_read_err <= 0) {
            break;
        }
        readStderrBuffer[bytes_read_err] = 0;
        receiveStdErr.append(readStderrBuffer);

        log("id:", getId(), " Reading from engine stderr: |" + receiveStdErr + "|");
        std::smatch match;

        if (regex_heartbeat.size() && regex_search(((const string) receiveStdErr).begin(), ((const string) receiveStdErr).end(), match, rgxPartial)) {
            debug("match[1].str(): ", match[1].str());
            notifyPartialResult(stoull(match[1].str()), fen);
        }

        result = NO_RESULT;
        if (regex_search(((const string) receiveStdErr).begin(), ((const string) receiveStdErr).end(), match, rgxTot)) {
            debug("match[1].str(): ", match[1].str());
            result = stoull(match[1].str());
        }
        if (result != NO_RESULT) {
            reading = false;
            cv.notify_all();
        }
    }
}

void Engine::run() {
    std::thread in(&Engine::readStdin, this);
    std::thread err(&Engine::readStderr, this);
    mutex mtx;
    unique_lock<mutex> lck(mtx);
    cv.wait(lck);
    err.detach();
    in.detach();
}

void Engine::endRun() {
    //if (!initialized)return;
    receiveOutput.clear();
    receiveStdErr.clear();
    info("endRun id ", getId(), " result: ", result);
    notifyTotResult(result, fen);
//    put("quit");
}

Engine::~Engine() {
    debug("~Engine");
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
    info("Send to engine id ", getId(), " |" + command + "\\n|");
    command.append("\n");
    int nbytes = command.length();
    assert(write(fd_p2c[1], command.c_str(), nbytes) == nbytes);

}

void Engine::setPosition(const string &fen1) {
    put(POSITION_FEN[protocol] + fen1);
    fen = fen1;
}

void Engine::init(const string &confFileName) {
    result = 0;

    IniFile iniFile(confFileName);
    GET_NAME_REGEX[0].assign("id name (.+)");
    GET_NAME_REGEX[1].assign("feature myname=\"(.+)\".*");

    while (true) {
        pair<string, string> *parameters = iniFile.get();
        if (!parameters)break;
        if (parameters->first == "path") {
            enginePath = parameters->second;
            if (!FileUtil::fileExists(enginePath)) {
                fatal("engine not found", enginePath);
                exit(1);
            }
        } else if (parameters->first == "protocol") {
            string prtcl = String(parameters->second).toLower();
            if (prtcl == "uci")protocol = PROTOCOL_TYPE::UCI; else if (prtcl == "xboard")protocol = PROTOCOL_TYPE::XBOARD; else {
                fatal("error protocol ", prtcl, " unknow");
                exit(1);
            }
        } else if (parameters->first == "uci_option_perft_thread_name") {
            uci_option_perft_thread_name = parameters->second;
        } else if (parameters->first == "uci_option_perft_thread_value") {
            uci_option_perft_thread_value = String::stoi(parameters->second);
        } else if (parameters->first == "uci_option_perft_hash_name") {
            uci_option_perft_hash_name = parameters->second;
        } else if (parameters->first == "uci_option_perft_hash_value") {
            uci_option_perft_hash_value = String::stoi(parameters->second);
        } else if (parameters->first == "regex_heartbeat") {
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
    info("load engine ", enginePath);
    pid_t childpid;
    assert(!pipe(fd_p2c) && !pipe(fd_c2p) && !pipe(stdErr));

    childpid = fork();

    assert(childpid >= 0);
    if (childpid == 0) {
        assert(dup2(fd_p2c[0], 0) == 0 && close(fd_p2c[0]) == 0 && close(fd_p2c[1]) == 0);
        assert(dup2(fd_c2p[1], 1) == 1 && close(fd_c2p[1]) == 0 && close(fd_c2p[0]) == 0);
        assert(dup2(stdErr[1], 2) >= 0);
        execl(enginePath.c_str(), enginePath.c_str(), (char *) 0);
        fatal("Failed to execute ", enginePath);
        exit(1);
    }
    close(fd_p2c[0]);
    close(fd_c2p[1]);
    close(stdErr[1]);
    /////////
    char readbuffer[2048];
    put(SEND_INIT_STRING[protocol]);
    receiveOutput.clear();
    initialized = false;
    int count = 0;
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
                if (uci_option_perft_thread_value) {
                    put("setoption name " + uci_option_perft_thread_name + string(" value ") + String(uci_option_perft_thread_value));
                }
                if (uci_option_perft_hash_value) {
                    put("setoption name " + uci_option_perft_hash_name + " value " + String(uci_option_perft_hash_value));
                }
                initialized = true;
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
    debug("id: ", getId(), receiveOutput);
    assert(initialized);

}
