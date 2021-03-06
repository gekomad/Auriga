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

#include <fstream>
#include <algorithm>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;


class FileUtil {
public:
    static bool fileExists(const string &filename) {
        ifstream inData;
        inData.open(filename);
        if (!inData) {
            return false;
        }
        inData.close();
        return true;
    }

    static bool createDirectory(const string &filename) {
#if defined(_WIN32)
        mkdir(filename.c_str());
#else
        mkdir(filename.c_str(), 0755);
#endif
        return true;
    }

    static int fileSize(const string &filename) {
        std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
        return in.tellg();
    }

    static string getFileName(const string &path1) {
        string path = path1;
        replace(path.begin(), path.end(), ':', '/');
        replace(path.begin(), path.end(), '\\', '/');
        istringstream iss(path);
        string token;
        while (getline(iss, token, '/'));
        return token;
    }

    static bool contains(const string &filename, const string keyword) {
        string line;
        ifstream in(filename);
        if (in.is_open()) {
            while (getline(in, line)) {
                if (line.find(keyword) != std::string::npos) {
                    in.close();
                    return true;
                }
            }
        }
        in.close();
        return false;
    }
};

