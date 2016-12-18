# Template project for C builds with Docker

## Background

The Dockerfile sets up an Ubuntu 16.04 build environment for C programs, using the following tools:
- CMake
- Ninja
- GCC / G++

Currently, CMake brings in any *.h and *.c files in the src/ directory

## Building

`./scripts/run_docker_build.sh`
