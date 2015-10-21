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

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include "../blockingThreadPool/Thread.h"
#include<arpa/inet.h>
#include "../namespaces/def.h"
#include <iostream>

#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "../util/Singleton.h"

using namespace std;
using namespace _def;

#if !defined DLOG_LEVEL
#if defined DEBUG_MODE
#define DLOG_LEVEL TRACE
#else
#define DLOG_LEVEL OFF
#endif
#endif


namespace _logger {
    class Logger : public Singleton<Logger> {
        friend class Singleton<Logger>;

    public:
        ofstream &getLogger() {
            return *logStream;
        };

        void setLogfile(const string &f) {
            logStream = new std::ofstream(f, std::ofstream::out);
        }

    private:
        ~Logger() {
            if (logStream) {
                logStream->close();
                delete (logStream);
            }
        }

        std::ofstream *logStream = nullptr;
    };

    static mutex _CoutSyncMutex;

    static enum LOG_LEVEL {
        TRACE = 0, DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4, FATAL = 5, OFF = 6, ALWAYS = 7
    } _LOG_LEVEL;
    static const string LOG_LEVEL_STRING[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL", "OFF", "LOG"};

#if defined(_WIN32)
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define LINE_INFO __FILENAME__,":",__LINE__," "

    template<typename T>
    void __log(T t) {
        cout << (t);
        Logger::getInstance().getLogger() << (t);
    }

    template<typename T, typename... Args>
    void __log(T t, Args... args) {
        cout << (t);
        Logger::getInstance().getLogger() << (t);
        __log(args...);
    }

    template<LOG_LEVEL type, typename T, typename... Args>
    void _log(T t, Args... args) {
        lock_guard<mutex> lock1(_CoutSyncMutex);
        cout << Time::getLocalTime() << " " << LOG_LEVEL_STRING[type] << " ";
        Logger::getInstance().getLogger() << Time::getLocalTime() << " " << LOG_LEVEL_STRING[type] << " ";
        __log(t, args...);
        cout << endl;
        Logger::getInstance().getLogger() << endl;
    }

#define log(...)                            {_log<LOG_LEVEL::ALWAYS>(LINE_INFO,__VA_ARGS__);}
#define trace(...) if (TRACE >= DLOG_LEVEL) {_log<LOG_LEVEL::TRACE>( LINE_INFO,__VA_ARGS__);}
#define debug(...) if (DEBUG >= DLOG_LEVEL) {_log<LOG_LEVEL::DEBUG>( LINE_INFO,__VA_ARGS__);}
#define info(...)  if (INFO  >= DLOG_LEVEL) {_log<LOG_LEVEL::INFO> ( LINE_INFO,__VA_ARGS__);}
#define warn(...)  if (WARN  >= DLOG_LEVEL) {_log<LOG_LEVEL::WARN> ( LINE_INFO,__VA_ARGS__);}
#define error(...) if (ERROR >= DLOG_LEVEL) {_log<LOG_LEVEL::ERROR>( LINE_INFO,__VA_ARGS__);}
#define fatal(...) if (FATAL >= DLOG_LEVEL) {_log<LOG_LEVEL::FATAL>( LINE_INFO,__VA_ARGS__);}

}
