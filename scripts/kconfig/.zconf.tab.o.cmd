cmd_scripts/kconfig/zconf.tab.o := gcc -Wp,-MD,scripts/kconfig/.zconf.tab.o.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu89   -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 -DCURSES_LOC="<ncurses.h>" -DNCURSES_WIDECHAR=1 -DLOCALE  -Iscripts/kconfig -c -o scripts/kconfig/zconf.tab.o scripts/kconfig/zconf.tab.c

source_scripts/kconfig/zconf.tab.o := scripts/kconfig/zconf.tab.c

deps_scripts/kconfig/zconf.tab.o := \
  /usr/include/stdc-predef.h \
  /usr/include/ctype.h \
  /usr/include/features.h \
  /usr/include/features-time64.h \
  /usr/include/bits/wordsize.h \
  /usr/include/bits/timesize.h \
  /usr/include/sys/cdefs.h \
  /usr/include/bits/long-double.h \
  /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-lp64.h \
  /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h \
  /usr/include/bits/time64.h \
  /usr/include/bits/endian.h \
  /usr/include/bits/endianness.h \
  /usr/include/bits/types/locale_t.h \
  /usr/include/bits/types/__locale_t.h \
  /usr/lib/gcc/aarch64-unknown-linux-gnu/12.1.0/include/stdarg.h \
  /usr/include/stdio.h \
  /usr/include/bits/libc-header-start.h \
  /usr/lib/gcc/aarch64-unknown-linux-gnu/12.1.0/include/stddef.h \
  /usr/include/bits/types/__fpos_t.h \
  /usr/include/bits/types/__mbstate_t.h \
  /usr/include/bits/types/__fpos64_t.h \
  /usr/include/bits/types/__FILE.h \
  /usr/include/bits/types/FILE.h \
  /usr/include/bits/types/struct_FILE.h \
  /usr/include/bits/stdio_lim.h \
  /usr/include/bits/floatn.h \
  /usr/include/bits/floatn-common.h \
  /usr/include/bits/stdio.h \
  /usr/include/stdlib.h \
  /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h \
  /usr/include/sys/types.h \
  /usr/include/bits/types/clock_t.h \
  /usr/include/bits/types/clockid_t.h \
  /usr/include/bits/types/time_t.h \
  /usr/include/bits/types/timer_t.h \
  /usr/include/bits/stdint-intn.h \
  /usr/include/endian.h \
  /usr/include/bits/byteswap.h \
  /usr/include/bits/uintn-identity.h \
  /usr/include/sys/select.h \
  /usr/include/bits/select.h \
  /usr/include/bits/types/sigset_t.h \
  /usr/include/bits/types/__sigset_t.h \
  /usr/include/bits/types/struct_timeval.h \
  /usr/include/bits/types/struct_timespec.h \
  /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/thread-shared-types.h \
  /usr/include/bits/pthreadtypes-arch.h \
  /usr/include/bits/atomic_wide_counter.h \
  /usr/include/bits/struct_mutex.h \
  /usr/include/bits/struct_rwlock.h \
  /usr/include/alloca.h \
  /usr/include/bits/stdlib-bsearch.h \
  /usr/include/bits/stdlib-float.h \
  /usr/include/string.h \
  /usr/include/strings.h \
  /usr/lib/gcc/aarch64-unknown-linux-gnu/12.1.0/include/stdbool.h \
  scripts/kconfig/lkc.h \
    $(wildcard include/config/prefix.h) \
    $(wildcard include/config/list.h) \
    $(wildcard include/config/y.h) \
  scripts/kconfig/expr.h \
    $(wildcard include/config/config.h) \
  /usr/include/assert.h \
  scripts/kconfig/list.h \
  /usr/include/libintl.h \
  /usr/include/locale.h \
  /usr/include/bits/locale.h \
  scripts/kconfig/lkc_proto.h \
  scripts/kconfig/zconf.hash.c \
  scripts/kconfig/zconf.lex.c \
  /usr/include/errno.h \
  /usr/include/bits/errno.h \
  /usr/include/linux/errno.h \
  /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h \
  /usr/include/asm-generic/errno-base.h \
  /usr/lib/gcc/aarch64-unknown-linux-gnu/12.1.0/include-fixed/limits.h \
  /usr/lib/gcc/aarch64-unknown-linux-gnu/12.1.0/include-fixed/syslimits.h \
  /usr/include/limits.h \
  /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h \
  /usr/include/linux/limits.h \
  /usr/include/bits/pthread_stack_min-dynamic.h \
  /usr/include/bits/pthread_stack_min.h \
  /usr/include/bits/posix2_lim.h \
  /usr/include/bits/xopen_lim.h \
  /usr/include/bits/uio_lim.h \
  /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h \
  /usr/include/bits/environments.h \
  /usr/include/bits/confname.h \
  /usr/include/bits/getopt_posix.h \
  /usr/include/bits/getopt_core.h \
  /usr/include/bits/unistd_ext.h \
  scripts/kconfig/util.c \
  scripts/kconfig/confdata.c \
    $(wildcard include/config/autoconfig.h) \
    $(wildcard include/config/overwriteconfig.h) \
    $(wildcard include/config/autoheader.h) \
    $(wildcard include/config/tristate.h) \
    $(wildcard include/config/probability.h) \
  /usr/include/sys/stat.h \
  /usr/include/bits/stat.h \
  /usr/include/bits/struct_stat.h \
  /usr/include/fcntl.h \
  /usr/include/bits/fcntl.h \
  /usr/include/bits/fcntl-linux.h \
  /usr/include/time.h \
  /usr/include/bits/time.h \
  /usr/include/bits/types/struct_tm.h \
  /usr/include/bits/types/struct_itimerspec.h \
  scripts/kconfig/expr.c \
  scripts/kconfig/symbol.c \
  /usr/include/regex.h \
  /usr/include/sys/utsname.h \
  /usr/include/bits/utsname.h \
  scripts/kconfig/menu.c \

scripts/kconfig/zconf.tab.o: $(deps_scripts/kconfig/zconf.tab.o)

$(deps_scripts/kconfig/zconf.tab.o):
