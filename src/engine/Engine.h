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

#include "../blockingThreadPool/Thread.h"
#include "../util/IniFile.h"
#include "../namespaces/def.h"
#include <unistd.h>
#include <string>
#include <iostream>
#include <regex>
using namespace std;
using namespace _def;
class Engine : public Thread {
public :
    static enum PROTOCOL_TYPE {
        UCI = 0, XBOARD = 1
    } _PROTOCOL_TYPE;

    Engine(const string &confFileName);

    void put(string);

    virtual void run();

    virtual void endRun();

    void setPosition(const string &fen);

    void init();

private:
    int uci_option_perft_thread_value=0;
    int uci_option_perft_hash_value=0;
    string uci_option_perft_thread_name;
    string regex_perft_moves;
    string uci_option_perft_hash_name;
    string receiveOutput;
    string receiveStdErr;
    string programPath;

    PROTOCOL_TYPE protocol;
    int fd_p2c[2], fd_c2p[2],stdErr[2];
    bool initialize;
    mutex putMutex;
    const string SEND_INIT_STRING[2] = {"uci", "ping 1"};
    const string RECEIVE_INIT_STRING[2] = {"uciok", "pong 1"};
    const string POSITION_FEN[2] = {"position fen ", "setboard "};
    std::regex rgx;
};


