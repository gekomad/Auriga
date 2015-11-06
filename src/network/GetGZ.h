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
#include "ResolveHost.h"

using namespace std;
using namespace _def;

class GetGZ {
private:
    std::regex rgxTot;
public:
    GetGZ() {
        rgxTot.assign(".*Content-Length: (\\d+).*");
    }

    bool get(const string &host, const int port, const string &url, const string &outFile) {
        char buf[4096];
        debug("resolving host ", host);
        string ip = ResolveHost::getIP(host);
        if (!ip.size()) {
            error("unknow host");
            return false;
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
        formBuffer << "Content-Type: application/x-gzip\n";
        formBuffer << "Host: " << host << "\n\n";

        string str = formBuffer.str();
        assert(send(sock, str.c_str(), str.size(), 0) == (int) str.size());
        int r;
        std::ofstream fout(outFile, std::ofstream::binary);
        char *header = 0;

        char startGzip[3];
        startGzip[0] = 31;
        startGzip[1] = 139;
        startGzip[2] = 0;
        string receiveStd;
        int totBytes = -1;
        int totWritten = 0;
      
        while (1) {
            r = recv(sock, buf, sizeof(buf), 0);
            if (r <= 0)break;
            buf[r] = 0;
            
            if (totBytes == -1) {
                receiveStd.append(buf);
                std::smatch match;
                if (regex_search(((const string) receiveStd).begin(), ((const string) receiveStd).end(), match, rgxTot)) {
                    string tot = match[1].str();
                    totBytes = std::stoi(tot);
                    totBytes--;//TODO
                    totBytes--;//TODO
                }
            }
            if (!header) {
                header = strstr(buf, startGzip);
                if (header) {
                    assert(totBytes!=-1);
                    totWritten += r - (header - buf) ;
                    fout.write(header, r - (header - buf) );
                    fout.flush();
                }
            } else {
                if (totWritten+r > totBytes){
                    fout.write(buf, totBytes-totWritten);
                    break;
                }
                totWritten += r;
                fout.write(buf, r);
                fout.flush();
            }
        }
        fout.close();
        if (!header) return false;
        return true;
    }
};
