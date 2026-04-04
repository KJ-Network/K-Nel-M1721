/*
 * TEE driver for goodix fingerprint sensor
 * Copyright (C) 2016 Goodix
 * Copyright (C) 2020 MeizuCustoms enthusiasts
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/input.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/delay.h>
#include <linux/ktime.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/of_gpio.h>
#include <linux/timer.h>
#include <linux/notifier.h>
#include <linux/fb.h>
#include <linux/pm_qos.h>
#include <linux/cpufreq.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <linux/time.h>
#include <linux/types.h>
#include <linux/workqueue.h>
#include <net/sock.h>
#include <net/netlink.h>
#include "gf_spi.h"

extern bool mback_disable;

static DECLARE_BITMAP(minors, N_SPI_MINORS);
static LIST_HEAD(device_list);
static DEFINE_MUTEX(device_list_lock);
static struct wakeup_source fp_wakeup_source;
static struct gf_dev gf;
static int pid = 0;
static struct sock *nl_sk = NULL;
static int gf_major = -1;

struct gf_key_map maps[] = {
	{ EV_KEY, KEY_HOME },
	{ EV_KEY, KEY_MENU },
	{ EV_KEY, KEY_BACK },
	{ EV_KEY, KEY_POWER },
	{ EV_KEY, KEY_F24 },
	{ EV_KEY, KEY_UP },
	{ EV_KEY, KEY_DOWN },
	{ EV_KEY, KEY_RIGHT },
	{ EV_KEY, KEY_LEFT },
	{ EV_KEY, KEY_CAMERA },
	{ EV_KEY, KEY_VOLUMEDOWN },
	{ EV_KEY, KEY_VOLUMEUP },
	{ EV_KEY, KEY_SEARCH },
	{ EV_KEY, KEY_CHAT },
};

static void sendnlmsg(char *message)
{
	struct sk_buff *skb_1;
	struct nlmsghdr *nlh;
	int len = NLMSG_SPACE(MAX_MSGSIZE);
	int slen = 0;
	int ret = 0;
	if (!message || !nl_sk || !pid)
		return ;
	skb_1 = alloc_skb(len, GFP_KERNEL);
	if (!skb_1) {
		gf_err("alloc_skb error.\n");
		return;
	}
	slen = strlen(message);
	nlh = nlmsg_put(skb_1, 0, 0, 0, MAX_MSGSIZE, 0);
	NETLINK_CB(skb_1).portid = 0;
	NETLINK_CB(skb_1).dst_group = 0;
	message[slen]= '\0';
	memcpy(NLMSG_DATA(nlh), message, slen+1);
	ret = netlink_unicast(nl_sk, skb_1, pid, MSG_DONTWAIT);
	if (!ret) {
		//kfree_skb(skb_1);
		gf_err("send msg from kernel to usespace failed ret 0x%x \n", ret);
	}
}

static void nl_data_ready(struct sk_buff *__skb)
{
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	char str[100];
	skb = skb_get(__skb);
	if (skb->len >= NLMSG_SPACE(0)) {
		nlh = nlmsg_hdr(skb);
		memcpy(str, NLMSG_DATA(nlh), sizeof(str));
		pid = nlh->nlmsg_pid;
		kfree_skb(skb);
	}
}

static int gf_parse_dts(struct gf_dev* gf_dev)
{
	int ret = 0;
    
	gf_dev->pwr_gpio = of_get_named_gpio(gf_dev->spi->dev.of_node, "goodix,gpio_pwr", 0);
	if (!gpio_is_valid(gf_dev->pwr_gpio)) {
		gf_err("PWR GPIO is invalid.\n");
		goto error;
	}
	ret = gpio_request(gf_dev->pwr_gpio, "goodix_pwr");
	if (ret) {
		gf_err("Error %d while processing PWR GPIO.\n", ret);
		goto error;
	}

	gf_dev->reset_gpio = of_get_named_gpio(gf_dev->spi->dev.of_node, "goodix,gpio_reset", 0);
	if (!gpio_is_valid(gf_dev->reset_gpio)) {
		gf_err("RESET GPIO is invalid.\n");
		goto error;
	}
	ret = gpio_request(gf_dev->reset_gpio, "goodix_reset");
	if (ret) {
		gf_err("Error %d while processing RESET GPIO.\n", ret);
		goto error;
	}
	
	gpio_direction_output(gf_dev->reset_gpio, 1);
    
	gf_dev->irq_gpio = of_get_named_gpio(gf_dev->spi->dev.of_node, "goodix,gpio_irq", 0);
	if (!gpio_is_valid(gf_dev->irq_gpio)) {
		gf_err("IRQ GPIO is invalid.\n");
		goto error;
	}
	ret = gpio_request(gf_dev->irq_gpio, "goodix_irq");
	if (ret) {
		gf_err("Error %d while processing IRQ GPIO.\n", ret);
		goto error;
	}
	gpio_direction_input(gf_dev->irq_gpio);
    gpio_direction_output(gf_dev->pwr_gpio, 1);
    
    return 0;
error:
    gf_info("IRQ [%d] | RST [%d] | PWR [%d]", gf_dev->irq_gpio, gf_dev->reset_gpio, gf_dev->pwr_gpio);
	return -1;
}

static void gf_cleanup(struct gf_dev* gf_dev)
{
	if (gpio_is_valid(gf_dev->irq_gpio)) {
		gpio_free(gf_dev->irq_gpio);
		gf_info("removed IRQ GPIO.\n");
	}
	if (gpio_is_valid(gf_dev->reset_gpio)) {
		gpio_free(gf_dev->reset_gpio);
		gf_info("removed RESET GPIO.\n");
	}
}

static int gf_power_on(struct gf_dev* gf_dev)
{
	if (gpio_is_valid(gf_dev->pwr_gpio))
		gpio_set_value(gf_dev->pwr_gpio, 1);
	msleep(10);
    gf_info("powered on.");
	return 0;
}

static int gf_power_off(struct gf_dev* gf_dev)
{
	if (gpio_is_valid(gf_dev->pwr_gpio))
		gpio_set_value(gf_dev->pwr_gpio, 0);
	msleep(10);
	gf_info("powered off.");
	return 0;
}

static int gf_hw_reset_low(struct gf_dev *gf_dev)
{
	if (gf_dev == NULL) {
		gf_err("gf_dev is null.\n");
		return -1;
	}
	gpio_direction_output(gf_dev->reset_gpio, 1);
	gpio_set_value(gf_dev->reset_gpio, 0);
    gf_info("low reset done.");
	return 0;
}

static int gf_hw_reset(struct gf_dev *gf_dev, unsigned int delay_ms)
{
	if (gf_dev == NULL) {
		gf_err("gf_dev is null.\n");
		return -1;
	}
	gpio_direction_output(gf_dev->reset_gpio, 1);
	gpio_set_value(gf_dev->reset_gpio, 0);
	usleep_range(3000, 4000);
	gpio_set_value(gf_dev->reset_gpio, 1);
	usleep_range(delay_ms * 1000, delay_ms * 1100);
    gf_info("reset done.");
	return 0;
}

static int gf_irq_num(struct gf_dev *gf_dev)
{
	if (gf_dev == NULL) {
		gf_err("gf_dev is null.\n");
		return -1;
	} else {
		return gpio_to_irq(gf_dev->irq_gpio);
	}
}

static void nav_event_input(struct gf_dev *gf_dev, gf_nav_event_t nav_event)
{
	uint32_t nav_input = 0;

	switch (nav_event) {
	case GF_NAV_FINGER_DOWN:
		gf_info("navigation finger down\n");
		break;

	case GF_NAV_FINGER_UP:
		gf_info("navigation finger up\n");
		break;

	case GF_NAV_DOWN:
		nav_input = GF_NAV_INPUT_DOWN;
		gf_info("navigation down\n");
		break;

	case GF_NAV_UP:
		nav_input = GF_NAV_INPUT_UP;
		gf_info("navigation up\n");
		break;

	case GF_NAV_LEFT:
		nav_input = GF_NAV_INPUT_LEFT;
		gf_info("navigation left\n");
		break;

	case GF_NAV_RIGHT:
		nav_input = GF_NAV_INPUT_RIGHT;
		gf_info("navigation right\n");
		break;

	case GF_NAV_CLICK:
		nav_input = GF_NAV_INPUT_CLICK;
		gf_info("navigation click\n");
		break;

	case GF_NAV_HEAVY:
		nav_input = GF_NAV_INPUT_HEAVY;
		gf_info("navigation heavy\n");
		break;

	case GF_NAV_LONG_PRESS:
		nav_input = GF_NAV_INPUT_LONG_PRESS;
		gf_info("navigation long press\n");
		break;

	case GF_NAV_DOUBLE_CLICK:
		nav_input = GF_NAV_INPUT_DOUBLE_CLICK;
		gf_info("navigation double click\n");
		break;

	default:
		gf_warn("unknown navigation event: %d\n", nav_event);
		break;
	}

	if ((nav_event != GF_NAV_FINGER_DOWN) && (nav_event != GF_NAV_FINGER_UP)) {
		input_report_key(gf_dev->input, nav_input, 1);
		input_sync(gf_dev->input);
		input_report_key(gf_dev->input, nav_input, 0);
		input_sync(gf_dev->input);
	}
}

static void gf_kernel_key_input(struct gf_dev *gf_dev, struct gf_key *gf_key)
{
	uint32_t key_input = 0;
	if (GF_KEY_HOME == gf_key->key) {
		key_input = KEY_BACK;
    } else {
		gf_err("Key %d is unsupported", gf_key->key);
		return;
	}

	gf_info("recieved key event[%d], key=%d, value=%d\n",
                        key_input, gf_key->key, gf_key->value);

	if (!mback_disable) {
		input_report_key(gf_dev->input, key_input, gf_key->value);
		input_sync(gf_dev->input);
	}
}

static long gf_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct gf_dev *gf_dev = &gf;
	struct gf_key gf_key;
	gf_nav_event_t nav_event = GF_NAV_NONE;
	int retval = 0;
	u8 netlink_route = NETLINK_TEST;
	struct gf_ioc_chip_info info;

    gf_info("command: 0x%x\n", cmd);
    
	if (_IOC_TYPE(cmd) != GF_IOC_MAGIC)
		return -ENODEV;

	if (_IOC_DIR(cmd) & _IOC_READ)
		retval = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		retval = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (retval)
		return -EFAULT;

	if (gf_dev->device_available == 0) {
		if ((cmd == GF_IOC_ENABLE_POWER) || (cmd == GF_IOC_DISABLE_POWER)) {
			gf_power_on(gf_dev);
		} else {
			gf_warn("Sensor is powered off currently. \n");
			return -ENODEV;
		}
	}

	switch (cmd) {
	case GF_IOC_INIT:
		gf_info("GF_IOC_INIT\n");
		if (copy_to_user((void __user *)arg, (void *)&netlink_route, sizeof(u8))) {
			retval = -EFAULT;
			break;
		}
		break;
    case GF_IOC_EXIT:
		gf_info("GF_IOC_EXIT\n");
		break;
    case GF_IOC_DISABLE_IRQ:
		gf_info("GF_IOC_DISABLE_IRQ\n");
		gf_dev->irq_enabled = 0;
        disable_irq_wake(gf_dev->irq);
		break;
	case GF_IOC_ENABLE_IRQ:
		gf_info("GF_IOC_ENABLE_IRQ\n");
		gf_dev->irq_enabled = 1;
        enable_irq_wake(gf_dev->irq);
		break;
	case GF_IOC_RESET:
		gf_info("GF_IOC_RESET\n");
		gf_hw_reset(gf_dev, 3);
		break;
    case GF_IOC_RELEASE_GPIO:
		gf_info("GF_IOC_RELEASE_GPIO\n");
		gf_dev->irq_enabled = 0;
        disable_irq_wake(gf_dev->irq);
		gf_cleanup(gf_dev);
		break;
	case GF_IOC_INPUT_KEY_EVENT:
		if (copy_from_user(&gf_key, (struct gf_key *)arg, sizeof(struct gf_key))) {
			gf_err("Failed to copy input key event from user to kernel\n");
			retval = -EFAULT;
			break;
		}

		gf_kernel_key_input(gf_dev, &gf_key);
		break;
	case GF_IOC_NAV_EVENT:
		gf_info("GF_IOC_NAV_EVENT\n");
		if (copy_from_user(&nav_event, (gf_nav_event_t *)arg, sizeof(gf_nav_event_t))) {
			gf_info("Failed to copy navigation event from user to kernel\n");
			retval = -EFAULT;
			break;
		}

		nav_event_input(gf_dev, nav_event);
		break;
	case GF_IOC_ENABLE_POWER:
		gf_info("GF_IOC_ENABLE_POWER\n");
		if (gf_dev->device_available == 1) {
			gf_warn("Sensor is already powered on.\n");
        } else {
			gf_power_on(gf_dev);
        }
		gf_dev->device_available = 1;
		break;
	case GF_IOC_DISABLE_POWER:
		gf_info("GF_IOC_DISABLE_POWER\n");
		if (gf_dev->device_available == 0) {
			gf_warn("Sensor is already powered off.\n");
        } else {
			gf_power_off(gf_dev);
        }
		gf_dev->device_available = 0;
		break;
	case GF_IOC_CHIP_INFO:
		gf_info("GF_IOC_CHIP_INFO\n");
		if (copy_from_user(&info, (struct gf_ioc_chip_info *)arg, sizeof(struct gf_ioc_chip_info))) {
			retval = -EFAULT;
			break;
		}
		gf_info("vendor_id: 0x%x\n", info.vendor_id);
		gf_info("mode: 0x%x\n", info.mode);
		gf_info("operation: 0x%x\n", info.operation);
		break;
    case GF_IOC_ENABLE_SPI_CLK:
		gf_info("GF_IOC_ENABLE_SPI_CLK\n");
		gf_info("Doesn't support control clock.\n");
		break;
	case GF_IOC_DISABLE_SPI_CLK:
		gf_info("GF_IOC_DISABLE_SPI_CLK\n");
		gf_info("Doesn't support control clock.\n");
		break;
    case GF_IOC_ENTER_SLEEP_MODE:
		gf_info("GF_IOC_ENTER_SLEEP_MODE\n");
		break;
	case GF_IOC_GET_FW_INFO:
		gf_info("GF_IOC_GET_FW_INFO. \n");
		break;
	case GF_IOC_REMOVE:
		gf_info("GF_IOC_REMOVE. \n");
		break;
	default:
		gf_warn("unsupported command: 0x%x\n", cmd);
		break;
	}

	return retval;
}

static long gf_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	return gf_ioctl(filp, cmd, (unsigned long)compat_ptr(arg));
}

static irqreturn_t gf_irq(int irq, void *handle)
{
	char temp = GF_NET_EVENT_IRQ;
	gf_info("enter\n");
	__pm_wakeup_event(&fp_wakeup_source, WAKEUP_HOLD_TIME);
	sendnlmsg(&temp);

	return IRQ_HANDLED;
}

static int gf_open(struct inode *inode, struct file *filp)
{
	struct gf_dev *gf_dev;
	int status = -ENXIO;

	mutex_lock(&device_list_lock);

	list_for_each_entry(gf_dev, &device_list, device_entry) {
		if (gf_dev->devt == inode->i_rdev) {
			gf_info("Found\n");
			status = 0;
			break;
		}
	}

	if (status == 0) {
        gf_dev->users++;
        filp->private_data = gf_dev;
        nonseekable_open(inode, filp);
        gf_info("Succeed to open device. IRQ = %d\n",
                gf_dev->irq);
        if (gf_dev->users == 1) {
            enable_irq_wake(gf_dev->irq);
            gf_dev->irq_enabled = 1;
        }
        gf_power_on(gf_dev);
        msleep(10);
        gf_hw_reset(gf_dev, 3);
        gf_dev->device_available = 1;
        status = 0;
	} else {
		gf_info("No device for minor %d\n", iminor(inode));
	}
	mutex_unlock(&device_list_lock);
	return status;
}

static int gf_release(struct inode *inode, struct file *filp)
{
	struct gf_dev *gf_dev;
	int status = 0;

	mutex_lock(&device_list_lock);
	gf_dev = filp->private_data;
	filp->private_data = NULL;

	/*last close?? */
	gf_dev->users--;
	if (!gf_dev->users) {
		gf_info("disable IRQ. IRQ = %d\n", gf_dev->irq);
		gf_dev->irq_enabled = 0;
        disable_irq_wake(gf_dev->irq);
		/*power off the sensor*/
		gf_dev->device_available = 0;
        gf_hw_reset_low(gf_dev);
		gf_power_off(gf_dev);
	}
	mutex_unlock(&device_list_lock);
	return status;
}

