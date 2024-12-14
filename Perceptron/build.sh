#!/bin/bash

mkdir -p build
cd build

if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]; then
    echo "Running on Windows with Ninja..."
    cmake -G "Ninja" ..
    ninja
else
    echo "Running on Linux/Unix..."
    cmake ..
    make
fi
