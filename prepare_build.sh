#!/bin/bash
mkdir -p build/release
mkdir -p build/debug

COMPILER=g++

cd build/release
cmake -DCMAKE_CXX_COMPILER=$COMPILER \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
      ../..

cd ../debug
cmake -DCMAKE_CXX_COMPILER=$COMPILER \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
      ../..

mv compile_commands.json ../..