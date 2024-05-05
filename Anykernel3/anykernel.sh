# AnyKernel3 Ramdisk Mod Script
# osm0sis @ xda-developers

## AnyKernel setup
# begin properties
properties() { '
kernel.string=K-Nel M1721
do.devicecheck=1
do.modules=0
do.systemless=1
do.cleanup=1
do.cleanuponabort=0
device.name1=m1721
device.name2=m6note
'; } # end properties

# shell variables
block=/dev/block/bootdevice/by-name/boot;
is_slot_device=0;
ramdisk_compression=auto;


## AnyKernel methods (DO NOT CHANGE)
# import patching functions/variables - see for reference
. tools/ak3-core.sh;

# Display if using Flyme or a custom ROM
is_flyme="$(file_getprop /system/build.prop 'ro.flyme.ui.version.code')"
if [[ -z $is_flyme ]]; then
    ui_print "你正在使用第三方Rom"
    ui_print "请确保您的Rom为安卓11及以上，否则内核可能无效"
else
    ui_print "你正在使用Flyme！"
    ui_print "本内核仅支持使用安卓11及以上Rom的设备"
    exit
fi

## AnyKernel install
dump_boot;

write_boot;
## end install

