#/usr/bin/sh
# K-Nel A-Only Installer
# By KeJia

ui_print '安装 K-Nel'
ui_print '安装后在模块列表可能会出现名为 "K-Nel A-Only Installer"的模块，无实际作用，如果出现，请卸载'
ui_print '在部分SU管理器上，脚本输出“安装成功”，而管理器输出错误，提示安装失败，这种情况无须理睬，内核已安装成功'

if test -z '$(grep m1721 /system/build.prop)';
then 
  echo '设备非 魅蓝 Note6，不能安装该内核'
  exit 1
fi

cd $MODPATH/tools/
chmod +x magiskboot

ui_print '提取 Boot 镜像。。。'
dd if=/dev/block/by-name/boot of=$MODPATH/boot.img
ui_print '解包 Boot 镜像。。。'
./magiskboot unpack $MODPATH/boot.img
ui_print '分解并更新内核。。。'
./magiskboot split $MODPATH/Image.gz-dtb
ui_print '打包 Boot 镜像。。。'
./magiskboot repack $MODPATH/boot.img
ui_print '刷入 Boot 镜像。。。'
dd if=new-boot.img of=/dev/block/by-name/boot
ui_print '安装完成'

ui_print '清理中。。。'
rm -rf $MODPATH
