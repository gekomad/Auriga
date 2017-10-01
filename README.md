Auriga
==========
<a href="https://en.wikipedia.org/wiki/Auriga_%28slave%29"><img src="http://cinnamonchess.altervista.org/auriga/img/logo.jpg" align="left" hspace="10" vspace="6"></a>

Auriga is an open source distributed collaborative and multi platform system for the calculation of the function Perft.

----------

### Version
1.1

### How does it work

The tree is divided into tasks, each task will be analyzed from a single machine (worker) and the result will be sent automatically to http://cinnamonchess.altervista.org/auriga. When all tasks are processed you will have the result of perft.
 
<img src="http://cinnamonchess.altervista.org/auriga/img/auriga.png">


### Features

- Available for both Unix, Windows, Mac and ARM
- Works with UCI and xboard chess engines
- GPL 3 License
- C++11 source
- Available PHP Web Site


### Binaries

Binaries are available [here][1].


### Compiling

Auriga requires C++11 or greater, use unique Makefile to compile for many architectures:

    $ make

    Makefile for cross-compile Linux/Windows/OSX/ARM

    make auriga64      [COMP=compiler]
    make auriga32      [COMP=compiler]
    make auriga-ARM    [COMP=compiler]

    g++ is the default compiler, add COMP=yourcompiler to use another compiler
     example: make auriga64 COMP=clang++



### License

Auriga is released under the GPLv3+ license.

### Credits

Auriga was written by Giuseppe Cannella at gmail dot com and uses [Cinnamon chess engine][2] and [ThreadPool][3]

  [1]: http://cinnamonchess.altervista.org/auriga/download.php
  [2]: http://cinnamonchess.altervista.org/
  [3]: https://github.com/gekomad/ThreadPool
