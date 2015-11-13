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
    std::regex rgxSize;
    unsigned char GZIP_HEADER[3] = {0x1f, 0x8b, 0x0};
public:
    GetGZ() {//TODO in .cpp
        rgxSize.assign(R"(.*Content-Length: (\d+)\r\nuuid_perft: (.+)\r\nuuid_task: (.+)\r.*)");
    }

    pair<string, string> get(const string &host, const int port, const string &url, const string &dataDir) {
        char buf[4096];
        debug("resolving host ", host);
        string ip = ResolveHost::getIP(host);
        if (!ip.size()) {
            error("unknow host ", host);
            return pair<string, string>("", "");
        }

        debug("resolved host ", host, " -> ", ip);
#ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(1,1), &wsaData) == SOCKET_ERROR) {
            error ("Error initialising WSA");
            return pair<string, string>("", "");
        }
#endif
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
        string receiveStd;
        int totBytes = -1;
        int totWritten = 0;
        string deb;
        string UUID_PERFT;
        string UUID_TASK;

        char *gzBuf = nullptr;

        ofstream tmp;
        string fileGzipped;
        while (1) {
#ifdef DEBUG_MODE
            memset(buf, 0, sizeof(buf));
#endif
            r = recv(sock, buf, sizeof(buf) - 1, 0);
            if (r <= 0)
                break;

#ifdef DEBUG_MODE
            deb.assign(buf);
#endif
            if (totBytes == -1) {
                buf[r] = 0;
                receiveStd.append(buf);
                std::smatch match;
                if (regex_search(receiveStd, match, rgxSize) && match.size() > 1) {
                    string tot = match[1].str();
                    UUID_PERFT = match[2].str();
                    UUID_TASK = match[3].str();
                    totBytes = std::stoi(tot);
                    string ini = dataDir + PATH_SEPARATOR + UUID_PERFT + PATH_SEPARATOR + UUID_PERFT + ".ini";
                    if (FileUtil::fileExists(ini)) {
                        info("file ", ini, " exists skip fetch")
                        return pair<string, string>(UUID_PERFT, UUID_TASK);
                    }
                    fileGzipped = dataDir + PATH_SEPARATOR + UUID_PERFT + PATH_SEPARATOR + UUID_PERFT + ".ini.gz";
                    tmp.open(fileGzipped);
                } else {
                    error("Error on fatching data");
                    return pair<string, string>("", "");
                }

                char *h = strstr(buf, (const char *) GZIP_HEADER);
                if (!h) {
                    error("error on fatching data");
                    return pair<string, string>("", "");
                }
                totWritten = r - (h - buf);
                tmp.write(h, totWritten);
                continue;
            }

            if (totWritten + r > totBytes) {
                int k = totBytes - totWritten;
                tmp.write(buf, k);
                totWritten += k;
                break;
            }
            totWritten += r;
            tmp.write(buf, r);
        }
        FileUtil::createDirectory(dataDir + PATH_SEPARATOR + UUID_PERFT);


        if (!tmp.is_open()) {
            return pair<string, string>("", "");
        }
        tmp.close();
        Compression compression;
        compression.decompress(fileGzipped);

        //std::remove(fileGzipped.c_str());

        return pair<string, string>(UUID_PERFT, UUID_TASK);
    }
};
