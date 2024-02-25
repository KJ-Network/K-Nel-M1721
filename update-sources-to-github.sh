#!/usr/bin/bash
git lfs track out/vmlinux
git add .
git submodule add https://github.com/tiann/kernelsu.git KernelSU
git commit -m "Automatically update source code using scripts"
git push origin Kernel