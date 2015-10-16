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
#include "../../network/Server.h"
#include "../../util/IniFile.h"
#include "NodeEntity.h"

using namespace _debug;
using namespace _def;

class NodeEntityDao {
public:
    NodeEntityDao(const string &iniFile1);


    const NodeEntity *getNodeEntity(const string &nodeUUID);

    string toString() const {
        string a;
        for (NodeEntity n:nodesEntity) {
            a.append(n.toString());
        }
        return a;
    }

    vector<NodeEntity> nodesEntity;//TODO implementare iterator
private:

    void readNode();


    string iniFileName;
};

