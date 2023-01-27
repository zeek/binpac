FROM opensuse/leap:15.4

# A version field to invalidate Cirrus's build cache when needed, as suggested in
# https://github.com/cirruslabs/cirrus-ci-docs/issues/544#issuecomment-566066822
ENV DOCKERFILE_VERSION 20220615

RUN zypper addrepo https://download.opensuse.org/repositories/openSUSE:Leap:15.4:Update/standard/openSUSE:Leap:15.4:Update.repo \
 && zypper refresh \
 && zypper in -y \
    bison \
    cmake \
    flex \
    gcc10 \
    gcc10-c++ \
    git \
    make \
  && rm -rf /var/cache/zypp

RUN update-alternatives --install /usr/bin/cc cc /usr/bin/gcc-10 100
RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-10 100
