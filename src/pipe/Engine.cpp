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
    pid_t childpid;
    char readbuffer[1024];

    string receive_output = "";

    if (pipe(fd_p2c) != 0 || pipe(fd_c2p) != 0) {
        cerr << "Failed to pipe\n";
        exit(1);
    }
    childpid = fork();

    if (childpid < 0) {
        cout << "Fork failed" << endl;
        exit(-1);
    } else if (childpid == 0) {
        if (dup2(fd_p2c[0], 0) != 0 || close(fd_p2c[0]) != 0 || close(fd_p2c[1]) != 0) {
            cerr << "Child: failed to set up standard input\n";
            exit(1);
        }
        if (dup2(fd_c2p[1], 1) != 1 || close(fd_c2p[1]) != 0 || close(fd_c2p[0]) != 0) {
            cerr << "Child: failed to set up standard output\n";
            exit(1);
        }

        execl(programName.c_str(), programName.c_str(), (char *) 0);
        cerr << "Failed to execute " << programName << endl;
        exit(1);
    } else {
        close(fd_p2c[0]);
        close(fd_c2p[1]);


        while (1) {
            bytes_read = read(fd_c2p[0], readbuffer, sizeof(readbuffer) - 1);

            if (bytes_read <= 0)
                break;

            readbuffer[bytes_read] = '\0';
            receive_output += readbuffer;
            cout << "From child: <<" << receive_output << ">>" << endl;
            receive_output.clear();
        }

    }
}

void Engine::endRun() {
    close(fd_c2p[0]);
    close(fd_p2c[1]);

}

void Engine::put(string command) {
    cout << "Writing to child: <<" << command << ">>" << endl;
    command.append("\n");
    int nbytes = command.length();
    if (write(fd_p2c[1], command.c_str(), nbytes) != nbytes) {
        cerr << "Parent: short write to child\n";
        exit(1);
    }

}
