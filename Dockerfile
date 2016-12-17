FROM ubuntu:16.04
MAINTAINER Kent Barnett

RUN apt-get update \
    && apt-get install --force-yes -y cmake ninja-build gcc g++ libncursesw5-dev

ENV entry_dir /data
WORKDIR $entry_dir  

ENTRYPOINT ["/data/start.sh"]
