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
    int writeStdout(const string &msg);

    ~PipePosix();

    const string readStdin();

    const string readStderr();

    bool init(const string &enginePath);
    int isZombie();
private:
    pid_t childpid;
    bool initialized;
    int fd_p2c[2], stdIn[2], stdErr[2];
};


#endif