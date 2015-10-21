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

#include "Random.h"
#include <iostream>

using namespace std;

class UUID {

public:

    static string getUUID() {
        char uuid1[37];
        char *uuid = uuid1;

        for (int i = 0; i < 8; i++, uuid++)
            ((*uuid = (Random::getRandom<int>(0, 16))) < 10) ? *uuid += 48 : *uuid += 55;

        *uuid++ = '-';
        for (int i = 0; i < 4; i++, uuid++)
            ((*uuid = (Random::getRandom<int>(0, 16))) < 10) ? *uuid += 48 : *uuid += 55;

        *uuid++ = '-';
        for (int i = 0; i < 4; i++, uuid++)
            ((*uuid = (Random::getRandom<int>(0, 16))) < 10) ? *uuid += 48 : *uuid += 55;

        *uuid++ = '-';
        for (int i = 0; i < 4; i++, uuid++)
            ((*uuid = (Random::getRandom<int>(0, 16))) < 10) ? *uuid += 48 : *uuid += 55;

        *uuid++ = '-';
        for (int i = 0; i < 12; i++, uuid++)
            ((*uuid = (Random::getRandom<int>(0, 16))) < 10) ? *uuid += 48 : *uuid += 55;

        *uuid = 0;
        return string(uuid1);
    }
};

