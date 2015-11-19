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

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include "../util/Compression.h"

#ifdef _WIN32
# include <winsock2.h>
# include <windows.h>
#else

# include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<arpa/inet.h>

#endif

#include <iostream>
#include "../threadPool/Thread.h"
#include "../namespaces/def.h"
#include <sstream>
#include "../util/Singleton.h"
#include "../util/logger.h"
#include "ResolveHost.h"

using namespace std;
using namespace _def;

class GetGZ {
private:
    std::regex rgxSize;
    unsigned char GZIP_HEADER[3] = {0x1f, 0x8b, 0x0};
public:
    GetGZ();

    pair<string, string> get(const string &host, const int port, const string &url, const string &dataDir);
};
