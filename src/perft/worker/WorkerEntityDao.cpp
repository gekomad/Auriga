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

#include "WorkerEntityDao.h"

WorkerEntityDao::WorkerEntityDao(const string &iniFile1) {
    iniFileName = iniFile1;
    readWorker();
    trace(toINIformat());
}

void WorkerEntityDao::readWorker() {
    if (!FileUtil::fileExists(iniFileName)) {
        warn("file ", iniFileName, " not found");
        return;
    }
    IniFile iniFile(iniFileName);

    pair<string, string> *parameters;
    while (true) {
        parameters = iniFile.get();
        if (!parameters)break;
        if (parameters->first == "host") {
            workerEntity.setHost(parameters->second);
        } else if (parameters->first == "country") {
            workerEntity.setWorkerCountry(parameters->second);
        } else if (parameters->first == "path") {
            workerEntity.setEnginePath(parameters->second);
        } else if (parameters->first == "regex_perft_moves") {
            if (!parameters->second.size()) {
                fatal("regex_perft_moves in ", iniFileName, " is mandatory");
                exit(1);
            }
            if (parameters->second.find("(\\d+)") == string::npos) {
                fatal("regex_perft_moves in ", iniFileName, " missing (\\d+)");
                exit(1);
            }
            if (String(parameters->second).endsWith("(\\d+)")) {
                fatal("regex_perft_moves in ", iniFileName, " ends with (\\d+)");
                exit(1);
            }
            String s = parameters->second;
#ifndef _WIN32
            s = s.replace("\\n", "\\r\\n");
#endif
            workerEntity.setRegex_perft_moves(s);
        } else if (parameters->first == "force_restart") {
            workerEntity.setForce_restart(parameters->second == "true");
        } else if (parameters->first == "instances") {
            workerEntity.setInstances(String::stoi(parameters->second));
        }
    }
    workerEntity.setOptions(readOptions());

}

vector<pair<string, string>> WorkerEntityDao::readOptions() {
    bool opt = false;
    IniFile iniFile(iniFileName);
    vector<pair<string, string>> options;
    pair<string, string> *parameters;
    while (true) {
        parameters = iniFile.get();
        if (!parameters)break;
        if (opt) {
            if (String(parameters->first).trimRight().endsWith("]"))break;
            pair<string, string> a(parameters->first, parameters->second);
            options.push_back(a);
        } else if (parameters->first == "[setoption_name_value]") {
            opt = true;
        }
    }
    return options;
}
