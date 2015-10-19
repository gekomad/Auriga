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

#include "Perft.h"

using namespace _perft;

Perft::Perft(const string &nodeUUID1, const string &masterFile1, const string &engineConfFile) {
    if (!FileUtil::fileExists(masterFile1)) {
        error("File not found ", masterFile1);
        exit(1);
    }
    if (!FileUtil::fileExists(engineConfFile)) {
        error("File not found ", engineConfFile);
        exit(1);
    }
    if (nodeUUID1.size() != 36) {
        error("nodeUUID malformed ", nodeUUID1);
        exit(1);
    }
    engineConf = engineConfFile;
    masterFile = masterFile1;
    nodeUUID = nodeUUID1;
}

u64 Perft::calculate() {
    auto start1 = std::chrono::high_resolution_clock::now();

    ThreadPool<Engine> threadPool;
    IniFile iniFile(engineConf);
    int threads = String::stoi(iniFile.getValue("uci_option_perft_thread_value"));
    if (!threads) {
        int ins = String::stoi(iniFile.getValue("n_instances"));
        threadPool.setNthread(ins == 0 ? 1 : ins);
    } else {
        threadPool.setNthread(1);
    }

    PerftTree perftTreeDao(masterFile);
    const NodeEntity *nodeEntity = perftTreeDao.getNodeEntity(nodeUUID);
    if (!nodeEntity) {
        error("nodeUUID ", nodeUUID, " not found");
        exit(1);
    }

    for (string fen:nodeEntity->getFen()) {
        Engine &e = threadPool.getNextThread();
        e.init(engineConf);
        e.registerObserverEngine(this);
        e.start();
        e.setPosition(fen);
        e.put("perft " + String(nodeEntity->getDepth()));
    }
    threadPool.joinAll();
    auto stop1 = std::chrono::high_resolution_clock::now();

    string timetot = Time::diffTimeToString(start1, stop1);

    cout << "Tot Perft moves for NodeUUID " << nodeUUID << " :" << TOT << " in " << timetot << endl;

}