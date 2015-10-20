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
#include "network/Client.h"
#include "namespaces/def.h"
//TODO usare u128
//TODO usare try catch



int main(int argc, char *argv[]) {
    const string uuid_perft = "DCA3EE5C-72B0-C793-FC11-57987B00A327";
    const string uuid_task = "aaaaaaaaaaaaaaaaaa";
    const string partial_moves = "2";
    const string tot = "0";
    const string engine = "cinnamon";
    const string author = "pippo";

    Client::getInstance().post(uuid_perft,uuid_task,partial_moves,tot,engine,author);
    return 0;

    cout << NAME << endl;
    GetOpt::parse(argc, argv);
    return 0;
}
