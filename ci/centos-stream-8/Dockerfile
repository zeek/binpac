FROM quay.io/centos/centos:stream8

# A version field to invalidate Cirrus's build cache when needed, as suggested in
# https://github.com/cirruslabs/cirrus-ci-docs/issues/544#issuecomment-566066822
ENV DOCKERFILE_VERSION 20220519

RUN dnf -y install https://dl.fedoraproject.org/pub/epel/epel-release-latest-8.noarch.rpm
RUN dnf config-manager --set-enabled powertools

RUN dnf -y install \
    bison \
    cmake \
    flex \
    gcc \
    gcc-c++ \
    git \
    make \
  && dnf clean all && rm -rf /var/cache/dnf
