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

ifndef SystemRoot
	# *nix
	RM=rm -f
	STRIP=strip
else
	RM=del
	STRIP=strip.exe
endif

ifeq ($(OS),Windows)
	LIBS=-static -static-libgcc -static-libstdc++ -lpthread -lws2_32
	EXE="auriga.exe"

else
	# *nix
	LIBS=-s -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
	ifeq ($(OS),OSX)
		LIBS= -static-libgcc -static-libstdc++ -lpthread
		OS=OSX
	else
		#Linux/arm
		STATIC="-static "
	endif
	EXE="auriga"

endif

PARAM="-std=c++11 -O3 -w -DDLOG_LEVEL=_INFO -DNDEBUG  "

help:

	@echo "Makefile for cross-compile Linux/Windows/OSX/ARM"
	@echo ""
	@echo "make auriga64-generic  OS=yourOS [COMP=comp]       > Unspecified 64-bit"
	@echo "make auriga32-generic  OS=yourOS [COMP=comp]       > Unspecified 32-bit"
	@echo "make auriga-ARM                  [COMP=comp]       > 32-bit arm cpu"
	@echo ""
	@echo "Supported OS:"
	@echo ""
	@echo "Linux"
	@echo "Windows"
	@echo "OSX"
	@echo "ARM"
	@echo ""
	@echo " example: make auriga64-generic OS=Windows COMP=x86_64-w64-mingw32-g++"
	@echo ""

build:

	$(MAKE) -j4 EXE=$(EXE) CFLAGS=$(PARAM)$(STATIC) all
	$(STRIP) $(EXE)

auriga32-generic:
	$(MAKE) LIBS="$(LIBS) lib/$(OS)/32/libCinnamon.a" ARC="-m32" build

auriga-ARM:
	$(MAKE) LIBS="$(LIBS) gtb/ARM/32/libgtb.a" ARC="-fno-gcse -w -fsigned-char" build

auriga64-generic:
	$(MAKE) LIBS="$(LIBS) lib/$(OS)/64/libCinnamon.a" ARC="-m64" build

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
