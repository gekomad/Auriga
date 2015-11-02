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

#include <iostream>
#include <vector>
#include "../../util/String.h"
#include "../../namespaces/debug.h"
#include "../../namespaces/def.h"

using namespace std;
using namespace _debug;
using namespace _def;

class WorkerEntity {
private:
    string host;
    vector<pair<string, string>> options;
    string workerName;

    string workerCountry;
    string enginePath;
    string regex_perft_moves;
    bool force_restart;
    int instances;
public:

    string toINIformat() const {
        string a;
        a.append("[auriga_server]");
        a.append("\nhost=" + host);

        a.append("[worker]");
        a.append("\nname=" + workerName);
        a.append("\ncountry=" + workerCountry);

        a.append("[engine]");
        a.append("\npath=" + enginePath);
        a.append("\nregex_perft_moves=" + regex_perft_moves);
        string s = force_restart ? "true" : "false";
        a.append("\nforce_restart=" + s);
        a.append("\ninstances=" + to_string(instances));

        a.append("[setoption_name_value]");
        for (pair<string, string> o:options) {
            a.append("\n").append(o.first + "=" + o.second);
        }
        return a;
    }


    const string &getHost() const {
        return host;
    }

    void setHost(const string &host) {
        WorkerEntity::host = host;
    }

    const vector<pair<string, string>> &getOptions() const {
        return options;
    }

    void setOptions(const vector<pair<string, string>> &options) {
        WorkerEntity::options = options;
    }

    const string &getWorkerName() const {
        return workerName;
    }

    void setWorkerName(const string &workerName) {
        WorkerEntity::workerName = workerName;
    }

    const string &getWorkerCountry() const {
        return workerCountry;
    }

    void setWorkerCountry(const string &workerCountry) {
        WorkerEntity::workerCountry = workerCountry;
    }

    const string &getEnginePath() const {
        return enginePath;
    }

    void setEnginePath(const string &enginePath) {
        WorkerEntity::enginePath = enginePath;
    }

    const string &getRegex_perft_moves() const {
        return regex_perft_moves;
    }

    void setRegex_perft_moves(const string &regex_perft_moves) {
        WorkerEntity::regex_perft_moves = regex_perft_moves;
    }

    bool isForce_restart() const {
        return force_restart;
    }

    void setForce_restart(bool force_restart) {
        WorkerEntity::force_restart = force_restart;
    }

    bool getForce_restart() {
        return force_restart;
    }

    int getInstances() const {
        return instances;
    }

    void setInstances(int instances) {
        WorkerEntity::instances = instances;
    }


};

