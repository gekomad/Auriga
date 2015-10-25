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

void HttpPost::postThread(const string &host, const int port, const string &uuid_perft, const string &uuid_task, const string &partial_moves, const string &tot, const string &engine, const string &author, const string &fen, const string &hours) {
    gc();
    if (!isDelayOK()) {
        info("don't send data to server, minimum time between 2 post is one hour");
        return;
    }
    info("Sending data to server ",host," ",port );
    Client *httpClient = new Client();
    httpClients.insert(httpClient);
    httpClient->init(host, port);
    httpClient->preparePost(uuid_perft, uuid_task, partial_moves, tot, engine, author, fen, hours);
    httpClient->start();
    httpClient->join();
//TODO    httpClient->detach();
}


bool HttpPost::isDelayOK() {
    auto now = std::chrono::high_resolution_clock::now();
    if (Time::diffTime(now, lastPost) < Time::HOUR_IN_SECONDS * 1000) {
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
