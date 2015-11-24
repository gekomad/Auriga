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


    int effectiveNtask = Ntask;
    int succDepth = 0;
    map<string, int> successorsFenMap;
    {
        vector<string> successorsFenVector;
        while (true) {
            successorsFenVector = wrapperCinnamon.getSuccessorsFen(fen, ++succDepth);
            if (successorsFenVector.size() >= Ntask)break;
            if (succDepth > depth - 2) {
                effectiveNtask = std::min((unsigned) successorsFenVector.size(), Ntask);
                break;
            }
        }
        for (string fen:successorsFenVector) {
            successorsFenMap[fen]++;
        }
    }

    info("Fen: ", fen);
    info("depth: ", depth);
    info("tot Ntask: ", Ntask);
    info("tot effectiveNtask: ", effectiveNtask);
    info("tot Fen: ", successorsFenMap.size());
    taskEntityList.reserve(effectiveNtask);
    for (int i = 0; i < effectiveNtask; i++) {
        TaskEntity task;
        task.setDepth(depth - succDepth);
        taskEntityList.push_back(task);
    }
    int c = 0;
    for (pair<string, int> f :successorsFenMap) {
        //equal fens in single task
        for (int i = 0; i < f.second; i++) {
            taskEntityList.at(c % effectiveNtask).addFen(f.first);
        }
        c++;
    }

    TaskEntityDao taskEntityDao(taskEntityList);
    res.append(taskEntityDao.toINIformat());
    string perftUUID = perftEntity.getUuid();

    FileUtil::createDirectory(aurigaRoot + PATH_SEPARATOR + "data");
    FileUtil::createDirectory(aurigaRoot + PATH_SEPARATOR + "data" + PATH_SEPARATOR + perftUUID);
    string filename = aurigaRoot + PATH_SEPARATOR + "data" + PATH_SEPARATOR + perftUUID + PATH_SEPARATOR + perftUUID + ".ini.gz";
    res.append("\n#END FILE");
    trace("\nwrite file", filename, "\n-------------\n", res, "\n-------------------");

    Compression compression;

    compression.compress(res, filename);
    info("Generated file " + filename);
    //CSV file for table perft
    ofstream myfile;
    myfile.open(aurigaRoot + PATH_SEPARATOR + "data" + PATH_SEPARATOR + perftUUID + PATH_SEPARATOR + "perft.txt");
    myfile << perftEntity.getUuid() + "|" + fen + "|" + to_string(depth) + "|" + to_string(Ntask) + "\n";
    myfile.close();
    //CSV file for table perft_tasks
    myfile.open(aurigaRoot + PATH_SEPARATOR + "data" + PATH_SEPARATOR + perftUUID + PATH_SEPARATOR + "perft_tasks.txt");
    for (TaskEntity taskEntity :taskEntityList) {
        myfile << perftEntity.getUuid() << "|" << taskEntity.getTaskUUID() << "|" << taskEntity.getFenList().size() << "\n";
    }
    myfile.close();

    //CSV file for table task_fens
    myfile.open(aurigaRoot + PATH_SEPARATOR + "data" + PATH_SEPARATOR + perftUUID + PATH_SEPARATOR + "task_fens.txt");
    for (TaskEntity taskEntity:taskEntityDao.getTaskEntity()) {
        for (String fen:taskEntity.getFenList()) {
            myfile << taskEntity.getTaskUUID() << "|" << fen << "|" << taskEntity.getDepth() << "\n";
        }
    }
    myfile.close();
    return perftEntity.getUuid();
}
