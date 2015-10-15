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
#include "pipe/Engine.h"
#include "perft/master/PerftSplitter.h"
using namespace std;


//TODO usare u128
//TODO usare tru catch



int main(int argc, char *argv[]) {

//    Engine e("/home/geko/crafty", Engine::PROTOCOL_TYPE::XBOARD);
//    Engine e("/home/geko/stockfish", Engine::PROTOCOL_TYPE::UCI);
//    Engine e("/home/geko/cinnamon", Engine::PROTOCOL_TYPE::UCI);
////    Engine e("/home/geko/cheng", Engine::PROTOCOL_TYPE::UCI);

//    e.init();
//    e.setPosition("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
//    e.put("perft 3");
//    e.put("quit");
//    e.join();

    PerftSplitter *  perftSplitter = &PerftSplitter::getInstance();
    perftSplitter->setServer(5002);
    perftSplitter->start();
    perftSplitter->join();
    GetOpt::parse(argc, argv);
    cout << "FINE AURIGA" << endl;
    return 0;
}
