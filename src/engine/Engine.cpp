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

Engine::Engine(const string &fileName1, PROTOCOL_TYPE type1) {
    programName = fileName1;
    type = type1;
    pid_t childpid;
    assert (!pipe(fd_p2c) && !pipe(fd_c2p) && !pipe(stdErr));

    childpid = fork();

    assert (childpid >= 0);
    if (childpid == 0) {
        assert (dup2(fd_p2c[0], 0) == 0 && close(fd_p2c[0]) == 0 && close(fd_p2c[1]) == 0);
        assert (dup2(fd_c2p[1], 1) == 1 && close(fd_c2p[1]) == 0 && close(fd_c2p[0]) == 0);
        assert (dup2(stdErr[1], 2) >= 0);
        execl(programName.c_str(), programName.c_str(), (char *) 0);
        cerr << "Failed to execute " << programName << endl;
        exit(1);
    }
    close(fd_p2c[0]);
    close(fd_c2p[1]);
    close(stdErr[1]);
}

void Engine::run() {
    int bytes_read;
    char readbuffer[1024];
    char readStderrBuffer[1024];
    while (1) {
        bytes_read = read(fd_c2p[0], readbuffer, sizeof(readbuffer) - 1);
        bytes_read += read(stdErr[0], readStderrBuffer, sizeof(readStderrBuffer) - 1);

        if (bytes_read <= 0)
            break;

        readbuffer[bytes_read] = '\0';
        receiveOutput += readbuffer;
        receiveStdErr+=readStderrBuffer;
        debug("Reading from engine stdout: |" + receiveOutput + "|");
        debug("Reading from engine stderr: |" + receiveStdErr + "|");

        if (receiveOutput.find("Nodes searched  :")!=string::npos||receiveStdErr.find("Nodes searched  :")!=string::npos)while(1)cout <<"trovato"<<endl;
        if (receiveOutput.find(RECEIVE_INIT_STRING[type])!=string::npos)initialize = true;
    }
}

void Engine::endRun() {
    debug("endRun");
    close(fd_c2p[0]);
    close(fd_p2c[1]);
}

void Engine::put(string command) {
    lock_guard<mutex> lock(putMutex);
    receiveOutput.clear();
    receiveStdErr.clear();
    debug("Writing to engine: |" + command + "\\n|");
    command.append("\n");
    int nbytes = command.length();
    assert (write(fd_p2c[1], command.c_str(), nbytes) == nbytes);

}

void Engine::setPosition(const string &fen) {
    put(POSITION_FEN[type] + fen);
}

void Engine::init() {
    start();
    sleep(1);
    put(SEND_INIT_STRING[type]);
    sleep(1);
//    assert(initialize);TODO

}
