FROM ubuntu:22.04

ENV DEBIAN_FRONTEND="noninteractive" TZ="America/Los_Angeles"

# A version field to invalidate Cirrus's build cache when needed, as suggested in
# https://github.com/cirruslabs/cirrus-ci-docs/issues/544#issuecomment-566066822
ENV DOCKERFILE_VERSION 20220614

RUN apt-get update && apt-get -y install \
    bison \
    cmake \
    flex \
    g++ \
    gcc \
    git \
    make \
  && apt autoclean \
  && rm -rf /var/lib/apt/lists/*
