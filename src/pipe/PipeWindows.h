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
#ifdef _WIN32
#pragma once

#include <iostream>
#include "Ipipe.h"

using namespace std;

class PipeWindows : public Ipipe {
public :
    int writeStdout(const string &msg) ;

    ~PipeWindows();

    const string readStdin() ;

    const string readStderr();

    bool init(const string &enginePath);
private:
    bool initialized;

    HANDLE g_hChildStd_IN_Wr = NULL;
    HANDLE g_hChildStd_OUT_Rd = NULL;
    HANDLE g_hChildStd_ERR_Rd = NULL;

    HANDLE g_hChildStd_IN_Rd = NULL;//TODO provare a chiudere subito e lasciarlo locale
    HANDLE g_hChildStd_OUT_Wr = NULL;//TODO provare a chiudere subito e lasciarlo locale
    HANDLE g_hChildStd_ERR_Wr = NULL;//TODO provare a chiudere subito e lasciarlo locale
};

#endif