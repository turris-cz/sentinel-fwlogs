# Wee need unstable for now because of check version
FROM debian:unstable

ENV HOME /root

RUN \
  apt-get update && \
  apt-get -y upgrade && \
  apt-get -y install --no-install-recommends \
    autoconf autoconf-archive automake libtool \
    make pkg-config gcc gperf \
    git ca-certificates \
    libczmq-dev libmsgpack-dev libconfig-dev libnetfilter-log-dev \
    check cppcheck valgrind lcov \
    zip xz-utils \
    && \
  apt-get clean

# Compile logc
RUN \
  git clone --branch "v0.1.0" "https://gitlab.nic.cz/turris/logc.git" && \
  cd logc && \
  ./bootstrap && ./configure --disable-tests --disable-linters --prefix=/usr && \
  make install && \
  cd .. && \
  rm -rf logc


CMD [ "bash" ]

# vim: ft=dockerfile
