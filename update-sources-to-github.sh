#!/usr/bin/bash
if [ -s out ]
then rm -rf out
fi
git submodule add https://github.com/tiann/kernelsu.git KernelSU
git add .
git commit -m "Automatically update source code using scripts"
git push origin Kernel