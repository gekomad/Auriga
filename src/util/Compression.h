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


#include <stdio.h>
#include <stdlib.h>
#include "../lib/zlib.h"
#include <errno.h>
#include <string.h>


class Compression {
public:
    int decompress(const string &fileGZ,const string &fileOut) {
        const int LENGTH = 0x1000;
        static const char *file_name = fileGZ.c_str();
        FILE *pFile = fopen(fileOut.c_str(), "w");
        gzFile file;
        file = gzopen(file_name, "r");
        if (!file) {
            fprintf(stderr, "gzopen of '%s' failed: %s.\n", file_name,
                    strerror(errno));
            exit(EXIT_FAILURE);
        }
        while (1) {
            int err;
            int bytes_read;
            unsigned char buffer[LENGTH];
            bytes_read = gzread(file, buffer, LENGTH - 1);
            buffer[bytes_read] = '\0';
            printf("%s", buffer);
            fwrite(buffer, sizeof(char), bytes_read, pFile);
            if (bytes_read < LENGTH - 1) {
                if (gzeof(file)) {
                    break;
                }
                else {
                    const char *error_string;
                    error_string = gzerror(file, &err);
                    if (err) {
                        fprintf(stderr, "Error: %s.\n", error_string);
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
        gzclose(file);
        fclose(pFile);
        return 0;
    }

#define CALL_ZLIB(x) {                                                  \
        int status;                                                     \
        status = x;                                                     \
        if (status < 0) {                                               \
            fprintf (stderr,                                            \
                     "%s:%d: %s returned a bad status of %d.\n",        \
                     __FILE__, __LINE__, #x, status);                   \
            exit (EXIT_FAILURE);                                        \
        }                                                               \
    }


    void strm_init(z_stream *strm) {
        strm->zalloc = Z_NULL;
        strm->zfree = Z_NULL;
        strm->opaque = Z_NULL;
        CALL_ZLIB (deflateInit2(strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                                15 | 16, 8,
                                Z_DEFAULT_STRATEGY));
    }


    void compress(const string &msg, const string &fileOut) {

        FILE *pFile = fopen(fileOut.c_str(), "wb");

        const int CHUNK = 0x4000;
        unsigned char out[CHUNK];
        z_stream strm;
        strm_init(&strm);
        strm.next_in = (unsigned char *) msg.c_str();
        strm.avail_in = msg.size();
        do {
            int have;
            strm.avail_out = CHUNK;
            strm.next_out = out;
            CALL_ZLIB (deflate(&strm, Z_FINISH));
            have = CHUNK - strm.avail_out;
            fwrite(out, sizeof(char), have, pFile);
        }
        while (strm.avail_out == 0);
        deflateEnd(&strm);
        fclose(pFile);

    }

};