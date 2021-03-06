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

#include <iomanip>
#include <atomic>
#include <fstream>
#include <map>
#include <unistd.h>
#include <set>
#include "../../lib/WrapperCinnamon.h"
#include "../../util/IniFile.h"
#include "../../util/FileUtil.h"
#include "../shared/PerftEntity.h"
#include "../task/TaskEntityDao.h"
#include "../../util/Compression.h"

class PerftSplitter {

public:

    static string generateMasterINI(const string &fen, const unsigned Ntask, const int depth, const string &iniDir);
};

