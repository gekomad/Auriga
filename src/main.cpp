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

#include <iostream>
#include "GetOpt.h"
#include <iostream>
#include <cstring>
#include "network/Client.h"
#include "namespaces/def.h"
#include "network/ResolveHost.h"
#include<iostream>

using namespace std;

int main(int argc, char *argv[]) {

    cout << NAME << endl;
#if UINTPTR_MAX != 0xffffffffffffffff
    //32 bit
    warn ("32bit version manage max 64bit results");
#endif
    GetOpt::parse(argc, argv);
    return 0;
}
