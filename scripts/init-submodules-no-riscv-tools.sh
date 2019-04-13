#! /bin/bash

BASE_DIR=`realpath ${0%/*}/..`
source $BASE_DIR/scripts/general.sh

# Script for faster checkout if you already have installed your RISC-V tools.

cd $BASE_DIR
git config --global submodule.riscv-tools.update none
git submodule update --init --recursive
git config --global --unset submodule.riscv-tools.update

# TODO: Remove FIRRTL patch in next rocket-bump (fixes const prop issue)
git -C $BASE_DIR/rocket-chip/firrtl checkout 9535e03020c6e654dae3ce7e95f4d8649405ce3d
