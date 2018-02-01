#!/usr/bin/env bash
set -x
git config --global submodule.riscv-tools.update none
git submodule update --init --recursive
git config --global --unset submodule.riscv-tools.update
cd rocket-chip/firrtl && sbt publishLocal
