#!/bin/bash

release=0
debug=0

if [[ -n $1 ]]; then
    case $1 in
        release)
            release=1
            ;;
        debug)
            debug=1
            ;;
        *)
            echo "unrecognized option $1"
            exit 1
            ;;
    esac
else
    release=1
    debug=1
fi

echo "Building release=$release, debug=$debug"

if [[ $release -eq 1 ]]; then
    ./cmake-wrapper.py -B build/release  -DCMAKE_BUILD_TYPE=Release
    ./cmake-wrapper.py --build build/release -t all
fi

if [[ $debug -eq 1 ]]; then
    ./cmake-wrapper.py -B build/debug -DCMAKE_BUILD_TYPE=Debug
    ./cmake-wrapper.py --build build/debug -t all
fi
