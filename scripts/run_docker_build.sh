#!/bin/bash -e

SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
REPO_DIR="$(dirname ${SCRIPT_DIR})"

docker run -it -v ${REPO_DIR}:/data buildbox
