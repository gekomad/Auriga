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
    static void help(char **argv) {
        string exe = FileUtil::getFileName(argv[0]);
        cout << "Calculate perft on task:\t" << exe << " --task WORKER.INI DIR PERFT_UUID TASK_UUID\n";
    }

    static void parse(int argc, char **argv) {
        vector<string> params;
        for (int i = 1; i < argc; i++) {
            params.push_back(argv[i]);
        }
        assert(params[0] == "--task");
        /*
        /root/
            |
             --perft_uuid/
                        |
                        --task_uuid/
                        |          |
                        |          --task_uuid.ini
                        |          --log/
                        |               |
                        |               --task_uuid.log
                        |
                        --task_uuid/
                                   |
                                   --task_uuid.ini
                                   --log/
                                        |
                                        --task_uuid.log
            |
             --perft_uuid/
                        |
                        --task_uuid/
                                   |
                                   --task_uuid.ini
                                   --log/
                                        |
                                        --task_uuid.log

        */
        //--task /home/geko/workspace/workspace_my/Auriga/conf/worker/stockfish.auriga.ini /home/geko/auriga_perft 8E42A477-83F1-8863-E05C-D68E4EA23236 3EC4AEB6-D764-9FBF-0991-5791A38CB691
        if (params.size() == 5) {

            string workerIniFile = params[1];
            if (!FileUtil::fileExists(workerIniFile)) {
                fatal("file not found ", workerIniFile);
                exit(0);
            }
            string dir = params[2];
            string perftUUID = params[3];
            string taskUUID = params[4];
            string perftIniFile = dir + "/" + perftUUID + "/" + perftUUID + ".ini";
            if (!FileUtil::fileExists(perftIniFile)) {
                fatal("file not found ", perftIniFile);
                exit(0);
            }
            int count = 0;
            string countS = "";
            string logpathTmp = dir + "/" + perftUUID + "/" + taskUUID + ".log";
            while (FileUtil::fileExists(logpathTmp + countS)) {
                countS = to_string(++count);
            };
            string logpath = logpathTmp + countS;
            Logger::getInstance().setLogfile(logpath);

            _perft::Perft perft(taskUUID, perftIniFile, workerIniFile);
            perft.calculate();

        } else {
            help(argv);
        }
    }
};



