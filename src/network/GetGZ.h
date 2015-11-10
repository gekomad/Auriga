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
public:
    GetGZ() {
        //.*XXXuuid_perftXXXuuid_depthXXXsize_fileXXX.*
        rgxSize.assign(".*XXX(.*)XXX(.*)XXX(\\d+)XXXSTART.*");
    }

    typedef unsigned char BYTE;
    const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";


    bool is_base64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }

    std::vector<BYTE> base64_decode(std::string const &encoded_string) {//TODO file util
        int in_len = encoded_string.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        BYTE char_array_4[4], char_array_3[3];
        std::vector<BYTE> ret;

        while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
            char_array_4[i++] = encoded_string[in_];
            in_++;
            if (i == 4) {
                for (i = 0; i < 4; i++)
                    char_array_4[i] = base64_chars.find(char_array_4[i]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    ret.push_back(char_array_3[i]);
                i = 0;
            }
        }

        if (i) {
            for (j = i; j < 4; j++)
                char_array_4[j] = 0;

            for (j = 0; j < 4; j++)
                char_array_4[j] = base64_chars.find(char_array_4[j]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
        }

        return ret;
    }


    pair<string, string> get(const string &host, const int port, const string &url, const string &dataDir) {
        char buf[4096];
        debug("resolving host ", host);
        string ip = ResolveHost::getIP(host);
        if (!ip.size()) {
            error("unknow host");
            return pair<string, string>("", "");
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
        string receiveStd;
        int totBytes = -1;
        int totWritten = 0;
        string deb;
        string UUID_PERFT;
        string UUID_TASK;
        string encode64;

        while (1) {
#ifdef DEBUG_MODE
            memset(buf, 0, sizeof(buf));
#endif
            r = recv(sock, buf, sizeof(buf) - 1, 0);
            if (r <= 0)break;

#ifdef DEBUG_MODE
            deb.assign(buf);
#endif
            if (totBytes == -1) {
                buf[r] = 0;
                receiveStd.append(buf);
                std::smatch match;
                if (regex_search(((const string) receiveStd).begin(), ((const string) receiveStd).end(), match, rgxSize)) {
                    UUID_PERFT = match[1].str();
                    UUID_TASK = match[2].str();
                    string tot = match[3].str();
                    totBytes = std::stoi(tot);

                    string ini = dataDir + "/" + UUID_PERFT + "/" + UUID_PERFT + ".ini";
                    if (FileUtil::fileExists(ini)) {
                        info("file ", ini, " exists skip fetch")
                        return pair<string, string>(UUID_PERFT, UUID_TASK);
                    }
                }
                char *h = strstr(buf, "XXXSTART");
                if(!h){
                    error("error on fatching data");
                    return pair<string, string>("", "");
                }
                h+=strlen("XXXSTART");
                totWritten=strlen(h);
                encode64.append(h);
                continue;
            }

            if (totWritten + r > totBytes) {
                encode64.append(buf, totBytes - totWritten);
                totWritten += totBytes - totWritten;
                break;
            }
            totWritten += r;
            encode64.append(buf, r);
        }
        std::vector<BYTE> x = base64_decode(encode64);

        FileUtil::createDirectory(dataDir + "/" + UUID_PERFT);
        string fileGzipped = dataDir + "/" + UUID_PERFT + "/" + UUID_PERFT + ".ini.gz";
        std::ofstream tmp(fileGzipped);
        for (int i = 0; i < x.size(); i++)
            tmp << x.at(i);
        tmp.close();

        Compression compression;
        compression.decompress(fileGzipped);
        //std::remove(fileGzipped.c_str());

        cout << "fine" << endl;
        return pair<string, string>(UUID_PERFT, UUID_TASK);
    }
};
