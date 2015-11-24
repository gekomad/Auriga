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
        string AURIGA_ROOT;
#if defined(_WIN32)
        AURIGA_ROOT="%AURIGA_ROOT%";
#else
        AURIGA_ROOT = "$AURIGA_ROOT";
#endif
        string exe = FileUtil::getFileName(argv[0]);

        cout << "Calculate perft on uuid_perft and uuid_task:\t" << AURIGA_ROOT << PATH_SEPARATOR << exe << " --task " << AURIGA_ROOT << " WORKER PERFT_UUID TASK_UUID\n";
        cout << "\texample: " << AURIGA_ROOT << PATH_SEPARATOR << exe << " --task  " << AURIGA_ROOT << "  stockfish 47E1A274-74D3-9A5D-6AD0-E3B06FD37D0F    AF02E872-10D0-DE6C-4089-1F23DB5D4324\n\n";
        cout << "Calculate perft on uuid_perft and random uuid_task:\t" << exe << " --task " << AURIGA_ROOT << " WORKER PERFT_UUID ?\n";
        cout << "\texample: " << AURIGA_ROOT << PATH_SEPARATOR << exe << " --task  " << AURIGA_ROOT << "  stockfish 47E1A274-74D3-9A5D-6AD0-E3B06FD37D0F    ?\n\n";
        cout << "Calculate perft on random uuid_perft and random uuid_task:\t" << exe << " --task " << AURIGA_ROOT << " WORKER ? ?\n";
        cout << "\texample: " << AURIGA_ROOT << PATH_SEPARATOR << exe << " --task  " << AURIGA_ROOT << "  stockfish ?    ?\n\n";
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
        if (params.size() == 5) {
            string aurigaRoot = params[1];
            string worker = params[2];
            string perftUUID = params[3];
            string taskUUID = params[4];

            string workerIniFile1 = aurigaRoot + PATH_SEPARATOR + "worker" + PATH_SEPARATOR + worker + ".worker.ini";
            if (!FileUtil::fileExists(workerIniFile1)) {
                fatal("file not found ", workerIniFile1);
                exit(1);
            }

            pair<string, string> uuids = fetch(workerIniFile1, aurigaRoot, perftUUID, taskUUID);
            if (!uuids.first.empty()) {
                string uuidTask = taskUUID == "?" ? uuids.second : taskUUID;
                doPerft(aurigaRoot, uuids.first, uuidTask, workerIniFile1);
            }

        } else {
            help(argv);
        }
    }

    static void doPerft(const string &aurigaRoot, const string &perftUUID, const string &taskUUID, const string &workerIniFile1) {

        int count = 0;
        string countS = "";
        string logpathTmp = aurigaRoot + PATH_SEPARATOR + "data" + PATH_SEPARATOR + perftUUID + PATH_SEPARATOR + taskUUID + ".log";
        while (FileUtil::fileExists(logpathTmp + countS)) {
            countS = to_string(++count);
        };
        string logpath = logpathTmp + countS;
        Logger::getInstance().setLogfile(logpath);

        string perftIniFile = aurigaRoot + PATH_SEPARATOR "data" + PATH_SEPARATOR + perftUUID + PATH_SEPARATOR + perftUUID + ".ini";
        if (!FileUtil::fileExists(perftIniFile)) {
            fatal("file not found ", perftIniFile);
            exit(1);
        }
        log("Do perft perft_uuid: ", perftUUID, " task_uuid: ", taskUUID);

        _perft::Perft perft(aurigaRoot, taskUUID, perftIniFile, workerIniFile1);
        perft.calculate();
    }

    static pair<string, string> fetch(const string &workerIniFile, const string &aurigaRoot, const string &perftUUID, const string &taskUUID) {

        string dataDir = aurigaRoot + PATH_SEPARATOR "data" + PATH_SEPARATOR;

        IniFile ini(workerIniFile);
        string aurigaHost = ini.getValue("host");

        //test connection
        if (ResolveHost::getIP(aurigaHost).empty()) {
            fatal("cant't resolve host ", aurigaHost);
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
        FileUtil::createDirectory(aurigaRoot + PATH_SEPARATOR + "data" + PATH_SEPARATOR);
        string fileName = dataDir + PATH_SEPARATOR + perftUUID + ".ini";
        pair<string, string> uuids = get.get(aurigaHost, aurigaPort, "php/downloadini.php?uuid_perft=" + perftUUID + "&uuid_task=" + taskUUID, aurigaRoot + PATH_SEPARATOR + "data" + PATH_SEPARATOR);
        if (uuids.first.empty()) {
            warn("no data to fetch");
            return pair<string, string>("", "");
        }
        return uuids;
    }
};



