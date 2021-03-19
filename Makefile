PLUGIN_NAME = rtxilfpRatiometer

HEADERS = rtxi-lfpRatiometer.h

SOURCES = rtxi-lfpRatiometer.cpp \
			moc_rtxi-lfpRatiometer.cpp

LIBS =

OS := $(shell uname)
CXX = g++

# lfpRatiometer
CXXFLAGS := $(CXXFLAGS) $(shell pkg-config --cflags lfpRatiometer)
LDFLAGS := $(LDFLAGS) $(shell pkg-config --libs lfpRatiometer)

# RTXI plug-in stuff
include Makefile.plugin_compile

print-% : ; @echo $* = $($*)