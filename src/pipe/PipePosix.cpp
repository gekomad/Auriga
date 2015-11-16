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

#include "PipePosix.h"

int PipePosix::writeStdout(const string &msg) {
    assert(initialized);
    return write(fd_p2c[1], msg.c_str(), msg.size());
};

PipePosix::~PipePosix() {
    if (initialized) {
        close(stdIn[0]);
        close(stdErr[0]);
        close(fd_p2c[1]);
    }
}

const string PipePosix::readStdin() {
    assert(initialized);
    char readbuffer[2048];
    int bytes_read = read(stdIn[0], readbuffer, sizeof(readbuffer) - 1);
    if (bytes_read <= 0) {
        return "";
    };
    readbuffer[bytes_read] = 0;
    return string(readbuffer);
};

const string PipePosix::readStderr() {
    assert(initialized);
    char readbuffer[2048];
    int bytes_read = read(stdErr[0], readbuffer, sizeof(readbuffer) - 1);
    if (bytes_read <= 0) {
        return "";
    };
    readbuffer[bytes_read] = 0;
    return string(readbuffer);
};

bool PipePosix::init(const string &enginePath) {
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

int PipePosix::isZombie() {
    bool iszombie = false;
    char pbuf[32];
    snprintf(pbuf, sizeof(pbuf), "/proc/%d/stat", (int) childpid);
    FILE *fpstat = fopen(pbuf, "r");
    if (fpstat) {
        int rpid = 0;
        char rcmd[32];
        char rstatc = 0;
        fscanf(fpstat, "%d %30s %c", &rpid, rcmd, &rstatc);
        iszombie = rstatc == 'Z';
        fclose(fpstat);
    }
    return iszombie ? (int) childpid : 0;
}

#endif
