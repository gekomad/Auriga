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

using namespace std;

class NodeEntity {
private:
    string nodeUUID;
    string host;
    int port;
    int cpu;
    //TODO int instance;
    string email;
    vector<string> fen;
    int depth;
public:

    NodeEntity() {
        nodeUUID = UUID::getUUID();
    }

    string toString() const{
        string a;
        a.append("------- NodeEntity -------\n");
        a.append("nodeUUID: " + nodeUUID);
        a.append("\nhost: " + host);
        a.append("\nport: " + String(port));
        a.append("\ncpu: " + String(cpu));
        a.append("\nemail: " + email);
        a.append("\ndepth: " + String(depth));
        for (string f:fen) {
            a.append("\nfen: " + f);
        }
        a.append("\n--------------------------");
        return a;
    }

    void setNodeUUID(const string &nodeUUID) {
        assert(nodeUUID.size() == 36);
        NodeEntity::nodeUUID = nodeUUID;
    }

    const string &getNodeUUID() const {
        return nodeUUID;
    }

    const string &getHost() const {
        return host;
    }

    void addFen(const string &fen1) {
        assert(fen1.size() > 5);
        fen.push_back(fen1);
    }

    void setHost(const string &host) {
        assert(host.size() > 1);
        NodeEntity::host = host;
    }

    int getPort() const {
        return port;
    }

    void setPort(int port) {
        NodeEntity::port = port;
    }

    int getCpu() const {
        return cpu;
    }

    void setCpu(int cpu) {
        assert(cpu > 0);
        NodeEntity::cpu = cpu;
    }

    const string &getEmail() const {
        return email;
    }

    void setEmail(const string &email) {
        NodeEntity::email = email;
    }

    const vector<string> &getFen() const {
        return fen;
    }

    void setFen(const vector<string> &fen) {
        NodeEntity::fen = fen;
    }

    int getDepth() const {
        return depth;
    }

    void setDepth(int depth) {
        assert(depth > 0);
        NodeEntity::depth = depth;
    }
};

