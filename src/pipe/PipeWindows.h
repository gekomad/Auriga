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
#ifdef(_WIN32)
#pragma once

#include <iostream>
#include "Ipipe.h"

using namespace std;

class PipeWindows : public Ipipe {
public :
    int writeStdout(const string &msg) {
        DWORD dwWritten;
        if (!WriteFile(g_hChildStd_IN_Wr, msg.c_str(), msg.size(), &dwWritten, NULL)) {
            return -1;
        };
        return dwWritten;

    };

    ~PipeWindows() {
        if (initialized) {
            CloseHandle(g_hChildStd_IN_Wr);
            CloseHandle(g_hChildStd_IN_Wr);
            CloseHandle(g_hChildStd_OUT_Wr);
            CloseHandle(g_hChildStd_OUT_Rd);
        }
    }

    const string readStdin() {
        DWORD dwRead;
        CHAR chBuf[2048];
        bool bSuccess = false;

        bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
        if (!bSuccess || dwRead == 0) {
            return "";
        }
        chBuf[dwRead] = 0;
        cout << "READ from engine: |" << chBuf << "|" << endl;
        return string(chBuf);
    }

    const string readStderr() { assert(0);return ""; }

    bool init(const string &enginePath) {
        TCHAR szCmdline[1024];
        strcpy(szCmdline, enginePath.c_str());
        PROCESS_INFORMATION piProcInfo;
        STARTUPINFO siStartInfo;
        bool bSuccess = false;


        ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));


        ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
        siStartInfo.cb = sizeof(STARTUPINFO);
        siStartInfo.hStdError = g_hChildStd_OUT_Wr;
        siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
        siStartInfo.hStdInput = g_hChildStd_IN_Rd;
        siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

        bSuccess = CreateProcess(NULL, szCmdline,     // command line
                                 NULL,          // process security attributes
                                 NULL,          // primary thread security attributes
                                 true,          // handles are inherited
                                 0,             // creation flags
                                 NULL,          // use parent's environment
                                 NULL,          // use parent's current directory
                                 &siStartInfo,  // STARTUPINFO pointer
                                 &piProcInfo);  // receives PROCESS_INFORMATION

        if (!bSuccess) {
            initialized = false;
        } else {

            CloseHandle(piProcInfo.hProcess);
            CloseHandle(piProcInfo.hThread);
        }
        initialized = true;
        return initialized;

    };
private:
    bool initialized;

    HANDLE g_hChildStd_IN_Rd = NULL;
    HANDLE g_hChildStd_IN_Wr = NULL;
    HANDLE g_hChildStd_OUT_Rd = NULL;
    HANDLE g_hChildStd_OUT_Wr = NULL;
};
#endif