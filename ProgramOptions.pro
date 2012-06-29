TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = libprogramoptions test

libprogramoptions.file = src/libProgramOptions.pro
test.file = test/tst_ProgramOptions.pro
test.depends += libprogramoptions

OTHER_FILES += README


