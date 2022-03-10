#!/bin/sh

g++ -c ./src/main.cpp `llvm-config --cxxflags` -o ./obj/Linux/Release/main.o
g++ -o ./bin/Linux/Release/program ./obj/Linux/Release/main.o `llvm-config --ldflags --system-libs --libs core`