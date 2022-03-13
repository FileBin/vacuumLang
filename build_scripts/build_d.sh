#!/bin/sh

#build binaries
g++ -c -g ./src/main.cpp `llvm-config --cxxflags` -o ./obj/Linux/Debug/main.o -std=gnu++11 -fexceptions

#link
g++ -g ./obj/Linux/Debug/main.o -o ./bin/Linux/Debug/program  `llvm-config --ldflags --system-libs --libs core` -std=gnu++11