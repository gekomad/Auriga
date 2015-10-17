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

Perft::Perft(const string& masterFile,const string& engineConfFile) {
    assert(FileUtil::fileExists(masterFile));
    assert(FileUtil::fileExists(engineConfFile));
    perftTreeDao = new PerftTree(masterFile);
    engineConf=engineConfFile;
}

u64 Perft::calculate(const string &nodeUUID) {
    const NodeEntity *nodeEntity = perftTreeDao->getNodeEntity(nodeUUID);

    ThreadPool<Engine> threadPool;
    threadPool.setNthread(nodeEntity->getCpu());


    for (string fen:nodeEntity->getFen()) {
        Engine &e = threadPool.getNextThread();
        e.init(engineConf);
        e.registerObserverEngine(this);
        e.start();
        e.setPosition(fen);
        e.put("perft " + String(nodeEntity->getDepth()));
    }
    threadPool.joinAll();

    cout << "TOT: " << TOT << endl;

}