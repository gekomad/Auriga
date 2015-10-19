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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include "../blockingThreadPool/Thread.h"
#include "Server.h"
#include<arpa/inet.h>
#include "../namespaces/def.h"
#include <iostream>

#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;
using namespace _def;

class Client {
public:
    void sendMsg(const string &host, int portno, const string &msg);

    virtual ~Client() { closeSocket = true; }

    static void post() {
//http://codereview.stackexchange.com/questions/51270/socket-http-post-request
        string user("pippo");

        string host = "127.0.0.1";
        int portno = 80;

        struct sockaddr_in server;

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        assert(sock != -1);

        server.sin_addr.s_addr = inet_addr(host.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(portno);

        assert(connect(sock, (struct sockaddr *) &server, sizeof(server)) >= 0);
        std::ostringstream formBuffer; // <<< here

        char dataType1[] = "a=";
        char dataType2[] = "&b=";
        char dataType3[] = "&c=";

        char FormAction[] = "index.php";

        // get: length of the actual content
        auto ContentLength = user.size() + user.size() + user.size() + strlen(dataType1) + strlen(dataType2) + strlen(dataType3);

        // header
        formBuffer << "POST " << FormAction << " HTTP/1.1\n";
        formBuffer << "Content-Type: application/x-www-form-urlencoded\n";
        formBuffer << "Host: \n";
        formBuffer << "Content-Length: " << std::to_string(ContentLength) << "\n\n";

        // actual content
        formBuffer << dataType1 << user;
        formBuffer << dataType2 << user;
        formBuffer << dataType3 << user;

        const auto str = formBuffer.str();

        std::cout << str << std::endl;

        assert(send(sock, str.data(), str.size(), 0) == (int)str.size());

    }

protected:
    static int N_CLIENT;
private:
    mutex clientMutex;
    bool closeSocket = false;
    static u64 TOT;

    static int endClient;
};