static const struct file_operations gf_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = gf_ioctl,
	.compat_ioctl = gf_compat_ioctl,
	.open = gf_open,
	.release = gf_release,
};

static int goodix_fb_state_chg_callback(struct notifier_block *nb,
		unsigned long val, void *data)
{
	struct gf_dev *gf_dev = NULL;
	struct fb_event *evdata = NULL;
	unsigned int blank;
	char temp = 0;

	if (val != FB_EARLY_EVENT_BLANK)
		return 0;

	gf_info("go to the fb state %d\n", (int)val);

	evdata = data;
	gf_dev = container_of(nb, struct gf_dev, gf_notifier);

	if (evdata && evdata->data && gf_dev) {
		blank = *(int *)(evdata->data);
		switch (blank) {
		case FB_BLANK_POWERDOWN:
			if (gf_dev->device_available == 1) {
				gf_dev->fb_black = 1;
				temp = GF_NET_EVENT_FB_BLACK;
				sendnlmsg(&temp);
			}
			break;
		case FB_BLANK_UNBLANK:
			if (gf_dev->device_available == 1) {
				gf_dev->fb_black = 0;
				temp = GF_NET_EVENT_FB_UNBLACK;
				sendnlmsg(&temp);
			}
			break;
		default:
			gf_info("default\n");
			break;
		}
	}
	return 0;
}

