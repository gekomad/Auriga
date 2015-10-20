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

#include "PerftEntityDao.h"

PerftEntityDao::PerftEntityDao(const string &iniFile1) {
    iniFileName = iniFile1;
    readPerft();

    trace(toINIformat());
}

void PerftEntityDao::readPerft() {
    IniFile iniFile(iniFileName);
    PerftEntity *task = nullptr;
    while (true) {
        pair<string, string> *parameters = iniFile.get();
        if (!parameters)break;
        if (parameters->first == "[perft]" || parameters->first == "[task]") {
            if (task) {
                perftEntity = task;
                break;
            }
            task = new PerftEntity();
        }

        if (task) {
           if (parameters->first == "uuid") {
               task->setUuid(parameters->second);
            } else if (parameters->first == "depth") {
               task->setDepth(stoi(parameters->second));
            } else if (parameters->first == "fen") {
               task->setFen(parameters->second);
            }
        }
    }
    if (task)perftEntity = task;
}


