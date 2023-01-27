FROM alpine:latest

# A version field to invalidate Cirrus's build cache when needed, as suggested in
# https://github.com/cirruslabs/cirrus-ci-docs/issues/544#issuecomment-566066822
ENV DOCKERFILE_VERSION 20230113

RUN apk add --no-cache \
  bash \
  bison \
  cmake \
  flex-dev \
  g++ \
  git \
  linux-headers \
  make
