#!/bin/sh
# Build rsmc release, then run this script inside a container to check if binary will work on a different system
# e.g. build release on Ubuntu and pass this script to an Alpine container
# make sure container has all tools (cmake 3.29+, clang, lld, ninja etc)

set -eu
SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
PROJECT_ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/../.." && pwd)
TEMP_DIR="$(mktemp -d)"

cleanup(){
    rm -rf "$TEMP_DIR"
}

trap cleanup EXIT

cp -r ./* "$TEMP_DIR"
cd "$TEMP_DIR"

mkdir -p dist
mv build/release/librsmc.so* dist

rm -rf build
cmake --workflow --preset build-release

rm build/release/librsmc.so*
mv dist/librsmc.so* build/release/

ctest --preset release
