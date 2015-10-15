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

#include "../../namespaces/def.h"
#include "../../network/Server.h"
#include "PerftTree.h"

using namespace _debug;
using namespace _def;

class PerftTreeDao {
public:
    PerftTreeDao(const string &iniFile1) {
        iniFile = iniFile1;
        iniFile = new IniFile(iniFile1);
        readFile();
    }

    ~PerftTreeDao() {
        if (iniFile) delete iniFile;
        iniFile = nullptr;
    }

private:
    void readFile();
    void readNode();
    PerftTree perftTree;
    const string &iniFile;
    IniFile *iniFile = nullptr;
};

