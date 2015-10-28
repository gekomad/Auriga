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
#include "network/Get.h"

using namespace _def;

class GetOptTask {
public:
    static void help(char **argv) {
        string exe = FileUtil::getFileName(argv[0]);
        cout << "Calculate perft on task:\t" << exe << " --task AURIGA_ROOT WORKER.INI DIR PERFT_UUID TASK_UUID [-fetch]\n";
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
     -worker
            |
            --stockfish-auriga.ini
            --crafty-auriga.ini
            .. ecc ..
     |
      --data
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
        //--task /home/geko/auriga_root stockfish.auriga.ini 8E42A477-83F1-8863-E05C-D68E4EA23236 3EC4AEB6-D764-9FBF-0991-5791A38CB691 -fetch
        if (params.size() >= 5) {

            string workerIniFile1 = params[1]+"/worker/"+params[2];
            if (!FileUtil::fileExists(workerIniFile1)) {
                fatal("file not found ", workerIniFile1);
                exit(1);
            }
            string aurigaRoot = params[1];
            string perftUUID = params[3];
            string taskUUID = params[4];
            if (params.size() == 6 && params[5] == "-fetch") {
                fetch(workerIniFile1, aurigaRoot, perftUUID);
            }
            string perftIniFile = aurigaRoot + "/data/" + perftUUID + "/" + perftUUID + ".ini";
            if (!FileUtil::fileExists(perftIniFile)) {
                fatal("file not found ", perftIniFile);
                exit(1);
            }
            int count = 0;
            string countS = "";
            string logpathTmp = aurigaRoot + "/data/" + perftUUID + "/" + taskUUID + ".log";
            while (FileUtil::fileExists(logpathTmp + countS)) {
                countS = to_string(++count);
            };
            string logpath = logpathTmp + countS;
            Logger::getInstance().setLogfile(logpath);

            _perft::Perft perft(taskUUID, perftIniFile, workerIniFile1);
            perft.calculate();

        } else {
            help(argv);
        }
    }

    static bool fetch(const string &workerIniFile, const string &aurigaRoot, const string &perftUUID) {

        string dir = aurigaRoot + "/data/" + perftUUID;
        if (FileUtil::fileExists(dir)) {
            warn("directory " + dir + " exists, skip fetch data");
            return true;
        }
        IniFile ini(workerIniFile);
        string aurigaHost = ini.getValue("host");
        int aurigaPort;
        if (aurigaHost.empty()) {
            fatal("auriga host not defined, can't fetch data");
            exit(1);
        } else {
            string port = ini.getValue("port");
            if (port.empty()) {
                fatal("auriga server port not defined, can't fetch data");
                exit(1);
            } else {
                aurigaPort = stoi(port);
            }
        }

        Get get;
        //<?php header("Content-Type:text/plain");echo htmlspecialchars(file_get_contents($_GET['id'].".ini")); ?>
        string iniString = get.get(aurigaHost, aurigaPort, "downloadini.php?id=" + perftUUID);
        if (iniString.empty()) {
            fatal("error on fetch data");
            exit(1);
        }
        FileUtil::createDirectory(aurigaRoot + "/data/" + perftUUID);
        string fileName = dir + "/" + perftUUID + ".ini";
        ofstream fout(fileName);
        fout << iniString;
        fout.close();
        return true;
    }
};



