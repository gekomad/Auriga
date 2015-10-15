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

#include <iostream>
#include "util/GetOpt.h"
//#include "engine/Engine.h"
//#include "perft/master/PerftSplitter.h"
#include "perft/shared/PerftTreeDao.h"
#include "engine/Engine.h"

using namespace std;


//TODO usare u128
//TODO usare try catch



int main(int argc, char *argv[]) {
//    PerftTreeDao* a=new PerftTreeDao("/home/geko/workspace/Auriga/src/master.ini");
//    PerftTree perftTree=a->getPerftTree();
//    vector<NodeEntity> nodes = perftTree.getNodesEntity();
//    PerftEntity p = perftTree.getPerftEntity();
//    cout <<nodes.size();

//    Engine e("/home/geko/crafty", Engine::PROTOCOL_TYPE::XBOARD);
//    Engine e("/home/geko/stockfish", Engine::PROTOCOL_TYPE::UCI);
//    Engine e("/home/geko/cinnamon", Engine::PROTOCOL_TYPE::UCI);
////    Engine e("/home/geko/cheng", Engine::PROTOCOL_TYPE::UCI);

//    e.init();
//    for(string fen:nodes[0].getFen()){
//        e.setPosition(fen);
//        e.put("perft "+String(nodes[0].getDepth()));
//    }

//    e.put("quit");
//    e.join();

//    PerftSplitter *  perftSplitter = &PerftSplitter::getInstance();
//    perftSplitter->setServer(5002);
//    perftSplitter->start();
//    perftSplitter->join();
    GetOpt::parse(argc, argv);
    debug ("END AURIGA");
    return 0;
}
