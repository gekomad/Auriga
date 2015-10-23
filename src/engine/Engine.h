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

#include "../threadPool/Thread.h"
#include "../util/IniFile.h"
#include "../namespaces/def.h"
#include "ObserverEngine.h"
#include <unistd.h>
#include <string>
#include <iostream>
#include <regex>
#include <atomic>
#include "../perft/worker/WorkerEntityDao.h"

using namespace std;
using namespace _def;

class Engine : public Thread {
public :
    static enum PROTOCOL_TYPE {
        UCI = 0, XBOARD = 1
    } _PROTOCOL_TYPE;

    Engine();

    ~Engine();

    void put(string);

    virtual void run();

    virtual void endRun();

    void setPosition(const string &fen);

    void init(const string &confFileName);

    void registerObserverEngine(ObserverEngine *obs) {
        observer = obs;
    }

private:

    string regex_heartbeat;
    string receiveOutput;
    string receiveStdErr;
    const u64 NO_RESULT = 0xffffffffffffffff;
    PROTOCOL_TYPE protocol;
    int fd_p2c[2], fd_c2p[2], stdErr[2];
    bool initialized = false;
    mutex putMutex;
    const string SEND_INIT_STRING[2] = {"uci", "ping 1"};
    const string XBOARD_REQUEST_NAME_STRING = "protover 2";
    const string RECEIVE_INIT_STRING[2] = {"uciok", "pong 1"};

    const string POSITION_FEN[2] = {"position fen ", "setboard "};
    std::regex rgxTot;
    std::regex rgxPartial;
    std::regex GET_NAME_REGEX[2];
    u64 result;

    void notifyTotResult(const u64 i, const string &fen,const string& engineName,const int hours);

    void notifyPartialResult(const u64 i, const string &fen,const string& engineName,const int hours);

    void readStdin();

    void readStderr();
    atomic<bool> reading;
    ObserverEngine *observer = nullptr;
    condition_variable cv;
    string engineName = "unknown_engine";
    string fen;
    bool forceRestart;
};


