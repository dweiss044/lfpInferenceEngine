CXX=g++
CXX_FLAGS=-std=c++11 -Iinclude

LD_FLAGS=-lfftw3 -lm

all: build/lfpRatiometer.o
	$(CXX) $(CXX_FLAGS) -o build/test src/main.cpp build/lfpRatiometer.o $(LD_FLAGS)

build/lfpRatiometer.o:
	$(CXX) $(CXX_FLAGS) -o build/lfpRatiometer.o -c src/lfpRatiometer.cpp
