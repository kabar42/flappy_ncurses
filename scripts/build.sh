#!/bin/bash -e

SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
REPO_DIR="$(dirname ${SCRIPT_DIR})"

mkdir -p "${REPO_DIR}/build"
pushd "${REPO_DIR}/build"

cmake -G"Ninja" "$REPO_DIR"
ninja

popd
