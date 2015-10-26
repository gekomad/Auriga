#    Auriga - Makefile for cross-compile
#    Copyright (C) Giuseppe Cannella
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

COMP=g++

NATIVE_ARCH="32"
ifndef SystemRoot
	# *nix
	ARC=
	LIBS=-s -Wl,--whole-archive -lpthread -Wl,--no-whole-archive

	ifeq ($(shell uname -s),Darwin)
		LIBS= -static-libgcc -static-libstdc++ -lpthread
		OS=OSX
	else
		PROFILE_USE_THREAD=" -fprofile-correction -fprofile-use "
		STATIC="-static "
		OS=Linux
	endif

	RM=rm -f
	PA="./"
	EXE="auriga"
	STRIP=strip
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		NATIVE_ARCH="64"
	endif
else
	# Windows
	ARC="-mno-ms-bitfields"
	OS=Windows
	RM=del
	PA=""
	LIBS=-static -static-libgcc -static-libstdc++
	EXE="auriga.exe"
	STRIP=strip.exe

	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
		NATIVE_ARCH="64"
	endif
endif

PARAM="-std=c++11 -O3 -w -pipe -DDLOG_LEVEL=_INFO -DNDEBUG -fno-exceptions -fno-rtti -fomit-frame-pointer -Wfloat-equal -Wpedantic -Wunreachable-code -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Wmain -Wundef -Winit-self -Wredundant-decls -Wcast-align -ffast-math -funroll-loops -Wno-long-long -Wcast-qual -Wextra -Wshadow "

help:

	@echo "Makefile for cross-compile Linux/Windows/OSX/ARM"
	@echo ""
	@echo "make auriga64-generic          > Unspecified 64-bit"
	@echo "make auriga32-generic          > Unspecified 32-bit"
	@echo "make auriga-ARM                > Optimized for arm cpu"

	@echo "g++ is the default compiler, add COMP=yourcompiler to use another compiler"
	@echo " example: make auriga64-generic COMP=clang++"
	@echo ""

build:

	$(MAKE) -j4 EXE=$(EXE) CFLAGS=$(PARAM)$(STATIC) all
	$(STRIP) $(EXE)

auriga32-generic:
	$(MAKE) LIBS="$(LIBS) lib/$(OS)/32/libCinnamon.a" ARC="$(ARC) -m32" build

auriga-ARM:
	$(MAKE) LIBS="$(LIBS) gtb/ARM/32/libgtb.a" ARC="$(ARC) -fno-gcse -w -fsigned-char" build

auriga64-generic:
	$(MAKE) LIBS="$(LIBS) lib/$(OS)/x86_64/libCinnamon.a" ARC="$(ARC) -m64" build

all:	Engine.o Client.o  PerftSplitter.o  Perft.o  PerftTree.o main.o TaskEntityDao.o  HttpPost.o  PerftEntityDao.o  WorkerEntityDao.o
	$(COMP) ${CFLAGS} ${ARC} -o ${EXE}  Engine.o Client.o  PerftSplitter.o  Perft.o  PerftTree.o main.o TaskEntityDao.o  HttpPost.o  PerftEntityDao.o  WorkerEntityDao.o ${LIBS}

default:
	help


Engine.o: engine/Engine.cpp
	$(COMP) -c engine/Engine.cpp ${CFLAGS} ${ARC}

Client.o: network/Client.cpp
	$(COMP) -c network/Client.cpp ${CFLAGS} ${ARC}

PerftSplitter.o: perft/master/PerftSplitter.cpp
	$(COMP) -c perft/master/PerftSplitter.cpp ${CFLAGS} ${ARC}

Perft.o: perft/task/Perft.cpp
	$(COMP) -c perft/task/Perft.cpp ${CFLAGS} ${ARC}

main.o: main.cpp
	$(COMP) -c main.cpp ${CFLAGS} ${ARC}

PerftTree.o: perft/shared/PerftTree.cpp
	$(COMP) -c perft/shared/PerftTree.cpp ${CFLAGS} ${ARC}

TaskEntityDao.o: perft/task/TaskEntityDao.cpp
	$(COMP) -c perft/task/TaskEntityDao.cpp ${CFLAGS} ${ARC}

HttpPost.o: perft/task/HttpPost.cpp
	$(COMP) -c perft/task/HttpPost.cpp ${CFLAGS} ${ARC}

PerftEntityDao.o: perft/shared/PerftEntityDao.cpp
	$(COMP) -c perft/shared/PerftEntityDao.cpp ${CFLAGS} ${ARC}

WorkerEntityDao.o: perft/worker/WorkerEntityDao.cpp
	$(COMP) -c perft/worker/WorkerEntityDao.cpp ${CFLAGS} ${ARC}

clean:
	$(RM) *.o $(EXE)
