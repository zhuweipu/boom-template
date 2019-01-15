#! /bin/bash

BASE_DIR=`realpath ${0%/*}/..`
source $BASE_DIR/scripts/general.sh

# Script for faster checkout if you already have installed your RISC-V tools.

cd $BASE_DIR
# top-level
git submodule update --init
# rocket-chip (skip tools)
(cd rocket-chip && git submodule update --init --recursive)
# torture submodules
(cd torture && git submodule update --init --recursive)
