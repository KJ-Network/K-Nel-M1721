#!/usr/bin/bash
echo 'Build Running······'
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

#Build Kernel
#build vanilla
make ${args} m1721_defconfig
make ${args} 2>&1
if [ -e out/arch/arm64/boot/Image.gz ]
then
    mv out/arch/arm64/boot/Image.gz out/Image-Vanilla.gz
    echo  'Build Vanilla Successed'
else
    echo  'Build Failed!'
    exit
fi

#build ksu
make ${args} m1721_ksu_defconfig
make ${args} 2>&1
if [ -e out/arch/arm64/boot/Image.gz ]
then
    mv out/arch/arm64/boot/Image.gz out/Image-KSU.gz
    echo  'Build KSU Successed'
else
    echo  'Build Failed!'
    exit
fi

echo 'Build Successed'

#pack kernel
if [ -e out/K-Nel-M1721.zip ]
    then rm out/K-Nel-M1721.zip
fi
#pack vanilla
if [ -e out/Image-Vanilla.gz ]
    then
    echo 'Pack Starting······'
    cp -r out/Image-Vanilla.gz Anykernel3/Image.gz
    cd Anykernel3
    zip -r ../out/K-Nel-M1721.zip *
    cd ..
    else
    echo 'Build Failed!'
    exit
fi
if [ -s out/K-Nel-M1721.zip ]
  then echo 'Pack Vanilla Successed!'
      else
        echo 'Pack Failed!'
        exit
fi

#pack ksu
if [ -e out/Image-KSU.gz ]
    then
    echo 'Pack Starting······'
    cp -r out/Image-KSU.gz Anykernel3/Image.gz
    cd Anykernel3
    zip -r ../out/K-Nel-M1721-KSU.zip *
    cd ..
    else
    echo 'Build Failed!'
    exit
fi
if [ -s out/K-Nel-M1721-KSU.zip ]
  then echo 'Pack KSU Successed!'
      else
        echo 'Pack Failed!'
        exit
fi
echo 'Pack Successed!'
