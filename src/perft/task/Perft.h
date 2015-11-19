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

#include "../task/TaskEntity.h"
#include "../../engine/Engine.h"
#include "../shared/PerftTree.h"
#include "../../threadPool/ThreadPool.h"
#include "../../engine/ObserverEngine.h"
#include "HttpPost.h"
#include "../../pipe/PipePosix.h"
#include "../../pipe/PipeWindows.h"

namespace _perft {
    class Perft : public ObserverEngine {
    public :
        Perft(const string& aurigaRoot1,const string &taskUUID, const string &masterFile, const string &engineConfFile);

        i128 calculate();

    private:
        static const int HEARTBEAT =           0b1;
        static const int OS_WIN =             0b10;
        static const int OS_APPLE =          0b100;
        static const int OS_LINUX =         0b1000;
        static const int OS_RASPBERRY =    0b10000;
        static const int OS_ODROID =      0b100000;
        static const int OS_UNIX =       0b1000000;

        string taskUUID;
        string masterFile;
        string aurigaRoot;
        i128 TOT = 0;
        string workerIniFile;
        string perftUUID;

        string author;

        void observerTotResult(const u64 result, const string &fen, const string &engineName, const int minutes, const int depth);

        void observerHeartbeat(const string &fen, const string &engineName, const int minutes, const int depth);

        int getOStype();

        mutex getResultMutex;
        string aurigaHost;
        int aurigaPort;
    };

}