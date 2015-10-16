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


#include "Perft.h"

Perft::Perft() {
    perftTreeDao = new PerftTreeDao("/home/geko/workspace/workspace_my/Auriga/src/master.ini");//TODO
}

Perft::~Perft() {
    if (perftTreeDao)delete perftTreeDao;
    perftTreeDao = nullptr;
}

u64 Perft::calculate(const string &nodeUUID) {
    const NodeEntity *nodeEntity = perftTreeDao->getNodeEntity(nodeUUID);

    ThreadPool<Engine> threadPool;
    threadPool.setNthread(nodeEntity->getCpu());
    for (Engine *e :threadPool.threadPool) {
        e->init("/home/geko/workspace/workspace_my/Auriga/src/stockfish.auriga.ini");
    }
    for (string fen:nodeEntity->getFen()) {
        Engine& e=threadPool.getNextThread();
        e.setPosition(fen);
        e.put("perft " + String(nodeEntity->getDepth()));
       // e.put("quit");
        e.start();
    }
    threadPool.joinAll();

    cout <<"axxxxxxxxxxxxxxxxxxxxxxx";
//    Engine e("/home/geko/workspace/workspace_my/Auriga/src/stockfish.auriga.ini");//TODO
//        Engine e("/home/geko/workspace/Auriga/src/cheng.auriga.ini");
//        Engine e("/home/geko/workspace/Auriga/src/crafty.auriga.ini");
//        Engine e("/home/geko/workspace/Auriga/src/cinnamon.auriga.ini");
//    e.init();
//
//    for (string fen:nodeEntity->getFen()) {
//        e.setPosition(fen);
//        e.put("perft " + String(nodeEntity->getDepth()));
//    }
//
//    e.put("quit");
//    e.join();
}