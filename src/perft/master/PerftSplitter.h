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

#include <iomanip>
#include <atomic>
#include <fstream>
#include <unistd.h>

#include <mutex>

#include "../../blockingThreadPool/ThreadPool.h"

#include <set>
#include "../../lib/WrapperCinnamon.h"
#include "RemoteNode.h"
#include "../../util/IniFile.h"
#include "../node/PerftParser.h"
#include "PerftSplitter.h"
#include "../../network/Client.h"


class PerftSplitter : public Thread, public ThreadPool<RemoteNode>, public Singleton<PerftSplitter> {
    friend class Singleton<PerftSplitter>;

public:


    void setParam(const string &fen1, int depth1, const string &distributedFile, int port);

    void setServer(int port1);

    ~PerftSplitter();

    virtual void run();

    virtual void endRun();


private:

    void receiveMsg(const Message &message);

    std::vector<tuple<string, int, int, string>> nodesSet;

    PerftSplitter() : ThreadPool(1) { };


    int depth;
    string fen;
    int port;

    void callRemoteNode();

    bool serverMode = false;
    Server *server = nullptr;
public:
    Server *getServer() const {
        return server;
    }
};