static struct class *gf_class;

static int gf_probe(struct spi_device *spi)
{
	struct gf_dev *gf_dev = &gf;
    struct device *dev;
	int status = -EINVAL;
	unsigned long minor;
	int i;
    
	/* Initialize the driver data */
	INIT_LIST_HEAD(&gf_dev->device_entry);
	gf_dev->spi = spi;
	gf_dev->irq_gpio = -EINVAL;
	gf_dev->reset_gpio = -EINVAL;
	gf_dev->pwr_gpio = -EINVAL;
	gf_dev->device_available = 0;
	gf_dev->fb_black = 0;

	if (gf_parse_dts(gf_dev)) {
        gf_err("parsing DTS error.\n");
        status = -EINVAL;
		goto error_hw;
    }

	gf_hw_reset(gf_dev, 70);
    if (strnstr(saved_command_line, "androidboot.mode=ffbm-01", strlen(saved_command_line)) ||
        strnstr(saved_command_line, "androidboot.mode=charger", strlen(saved_command_line))) {
        gf_warn("disable Goodix, we are in offline charging mode.");
        status = -1;
        gf_dev->device_available = 0;
        gf_power_off(&gf);
        return status;
    }
    
	mutex_lock(&device_list_lock);
	minor = find_first_zero_bit(minors, N_SPI_MINORS);
	if (minor < N_SPI_MINORS) {
		gf_dev->devt = MKDEV(gf_major, minor);
		dev = device_create(gf_class, &gf_dev->spi->dev, gf_dev->devt,
				gf_dev, "goodix_fp");
		status = IS_ERR(dev) ? PTR_ERR(dev) : 0;
	} else {
		gf_err("no minor number available!\n");
		status = -ENODEV;
		mutex_unlock(&device_list_lock);
		goto error_hw;
	}

	if (status == 0) {
		set_bit(minor, minors);
		list_add(&gf_dev->device_entry, &device_list);
	} else {
		gf_dev->devt = 0;
	}

	mutex_unlock(&device_list_lock);

	if (status == 0) {
		/*input device subsystem */
		gf_dev->input = input_allocate_device();
		if (gf_dev->input == NULL) {
			gf_err("failed to allocate input device\n");
			status = -ENOMEM;
			goto error_dev;
		}

		for (i = 0; i < ARRAY_SIZE(maps); i++)
			input_set_capability(gf_dev->input, maps[i].type, maps[i].code);

		gf_dev->input->name = "gf-keys";
		status = input_register_device(gf_dev->input);
		if (status) {
			gf_err("failed to register input device\n");
			goto error_input;
		}
	}

	gf_dev->gf_notifier.notifier_call = goodix_fb_state_chg_callback;
	if (fb_register_client(&gf_dev->gf_notifier))
		pr_err("%s: Fail to register fb notifier\n", __func__);

	gf_dev->irq = gf_irq_num(gf_dev);

	wakeup_source_init(&fp_wakeup_source, "fp_wakeup_source");
	status = request_threaded_irq(gf_dev->irq, NULL, gf_irq,
			IRQF_TRIGGER_RISING | IRQF_ONESHOT,
			"gf", gf_dev);

	if (status) {
		gf_err("failed to request IRQ %d\n", gf_dev->irq);
		goto err_irq;
	}
	irq_set_irq_wake(gf_dev->irq, 1);
	gf.irq_enabled = 0;
    disable_irq_wake(gf_dev->irq);

	gf_info("gf_spi ver. %d.%d.%d-%s, %s\n", VER_MAJOR, VER_MINOR, VER_EXTRA, VER_TYPE, VER_DESC);
	return status;

err_irq:
		input_unregister_device(gf_dev->input);
error_input:
	if (gf_dev->input != NULL)
		input_free_device(gf_dev->input);
error_dev:
	if (gf_dev->devt != 0) {
		gf_err("status = %d\n", status);
		mutex_lock(&device_list_lock);
		list_del(&gf_dev->device_entry);
		device_destroy(gf_class, gf_dev->devt);
		clear_bit(MINOR(gf_dev->devt), minors);
		mutex_unlock(&device_list_lock);
	}
error_hw:
	gf_cleanup(gf_dev);
	gf_dev->device_available = 0;

	return status;
}

