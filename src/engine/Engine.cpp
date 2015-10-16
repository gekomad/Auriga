/*
    https://github.com/gekomad/Auriga
    Copyright (C) GiuSEPARATORpe Cannella

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

void Engine::run() {
    int bytes_read;
    int bytes_read_err;
    char readbuffer[1024];
    char readStderrBuffer[1024];
    while (initialized) {
        bytes_read = read(fd_c2p[0], readbuffer, sizeof(readbuffer) - 1);
        readbuffer[bytes_read] = 0;
        bytes_read_err = read(stdErr[0], readStderrBuffer, sizeof(readStderrBuffer) - 1);
        readbuffer[bytes_read_err] = 0;
        if (!initialized || bytes_read + bytes_read_err <= 0) {
            break;
        }
        receiveOutput.append(readbuffer);
        receiveStdErr.append(readStderrBuffer);
        debug("id: ",getId(),"Reading from engine stdout: |" + receiveOutput + "|");
        debug("id: ",getId(),"Reading from engine stderr: |" + receiveStdErr + "|");
        std::smatch match;
        result = -1;
        if (regex_search(((const string) receiveOutput).begin(), ((const string) receiveOutput).end(), match, rgx)) {
            result = stoull(match[1].str());
        } else if (regex_search(((const string) receiveStdErr).begin(), ((const string) receiveStdErr).end(), match, rgx)) {
            result = stoull(match[1].str());
        }
        if (result != -1) {
            initialized = false;
        }
    }
}


void Engine::endRun() {
    initialized = false;
    receiveOutput.clear();
    receiveStdErr.clear();
    info("endRun id", getId(), " result: ", result);
    notifyEndEngine(result);
//    put("quit");
    sleep(1);
}

Engine::~Engine() {
    close(fd_c2p[0]);
    close(fd_p2c[1]);
}

void Engine::put(string command) {
    lock_guard<mutex> lock(putMutex);
    receiveOutput.clear();
    receiveStdErr.clear();
    info("Writing to engine id ",getId()," |" + command + "\\n|");
    command.append("\n");
    int nbytes = command.length();
    int y = write(fd_p2c[1], command.c_str(), nbytes);
    if (y != nbytes) {
        error("id: ",getId()," ERROR bytes to write: ", nbytes, " bytes written: ", y, " command: ", command, "  xxxxxxxxxxxxxxxxx error xxxxxxxxxxxxxxxxx");
        y = write(fd_p2c[1], command.c_str(), nbytes);
        if (y != nbytes) {
            error("id: ",getId()," ABORT bytes to write: ", nbytes, " bytes written: ", y, " command: ", command, "  xxxxxxxxxxxxxxxxx error xxxxxxxxxxxxxxxxx");
        }
    }
}

void Engine::setPosition(const string &fen) {
    put(POSITION_FEN[protocol] + fen);
}

void Engine::init(const string &confFileName) {
    IniFile iniFile(confFileName);

    while (true) {
        pair<string, string> *parameters = iniFile.get();
        if (!parameters)break;
        if (parameters->first == "path") {
            programPath = parameters->second;
        } else if (parameters->first == "protocol") {
            protocol = String(parameters->second).toLower() == "uci" ? PROTOCOL_TYPE::UCI : PROTOCOL_TYPE::XBOARD;
        } else if (parameters->first == "uci_option_perft_thread_name") {
            uci_option_perft_thread_name == parameters->second;
        } else if (parameters->first == "uci_option_perft_thread_value") {
            uci_option_perft_thread_value = stoi(parameters->second);
        } else if (parameters->first == "uci_option_perft_hash_name") {
            uci_option_perft_hash_name == parameters->second;
        } else if (parameters->first == "uci_option_perft_hash_value") {
            uci_option_perft_hash_value = stoi(parameters->second);
        } else if (parameters->first == "regex_perft_moves") {
            regex_perft_moves = parameters->second;
            rgx.assign(regex_perft_moves);
        }
    }

    pid_t childpid;
    assert(!pipe(fd_p2c) && !pipe(fd_c2p) && !pipe(stdErr));

    childpid = fork();

    assert(childpid >= 0);
    if (childpid == 0) {
        assert(dup2(fd_p2c[0], 0) == 0 && close(fd_p2c[0]) == 0 && close(fd_p2c[1]) == 0);
        assert(dup2(fd_c2p[1], 1) == 1 && close(fd_c2p[1]) == 0 && close(fd_c2p[0]) == 0);
        assert(dup2(stdErr[1], 2) >= 0);
        execl(programPath.c_str(), programPath.c_str(), (char *) 0);
        cerr << "Failed to execute " << programPath << endl;
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
    while ((count++) < 100) {
        int bytes_read = read(fd_c2p[0], readbuffer, sizeof(readbuffer) - 1);
        if (bytes_read <= 0)break;
        readbuffer[bytes_read] = 0;
        receiveOutput.append(readbuffer);
        if (receiveOutput.find(RECEIVE_INIT_STRING[protocol]) != string::npos) {
            initialized = true;
            break;
        }
    }
    debug("id: ",getId(),receiveOutput);
    assert(initialized);

}
