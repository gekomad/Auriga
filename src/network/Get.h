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

#ifdef __WIN32__
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

class Get {
public:
    string get(const string &host, const int port, const string &url) {
        string receiveBuffer;
        char buf[1024];
        debug("resolving host ", host);
        string ip = ResolveHost::getIP(host);
        if (!ip.size()) {
            error("unknow host");
            return "";
        }
        debug("resolved host ", host, " -> ", ip);
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        assert(sock != -1);
        struct sockaddr_in server;
        server.sin_addr.s_addr = inet_addr(ip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(port);

        assert(connect(sock, (struct sockaddr *) &server, sizeof(server)) >= 0);

        std::ostringstream formBuffer;
        formBuffer << "GET /" << url << " HTTP/1.1\n";
//        formBuffer << "Content-Type: application/x-www-form-urlencoded\n";
        formBuffer << "Host: " << host << "\n\n";

        string str = formBuffer.str();
        assert(send(sock, str.c_str(), str.size(), 0) == (int) str.size());
        int r;
        while (1) {
            r = recv(sock, buf, sizeof(buf), 0);
            if (r <= 0)break;
            buf[r]=0;
            receiveBuffer.append(buf);

        }
        std::size_t pos = receiveBuffer.find("#auriga");
        if(pos==-1)return "";
        std::string str3 = receiveBuffer.substr (pos);

        debug (str3);
        return str3;
    }



};
