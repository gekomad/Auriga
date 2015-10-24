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

#include "../../network/Client.h"
#include "../../util/Time.h"
#include <set>

using namespace std;
using namespace _def;

class HttpPost : public Singleton<HttpPost> {
    friend class Singleton<HttpPost>;

public:
    void postThread(const string &host, const int port, const string &uuid_perft, const string &uuid_task, const string &partial_moves, const string &tot, const string &engine, const string &author, const string &fen, const string& hours);

private:
    high_resolution_clock::time_point lastPost = std::chrono::high_resolution_clock::now();
    std::set<Client *> httpClients;

    bool isDelayOK();

    void gc();
};
