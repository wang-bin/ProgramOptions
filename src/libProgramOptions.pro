TEMPLATE = lib

QT       =
CONFIG   += console #exceptions
CONFIG   -= app_bundle

CONFIG *= programoptions-buildlib
PROJECTROOT = $$PWD/..
include($$PROJECTROOT/deploy.pri)
isEmpty(BUILD_DIR):BUILD_DIR=$$(BUILD_DIR)
isEmpty(BUILD_DIR):BUILD_DIR=$$[BUILD_DIR]
isEmpty(BUILD_DIR):BUILD_IN_SRC = yes

!isEmpty(BUILD_IN_SRC):BUILD_DIR=$$OUT_PWD/../out
!include(libProgramOptions.pri): error(could not find libProgramOptions.pri)

#src
unix: SOURCES += 
else:win32: SOURCES += 

SOURCES += ProgramOptions.cpp AnyBasic.cpp

HEADERS += programoptions_global.h ProgramOptions.h AnyBasic.h lexical_cast.h



