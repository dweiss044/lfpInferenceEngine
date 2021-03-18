PLUGIN_NAME = rtxilfpRatiometer

HEADERS = rtxi-lfpRatiometer.h

SOURCES = rtxi-lfpRatiometer.cpp \
			moc_rtxi-lfpRatiometer.cpp

LIBS = -lfftw3 -lm

### Do note edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile