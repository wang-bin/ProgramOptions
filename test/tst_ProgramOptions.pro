TEMPLATE = app


TARGET = tst_programoptions
PROJECTROOT = $$PWD/..
include(../src/libProgramOptions.pri)

#win32:LIBS += -lUser32

SOURCES += tst_options.cpp
HEADERS += 
