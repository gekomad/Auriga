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


using namespace std;
using namespace _def;

class Post : public Thread {
public:

    void run();

    void endRun();

    void preparePost(const string &uuid_perft, const string &uuid_task, const string &hearthbeat, const string &tot, const string &engine, const string &author, const string &fen, const string &minutes, const string &depth);

    void init(const string &host1, const int port1);

    bool isTerminated() { return terminated; }

private:
    bool terminated = false;
    int sock;
    string str;
    string host;
    int port;
    string ip;
};
