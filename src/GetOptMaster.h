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
        string AURIGA_ROOT;
#if defined(_WIN32)
        AURIGA_ROOT="%AURIGA_ROOT%";
         AURIGA_ROOT_EX = "c:\\chess\\auriga_root";
#else
        AURIGA_ROOT = "$AURIGA_ROOT";
#endif
        string exe = FileUtil::getFileName(argv[0]);
        cout << "Generate ini file:\t" << exe << " --generate-ini " << AURIGA_ROOT << " Ntask \"FEN\" DEPTH\n";
        cout << "\texample: " << AURIGA_ROOT << PATH_SEPARATOR << exe << " --generate-ini  " << AURIGA_ROOT << "  1000 \"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\" 15\n\n";
        cout << "Calculate Ntasks on reduce:\t" << exe << " --calculate-tasks \"FEN\"\n";
        cout << "\texample: " << AURIGA_ROOT << PATH_SEPARATOR << exe << " --calculate-tasks  " << " \"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\" \n\n";
    }

    static void parse(int argc, char **argv) {
        vector<string> params;
        for (int i = 1; i < argc; i++) {
            params.push_back(argv[i]);
        }

        if (params[0] == "--calculate-tasks" && params.size() == 2) {
            string fen = params[1];

            WrapperCinnamon wrapperCinnamon;
            unsigned tasksReduce = 0;
            int depth=1;
            while (tasksReduce < 10000) {
                tasksReduce = wrapperCinnamon.perft(fen, depth);
                cout << "N tasks: " << tasksReduce << " reduce depth " << depth << endl;
                depth++;
            }

        } else if (params[0] == "--generate-ini" && params.size() == 5) {
            string aurigaRoot = params[1];
            int Ntask = stoi(params[2]);
            string fen = params[3];
            int depth = stoi(params[4]);

            string perftUUID = PerftSplitter::generateMasterINI(fen, Ntask, depth, aurigaRoot);
            cout << "X" << perftUUID << "X\n";
        } else {
            help(argv);
        }
    }
};



