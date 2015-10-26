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
#ifndef _WIN32
#pragma once

#include <iostream>
#include "Ipipe.h"
#include <unistd.h>

using namespace std;

class PipePosix : public Ipipe {
public :
    int writeStdout(const string &msg) {
        assert(initialized);
        return write(fd_p2c[1], msg.c_str(), msg.size());
    };

    ~PipePosix() {
        if (initialized) {
            close(stdIn[0]);
            close(fd_p2c[1]);
        }
    }

    const string readStdin() {
        assert(initialized);
        char readbuffer[2048];
        int bytes_read = read(stdIn[0], readbuffer, sizeof(readbuffer) - 1);
        if (bytes_read <= 0) {
            return "";
        };
        readbuffer[bytes_read] = 0;
        return string(readbuffer);
    };

    const string readStderr() {
        assert(initialized);
        char readbuffer[2048];
        int bytes_read = read(stdErr[0], readbuffer, sizeof(readbuffer) - 1);
        if (bytes_read <= 0) {
            return "";
        };
        readbuffer[bytes_read] = 0;
        return string(readbuffer);
    };

    bool init(const string &enginePath) {
        initialized = false;
        if (!FileUtil::fileExists(enginePath)) {
            warn("engine not found", enginePath);
            return initialized;
        }
        assert(!pipe(fd_p2c) && !pipe(stdIn) && !pipe(stdErr));

        childpid = fork();

        assert(childpid >= 0);
        if (childpid == 0) {
            assert(dup2(fd_p2c[0], 0) == 0 && close(fd_p2c[0]) == 0 && close(fd_p2c[1]) == 0);
            assert(dup2(stdIn[1], 1) == 1 && close(stdIn[1]) == 0 && close(stdIn[0]) == 0);
            assert(dup2(stdErr[1], 2) >= 0);
            execl(enginePath.c_str(), enginePath.c_str(), (char *) 0);
            exit(0);
        }
        close(fd_p2c[0]);
        close(stdIn[1]);
        close(stdErr[1]);
        initialized = true;

        return initialized;
    };
private:
    pid_t childpid;
    bool initialized;
    int fd_p2c[2], stdIn[2], stdErr[2];
};


#endif