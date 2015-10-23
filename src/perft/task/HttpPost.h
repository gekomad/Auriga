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

using namespace std;
using namespace _def;
using namespace _logger;

class LastPost : public Singleton<LastPost> {
    friend class Singleton<LastPost>;

public:
    bool isDelayOK() {
        auto now = std::chrono::high_resolution_clock::now();
        if (Time::diffTime(now,lastPost) < Time::HOUR_IN_SECONDS * 1000) {
            return false;
        }
        lastPost = now;
        return true;
    }

private:
    high_resolution_clock::time_point lastPost = std::chrono::high_resolution_clock::now();
};

class HttpPost {
public:
    static void postThread(const string &host, const int port, const string &uuid_perft, const string &uuid_task, const string &partial_moves, const string &tot, const string &engine, const string &author, const string &fen) {

        if (!LastPost::getInstance().isDelayOK()) {
            info("don't send data to server, minimum time between 2 post is one hour");
            return;
        }
        Client *httpClient = new Client();//TODO delete?
        httpClient->init(host, port);
        httpClient->preparePost(uuid_perft, uuid_task, partial_moves, tot, engine, author, fen);
        httpClient->start();
        httpClient->detach();
    }

};
