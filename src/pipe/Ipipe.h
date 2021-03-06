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

#include <iostream>
#include "../namespaces/def.h"
#include "../util/logger.h"

using namespace std;

class Ipipe {
public :
    virtual int writeStdout(const string &msg) = 0;

    virtual const string readStderr() = 0;

    virtual const string readStdin() = 0;

    virtual bool init(const string &enginePath) = 0;

    virtual int isZombie() = 0;

};


