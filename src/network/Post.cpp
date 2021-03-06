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

#include "Post.h"
#include "ResolveHost.h"

bool Post::init(const string &host1, const int port1) {
    if (host1.empty()) {
        return false;
    }
    host = host1;
    port = port1;
    debug("resolving host ", host);
    ip = ResolveHost::getIP(host);
    if (ip.empty()) {
        error("unknow host ", host);
        return false;
    }
    debug("resolved host ", host, "->", ip);
    return true;
}

void Post::preparePost(const string &personalUUID, const string &uuid_perft, const string &uuid_task, const string &heartbeat, const string &tot, const string &engine, const string &author, const string &fen, const string &minutes, const string &depth) {

    info("send data ", personalUUID, " ", uuid_perft, " ", uuid_task, " ", heartbeat, " ", tot, " ", engine, " ", author, " ", fen);

    std::ostringstream formBuffer;

    string dataType0 = "personal_uuid=";
    string dataType1 = "&uuid_perft=";
    string dataType2 = "&uuid_task=";
    string dataType3 = "&heartbeat=";

    string dataType4 = "&tot=";
    string dataType5 = "&engine=";
    string dataType6 = "&author=";
    string dataType7 = "&fen=";
    string dataType8 = "&minutes=";
    string dataType9 = "&depth=";

    string FormAction = string("https://").append(ip).append(to_string(port)).append("/insert_task.php");

    auto ContentLength = personalUUID.size() + uuid_perft.size() + uuid_task.size() + heartbeat.size() + tot.size() + engine.size() + author.size() + fen.size() + minutes.size() + depth.size() + dataType0.size() + dataType1.size() + dataType2.size() + dataType3.size() + dataType4.size() + dataType5.size() + dataType6.size() + dataType7.size() + dataType8.size() + dataType9.size();

    // header
    formBuffer << "POST " << FormAction.c_str() << " HTTP/1.1\n";
    formBuffer << "Content-Type: application/x-www-form-urlencoded\n";
    formBuffer << "Host: " << host << "\n";
    formBuffer << "Content-Length: " << std::to_string(ContentLength) << "\n\n";

    // actual content
    formBuffer << dataType0 << personalUUID;
    formBuffer << dataType1 << uuid_perft;
    formBuffer << dataType2 << uuid_task;
    formBuffer << dataType3 << heartbeat;
    formBuffer << dataType4 << tot;
    formBuffer << dataType5 << engine;
    formBuffer << dataType6 << author;
    formBuffer << dataType7 << fen;
    formBuffer << dataType8 << minutes;
    formBuffer << dataType9 << depth;
    str = formBuffer.str();
}

void Post::run() {

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

void Post::endRun() {
    terminated = true;
    debug("Post::endRun")
}
