#! /bin/bash

BASE_DIR=`realpath ${0%/*}/..`
source $BASE_DIR/scripts/general.sh

$BASE_DIR/scripts/init-submodules-no-riscv-tools.sh

if [[ ! -d $TOOLS_DIR ]]; then
  git clone https://github.com/riscv/riscv-tools.git $TOOLS_DIR
fi
git -C $TOOLS_DIR checkout $TOOLS_HASH
git -C $TOOLS_DIR submodule update --init --recursive
