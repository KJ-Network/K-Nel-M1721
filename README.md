# K-Nel-M1721
![Alt text](/logo.png?raw=true "A Custom Kernel Base For Meizu M6 Note")
* 本仓库为K-Nel M1721的仓库
* 该仓库仅存储为魅蓝Note6编译的K-Nel内核
* 内核维护者（K-Nel Developers）：
* 1. [科家（KNKJ55）](http://kjblogs.rovn.ink/about/KeJia/)
* 2. 酷安@[刘健军1](http://www.coolapk.com/u/23303336)
* 编译使用设备：红米 K40
* 编译使用系统：Chroot - Arch Linux Aarch64
* 测试通过机型：魅蓝 Note6
* 内核目标机型：魅蓝 Note6
* 已实现功能：
* 1.KernelSU
* 2.Kallsyms
* 3.Kernel-base Virtual Machine
* 4.Zram（默认压缩格式为lz4，有zstd，lz4hc等压缩格式，支持回写）
* 5.Lxc-Docker
* 如果您有需要，可以使用本仓库的源码重新编译K-Nel，您也可以基于K-Nel制作衍生版内核（自己用随意，公开出来请标注“基于 K-Nel 构建”），但您需要自备工具链（本内核需要clang与gcc交叉编译）
# 注意！
# 刷机有风险，搞机需谨慎！
# 本仓库中的内核仅适用于魅蓝Note6安卓11以上的第三方ROM,不支持如Flyme的安卓7一类使用Linux Kernel 3.x的机型！
