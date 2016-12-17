#!/bin/bash -e

SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
REPO_DIR="$(dirname ${SCRIPT_DIR})"

BOXNAME=buildbox

docker build -t ${BOXNAME} ${REPO_DIR}
docker run --rm -it -v ${REPO_DIR}:/data ${BOXNAME}
