#!/usr/bin/env bash
clang++ -Weverything main.cpp -O0 -g -o tests -framework CoreFoundation
mkdir -p bin/tests.app/Contents/MacOS/
cp tests bin/tests.app/Contents/MacOS/
./tests bin/tests.app
