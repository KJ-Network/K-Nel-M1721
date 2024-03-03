#!/usr/bin/bash
if [ -s out ]
then rm -rf out
fi
git add .
git submodule add https://github.com/tiann/kernelsu.git KernelSU
git commit -m "Automatically update source code using scripts"
git push origin Kernel