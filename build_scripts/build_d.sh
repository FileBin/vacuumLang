#!/bin/sh

#build binaries
g++ -c -g ./src/main.cpp `llvm-config --cppflags --ldflags` -o ./obj/Linux/Debug/main.o -std=gnu++17 -fexceptions

#link
g++ -g ./obj/Linux/Debug/main.o `llvm-config --system-libs --libs all` -o ./bin/Linux/Debug/program
