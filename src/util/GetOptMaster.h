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

#include "../perft/master/PerftSplitter.h"
#include "../namespaces/def.h"
#include "../perft/shared/PerftTreeDao.h"

class GetOptMaster {
public:

    static void parse(int argc, char **argv) {
        vector<string> params;
        for (int i = 1; i < argc; i++) {
            params.push_back(argv[i]);
        }
        assert(params[0] == "--master");
        // ./auriga --master -generate_ini file.txt
        if (params.size() == 3 && params[1] == "-generate_ini") {
            string nodesFile = params[2];

            PerftSplitter::generateINI(nodesFile);
        }
    }
};



