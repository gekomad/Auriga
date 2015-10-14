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

#pragma once

#include "../blockingThreadPool/Thread.h"
#include <unistd.h>
#include <string>
#include <iostream>

class Engine : public Thread {
public :

    Engine(const string &fileName1, const string &type1) {
        programName = fileName1;
        type = type1;
    }

    void put(string);
    virtual void run();

    virtual void endRun();

private:
    string programName;
    string type;
    int fd_p2c[2], fd_c2p[2];
};

