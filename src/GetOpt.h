/*
    Cinnamon UCI chess engine
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

#include <iostream>
#include "util/FileUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "GetOptMaster.h"
#include "GetOptTask.h"
#include <cstring>

using namespace std;

class GetOpt {

public:
    static void help(char **argv) {
        string exe = FileUtil::getFileName(argv[0]);
        GetOptTask::help(argv);
        GetOptMaster::help(argv);
    }

    static void parse(int argc, char **argv) {

        if (argc > 1 && !strcmp(argv[1], "--task")) {
            GetOptTask::parse(argc, argv);
            return;
        }
        if (argc > 1 && !strcmp(argv[1], "--generate-ini")) {
            GetOptMaster::parse(argc, argv);
            return;
        }
        help(argv);
    }
};



