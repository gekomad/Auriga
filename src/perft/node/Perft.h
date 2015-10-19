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

#include "../shared/NodeEntity.h"
#include "../../engine/Engine.h"
#include "../shared/PerftTree.h"
#include "../../blockingThreadPool/ThreadPool.h"
#include "../../engine/ObserverEngine.h"

namespace _perft {
    class Perft : public ObserverEngine {
    public :
        Perft(const string &masterFile, const string &engineConfFile);

        ~Perft() {
            if (perftTreeDao)delete perftTreeDao;
            perftTreeDao = nullptr;
        }

        u64 calculate(const string &nodeUUID);

    private:

        PerftTree *perftTreeDao = nullptr;
        u64 TOT = 0;

        void observerTotResult(const u64 result) {
            TOT += result;
            info("TOT: ", TOT);
        }
        void observerPartialResult(const u64 result) {
            info("partial result: ", result);
        }

        string engineConf;
    };

}