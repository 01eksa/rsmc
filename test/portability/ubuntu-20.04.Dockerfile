FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC
RUN apt update \
    && apt-get install -y --no-install-recommends clang lld ninja-build python3-pip ca-certificates \
    && pip3 install --no-cache-dir cmake
WORKDIR /workdir
