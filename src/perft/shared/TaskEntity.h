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
#include "../../util/UUID.h"
#include "../../util/String.h"
#include "../../namespaces/debug.h"
#include "../../namespaces/def.h"

using namespace std;
using namespace _debug;
using namespace _def;

class TaskEntity {
private:
    string taskUUID;
    vector<string> fen;
    int depth;
public:

    TaskEntity(const vector<string> &fen, int depth) : TaskEntity() {
        setFen(fen);
        setDepth(depth);
    }


    TaskEntity() {
        taskUUID = UUID::getUUID();
    }

    string toINIformat() const {
        string a;
        a.append("[task]");
        a.append("\ntaskUUID=" + taskUUID);
        a.append("\ndepth=" + String(depth));
        for (string f:fen) {
            a.append("\nfen=" + f);
        }
        return a;
    }

    void setTaskUUID(const string &taskUUID) {
        assert(taskUUID.size() == 36);
        TaskEntity::taskUUID = taskUUID;
    }

    const string &getTaskUUID() const {
        return taskUUID;
    }

    void addFen(const string &fen1) {
        assert(fen1.size() > 5);
        fen.push_back(fen1);
    }

    const vector<string> &getFen() const {
        return fen;
    }

    void setFen(const vector<string> &fen) {
        TaskEntity::fen = fen;
    }

    int getDepth() const {
        return depth;
    }

    void setDepth(int depth) {
        assert(depth > 0);
        TaskEntity::depth = depth;
    }
};

