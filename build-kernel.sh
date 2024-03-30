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

#build without zram
make ${args} m1721_without_ksu_defconfig
#make ${args} xxxx_defconfig
make ${args} 2>&1 | tee build-without-ksu.log

if [ -s out/arch/arm64/boot/Image.gz ]
then mv out/arch/arm64/boot/Image.gz out/arch/arm64/boot/Image-Without-KSU.gz
else
echo '----- 编译失败！-----'
exit
fi

#build
make ${args} m1721_defconfig
#make ${args} xxxx_defconfig
make ${args} 2>&1 | tee build-standard.log

if [ -s out/arch/arm64/boot/Image.gz ]
then echo '----- 编译结束！ -----'
else
echo '----- 编译失败！-----'
exit
fi

echo '----- 开始打包······ -----'
if [ -s out/arch/arm64/boot/Image.gz ]
then cd Anykernel3
else
echo '----- 打包失败！-----'
exit
fi
mv ../out/arch/arm64/boot/Image.gz ./
if [ -e '../K-Nel M1721 Canary (Auto Make).zip' ]
then rm '../K-Nel M1721 Canary (Auto Make).zip'
fi
zip -r '../K-Nel M1721 Canary (Auto Make).zip' *
rm Image.gz
if [ -e '../K-Nel M1721 Canary Without KSU (Auto Make).zip' ]
then rm '../K-Nel M1721 Canary Without KSU (Auto Make).zip'
fi
mv ../out/arch/arm64/boot/Image-Without-KSU.gz ./Image.gz
zip -r '../K-Nel M1721 Canary Without KSU (Auto Make).zip' *
if [ -s '../K-Nel M1721 Canary (Auto Make).zip' ]
then test
else exit
fi
if [ -s '../K-Nel M1721 Canary Without KSU (Auto Make).zip' ]
then test
else exit
fi
echo '----- 打包结束！ -----'
echo '内核已在当前目录下自动打包，标准版名为“K-Nel M1721 Canary (Auto Make).zip”，无KernelSU版名为“K-Nel M1721 Canary (Auto Make).zip”'
