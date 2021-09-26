#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
BASE_DIR="$( dirname "$SCRIPT_DIR" )"

format() {
    echo "Running clang-format on: $(basename $1)"
    clang-format -i $1
}

FILES=$(find $BASE_DIR -iname *.h -o -iname *.cpp)
for FILE in $FILES
do
    format "$FILE"
done

