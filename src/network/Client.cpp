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

#include "Client.h"
#include "ResolveHost.h"

// HttpPost::getInstance().postThread("auriga-cinnamon.rhcloud.com", 80, "92165e61-7afc-11e5-ab63-0ec7b1e84563", "task_uuid", "2", "3", "engineName", "author", "fen","1");

void Client::init(const string &host1, const int port1) {
    host = host1;
    port = port1;
    debug("resolving host ", host);
    ip = ResolveHost::getIP(host);
    if (!ip.size()) {
        fatal("unknow host");
        exit(1);
    }
    debug("resolved host ", host, "->", ip);
}

void Client::preparePost(const string &uuid_perft, const string &uuid_task, const string &partial_moves, const string &tot, const string &engine, const string &author, const string &fen, const string &hours) {

    info("send data ", uuid_perft, " ", uuid_task, " ", partial_moves, " ", tot, " ", engine, " ", author, " ", fen);

    std::ostringstream formBuffer;

    char dataType1[] = "uuid_perft=";
    char dataType2[] = "&uuid_task=";
    char dataType3[] = "&partial_moves=";

    char dataType4[] = "&tot=";
    char dataType5[] = "&engine=";
    char dataType6[] = "&author=";
    char dataType7[] = "&fen=";
    char dataType8[] = "&hours=";

    string FormAction = string("https://").append(ip).append(to_string(port)).append("/insert_task.php");
//    string FormAction = "http://127.0.0.1/auriga/insert_task.php";
    auto ContentLength = uuid_perft.size() + uuid_task.size() + partial_moves.size() + tot.size() + engine.size() + author.size() + fen.size() + hours.size() + strlen(dataType1) + strlen(dataType2) + strlen(dataType3) + strlen(dataType4) + strlen(dataType5) + strlen(dataType6) + strlen(dataType7) + strlen(dataType8);


    // header
    formBuffer << "POST " << FormAction.c_str() << " HTTP/1.1\n";
    formBuffer << "Content-Type: application/x-www-form-urlencoded\n";
    formBuffer << "Host: " << host << "\n";
    formBuffer << "Content-Length: " << std::to_string(ContentLength) << "\n\n";

    // actual content
    formBuffer << dataType1 << uuid_perft;
    formBuffer << dataType2 << uuid_task;
    formBuffer << dataType3 << partial_moves;
    formBuffer << dataType4 << tot;
    formBuffer << dataType5 << engine;
    formBuffer << dataType6 << author;
    formBuffer << dataType7 << fen;
    formBuffer << dataType8 << hours;
    str = formBuffer.str();

}

void Client::run() {

    sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock != -1);
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    debug(str);
    assert(connect(sock, (struct sockaddr *) &server, sizeof(server)) >= 0);
    assert(send(sock, str.data(), str.size(), 0) == (int) str.size());
    debug ("ok data sent");
}

void Client::endRun() {
    terminated = true;
    debug("Client::endRun")
}
