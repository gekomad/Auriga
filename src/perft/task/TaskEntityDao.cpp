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

#include "TaskEntityDao.h"

TaskEntityDao::TaskEntityDao(const string &iniFile1) {
    iniFileName = iniFile1;
    readTask();
    trace(toINIformat());
}

TaskEntityDao::TaskEntityDao(const vector<TaskEntity> &taskEntity1) {
    taskEntity.clear();
    for (TaskEntity n:taskEntity1)taskEntity.push_back(n);
}

void TaskEntityDao::readTask() {
    if (!FileUtil::fileExists(iniFileName)) {
        warn("file ", iniFileName, " not found");
        return;
    }
    IniFile iniFile(iniFileName);
    TaskEntity *task = nullptr;
    while (true) {
        pair<string, string> *parameters = iniFile.get();
        if (!parameters)break;
        if (parameters->first == "[task]") {
            if (task)
                taskEntity.push_back(*task);
            task = new TaskEntity();
        }

        if (task) {
            if (parameters->first == "task_uuid") {
                task->setTaskUUID(parameters->second);
            } else if (parameters->first == "depth") {
                task->setDepth(stoi(parameters->second));
            } else if (parameters->first == "fen") {
                task->addFen(parameters->second);
            }
        }
    }
    if (task)taskEntity.push_back(*task);
}


const TaskEntity *TaskEntityDao::getTaskEntity(const string &taskUUID) {
    for (unsigned i = 0; i < taskEntity.size(); i++) {
        if (!taskEntity[i].getTaskUUID().compare(taskUUID)) {
            return &taskEntity[i];
        }
    }
    return nullptr;
}
