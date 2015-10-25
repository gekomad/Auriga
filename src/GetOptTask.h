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

#include "perft/task/Perft.h"
#include "namespaces/def.h"

using namespace _def;

class GetOptTask {
public:

    static void parse(int argc, char **argv) {
        vector<string> params;
        for (int i = 1; i < argc; i++) {
            params.push_back(argv[i]);
        }
        assert(params[0] == "--task");
        //--task -start /home/geko/workspace/Auriga/src/conf/worker/stockfish.auriga.ini /home/geko/02A85E37-CE45-A881-A98B-557671CDF051 37BE4F67-E060-001F-D578-794FC3681C07
        if ((params.size() == 5 && params[1] == "-start") || (params.size() == 7 && params[5] == "-log")) {

            string engine = params[2];
            if (!FileUtil::fileExists(engine)) {
                fatal("file not found ", engine);
                exit(0);
            }
            string master = params[3];
            if (!FileUtil::fileExists(master)) {
                fatal("file not found ", master);
                exit(0);
            }
            string taskUUID = params[4];
            IniFile ini(master);
            string perftUUID = ini.getValue("perft_uuid");

            string dirlog = perftUUID;//.append("/").append(taskUUID) + ".log";
            if (params.size() == 7) {
                dirlog = string(params[6]) + "/" + dirlog;
            }
            string logpath = dirlog + "/" + taskUUID + ".log";
            Logger::getInstance().setLogfile(logpath);

            _perft::Perft perft(taskUUID, master, engine);
            perft.calculate();

        } else {
            cout << "err" << endl;//TODO
        }
    }
};



