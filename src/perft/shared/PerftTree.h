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

#include "../../namespaces/def.h"
#include "PerftEntityDao.h"
#include "../task/TaskEntityDao.h"
#include "../../util/IniFile.h"

using namespace _debug;
using namespace _def;

class PerftTree {
public:
    PerftTree(const string &iniFile1);

    const TaskEntity *getTaskEntity(const string &taskUUID) {
        return taskEntityDao->getTaskEntity(taskUUID);
    }

    string toINIformat() const {
        string a = perftEntityDao->toINIformat();
        a.append(taskEntityDao->toINIformat());
        return a;
    }

    ~PerftTree() {
        if (perftEntityDao)delete perftEntityDao;
        if (taskEntityDao)delete taskEntityDao;
    }


    const PerftEntity *getPerftEntity() const {
        return perftEntityDao->getPerftEntity();
    }

private:

    PerftEntityDao *perftEntityDao = nullptr;
    TaskEntityDao *taskEntityDao = nullptr;

    string iniFileName;
};

