#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
BASE_DIR="$( dirname "$SCRIPT_DIR" )"
BUILD_DIR="$BASE_DIR/Build"

format() {
    echo "Running clang-format on: $(basename $1)"
    clang-format -i $1 2>/dev/null
}

FILES=$(find $BASE_DIR -iname *.h -o -iname *.cpp -o -path $BUILD_DIR -prune)
for FILE in $FILES
do
    format "$FILE"
done

