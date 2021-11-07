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
        esac
done
