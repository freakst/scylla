FROM ubuntu:focal

LABEL maintainer Oleh B. <the.art.of.coding@gmail.com>

# Pull in dependencies
RUN apt-get update \
    && apt-get install -y gnupg2 curl systemctl

# Install scylla and additional appliances
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 5e08fbd8b5d6ec9c \
    && curl -L --output /etc/apt/sources.list.d/scylla.list http://downloads.scylladb.com/deb/ubuntu/scylla-4.4-focal.list \
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y scylla  openjdk-8-jre-headless \
    && update-java-alternatives --jre-headless -s java-1.8.0-openjdk-amd64

# Generate basic config.
RUN scylla_setup --no-raid-setup --no-kernel-check --no-verify-package --no-enable-service  --no-ntp-setup --no-coredump-setup --no-sysconfig-setup --io-setup 0 --no-node-exporter --no-cpuscaling-setup --no-fstrim-setup --no-memory-setup \
#    && scylla_dev_mode_setup --developer-mode 1


WORKDIR /usr/bin

CMD ["/usr/bin/scylla", "--options-file", "/etc/scylla/scylla.yaml", "--developer-mode", "1", "--smp", "1"]
