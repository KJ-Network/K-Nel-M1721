ifeq ($(PLATFORM_VERSION_CODENAME),)
ifneq ($(CONFIG_MACH_MEIZU),y)
dtb-$(CONFIG_ARCH_QCOM)	+= apq8016-sbc.dtb msm8916-mtp.dtb
dtb-$(CONFIG_ARCH_QCOM)	+= msm8996-mtp.dtb
dtb-$(CONFIG_ARCH_QCOM)	+= apq8096-db820c.dtb
endif
endif

ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)

dtbo-$(CONFIG_ARCH_QM215) +=qm215-qrd-overlay.dtbo \
	qcm2150-qrd-overlay.dtbo
dtbo-$(CONFIG_ARCH_QM215) +=qm215-qrd-smb1360-overlay.dtbo

dtbo-$(CONFIG_ARCH_MSM8953) += msm8953-mtp-overlay.dtbo \
	msm8953-cdp-overlay.dtbo \
	msm8953-rcm-overlay.dtbo \
	msm8953-ipc-overlay.dtbo \
	msm8953-qrd-overlay.dtbo \
	msm8953-iot-mtp-overlay.dtbo \
	msm8953-ext-codec-mtp-overlay.dtbo \
	msm8953-ext-codec-rcm-overlay.dtbo \
	msm8953-cdp-1200p-overlay.dtbo

apq8017-mtp-overlay.dtbo-base := apq8017-pmi8950.dtb \
	apq8017-pmi8937.dtb

apq8017-cdp-overlay.dtbo-base := apq8017-pmi8950.dtb \
	apq8017-pmi8937.dtb

apq8017-cdp-wcd-rome-overlay.dtbo-base := apq8017-pmi8950.dtb \
	apq8017-pmi8937.dtb

qm215-qrd-overlay.dtbo-base := qm215.dtb
qcm2150-qrd-overlay.dtbo-base := qcm2150.dtb
qm215-qrd-smb1360-overlay.dtbo-base := qm215.dtb

msm8953-mtp-overlay.dtbo-base := sdm450.dtb \
	msm8953.dtb \
	apq8053.dtb \
	sda450.dtb \
	msm8953-pmi8940.dtb \
	msm8953-pmi8937.dtb \
	sdm450-pmi8940.dtb \
	sdm450-pmi8937.dtb
msm8953-cdp-overlay.dtbo-base := sdm450.dtb \
	msm8953.dtb \
	apq8053.dtb \
	sda450.dtb \
	msm8953-pmi8940.dtb \
	msm8953-pmi8937.dtb
msm8953-rcm-overlay.dtbo-base := sdm450.dtb \
	msm8953.dtb \
	apq8053.dtb
msm8953-ipc-overlay.dtbo-base := msm8953.dtb \
	apq8053.dtb
msm8953-qrd-overlay.dtbo-base := sdm450.dtb \
	msm8953.dtb
msm8953-iot-mtp-overlay.dtbo-base := sdm450.dtb \
	msm8953.dtb \
	apq8053.dtb
msm8953-ext-codec-mtp-overlay.dtbo-base := msm8953.dtb \
	apq8053.dtb \
	msm8953-pmi8940.dtb \
	msm8953-pmi8937.dtb
msm8953-ext-codec-rcm-overlay.dtbo-base := msm8953.dtb \
	apq8053.dtb
msm8953-cdp-1200p-overlay.dtbo-base := msm8953.dtb

else
ifeq ($(CONFIG_MACH_MEIZU_MSM8953),y)
dtb-$(CONFIG_MACH_MEIZU_M1721) += msm8953-qrd-sku3-m1721.dtb
else
dtb-$(CONFIG_ARCH_MSM8953) += msm8953-cdp.dtb \
	msm8953-mtp.dtb \
	msm8953-ext-codec-mtp.dtb \
	msm8953-qrd-sku3.dtb \
	msm8953-rcm.dtb \
	apq8053-rcm.dtb \
	msm8953-ext-codec-rcm.dtb \
	apq8053-cdp.dtb \
	apq8053-ipc.dtb \
	msm8953-ipc.dtb \
	apq8053-mtp.dtb \
	apq8053-ext-audio-mtp.dtb \
	apq8053-ext-codec-rcm.dtb \
	msm8953-cdp-1200p.dtb \
	msm8953-iot-mtp.dtb \
	apq8053-iot-mtp.dtb \
	apq8053-lite-dragon-v1.0.dtb \
	apq8053-lite-dragon-v2.0.dtb \
	apq8053-lite-dragon-v2.1.dtb \
	apq8053-lite-dragon-v2.2.dtb \
	apq8053-lite-dragon-v2.2.1.dtb \
	apq8053-lite-dragon-v2.3.dtb \
	apq8053-lite-dragon-v2.4.dtb \
	apq8053-lat-concam-dev.dtb \
	apq8053-lat-concam-proto.dtb \
	msm8953-pmi8940-cdp.dtb \
	msm8953-pmi8940-mtp.dtb \
	msm8953-pmi8937-cdp.dtb \
	msm8953-pmi8937-mtp.dtb \
	msm8953-pmi8940-ext-codec-mtp.dtb \
	msm8953-pmi8937-ext-codec-mtp.dtb \
	msm8953-pmi632-cdp-s2.dtb \
	apq8053-batcam.dtb
endif

dtb-$(CONFIG_ARCH_MSM8937) += msm8937-pmi8950-mtp.dtb \
	msm8937-interposer-sdm439-cdp.dtb \
	msm8937-interposer-sdm439-mtp.dtb \
	msm8937-interposer-sdm439-qrd.dtb \
	msm8937-interposer-sdm429-cdp.dtb \
	msm8937-interposer-sdm429-mtp.dtb

dtb-$(CONFIG_ARCH_QM215) += qm215-qrd.dtb \
	qcm2150-qrd.dtb
dtb-$(CONFIG_ARCH_QM215) += qm215-qrd-smb1360.dtb

endif

always		:= $(dtb-y)
subdir-y	:= $(dts-dirs)
clean-files	:= *.dtb *.dtbo
