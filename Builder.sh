#!/usr/bin/bash
echo 'Please enter a version name:' 
read -r VERSION
export LOCALVERSION=$VERSION
args="-j6 \
ARCH=arm64 \
SUBARCH=arm64 \
O=out \
CC=/home/kejia/Toolchain/clang/bin/clang \
CROSS_COMPILE=/home/kejia/Toolchain/gcc64/bin/aarch64-linux-android- \
CROSS_COMPILE_ARM32=/home/kejia/Toolchain/gcc32/bin/arm-linux-androideabi- \
CLANG_TRIPLE=aarch64-linux-gnu- \
CONFIG_DEBUG_SECTION_MISMATCH=y "

echo 'Building······'
#build vanilla
make ${args} m1721_defconfig
make ${args} 2>&1
if [ -e out/arch/arm64/boot/Image.gz-dtb ]
then
    mv out/arch/arm64/boot/Image.gz-dtb out/KernelImage-Vanilla-dtb
    echo  'Build Vanilla Successed'
else
    echo  'Build Failed!'
    exit
fi

#build ksu
make ${args} m1721_ksu_defconfig
make ${args} 2>&1
if [ -e out/arch/arm64/boot/Image.gz-dtb ]
then
    mv out/arch/arm64/boot/Image.gz-dtb out/KernelImage-KSU-dtb
    echo  'Build KSU Successed'
else
    echo  'Build Failed!'
    exit
fi

echo 'Build Finished'

#pack kernel
if [ -e out/K-Nel-M1721.zip ]
    then rm out/K-Nel-M1721.zip
fi
#pack vanilla
if [ -e out/KernelImage-Vanilla-dtb ]
    then
    echo 'Pack Starting······'
    cp -r out/KernelImage-Vanilla-dtb Anykernel3/Image.gz-dtb
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
if [ -e out/KernelImage-KSU-dtb ]
    then
    echo 'Pack Starting······'
    cp -r out/KernelImage-KSU-dtb Anykernel3/Image.gz
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
echo 'Pack Finished!'
