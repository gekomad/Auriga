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

#include "HttpPost.h"

void HttpPost::postThread(const string &host, const int port, const string &personalUUID, const string &uuid_perft, const string &uuid_task, const string &heartbeat, const string &tot, const string &engine, const string &author, const string &fen, const string &minutes, const string &depth) {
    info("Sending data to server host: ", host, " port: ", port, " uuid_perft: ", uuid_perft, " uuid_task: ", uuid_task, " heartbeat: ", heartbeat, " tot: ", tot, " engine: ", engine, " author: ", author, " fen: ", fen, " minutes: ", minutes, " depth: ", depth);
    gc();
    if (!isDelayOK()) {
        info("don't send data to server, minimum time between 2 post is 1 minute");
        return;
    }

    Post *httpClient = new Post();
    if (httpClient->init(host, port)) {
        httpClients.insert(httpClient);
        httpClient->preparePost(personalUUID,uuid_perft, uuid_task, heartbeat, tot, engine, author, fen, minutes, depth);
        httpClient->start();
        httpClient->join();
    } else {
        delete httpClient;
    }

}


bool HttpPost::isDelayOK() {
    auto now = std::chrono::high_resolution_clock::now();
    if (Time::diffTime(now, lastPost) < Time::MINUTE_IN_SECONDS * 1 * 1000) {
        return false;
    }
    lastPost = now;
    return true;
}

void HttpPost::gc() {
    for (auto t : httpClients) {
        if (t->isTerminated()) {
            t->join();
            httpClients.erase(t);
        }
    }
}
