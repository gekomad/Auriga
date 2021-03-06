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

#include "../../util/IniFile.h"
#include "WorkerEntity.h"

using namespace _debug;


class WorkerEntityDao {
public:
    WorkerEntityDao(const string &iniFile1);

    string toINIformat() const {
        return workerEntity.toINIformat();
    }

    WorkerEntity &getWorkerEntity() {
        return workerEntity;
    }

private:
    WorkerEntity workerEntity;

    void readWorker();

    string iniFileName;

    vector<pair<string, string>> readOptions();
};

