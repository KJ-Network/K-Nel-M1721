#! /bin/bash
echo '----- 编译开始······ -----'
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


#build
make ${args} m1721_defconfig
#make ${args} xxxx_defconfig
make ${args} 2>&1 | tee build.log

echo '----- 编译结束！ -----'
echo '----- 开始打包······ -----'
cd Anykernel3
cp ../out/arch/arm64/boot/Image.gz ./
zip -r '../K-Nel M1721.zip' *
echo '----- 打包结束！ -----'
echo '内核已在当前目录下自动打包，文件名“K-Nel M1721.zip”'