static int gf_remove(struct spi_device *spi)
{
	struct gf_dev *gf_dev = &gf;

	/* make sure ops on existing fds can abort cleanly */
	if (gf_dev->irq)
		free_irq(gf_dev->irq, gf_dev);

	if (gf_dev->input != NULL)
		input_unregister_device(gf_dev->input);

	input_free_device(gf_dev->input);

	/* prevent new opens */
	mutex_lock(&device_list_lock);
	list_del(&gf_dev->device_entry);
	device_destroy(gf_class, gf_dev->devt);
	clear_bit(MINOR(gf_dev->devt), minors);

	if (gf_dev->users == 0) {
		gf_cleanup(gf_dev);
		mutex_unlock(&device_list_lock);
	}

	wakeup_source_trash(&fp_wakeup_source);

	fb_unregister_client(&gf_dev->gf_notifier);

	return 0;
}

static struct of_device_id gx_match_table[] = {
	{ .compatible = "goodix,fingerprint" },
	{},
};

static struct spi_driver gf_driver = {
	.driver = {
		.name = "goodix_fp",
		.owner = THIS_MODULE,
		.of_match_table = gx_match_table,
	},
	.probe = gf_probe,
	.remove = gf_remove,
};

static int __init gf_init(void)
{
	int status = 0;
    struct netlink_kernel_cfg netlink_cfg;

	/* Claim our 256 reserved device numbers.  Then register a class
	 * that will key udev/mdev to add/remove /dev nodes.  Last, register
	 * the driver which manages those device numbers.
	 */

	BUILD_BUG_ON(N_SPI_MINORS > 256);
	/* Dynamically allocate a major */
	gf_major = register_chrdev(0, CHRD_DRIVER_NAME, &gf_fops);
	if (gf_major < 0) {
		gf_err("Failed to register char device!\n");
		return gf_major;
	}
	gf_class = class_create(THIS_MODULE, "goodix_fp");
	if (IS_ERR(gf_class)) {
		unregister_chrdev(gf_major, gf_driver.driver.name);
		gf_err("Failed to create class.\n");
		return PTR_ERR(gf_class);
	}
	status = spi_register_driver(&gf_driver);
	if (status < 0) {
		class_destroy(gf_class);
		unregister_chrdev(gf_major, gf_driver.driver.name);
		gf_err("Failed to register SPI driver.\n");
	}

	memset(&netlink_cfg, 0, sizeof(struct netlink_kernel_cfg));
	netlink_cfg.groups = 0;
	netlink_cfg.flags = 0;
	netlink_cfg.input = nl_data_ready;
	netlink_cfg.cb_mutex = NULL;
	nl_sk = netlink_kernel_create(&init_net, NETLINK_TEST,
			&netlink_cfg);
	if (!nl_sk) {
		gf_err("netlink: create netlink socket error\n");
        status = 1;
	}
	gf_info("status = 0x%x\n", status);
	return status;
}
module_init(gf_init);

static void __exit gf_exit(void)
{
	struct gf_dev *gf_dev = &gf;

	if (nl_sk != NULL) {
		netlink_kernel_release(nl_sk);
		nl_sk = NULL;
	}

	fb_unregister_client(&gf_dev->gf_notifier);
	gf_info("netlink: self module exited\n");
	spi_unregister_driver(&gf_driver);
	class_destroy(gf_class);
	if (gf_major >= 0)
		unregister_chrdev(gf_major, gf_driver.driver.name);
}
module_exit(gf_exit);

MODULE_AUTHOR("Jiangtao Yi, <yijiangtao@goodix.com>");
MODULE_AUTHOR("Jandy Gou, <gouqingsong@goodix.com>");
MODULE_DESCRIPTION("goodix fingerprint sensor device driver");
MODULE_LICENSE("GPL");
