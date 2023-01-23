CXX=g++
CXX_FLAGS=-std=c++11 -Iinclude
PYTHON_CXX_FLAGS=-I/home/dweiss/anaconda3/envs/hmm/include/python3.10 -I/home/dweiss/anaconda3/envs/hmm/include/python3.10 -Wno-unused-result -Wsign-compare -march=nocona -mtune=haswell -ftree-vectorize -fPIC -fstack-protector-strong -fno-plt -O3 -ffunction-sections -pipe -isystem /home/dweiss/anaconda3/envs/hmm/include -fdebug-prefix-map=/croot/python-split_1669298683653/work=/usr/local/src/conda/python-3.10.8 -fdebug-prefix-map=/home/dweiss/anaconda3/envs/hmm=/usr/local/src/conda-prefix -fuse-linker-plugin -ffat-lto-objects -flto-partition=none -flto -DNDEBUG -fwrapv -O3 -Wall

LD_FLAGS=-lfftw3 -lm
PYTHON_LD_FLAGS=-L/home/dweiss/anaconda3/envs/hmm/lib/python3.10/config-3.10-x86_64-linux-gnu -L/home/dweiss/anaconda3/envs/hmm/lib -lpython3.10 -lcrypt -lpthread -ldl -lutil -lm -lm

all: build/lfpRatiometer.o
	$(CXX) $(CXX_FLAGS) $(PYTHON_CXX_FLAGS) -o build/test src/main.cpp src/lfpRatiometer.cpp src/lfpInferenceEngine.cpp $(PYTHON_LD_FLAGS) $(LD_FLAGS)

build/lfpRatiometer.o:
	$(CXX) $(CXX_FLAGS) -o build/lfpRatiometer.o -c src/lfpRatiometer.cpp 