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
#include "network/GetGZ.h"

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
     |      |
     |      --stockfish-auriga.ini
     |      --crafty-auriga.ini
     |      .. ecc ..
     |
      --data
            |
             --perft_uuid1/
                        |
                        --task_uuid1/
                        |          |
                        |          --task_uuid1.ini
                        |          |
                        |          --task_uuid1.log
                        |
                        --task_uuid2/
                                   |
                                   --task_uuid2.ini
                                   |
                                   --task_uuid2.log
            |
             --perft_uuid2/
                        |
                        --task_uuid3/
                                   |
                                   --task_uuid3.ini
                                   |
                                   --task_uuid3.log

        */
        //--task /home/geko/auriga_root cinnamon.auriga.ini  -fetch_random
        if (params.size() == 4 && params[3] == "-fetch_random") {
            string aurigaRoot = params[1];
            string workerIniFile1 = aurigaRoot + "/worker/" + params[2];
            pair<string, string> uuids = fetch(workerIniFile1, aurigaRoot);
            doPerft(aurigaRoot, uuids.first, uuids.second, workerIniFile1);
        }
        //--task /home/geko/auriga_root cinnamon.auriga.ini DEE504F4-40A1-1A31-8F50-7BAC48DCC17G 6F55F6CF-DC1E-E4DD-D547-G5BB6A9G6BAD -fetch
        if (params.size() >= 5) {

            string workerIniFile1 = params[1] + "/worker/" + params[2];
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

            doPerft(aurigaRoot, perftUUID, taskUUID, workerIniFile1);

        } else {
            help(argv);
        }
    }

    static void doPerft(const string &aurigaRoot, const string &perftUUID, const string &taskUUID, const string &workerIniFile1) {

        int count = 0;
        string countS = "";
        string logpathTmp = aurigaRoot + "/data/" + perftUUID + "/" + taskUUID + ".log";
        while (FileUtil::fileExists(logpathTmp + countS)) {
            countS = to_string(++count);
        };
        string logpath = logpathTmp + countS;
        Logger::getInstance().setLogfile(logpath);

        string perftIniFile = aurigaRoot + "/data/" + perftUUID + "/" + perftUUID + ".ini";
        if (!FileUtil::fileExists(perftIniFile)) {
            fatal("file not found ", perftIniFile);
            exit(1);
        }
        log("Do perft perft_uuid: ", perftUUID, " task_uuid: ", taskUUID);

        _perft::Perft perft(taskUUID, perftIniFile, workerIniFile1);
        perft.calculate();
    }

    static pair<string, string> fetch(const string &workerIniFile, const string &aurigaRoot, const string &perftUUID = "") {

        string dataDir = aurigaRoot + "/data/";
        if (!perftUUID.empty()) {
            if (FileUtil::fileExists(dataDir + "/" + perftUUID + "/" + perftUUID + ".ini")) {
                warn("file " + dataDir + "/" + perftUUID + "/" + perftUUID + ".ini exists, skip fetch data");
                return pair<string, string>(perftUUID, "");
            }
        }
        IniFile ini(workerIniFile);
        string aurigaHost = ini.getValue("host");

        //test connection
        if (ResolveHost::getIP(aurigaHost).empty()) {
            fatal("cant't resolve host");
            exit(1);
        }

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

        GetGZ get;
        FileUtil::createDirectory(aurigaRoot + "/data/");
        string fileName = dataDir + "/" + perftUUID + ".ini";
        pair<string, string> uuids = get.get(aurigaHost, aurigaPort, "downloadini.php?id=" + perftUUID, aurigaRoot + "/data/");
        if (uuids.first.empty()) {
            warn("no data to fetch");
            return pair<string, string>("", "");
        }
        return uuids;
    }
};



