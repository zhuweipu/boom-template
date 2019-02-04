#! /bin/bash

# Script for faster checkout if you already have installed your RISC-V tools.

git config --global submodule.riscv-tools.update none
git submodule update --init --recursive
git config --global --unset submodule.riscv-tools.update
