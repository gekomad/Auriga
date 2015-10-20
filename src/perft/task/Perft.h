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

#include "../shared/TaskEntity.h"
#include "../../engine/Engine.h"
#include "../shared/PerftTree.h"
#include "../../blockingThreadPool/ThreadPool.h"
#include "../../engine/ObserverEngine.h"
#include "../../network/Client.h"

namespace _perft {
    class Perft : public ObserverEngine {
    public :
        Perft(const string &taskUUID, const string &masterFile, const string &engineConfFile);

        __int128_t calculate();

    private:
        string taskUUID;
        string masterFile;
        __int128_t TOT = 0;
        string engineConf;

        void observerTotResult(const u64 result) {
            TOT += result;
            info("TOT: ", String::toString(TOT));

            Client::getInstance().post(perftUUID, taskUUID, "0", String::toString(TOT), engineName, author,"fen567");
        }

        void observerPartialResult(const u64 result) {
            info("partial result: ", result);
            Client::getInstance().post(perftUUID, taskUUID, to_string(result), "0", engineName, author,"fen123");
        }

        string perftUUID;
        string engineName;
        string author;
    };

}