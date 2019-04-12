#! /bin/bash

BASE_DIR=`realpath ${0%/*}/..`
source $BASE_DIR/scripts/general.sh

echo "cd rocket-chip/riscv-tools"
cd rocket-chip/riscv-tools

./build.sh
retVal=$?
if [ ! $retVal -eq 0 ]; then
    echo "A error has been encountered while building the toolchain."
fi
exit $retVal
