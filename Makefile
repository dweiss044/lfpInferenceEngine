PLUGIN_NAME = lfpRatiometer

HEADERS = lfpRatiometer.h

SOURCES = lfpRatiometer.cpp \
			moc_lfpRatiometer.cpp

LIBS = -L/usr/lib/x86_64-linux-gnu -lfftw3 -lm

### Do note edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile