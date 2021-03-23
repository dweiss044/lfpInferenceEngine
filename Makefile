PLUGIN_NAME = rtxilfpRatiometer

HEADERS = rtxi-lfpRatiometer.h

SOURCES = rtxi-lfpRatiometer.cpp \
			moc_rtxi-lfpRatiometer.cpp

LIBS =

OS := $(shell uname)
CXX = g++

# 

# FFTW3
CXXFLAGS := $(CXXFLAGS) $(shell pkg-config --cflags fftw3)
LDFLAGS := $(LDFLAGS) $(shell pkg-config --libs fftw3)

# lfpRatiometer
CXXFLAGS := $(CXXFLAGS) $(shell pkg-config --cflags lfpRatiometer)
LDFLAGS := $(LDFLAGS) $(shell pkg-config --libs lfpRatiometer)

# CXXFLAGS := $(CXXFLAGS) -rpath  /home/amborsa10/.local/lib

# RTXI plug-in stuff
include Makefile.plugin_compile

print-% : ; @echo $* = $($*)

cleanrtxi :
		sudo rm -rf /usr/local/lib/rtxi/$(PLUGIN_NAME).*
