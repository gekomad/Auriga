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

#include "perft/master/PerftSplitter.h"
#include "namespaces/def.h"

class GetOptMaster {
public:
    static void help(char **argv) {
        string exe = FileUtil::getFileName(argv[0]);
        cout << "Generate ini file:\t\t" << exe << " --generate-ini AURIGA_ROOT Ntask \"FEN\" DEPTH\n";
    }

    static void parse(int argc, char **argv) {
        vector<string> params;
        for (int i = 1; i < argc; i++) {
            params.push_back(argv[i]);
        }
        assert(params[0] == "--generate-ini");

        //--generate-ini /home/geko/auriga_root 100 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -" 8
        if (params.size() == 5) {
            string aurigaRoot = params[1];
            int Ntask = stoi(params[2]);
            string fen = params[3];
            int depth = stoi(params[4]);

            PerftSplitter::generateMasterINI(fen, Ntask, depth, aurigaRoot);
        } else {
            help(argv);
        }
    }
};



