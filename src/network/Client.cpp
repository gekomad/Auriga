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

void Client::post(const string &uuid_perft, const string &uuid_task, const string &partial_moves, const string &tot, const string &engine, const string &author) {
    info("send data ", uuid_perft, " ", uuid_task, " ", partial_moves, " ", tot, " ", engine, " ", author);
    string host = "127.0.0.1";
    int portno = 80;

    struct sockaddr_in server;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock != -1);

    server.sin_addr.s_addr = inet_addr(host.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(portno);

    assert(connect(sock, (struct sockaddr *) &server, sizeof(server)) >= 0);
    std::ostringstream formBuffer; // <<< here

    char dataType1[] = "uuid_perft=";
    char dataType2[] = "&uuid_task=";
    char dataType3[] = "&partial_moves=";

    char dataType4[] = "&tot=";
    char dataType5[] = "&engine=";
    char dataType6[] = "&author=";

    char FormAction[] = "http://localhost/insert_task.php";

    // get: length of the actual content
    auto ContentLength = uuid_perft.size() + uuid_task.size() + partial_moves.size() + tot.size() + engine.size() + author.size() + strlen(dataType1) + strlen(dataType2) + strlen(dataType3) + strlen(dataType4) + strlen(dataType5) + strlen(dataType6);

    // header
    formBuffer << "POST " << FormAction << " HTTP/1.1\n";
    formBuffer << "Content-Type: application/x-www-form-urlencoded\n";
    formBuffer << "Host: \n";
    formBuffer << "Content-Length: " << std::to_string(ContentLength) << "\n\n";

    // actual content
    formBuffer << dataType1 << uuid_perft;
    formBuffer << dataType2 << uuid_task;
    formBuffer << dataType3 << partial_moves;
    formBuffer << dataType4 << tot;
    formBuffer << dataType5 << engine;
    formBuffer << dataType6 << author;
    const auto str = formBuffer.str();
    std::cout << str << std::endl;
    assert(send(sock, str.data(), str.size(), 0) == (int) str.size());
}

