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

#include "PerftTreeDao.h"

void PerftTreeDao::readNode() {

    NodeEntity *node = nullptr;
    while (true) {
        pair<string, string> *parameters = iniFile.get();
        if (!parameters)break;
        if (parameters->first == "[node]" || parameters->first == "[perft]") {
            if (node)perftTree.addNodeEntity(node);
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
    if (node)perftTree.addNodeEntity(node);
}

void PerftTreeDao::readPerft() {

    PerftEntity *node = nullptr;
    while (true) {
        pair<string, string> *parameters = iniFile.get();
        if (!parameters)break;
        if (parameters->first == "[perft]" || parameters->first == "[node]") {
            if (node){
                perftTree.setPerftEntity(node);
                break;
            }
            node = new PerftEntity();
        }

        if (node) {
            if (parameters->first == "masterHost") {
                node->setMasterHost(parameters->second);
            } else if (parameters->first == "masterPort") {
                node->setMasterPort(parameters->second);
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
    if (node)perftTree.setPerftEntity(node);
}


void PerftTreeDao::readFile() {
    readPerft();
    readNode();
////std::vector<tuple<string, int, int, string>> PerftSplitter::getRemoteNodes(const string &distributedFile) {
//
//
//    string nodeIp;
//    int nodeNcores;
//    int nodeHash;
//    string nodeDumpfile;
//
//
//    while (true) {
//        pair<string, string> *parameters = iniFile.get();
//        if (!parameters) {
//            break;
//        }
//        if (parameters->first == "[node]") {
//            readNode();
//
//
//            info(nodesSet.size(), "nodes");
//            perftTree = nodesSet;
//        }
//    }
}


