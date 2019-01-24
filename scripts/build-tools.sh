#! /bin/bash

# top-level
git submodule update --init
# rocket-chip (skip tools)
(cd rocket-chip && git submodule update --init riscv-tools)
(cd rocket-chip/riscv-tools && git submodule update --init --recursive riscv-isa-sim riscv-fesvr riscv-pk riscv-opcodes riscv-tests riscv-gnu-toolchain riscv-openocd)

echo "cd rocket-chip/riscv-tools"
cd rocket-chip/riscv-tools

./build.sh
retVal=$?
if [ ! $retVal -eq 0 ]; then
    echo "A error has been encountered while building the toolchain."
fi
exit $retVal
