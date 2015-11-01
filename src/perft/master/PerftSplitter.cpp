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

string PerftSplitter::generateMasterINI(const string &fen, const unsigned Ntask, const int depth, const string &aurigaRoot) {
    assert(depth > 1);
    string res = "#auriga ini file - AUTO-GENERATED FILE - DO NOT EDIT\n\n";

    PerftEntity perftEntity(fen, depth);
    res.append(perftEntity.toINIformat());

    vector<TaskEntity> taskEntityList;

    WrapperCinnamon wrapperCinnamon;

    vector<string> successorsFen;

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
    taskEntityList.reserve(effectiveNtask);
    for (int i = 0; i < effectiveNtask; i++) {

        TaskEntity task;
        task.setDepth(depth - succDepth);
        taskEntityList.push_back(task);
    }
    int c = 0;
    for (unsigned i = 0; i < successorsFen.size(); i++) {
        taskEntityList.at((c++) % effectiveNtask).addFen(successorsFen[i]);
    }

    TaskEntityDao taskEntityDao(taskEntityList);
    res.append(taskEntityDao.toINIformat());
    string perftUUID = perftEntity.getUuid();

    FileUtil::createDirectory(aurigaRoot + "/data");
    FileUtil::createDirectory(aurigaRoot + "/data/" + perftUUID);
    string filename = aurigaRoot + "/data/" + perftUUID + "/" + perftUUID + ".ini";

    trace("\nwrite file", filename, "\n-------------\n", res, "\n-------------------");
    ofstream myfile;
    myfile.open(filename);
    myfile << res;
    myfile.close();
    info("Generated file " + filename);
    //CSV file for table perft
    myfile.open(aurigaRoot + "/data/" + perftUUID + "/perft.txt");
    myfile << perftEntity.getUuid() + "|" + fen + "|" + to_string(depth) + "|" + to_string(Ntask) + "\n";
    myfile.close();
    //CSV file for table perft_tasks
    myfile.open(aurigaRoot + "/data/" + perftUUID + "/perft_tasks.txt");
    for (TaskEntity taskEntity :taskEntityList) {
        myfile << perftEntity.getUuid() << "|" << taskEntity.getTaskUUID() << "|" << taskEntity.getFenList().size() << "\n";
    }
    myfile.close();

    //CSV file for table task_fens
    myfile.open(aurigaRoot + "/data/" + perftUUID + "/task_fens.txt");
    for (TaskEntity taskEntity:taskEntityDao.getTaskEntity()) {
        for (String fen:taskEntity.getFenList()) {
            myfile << taskEntity.getTaskUUID() << "|" << fen << "|" << taskEntity.getDepth() << "\n";
        }
    }
    myfile.close();
    return perftEntity.getUuid();
}
