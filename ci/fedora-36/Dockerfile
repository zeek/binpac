FROM fedora:36

# A version field to invalidate Cirrus's build cache when needed, as suggested in
# https://github.com/cirruslabs/cirrus-ci-docs/issues/544#issuecomment-566066822
ENV DOCKERFILE_VERSION 20220614

RUN dnf -y install \
    bison \
    cmake \
    flex \
    gcc \
    gcc-c++ \
    git \
    make \
  && dnf clean all && rm -rf /var/cache/dnf
