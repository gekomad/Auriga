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
#include <regex>
#include "../namespaces/debug.h"
#include "logger.h"
#include "FileUtil.h"

using namespace std;
using namespace _debug;

class IniFile {
public:

    IniFile(const string &fileName1) {
        fileName = fileName1;
        if (FileUtil::fileSize(fileName) <= 0) {
            debug("file size 0: ", fileName);
            return;
        }
        endFile = true;
        inData.open(fileName, std::ofstream::in);
        if (inData.is_open()) {
            endFile = false;
        } else {
            warn("file not found: ", fileName);
        }
        rgxLine.assign("^(.+?)=(.*)$");
        rgxTag.assign("^\\[.+\\]$");
    }

    ~IniFile() {
        if (endFile) {
            inData.close();
        }
    }

    string getValue(const string &value) {
        IniFile file(fileName);
        while (true) {
            pair<string, string> *parameters = file.get();
            if (!parameters)return "";
            if (parameters->first == value) {
                return parameters->second;
            }
        }
    }

    pair<string, string> *get() {
        std::smatch match;
        string line;
        while (!endFile) {
            if (inData.eof()) {
                endFile = true;
                return nullptr;
            }
            getline(inData, line);
            trace("read fileName: ", line);
            if (!line.size())continue;
            if (line.at(0) == '#' || line.at(0) == ';')continue;

            const string line2 = line;
            if (regex_search(line2, match, rgxTag)) {
                params.first = line;
                params.second = "";
            } else if (regex_search(line2, match, rgxLine) && match.size() > 1) {
                params.first = String(match[1]).trim();
                if (!params.first.size())continue;
                params.second = match[2];
            } else {
                continue;
            }
            return &params;
        }

        return nullptr;
    };

private:

    std::regex rgxLine;
    std::regex rgxTag;
    bool endFile = true;
    ifstream inData;
    string fileName;
    pair<string, string> params;
};

