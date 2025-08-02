#!/usr/bin/env sh

GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
RED=$(tput setaf 1)
CYAN=$(tput setaf 6)
BRIGHT=$(tput bold)
RESET=$(tput sgr0)

HELP="
${CYAN}${BRIGHT}Flags:${RESET}
    
  ${GREEN}--clean:   ${YELLOW}cleans build directories
  ${GREEN}--local:   ${YELLOW}generates a local library and header files
  ${GREEN}--system:  ${YELLOW}generates a system library and header files
  ${GREEN}--test:    ${YELLOW}runs a test in the test directory by path (e.g., test/file.c) 
  ${GREEN}--debug:   ${YELLOW}uses valgrind to test programs in the test directory 

${CYAN}${BRIGHT}Examples:${RESET}

  ${GREEN}./build.sh --clean                     ${YELLOW}(clean build directories)
  ${GREEN}./build.sh --local ([--release])       ${YELLOW}(installs local library with optional release mode)
  ${GREEN}./build.sh --system ([--release])      ${YELLOW}(installs system library with optional release mode)
  ${GREEN}./build.sh --test [test/file.c]        ${YELLOW}(tests given test name without valgrind)    
  ${GREEN}./build.sh --debug [test/file.c]       ${YELLOW}(tests given test name without valgrind)    

${CYAN}${BRIGHT}Notes:${RESET}

    ${GREEN}- Test programs rely on a local installation of ctk via build.sh --local
    ${GREEN}- If a local copy of ctk was not generated, 
	  build.sh --test will auto generate a local library
"

msg() {
    printf "%s%s%s\n" "$GREEN" "$@" "$RESET"  
}

panic() {	
    printf "%s%s%s%s\n" "$RED" "[BUILD ERROR]: " "$@" "$RESET"  
	exit 1
}

help() {
	printf "%s%s\n" "$HELP" "$RESET"  
	exit 1
}

run() {
    ./build.sh --clean
    if [ $# -eq 0 ]; then
		panic "No arguments supplied"
    fi

    if [ -z "$1" ]; then
        panic "Running CTK tests requires a file argument"
    fi

    if ! [ -f "$1" ]; then
    	panic "$1" "does not exist"
    fi

    if ! [ "${1##*.}" = "c" ]; then
    	panic "$1" "is not a source file"
    fi    

    ./build.sh --local
	cd test/ || panic "Could not change directory to '${PWD}/test/'" 
    (
        rm -rf build/
        mkdir -p build/
        cd build || exit 1
        cmake .. "-DCMAKE_BUILD_TYPE=Debug" -DTEST_NAME="$(basename "$1")" "$2"
        make
    )
}

if [ "$1" = "--clean" ]; then
    rm -rf build
    rm -rf out
    rm -f test/vgcore.*
    rm -f vgcore.*
elif [ "$1" = "--local" ]; then
    ./build.sh --clean 
    mkdir -p build
    mkdir -p out
    cd build || panic "Could not change directory to ${PWD}/build/" 

	if [ "$2" = "--release" ]; then
		cmake .. "-DLOCAL_BUILD=ON" "-DCMAKE_BUILD_TYPE=Release"
	else
		cmake .. "-DLOCAL_BUILD=ON"  "-DCMAKE_BUILD_TYPE=Debug"
	fi

    make install
    mv install/* ../out
elif [ "$1" = "--system" ]; then
    ./build.sh --clean 
    mkdir -p build
    cd build || panic "Could not change directory to ${PWD}/build/"

	if [ "$2" = "--release" ]; then
		cmake .. "-DCMAKE_BUILD_TYPE=Release"
	else
		cmake .. "-DCMAKE_BUILD_TYPE=Debug"
	fi

    sudo make install
elif [ "$1" = "--test" ]; then
    run "$2" "-DCMAKE_BUILD_TYPE=Debug"
    ./build/test
elif [ "$1" = "--debug" ]; then
    run "$2" "-DCMAKE_BUILD_TYPE=Debug"
    valgrind -s --leak-check=full --track-origins=yes ./build/test
    rm -rf build/test
else 
	help
fi
