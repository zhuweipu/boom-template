#! /bin/bash

BASE_DIR=`realpath ${0%/*}/..`
source $BASE_DIR/scripts/general.sh

# Script for faster checkout if you already have installed your RISC-V tools.

cd $BASE_DIR
git config --global submodule.riscv-tools.update none
git submodule update --init --recursive
git config --global --unset submodule.riscv-tools.update
