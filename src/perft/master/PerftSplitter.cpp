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


#include "PerftSplitter.h"
#include "../../util/UUID.h"
#include "../../network/util/Network.h"
#include "../shared/NodeEntityDao.h"

PerftSplitter::~PerftSplitter() {
    debug("~PerftSplitter()");
}

void PerftSplitter::setServer(int port1) {
    debug("SERVER MODE on port", port1);
    serverMode = true;
    port = port1;
}

void PerftSplitter::setParam(const string &fen1, int depth1, const string &distributedFile, int port1) {
    debug("setParam");
    serverMode = false;
    fen = fen1;
    assert (fen.empty());

    depth = depth1;
    if (depth <= 0)depth = 1;

    port = port1;
//    nodesSet = getRemoteNodes(distributedFile);
}

void PerftSplitter::run() {
    debug("run");

    if (serverMode) {
        server = new Server(port, new PerftParser());//TODO delete PerftParser deleteserver??
        server->start();
        usleep(10000);//wait complete startup

    } else {
        callRemoteNode();
    }
}

void PerftSplitter::endRun() {
    debug("endRun");

}

void PerftSplitter::receiveMsg(const Message &message) {
    info("PerftServer:: receive msg from host: ", message.getHost(), message.getSerializedString());

    if (message.getTot() != 0xffffffffffffffff)info("PerftServer::tot:", message.getTot());

    if (message.getPartial() != 0xffffffffffffffff) info("PerftServer::partial:", message.getPartial());

    if (message.getTot() != 0xffffffffffffffff) {
        for (unsigned i = 0; i < threadPool.size(); i++) {
            if (threadPool.at(i)->getHost() == message.getHost()) {
                threadPool.at(i)->endWork();
                break;
            }
        }
    }
}

void PerftSplitter::callRemoteNode() {
    debug("callRemoteNode");
    WrapperCinnamon wrapperCinnamon;
    assert(nodesSet.size());
    int totMoves = wrapperCinnamon.getTotMoves(fen);

    unsigned totMachine = 0;
    int c = 0;

    for (totMachine = 0; totMachine < nodesSet.size(); totMachine++) {
        c += std::get<1>(nodesSet[totMachine]);
        if (c >= totMoves)break;
    }

    int from = 0;
    int to = 0;
    setNthread(totMachine);
    int block = totMoves / totMachine;
    int lastBlock = totMoves % totMachine;
    for (unsigned i = 0; i < totMachine; i++) {
        RemoteNode &remoteNode = getNextThread();
        to += block;
        if (i == totMachine - 1)to += lastBlock;
        debug(from + " " + to);
        cout << from << " " << to << endl;
        remoteNode.setRemoteNode(port, fen, depth, from, to - 1, nodesSet[i]);
        from = to;
    }
    startAll();
    joinAll();
}

void PerftSplitter::generateMasterINI(const string &nodesFile, const string &fen, const int depth, const string &email, const int port) {
    string res = "#auriga ini file - AUTO-GENERATED FILE - DO NOT EDIT\n[perft]\n\n";
    res.append("uuid=").append(UUID::getUUID()).append("\n");
    res.append("fen=").append(fen).append("\n");
    res.append("depth=").append(String(depth)).append("\n");
    res.append("email=").append(email).append("\n");
    res.append("masterPort=").append(String(port)).append("\n");
    res.append("masterHost=").append(Network::getIp()).append("\n\n");


    NodeEntityDao nodeEntityDao(nodesFile);
    int cpu=0;
    for(NodeEntity n:nodeEntityDao.nodesEntity){
        cpu+=n.getCpu();
    }
    WrapperCinnamon wrapperCinnamon;
    vector<string> totMoves = wrapperCinnamon.getTotMoves(fen);

    cout <<"tot cpu: "<<cpu<<endl;
    cout <<"totMoves: "<<totMoves<<endl;
    cout << nodesFile << endl;
}
