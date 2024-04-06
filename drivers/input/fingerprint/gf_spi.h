/*
 * driver definition for sensor driver
 *
 * Coypright (c) 2017 Goodix
 */
#ifndef __GF_SPI_H
#define __GF_SPI_H

#include <linux/types.h>
#include <linux/notifier.h>
/**********************************************************/
enum FP_MODE{
	GF_IMAGE_MODE = 0,
	GF_KEY_MODE,
	GF_SLEEP_MODE,
	GF_FF_MODE,
	GF_DEBUG_MODE = 0x56
};

#define GF_NAV_INPUT_UP		   	    KEY_UP
#define GF_NAV_INPUT_DOWN		    KEY_DOWN
#define GF_NAV_INPUT_LEFT	        KEY_LEFT
#define GF_NAV_INPUT_RIGHT		    KEY_RIGHT
#define GF_NAV_INPUT_CLICK		    KEY_VOLUMEDOWN
#define GF_NAV_INPUT_DOUBLE_CLICK	KEY_VOLUMEUP
#define GF_NAV_INPUT_LONG_PRESS		KEY_SEARCH
#define GF_NAV_INPUT_HEAVY		    KEY_CHAT
#define GF_KEY_INPUT_HOME		    KEY_HOME
#define GF_KEY_INPUT_MENU		    KEY_MENU
#define GF_KEY_INPUT_BACK		    KEY_BACK
#define GF_KEY_INPUT_POWER		    KEY_POWER
#define GF_KEY_INPUT_CAMERA		    KEY_CAMERA

typedef enum gf_nav_event {
	GF_NAV_NONE = 0,
	GF_NAV_FINGER_UP,
	GF_NAV_FINGER_DOWN,
	GF_NAV_UP,
	GF_NAV_DOWN,
	GF_NAV_LEFT,
	GF_NAV_RIGHT,
	GF_NAV_CLICK,
	GF_NAV_HEAVY,
	GF_NAV_LONG_PRESS,
	GF_NAV_DOUBLE_CLICK,
} gf_nav_event_t;

typedef enum gf_key_event {
	GF_KEY_NONE = 0,
	GF_KEY_HOME,
	GF_KEY_POWER,
	GF_KEY_MENU,
	GF_KEY_BACK,
	GF_KEY_CAMERA,
    GF_KEY_LONG_PRESS,
} gf_key_event_t;

struct gf_key {
	enum gf_key_event key;
	uint32_t value;   /* key down = 1, key up = 0 */
};

struct gf_key_map {
	unsigned int type;
	unsigned int code;
};

struct gf_ioc_chip_info {
	unsigned char vendor_id;
	unsigned char mode;
	unsigned char operation;
	unsigned char reserved[5];
};

#define GF_IOC_MAGIC    'g'
#define GF_IOC_INIT             _IOR(GF_IOC_MAGIC, 0, uint8_t)
#define GF_IOC_EXIT             _IO(GF_IOC_MAGIC, 1)
#define GF_IOC_RESET            _IO(GF_IOC_MAGIC, 2)
#define GF_IOC_ENABLE_IRQ       _IO(GF_IOC_MAGIC, 3)
#define GF_IOC_DISABLE_IRQ      _IO(GF_IOC_MAGIC, 4)
#define GF_IOC_ENABLE_SPI_CLK   _IOW(GF_IOC_MAGIC, 5, uint32_t)
#define GF_IOC_DISABLE_SPI_CLK  _IO(GF_IOC_MAGIC, 6)
#define GF_IOC_ENABLE_POWER     _IO(GF_IOC_MAGIC, 7)
#define GF_IOC_DISABLE_POWER    _IO(GF_IOC_MAGIC, 8)
#define GF_IOC_INPUT_KEY_EVENT  _IOW(GF_IOC_MAGIC, 9, struct gf_key)
#define GF_IOC_ENTER_SLEEP_MODE _IO(GF_IOC_MAGIC, 10)
#define GF_IOC_GET_FW_INFO      _IOR(GF_IOC_MAGIC, 11, uint8_t)
#define GF_IOC_REMOVE           _IO(GF_IOC_MAGIC, 12)
#define GF_IOC_CHIP_INFO        _IOR(GF_IOC_MAGIC, 13, struct gf_ioc_chip_info)
#define GF_IOC_NAV_EVENT	    _IOW(GF_IOC_MAGIC, 14, gf_nav_event_t)
#define GF_IOC_ENABLE_GPIO  	_IO(GF_IOC_MAGIC, 15)
#define GF_IOC_RELEASE_GPIO  	_IO(GF_IOC_MAGIC, 16)

#define GF_NET_EVENT_IRQ 1
#define GF_NET_EVENT_FB_BLACK 2
#define GF_NET_EVENT_FB_UNBLACK 3
#define NETLINK_TEST 25
#define MAX_MSGSIZE 32

#define VER_MAJOR   1
#define VER_MINOR   2
#define VER_EXTRA   1
#define VER_TYPE    "meizusucks-debug"
#define VER_DESC    "part of the MeizuSucks project."

#define WAKEUP_HOLD_TIME 2000 /* in ms */

#define GF_SPIDEV_NAME     "goodix,fingerprint"
/*device name after register in charater*/
#define GF_DEV_NAME            "goodix_fp"
#define	GF_INPUT_NAME	    "gf-keys"	/*"goodix_fp" */
#define	CHRD_DRIVER_NAME	"goodix_fp_spi"
#define	CLASS_NAME		    "goodix_fp"

#define N_SPI_MINORS		32	/* ... up to 256 */

#define gf_info(fmt, args...) pr_info("gf: %s: " fmt, __func__, ##args);
#define gf_warn(fmt, args...) pr_warn("gf: %s: " fmt, __func__, ##args);
#define gf_err(fmt, args...)  pr_err("gf: %s: " fmt, __func__, ##args);

struct gf_dev {
	dev_t devt;
	struct list_head device_entry;
	struct spi_device *spi;
	struct clk *core_clk;
	struct clk *iface_clk;

	struct input_dev *input;
	/* buffer is NULL unless this device is open (users > 0) */
	unsigned users;
	signed irq_gpio;
	signed reset_gpio;
	signed pwr_gpio;
	int irq;
	int irq_enabled;
	int clk_enabled;
	struct notifier_block gf_notifier;
	char device_available;
	char fb_black;
	u8 *spi_buffer;
};
#endif /*__GF_SPI_H*/
