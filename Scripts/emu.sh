#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
BASE_DIR="$( dirname "$SCRIPT_DIR" )"
BUILD_DIR="$BASE_DIR/Build"
TOOLS_DIR="$BASE_DIR/Tools"

EMSDK_DIR="$TOOLS_DIR/emsdk"
EMSCRIPTEN_DIR="$EMSDK_DIR/upstream/emscripten"

clang_format() {
    FILES=$(find $BASE_DIR -iname *.h -o -iname *.cpp -o -path $BUILD_DIR -prune -o -path $TOOLS_DIR -prune)
    for FILE in $FILES
    do
        echo "Running clang-format on: $(basename $FILE)"
        clang-format -i $FILE 2>/dev/null
    done
}

build_emscripten() {
    pushd Tools/ >/dev/null
        git clone https://github.com/emscripten-core/emsdk.git
        cd emsdk
        ./emsdk install latest
        ./emsdk activate latest
        source ./emsdk_env.sh
    popd >/dev/null
}

build_fmt() {
    pushd Tools/ >/dev/null
        git clone https://github.com/fmtlib/fmt.git
        cd fmt/
        mkdir -p build
        cd build
        $EMSCRIPTEN_DIR/emcmake cmake .. -GNinja
        ninja install 
    popd >/dev/null
}

build_emu_usage() {
    echo "build: must specify build type toolchain, sdl or wasm"
}

build_emu() {
    mkdir -p $BUILD_DIR

    if [[ "$1" == "sdl" ]]; then
        CMAKE="cmake"
    fi

    if [[ "$1" == "wasm" ]]; then
        CMAKE="$EMSCRIPTEN_DIR/emcmake cmake"
    fi
    
    pushd $BUILD_DIR >/dev/null
        $CMAKE .. -GNinja
        ninja 
    popd >/dev/null
}

build() {
    if [[ "$1" != "sdl" ]] && [[ "$1" != "wasm" ]] && [[ "$1" != "toolchain" ]]; then
        build_emu_usage
        exit 1
    fi
   
    if [[ "$1" == "sdl" ]]; then
        build_emu sdl
    fi
    
    if [[ "$1" == "wasm" ]]; then
        build_emu wasm
    fi
    
    if [[ "$1" == "toolchain" ]]; then
        build_emscripten
        build_fmt
    fi
}

usage() {
    echo -e "emu: simple command line tool to build, run, and format the emu project\n"
    echo -e "USAGE:"
    echo -e "\t./Scripts/emu [ format | build [ toolchain | sdl | wasm ] ]\n"
    echo -e "ARGUMENT OPTIONS:"
    echo -e "\t[ format ] : Calls clang-format on all .cpp and .h files in the repository"
    echo -e "\t[ build toolchain ] : Creates the necessary libraries for the build process. This downloads the latest version of emscripten and intsalls in the Tools/ directory. Also builds the fmt library for WASM compatability."
    echo -e "\t[ build sdl ] : Builds the project to run with the SDL windows only. Uses g++ for compilation."
    echo -e "\t[ build wasm ] : Builds the project to run WASM. Ouputs a wasm, js, and html file. Uses emscripten for compilation."
}

if [ $# -eq 0 ]; then
    usage
fi

while [[ $# -gt 0 ]]; do
    CMD="$1"

    case $CMD in
        format)
            clang_format
            shift
            shift 
            ;;
        build)
            build $2
            shift
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            usage
            exit 1
            ;;
        esac
done
