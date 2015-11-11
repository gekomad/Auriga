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
#include <regex>

#ifdef _WIN32
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
#ifdef _WIN32
        WSADATA wsaData;
        WORD version;
        int error;

        version = MAKEWORD( 2, 0 );
        error = WSAStartup( version, &wsaData );
#endif


        string ip = Dns::getInstance().getIp(hostname);
        if (!ip.empty())
            return ip;

        ip = getHeuristic(hostname);
        if (!ip.empty())
            return ip;
        struct hostent *hp = gethostbyname(hostname.c_str());
        if (hp != NULL) {
            unsigned int i = 0;
            while (hp->h_addr_list[i] != NULL) {
                return  inet_ntoa(*(struct in_addr *) (hp->h_addr_list[i])) ;
                i++;
            }
        }
        warn(hostname, " unresolved");
        return "";
    }

private:
    static void updateDns(const string &host, const string &ip) {
        Dns::getInstance()[host] = ip;
    }

    static const string getHeuristic(const string &host) {
        if (host == "localhost") {
            updateDns(host, "127.0.0.1");
            return "127.0.0.1";
        }
        if (isIP(host)) {
            updateDns(host, host);
            return host;
        }
        return "";
    }

    static bool isIP(const string &host) {
        static const std::regex regIp(R"(^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)");
        if (std::regex_match(host.begin(), host.end(), regIp))
            return true;
        return false;
    }

};