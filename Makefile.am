bin_PROGRAMS = howiphp

howiphp_CXXFLAGS=-O3 -Wall -fmessage-length=0 -std=c++11 -lboost_system -lgit2 -lboost_filesystem

LDFLAGS_BOOST = \
    $(BOOST_SYSTEM_LDFLAGS) \
    $(BOOST_PROGRAM_OPTIONS_LDFLAGS)

LIBS_BOOST = \
    $(BOOST_SYSTEM_LIBS) \
    $(BOOST_PROGRAM_OPTIONS_LIBS) \
    $(BOOST_FILESYSTEM_LIBS)

howiphp_CPPFLAGS = $(BOOST_CPPFLAGS)
howiphp_LDFLAGS = $(LDFLAGS_BOOST)
LIBS += $(LIBS_BOOST)

howiphp_SOURCES = \
	src/howiphp.cpp src/howiphp.h \
	src/CDN/CDN.cpp src/CDN/CDN.h \
	src/Check/Check.cpp src/Check/Check.h \
	src/CompareVersion/CompareVersion.cpp src/CompareVersion/CompareVersion.h \
	src/Config/Config.cpp src/Config/Config.h \
	src/Dependencies/Dependencies.cpp src/Depenedencies/Dependencies.h \
	src/GitWorker/GitWorker.cpp src/GitWorker/GitWorker.h \
	src/List/List.cpp src/List/List.h \
	src/Output/Output.cpp src/Output/Output.h \
	src/PHPbuilder/PHPbuilder.cpp src/PHPbuilder/PHPbuilder.h \
	src/ProgressBar/ProgressBar.cpp src/ProgressBar/ProgressBar.h \
	src/UseVersion/UseVersion.cpp src/UseVersion/UseVersion.h \
	vendor/jsoncpp/dist/jsoncpp.cpp vendor/jsoncpp/dist/json/json.h vendor/jsoncpp/dist/json/json-forwards.h
