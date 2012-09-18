TEMPLATE = lib

QT += 
QT       -= gui
CONFIG   += console
CONFIG   -= app_bundle

CONFIG *= programoptions-buildlib
PROJECTROOT = $$PWD/..
isEmpty(BUILD_DIR):BUILD_DIR=$$(BUILD_DIR)
isEmpty(BUILD_DIR):BUILD_DIR=$$[BUILD_DIR]
isEmpty(BUILD_DIR):BUILD_IN_SRC = yes

!isEmpty(BUILD_IN_SRC):BUILD_DIR=$$PROJECTROOT/out
!include(libProgramOptions.pri): error(could not find libProgramOptions.pri)

#src
unix: SOURCES += 
else:win32: SOURCES += 

SOURCES += ProgramOptions.cpp AnyBasic.cpp

HEADERS += programoptions_global.h ProgramOptions.h AnyBasic.h lexical_cast.h



