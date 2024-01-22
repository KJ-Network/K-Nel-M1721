
#! /bin/bash
#env
#clang版本9 , binutils版本2.27
export ARCH=arm64
export SUBARCH=arm64
export PATH="/root/Toolchain/clang-r399163b/bin:/root/Toolchain/gcc64/bin:/root/Toolchain/gcc32/bin:$PATH"

args="-j6 \
ARCH=arm64 \
SUBARCH=arm64 \
O=out \
CC=clang \
CROSS_COMPILE=aarch64-linux-android- \
CROSS_COMPILE_ARM32=arm-linux-androideabi- \
CLANG_TRIPLE=aarch64-linux-gnu- "

#clean
make clean && make mrproper
rm kernel.log && rm -rf out 
mkdir -p out

#build
make ${args} m1721_defconfig
#make ${args} xxxx_defconfig
make ${args} 2>&1 | tee kernel.log
