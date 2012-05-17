#!/bin/bash
# Written by eXerigumo Clanjor, Wang Bin

script_name=${0##*/}
#type=app, lib
[ -n "`uname |grep NT`" ] && EXE=.exe #mingw and cygwin
TARGET=${PWD##*/}$EXE
OBJDIR=
DEFINES=
CXXFLAGS="-s"
CXXOPTIMIZE="-O2"
LFLAGS=
#SOURCES=(`ls *.c *.cpp *.cxx *.cc 2>/dev/null`) #No such file or directory
SOURCES=(`find . -name "*.c" -or -name "*.cc" -or -name "*.cxx" -or -name "*.cpp" -depth 2>/dev/null`)
SOURCES=(${SOURCES[@]/.\//}) #remove leading ./ from find
OBJECTS=()

show_help() {
	echo -e "\nOPTIONS:
	--help, -h
		Show this help
	--target
		target name
	--objdir
		objects' dir.
	--debug
		Enable debug mode
	-Ofast
		Use -Ofast instead of -O2
	-Ddefine
	-Llibdir -llib"
	exit 0
}

error() {
	echo -e "\e[1;31merror:\e[m $1"
	exit 1
}

main() {
	while ! [ -z "$1" ]; do
		case $1 in
			-h|--help)	show_help;;
			--target)
				TARGET=$2
				shift 2
				;;
			--objdir)
				OBJDIR=$2/
				shift 2
				;;
			--debug)
				CXXFLAGS="-g -DDEBUG"
				CXXOPTIMIZE=	# No optimization
				shift ;;
			-Ofast)
				CXXOPTIMIZE="-Ofast"
				shift ;;
			-D?*)
				DEFINES="${DEFINES} $1"
				shift ;;
			-L?*|-l?*)
				LFLAGS="$LFLAGS $1"
				shift ;;
			-I?*)
				CXXFLAGS="$CXXFLAGS $1"
				shift ;;
			--clean)
				make cleanall
				rm -f Makefile
				exit 0
				;;
			*)	error "unknown argument: $1" ;;
		esac
	done

	generate_makefile
}

generate_makefile() {
	local NUM_SRCS=${#SOURCES[@]}
	for((i=0;i<NUM_SRCS;++i))
	do
		OBJECTS[i]=${OBJDIR}${SOURCES[i]/.*/.o}
		local objdir=${OBJECTS[i]%/*};
		[ $objdir != ${OBJECTS[i]} ] && [ ! -d $objdir ] && mkdir -p $objdir
		generate_dep "${SOURCES[i]}"
	done
unset objdir
cat >Makefile<<EOF
#Makefile generated by $script_name. `date`
CXXFLAGS = -Wall $CXXFLAGS $CXXOPTIMIZE $DEFINES
LFLAGS = $LFLAGS

SOURCES = ${SOURCES[@]}

${OBJDIR}%.o: %.cpp
#	-test ! -d \$(@D) && mkdir -p \$(@D)
	\$(CXX) \$(CXXFLAGS) -c \$< -o \$@
${OBJDIR}%.o: %.cxx
	\$(CXX) \$(CXXFLAGS) -c \$< -o \$@
${OBJDIR}%.o: %.cc
	\$(CXX) \$(CXXFLAGS) -c \$< -o \$@
${OBJDIR}%.o: %.c
	\$(CXX) \$(CXXFLAGS) -c \$< -o \$@
	
all: $TARGET

$TARGET: ${OBJECTS[*]}
	\$(CXX) -o \$@ \$^ \$(LFLAGS)

clean:
	rm -f ${OBJECTS[@]}

cleanall: clean
	rm -f $TARGET

rebuild: clean all

EOF
	echo -e "$depends" >> Makefile
}

generate_dep() {
	printf "Solving dependencies of $1...\n"
	gxx_dep=`g++ $CXXFLAGS $DEFINES -MM $1 2>/dev/null`  #can not be local gxx_dep, always truw
	[ $? -ne 0 ] && error "unexpected failure of g++."
	depends="${depends}\n${OBJDIR}$gxx_dep"
}

main $*
