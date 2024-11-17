cmd_scripts/basic/fixdep := gcc -Wp,-MD,scripts/basic/.fixdep.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu89 -Wno-unused-value -Wno-unused-parameter -Wno-missing-field-initializers     -o scripts/basic/fixdep scripts/basic/fixdep.c  

source_scripts/basic/fixdep := scripts/basic/fixdep.c

deps_scripts/basic/fixdep := \
    $(wildcard include/config/his/driver.h) \
    $(wildcard include/config/my/option.h) \
    $(wildcard include/config/foo.h) \
    $(wildcard include/config/boom.h) \
  /data/data/com.termux/files/usr/include/sys/types.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/stddef.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/__stddef_header_macro.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/__stddef_ptrdiff_t.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/__stddef_size_t.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/__stddef_wchar_t.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/__stddef_null.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/__stddef_offsetof.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/stdint.h \
  /data/data/com.termux/files/usr/include/stdint.h \
  /data/data/com.termux/files/usr/include/bits/wchar_limits.h \
  /data/data/com.termux/files/usr/include/sys/cdefs.h \
  /data/data/com.termux/files/usr/include/android/versioning.h \
  /data/data/com.termux/files/usr/include/android/api-level.h \
  /data/data/com.termux/files/usr/include/bits/get_device_api_level_inlines.h \
  /data/data/com.termux/files/usr/include/android/ndk-version.h \
  /data/data/com.termux/files/usr/include/linux/types.h \
  /data/data/com.termux/files/usr/include/aarch64-linux-android/asm/types.h \
  /data/data/com.termux/files/usr/include/asm-generic/types.h \
  /data/data/com.termux/files/usr/include/asm-generic/int-ll64.h \
  /data/data/com.termux/files/usr/include/aarch64-linux-android/asm/bitsperlong.h \
  /data/data/com.termux/files/usr/include/asm-generic/bitsperlong.h \
  /data/data/com.termux/files/usr/include/linux/posix_types.h \
  /data/data/com.termux/files/usr/include/linux/stddef.h \
  /data/data/com.termux/files/usr/include/linux/compiler_types.h \
  /data/data/com.termux/files/usr/include/linux/compiler.h \
  /data/data/com.termux/files/usr/include/aarch64-linux-android/asm/posix_types.h \
  /data/data/com.termux/files/usr/include/asm-generic/posix_types.h \
  /data/data/com.termux/files/usr/include/bits/pthread_types.h \
  /data/data/com.termux/files/usr/include/sys/stat.h \
  /data/data/com.termux/files/usr/include/bits/timespec.h \
  /data/data/com.termux/files/usr/include/linux/stat.h \
  /data/data/com.termux/files/usr/include/sys/mman.h \
  /data/data/com.termux/files/usr/include/linux/memfd.h \
  /data/data/com.termux/files/usr/include/asm-generic/hugetlb_encode.h \
  /data/data/com.termux/files/usr/include/linux/mman.h \
  /data/data/com.termux/files/usr/include/aarch64-linux-android/asm/mman.h \
  /data/data/com.termux/files/usr/include/asm-generic/mman.h \
  /data/data/com.termux/files/usr/include/asm-generic/mman-common.h \
  /data/data/com.termux/files/usr/include/linux/uio.h \
  /data/data/com.termux/files/usr/include/unistd.h \
  /data/data/com.termux/files/usr/include/sys/select.h \
  /data/data/com.termux/files/usr/include/linux/time.h \
  /data/data/com.termux/files/usr/include/linux/time_types.h \
  /data/data/com.termux/files/usr/include/signal.h \
  /data/data/com.termux/files/usr/include/aarch64-linux-android/asm/sigcontext.h \
  /data/data/com.termux/files/usr/include/aarch64-linux-android/asm/sve_context.h \
  /data/data/com.termux/files/usr/include/bits/signal_types.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/limits.h \
  /data/data/com.termux/files/usr/include/limits.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/float.h \
  /data/data/com.termux/files/usr/include/linux/limits.h \
  /data/data/com.termux/files/usr/include/bits/posix_limits.h \
  /data/data/com.termux/files/usr/include/linux/signal.h \
  /data/data/com.termux/files/usr/include/aarch64-linux-android/asm/signal.h \
  /data/data/com.termux/files/usr/include/asm-generic/signal.h \
  /data/data/com.termux/files/usr/include/asm-generic/signal-defs.h \
  /data/data/com.termux/files/usr/include/aarch64-linux-android/asm/siginfo.h \
  /data/data/com.termux/files/usr/include/asm-generic/siginfo.h \
  /data/data/com.termux/files/usr/include/sys/ucontext.h \
  /data/data/com.termux/files/usr/include/sys/user.h \
  /data/data/com.termux/files/usr/include/bits/page_size.h \
  /data/data/com.termux/files/usr/include/bits/fcntl.h \
  /data/data/com.termux/files/usr/include/bits/getentropy.h \
  /data/data/com.termux/files/usr/include/bits/getopt.h \
  /data/data/com.termux/files/usr/include/bits/ioctl.h \
  /data/data/com.termux/files/usr/include/bits/lockf.h \
  /data/data/com.termux/files/usr/include/bits/seek_constants.h \
  /data/data/com.termux/files/usr/include/bits/sysconf.h \
  /data/data/com.termux/files/usr/include/android/legacy_unistd_inlines.h \
  /data/data/com.termux/files/usr/include/bits/swab.h \
  /data/data/com.termux/files/usr/include/fcntl.h \
  /data/data/com.termux/files/usr/include/linux/fadvise.h \
  /data/data/com.termux/files/usr/include/linux/falloc.h \
  /data/data/com.termux/files/usr/include/linux/fcntl.h \
  /data/data/com.termux/files/usr/include/aarch64-linux-android/asm/fcntl.h \
  /data/data/com.termux/files/usr/include/asm-generic/fcntl.h \
  /data/data/com.termux/files/usr/include/bits/flock64.h \
  /data/data/com.termux/files/usr/include/bits/flock.h \
  /data/data/com.termux/files/usr/include/linux/openat2.h \
  /data/data/com.termux/files/usr/include/string.h \
  /data/data/com.termux/files/usr/include/xlocale.h \
  /data/data/com.termux/files/usr/include/bits/strcasecmp.h \
  /data/data/com.termux/files/usr/include/stdlib.h \
  /data/data/com.termux/files/usr/include/alloca.h \
  /data/data/com.termux/files/usr/include/bits/wait.h \
  /data/data/com.termux/files/usr/include/linux/wait.h \
  /data/data/com.termux/files/usr/include/malloc.h \
  /data/data/com.termux/files/usr/include/stdio.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/stdarg.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/__stdarg_header_macro.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/__stdarg___gnuc_va_list.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/__stdarg_va_list.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/__stdarg_va_arg.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/__stdarg___va_copy.h \
  /data/data/com.termux/files/usr/lib/clang/19/include/__stdarg_va_copy.h \
  /data/data/com.termux/files/usr/include/android/legacy_stdlib_inlines.h \
  /data/data/com.termux/files/usr/include/ctype.h \
  /data/data/com.termux/files/usr/include/arpa/inet.h \
  /data/data/com.termux/files/usr/include/netinet/in.h \
  /data/data/com.termux/files/usr/include/endian.h \
  /data/data/com.termux/files/usr/include/sys/endian.h \
  /data/data/com.termux/files/usr/include/netinet/in6.h \
  /data/data/com.termux/files/usr/include/linux/in6.h \
  /data/data/com.termux/files/usr/include/linux/libc-compat.h \
    $(wildcard include/config/data.h) \
  /data/data/com.termux/files/usr/include/sys/socket.h \
  /data/data/com.termux/files/usr/include/aarch64-linux-android/asm/socket.h \
  /data/data/com.termux/files/usr/include/asm-generic/socket.h \
  /data/data/com.termux/files/usr/include/aarch64-linux-android/asm/sockios.h \
  /data/data/com.termux/files/usr/include/asm-generic/sockios.h \
  /data/data/com.termux/files/usr/include/linux/sockios.h \
  /data/data/com.termux/files/usr/include/bits/sockaddr_storage.h \
  /data/data/com.termux/files/usr/include/bits/sa_family_t.h \
  /data/data/com.termux/files/usr/include/linux/in.h \
  /data/data/com.termux/files/usr/include/bits/ip_msfilter.h \
  /data/data/com.termux/files/usr/include/bits/in_addr.h \
  /data/data/com.termux/files/usr/include/bits/ip_mreq_source.h \
  /data/data/com.termux/files/usr/include/linux/socket.h \
  /data/data/com.termux/files/usr/include/aarch64-linux-android/asm/byteorder.h \
  /data/data/com.termux/files/usr/include/linux/byteorder/little_endian.h \
  /data/data/com.termux/files/usr/include/linux/swab.h \
  /data/data/com.termux/files/usr/include/aarch64-linux-android/asm/swab.h \
  /data/data/com.termux/files/usr/include/asm-generic/swab.h \
  /data/data/com.termux/files/usr/include/linux/ipv6.h \

scripts/basic/fixdep: $(deps_scripts/basic/fixdep)

$(deps_scripts/basic/fixdep):
