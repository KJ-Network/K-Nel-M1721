# K-Nel M1721
![Alt text](/android/K-Nel/logo.png?raw=true "A Custom Kernel Base For Meizu M6 Note")
* 本仓库为 K-Nel M1721 的仓库，该仓库仅存储为 魅蓝 Note6 编译的K-Nel内核
* 内核维护者（K-Nel Developers）：
* 1. [科家（KNKJ55）](http://knkj55.github.io/about/kejia/)
* 2. 酷安@[刘健军1](http://www.coolapk.com/u/23303336)
* 编译环境：Github Actions - Ubuntu 22.04
* 工具链：Proton Clang, Linaro Gcc (armhf), Linaro Gcc (aarch64)
* 测试机型：魅蓝 Note6
* 内核特色：
* 1.eBPF
* 2.完整Kallsyms
* 3.Kernel-base Virtual Machine
* 4.Zram（默认压缩格式为lz4，有zstd，lz4hc等压缩格式，支持回写）
* 5.Lxc Docker
* 6.Kali Linux Nethunter
* 7.Re:kernel
* 8.le9ec
* 如果您有需要，可以使用本仓库的源码重新编译 K-Nel ，您也可以基于 K-Nel 制作衍生版内核（自己用随意，公开出来请标注“基于 K-Nel 构建”）
* PS: 刷机有风险，搞机需谨慎！
---
* 该内核基于 github@meizucustoms/android_kernel_meizu_m1721 修改
* 感谢 github@tdrkDev
