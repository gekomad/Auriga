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

#include "PerftTreeDao.h"

PerftTreeDao::PerftTreeDao(const string &iniFile1) {
    iniFileName = iniFile1;
    readPerft();
    readNode();
    trace(toString());
}

void PerftTreeDao::readNode() {
    IniFile iniFile(iniFileName);
    NodeEntity *node = nullptr;
    while (true) {
        pair<string, string> *parameters = iniFile.get();
        if (!parameters)break;
        if (parameters->first == "[node]") {
            if (node)
                perftTree.addNodeEntity(*node);
            node = new NodeEntity();
        }

        if (node) {
            if (parameters->first == "host") {
                node->setHost(parameters->second);
            } else if (parameters->first == "cpu") {
                node->setCpu(stoi(parameters->second));
            } else if (parameters->first == "nodeUUID") {
                node->setNodeUUID(parameters->second);
            } else if (parameters->first == "port") {
                node->setPort(stoi(parameters->second));
            } else if (parameters->first == "email") {
                node->setEmail(parameters->second);
            } else if (parameters->first == "depth") {
                node->setDepth(stoi(parameters->second));
            } else if (parameters->first == "fen") {
                node->addFen(parameters->second);
            }
        }
    }
    if (node)perftTree.addNodeEntity(*node);
}

void PerftTreeDao::readPerft() {
    IniFile iniFile(iniFileName);
    PerftEntity *node = nullptr;
    while (true) {
        pair<string, string> *parameters = iniFile.get();
        if (!parameters)break;
        if (parameters->first == "[perft]" || parameters->first == "[node]") {
            if (node) {
                perftTree.setPerftEntity(*node);
                break;
            }
            node = new PerftEntity();
        }

        if (node) {
            if (parameters->first == "masterHost") {
                node->setMasterHost(parameters->second);
            } else if (parameters->first == "masterPort") {
                node->setMasterPort(stoi(parameters->second));
            } else if (parameters->first == "uuid") {
                node->setUuid(parameters->second);
            } else if (parameters->first == "email") {
                node->setEmail(parameters->second);
            } else if (parameters->first == "depth") {
                node->setDepth(stoi(parameters->second));
            } else if (parameters->first == "fen") {
                node->setFen(parameters->second);
            }
        }
    }
    if (node)perftTree.setPerftEntity(*node);
}


const NodeEntity *PerftTreeDao::getNodeEntity(const string &nodeUUID) {
    for (int i = 0; i < perftTree.getNodesEntity().size(); i++) {
//    for (NodeEntity p: perftTree.getNodesEntity()) {
//        if (!p.getNodeUUID().compare(nodeUUID)) {
//            return &p;
//        }
        if (!perftTree.getNodesEntity()[i].getNodeUUID().compare(nodeUUID)) {
            return &perftTree.getNodesEntity()[i];
        }
    }
    return nullptr;
}
