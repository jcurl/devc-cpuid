#!/bin/bash

function build_qnx() {
    echo -e "\n\e[33m==== \e[93mBUILDING FOR QNX\e[0m"
    CWD=`pwd`

    if [ -d build_qnx ]; then
      rm -rf build_qnx
    fi

    cmake -S . -B build_qnx -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=toolchain/qcc710_x86_64 && \
    cd build_qnx && \
    make

    cd ${CWD}
}

function build_linux() {
    echo -e "\n\e[33m==== \e[93mBUILDING FOR LINUX\e[0m"
    CWD=`pwd`

    if [ -d build_lin ]; then
      rm -rf build_lin
    fi

    cmake -S . -B build_lin -DSANITIZE_ADDRESS=on && \
    cd build_lin && \
    make

    cd ${CWD}
}

function build_linux_release() {
    echo -e "\n\e[33m==== \e[93mBUILDING FOR LINUX release\e[0m"
    CWD=`pwd`

    if [ -d build_lin_rel ]; then
      rm -rf build_lin_rel
    fi

    cmake -S . -B build_lin_rel -DCMAKE_BUILD_TYPE=Release && \
    cd build_lin_rel && \
    make

    cd ${CWD}
}

function build_clang() {
    echo -e "\n\e[33m==== \e[93mBUILDING FOR LINUX-clang\e[0m"
    CWD=`pwd`

    if [ -d build_clang ]; then
      rm -rf build_clang
    fi

    cmake -S . -B build_clang -DCMAKE_TOOLCHAIN_FILE=toolchain/clang -DSANITIZE_ADDRESS=on && \
    cd build_clang && \
    make

    cd ${CWD}
}

build_qnx
build_linux
build_linux_release
build_clang
