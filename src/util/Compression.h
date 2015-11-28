/*
    Cinnamon UCI chess engine
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


#include "../lib/zlib.h"
#include "logger.h"
#include "../namespaces/def.h"
#include <string.h>
#include <regex>

using namespace std;

class Compression {
private:
    std::regex rgxName;
public:
    Compression() {
        rgxName.assign("(.*)\\.gz");
    }

    string decompress(const string &fileGZ) {
        string fileOut;
        std::smatch match;
        if (regex_search(fileGZ, match, rgxName) && match.size() > 1) {
            fileOut = match[1].str();
        }
        const int LENGTH = 0x1000;
        static const char *file_name = fileGZ.c_str();
        FILE *pFile = fopen(fileOut.c_str(), "w");
        gzFile file;
        file = gzopen(file_name, "rb");
        if (!file) {
            fatal("error on opening file ", file_name);
            exit(1);
        }
        while (1) {
            int err;
            int bytes_read;
            unsigned char buffer[LENGTH];
            bytes_read = gzread(file, buffer, LENGTH - 1);
            buffer[bytes_read] = '\0';
            debug(buffer);
            fwrite(buffer, sizeof(char), bytes_read, pFile);
            if (bytes_read < LENGTH - 1) {
                if (gzeof(file)) {
                    break;
                } else {
                    const char *error_string;
                    error_string = gzerror(file, &err);
                    if (err) {
                        fatal("error on decompressing file ", file, " ", error_string);
                        exit(1);
                    }
                }
            }
        }
        gzclose(file);
        fclose(pFile);
        return fileOut;
    }

    void compress(const string &msg, const string &fileOut) {
        FILE *pFile = fopen(fileOut.c_str(), "wb");
        const int CHUNK = 0x4000;
        unsigned char out[CHUNK];
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        assert (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 | 16, 8, Z_DEFAULT_STRATEGY) >= 0);

        strm.next_in = (unsigned char *) msg.c_str();
        strm.avail_in = msg.size();
        do {
            int have;
            strm.avail_out = CHUNK;
            strm.next_out = out;
            assert (deflate(&strm, Z_FINISH) >= 0);
            have = CHUNK - strm.avail_out;
            fwrite(out, sizeof(char), have, pFile);
        } while (strm.avail_out == 0);
        deflateEnd(&strm);
        fclose(pFile);

    }

};