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
#include <errno.h>

#ifdef __WIN32__
# include <winsock2.h>
# include <windows.h>
#else

# include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<arpa/inet.h>

#endif

#include "../util/logger.h"
#include "Dns.h"

using namespace std;

class ResolveHost {
public:

    static string getIP(const string &hostname) {
        string ip = Dns::getInstance().getIp(hostname);
        if (!ip.empty())
            return ip;
        struct hostent *he;
        struct in_addr **addr_list;
        int i;

        if ((he = gethostbyname(hostname.c_str())) == NULL) {
            warn(hostname, " unresolved");
            return "";
        }

        addr_list = (struct in_addr **) he->h_addr_list;

        for (i = 0; addr_list[i] != NULL; i++) {
            Dns::getInstance()[hostname] = inet_ntoa(*addr_list[i]);
            return inet_ntoa(*addr_list[i]);
        }
        warn(hostname, " unresolved");
        return "";
    }

private:

};