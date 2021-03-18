PLUGIN_NAME = rtxilfpRatiometer

HEADERS = rtxilfpRatiometer.h

SOURCES = rtxilfpRatiometer.cpp \
			moc_rtxilfpRatiometer.cpp

LIBS = -lfftw3 -lm

### Do note edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile