
FROM ubuntu:focal

LABEL maintainer Oleh B. <the.art.of.coding@gmail.com>

# Pull in dependencies
RUN apt-get update \
    && apt-get install -y gnupg2 curl

# Install scylla and additional appliances
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 5e08fbd8b5d6ec9c \
    && curl -L --output /etc/apt/sources.list.d/scylla.list http://downloads.scylladb.com/deb/ubuntu/scylla-4.5-focal.list \
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y scylla  openjdk-8-jre-headless \
    && update-java-alternatives --jre-headless -s java-1.8.0-openjdk-amd64 \
    && scylla_dev_mode_setup --developer-mode 1

WORKDIR /usr/src

CMD ["/bin/bash"]
