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

using namespace std;

#include "pipe/Engine.h"

int main(int argc, const char *argv[]) {

//    Engine e("/home/geko/crafty", Engine::PROTOCOL_TYPE::XBOARD);
    Engine e("/home/geko/stockfish", Engine::PROTOCOL_TYPE::UCI);
//    Engine e("/home/geko/cinnamon", Engine::PROTOCOL_TYPE::UCI);
//    Engine e("/home/geko/cheng", Engine::PROTOCOL_TYPE::UCI);

    e.start();
    e.setPosition("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    e.put("perft 5");
    e.put("quit");
    e.join();
    cout << "FINE AURIGA" << endl;
    return 0;
}
