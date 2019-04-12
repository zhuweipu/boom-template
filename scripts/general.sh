#! /bin/bash

BASE_DIR=`realpath ${0%/*}/..`
TOOLS_DIR=$1

ROCKET_DIR=$BASE_DIR/rocket-chip
if [[ $TOOLS_DIR == "" ]]; then
  TOOLS_DIR=`realpath $BASE_DIR/../riscv-tools`
fi
TOOLS_HASH=`cat $ROCKET_DIR/riscv-tools.hash`

set -ex
