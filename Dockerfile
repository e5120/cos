# setting base image
FROM debian:jessie

MAINTAINER tetsuya

# install tools
USER root
RUN set -x && \
    apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y sudo && \
    apt-get install -y curl && \
    apt-get install -y wget && \
    apt-get install -y make && \
    apt-get install -y gcc && \
    apt-get install -y nasm && \
    apt-get install -y mtools && \
    apt-get install -y git && \
    apt-get install -y file && \
    apt-get install -y qemu && \
    apt-get install -y binutils && \
    apt-get install -y build-essential && \
    apt-get install -y nkf

# add user and enable `sudo` command
RUN set -x && \
    groupadd -g 1000 tetsuya && \
    useradd -g tetsuya --create-home -G sudo -m -s /bin/bash tetsuya && \
    echo 'tetsuya:password' | chpasswd

ENV HOME=/home/tetsuya
USER tetsuya
WORKDIR /home/tetsuya

# install z-tools
#RUN set -x && \
#    wget http://hrb.osask.jp/z_tools.tar.bz2 -O $HOME/z_tools.tar.bz2 && \
#    cd $HOME && \
#    tar -jxvf z_tools.tar.bz2 && \
#    rm z_tools.tar.bz2

RUN chown -R tetsuya:tetsuya $HOME/*

# for shared volumes
RUN mkdir shared
