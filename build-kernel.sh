#!/usr/bin/bash
echo 'Build Running······'
export ARCH=arm64
export SUBARCH=arm64
export PATH="/root/K-Nel-Projects/Toolchain/clang-r399163b/bin:/root/K-Nel-Projects/Toolchain/gcc64/bin:/root/K-Nel-Projects/Toolchain/gcc32/bin:$PATH"

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
make ${args} 2>&1

if [ -s out/arch/arm64/boot/Image.gz ]
then echo 'Build Finished!'
else
echo 'Build Failed!'
exit
fi
if [ -e '/root/K-Nel-Projects/OutFiles/K-Nel-M1721.zip' ]
    then rm '/root/K-Nel-Projects/OutFiles/K-Nel-M1721.zip'
    fi
    if [ -e out/arch/arm64/boot/Image.gz ]
        then
        echo 'Pack Starting······'
        cp -r out/arch/arm64/boot/Image.gz Anykernel3
        cd Anykernel3
        zip -r '/root/K-Nel-Projects/OutFiles/K-Nel-M1721.zip' *
        else
        echo 'Build Failed!'
        exit
    fi
if [ -s '/root/K-Nel-Projects/OutFiles/K-Nel-M1721.zip' ]
  then echo 'Pack Successed!'
      else
        echo 'Pack Failed!'
        exit
fi