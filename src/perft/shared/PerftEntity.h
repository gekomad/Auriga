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

#include <iostream>
#include <vector>
#include "../../util/UUID.h"
#include "../../util/String.h"
#include "../../network/util/Network.h"
#include "../../namespaces/def.h"

using namespace std;

class PerftEntity {
private:
    string uuid;
    string fen;
    int depth;
public:

    PerftEntity(const string &fen, int depth) : PerftEntity() {
        setFen(fen);
        setDepth(depth);
    }

    PerftEntity() {
        uuid = UUID::getUUID();
    }

    string toINIformat() const {
        string a;
        a.append("[perft]");
        a.append("\nperft_uuid=" + uuid);
        a.append("\nfen=" + fen);
        a.append("\ndepth=" + String(depth));

        return a;
    }

    const string &getUuid() const {
        return uuid;
    }

    void setUuid(const string &uuid) {
        assert(uuid.size() == 36);
        PerftEntity::uuid = uuid;
    }

    const string &getFen() const {
        return fen;
    }

    void setFen(const string &fen1) {
        assert(fen1.size() > 5);
        PerftEntity::fen = fen1;
    }

    int getDepth() const {
        return depth;
    }

    void setDepth(int depth) {
        assert(depth > 0);
        PerftEntity::depth = depth;
    }
};