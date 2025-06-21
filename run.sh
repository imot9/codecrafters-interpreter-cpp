#!/bin/sh

set -e # Exit early if any commands fail

cmake -B build -S . 
cmake --build ./build

exec $(dirname $0)/build/interpreter "$@"
