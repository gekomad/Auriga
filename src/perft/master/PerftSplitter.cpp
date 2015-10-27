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

#include "PerftSplitter.h"

void PerftSplitter::generateMasterINI(const string &fen, const unsigned Ntask, const int depth, const string &iniDir) {
    assert(depth > 1);
    string res = "#auriga ini file - AUTO-GENERATED FILE - DO NOT EDIT\n\n";

    PerftEntity perftEntity(fen, depth);
    res.append(perftEntity.toINIformat());

    vector<TaskEntity> taskEntity;

    WrapperCinnamon wrapperCinnamon;

    vector<string> successorsFen;// = wrapperCinnamon.getSuccessorsFen(fen, 1);

    int effectiveNtask = Ntask;
    int succDepth = 0;
    while (true) {
        successorsFen = wrapperCinnamon.getSuccessorsFen(fen, ++succDepth);
        if (successorsFen.size() >= Ntask)break;
        if (succDepth > depth - 2) {
            effectiveNtask = std::min((unsigned) successorsFen.size(), Ntask);
            break;
        }
    }
    info("Fen: ", fen);
    info("depth: ", depth);
    info("tot Ntask: ", Ntask);
    info("tot effectiveNtask: ", effectiveNtask);
    info("tot Fen: ", successorsFen.size());
    taskEntity.reserve(effectiveNtask);
    for (int i = 0; i < effectiveNtask; i++) {

        TaskEntity task;
        task.setDepth(depth - succDepth);
        taskEntity.push_back(task);
    }
    int c = 0;
    for (unsigned i = 0; i < successorsFen.size(); i++) {
        taskEntity.at((c++) % effectiveNtask).addFen(successorsFen[i]);
    }

    TaskEntityDao taskEntityDao(taskEntity);
    res.append(taskEntityDao.toINIformat());
    string perftUUID = perftEntity.getUuid();
    FileUtil::createDirectory(iniDir + "/" + perftUUID);
    string filename = iniDir + "/" + perftUUID + "/" + perftUUID + ".ini";

    trace("\nwrite file", filename, "\n-------------\n", res, "\n-------------------");
    ofstream myfile;
    myfile.open(filename);
    myfile << res;
    myfile.close();
    //CSV file for table perft
    myfile.open(perftUUID+"/perft_table.tmp");
    myfile << perftEntity.getUuid() + "|" + fen + "|" + to_string(depth) + "|" + to_string(Ntask) + "\n";
    myfile.close();

}
