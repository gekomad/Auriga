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

#include "NodeEntityDao.h"

NodeEntityDao::NodeEntityDao(const string &iniFile1) {
    iniFileName = iniFile1;
    readNode();
    trace(toINIformat());
}

NodeEntityDao::NodeEntityDao(const vector<NodeEntity>& nodesEntity1) {
    nodesEntity.clear();
    for (NodeEntity n:nodesEntity1)nodesEntity.push_back(n);
}

void NodeEntityDao::readNode() {
    if (!FileUtil::fileExists(iniFileName)) {
        warn("file ", iniFileName, " not found");
        return;
    }
    IniFile iniFile(iniFileName);
    NodeEntity *node = nullptr;
    while (true) {
        pair<string, string> *parameters = iniFile.get();
        if (!parameters)break;
        if (parameters->first == "[node]") {
            if (node)
                nodesEntity.push_back(*node);
            node = new NodeEntity();
        }

        if (node) {
            if (parameters->first == "nodeUUID") {
                node->setNodeUUID(parameters->second);
            }  else if (parameters->first == "depth") {
                node->setDepth(stoi(parameters->second));
            } else if (parameters->first == "fen") {
                node->addFen(parameters->second);
            }
        }
    }
    if (node)nodesEntity.push_back(*node);
}


const NodeEntity *NodeEntityDao::getNodeEntity(const string &nodeUUID) {
    for (int i = 0; i < nodesEntity.size(); i++) {
        if (!nodesEntity[i].getNodeUUID().compare(nodeUUID)) {
            return &nodesEntity[i];
        }
    }
    return nullptr;
}
