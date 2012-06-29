TEMPLATE = app


TARGET = tst_programoptions
include(../src/libProgramOptions.pri)

#win32:LIBS += -lUser32

SOURCES += tst_options.cpp
HEADERS += 
