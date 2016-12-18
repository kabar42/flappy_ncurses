#!/bin/bash -e

SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
REPO_DIR="$(dirname ${SCRIPT_DIR})"

function usage {
cat <<-EOF
Run a build in the docker container
USAGE: run_docker_build [options]
OPTIONS:
    -b      Build the docker image before using it
EOF
}

BOXNAME=buildbox
BUILD_IT=false

case "$1" in
    "")
    ;;
    -b)
    BUILD_IT=true
    ;;
    *)
    usage && exit 0
    ;;
esac
    
if [ $BUILD_IT == true ]; then
    docker build -t ${BOXNAME} ${REPO_DIR}
fi
docker run --rm -it -v ${REPO_DIR}:/data ${BOXNAME}
