#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/gpio/consumer.h>
#include <linux/of_gpio.h>
#include <linux/property.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt, __func__

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VINOD");
MODULE_DESCRIPTION("GPIO SysFS Platform Driver");

static int gpio_sysfs_probe(struct platform_device *pdev);
static void gpio_sysfs_remove(struct platform_device *pdev);

struct gpiodev_private_data {
    char label[20];
    struct gpio_desc *desc;
};

struct gpiodrv_private_data {
    int total_devices;
    struct class *class_gpio;
};

static struct gpiodrv_private_data gpio_drv_data;
ssize_t direction_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t direction_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t value_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t value_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t label_show(struct device *dev, struct device_attribute *attr, char *buf);



/* ---------- SYSFS Callbacks ---------- */

ssize_t direction_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct gpiodev_private_data *dev_data = dev_get_drvdata(dev);
    int dir;
    const char *direction;

    dir = gpiod_get_direction(dev_data->desc);
    if (dir < 0)
        return dir;

    direction = (dir == 0) ? "out" : "in";
    return sprintf(buf, "%s\n", direction);
}

ssize_t direction_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    int ret;
    struct gpiodev_private_data *dev_data = dev_get_drvdata(dev);

    if (sysfs_streq(buf, "in"))
        ret = gpiod_direction_input(dev_data->desc);
    else if (sysfs_streq(buf, "out"))
        ret = gpiod_direction_output(dev_data->desc, 0);
    else
        ret = -EINVAL;

    return ret ? ret : count;
}

ssize_t value_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct gpiodev_private_data *dev_data = dev_get_drvdata(dev);
    int value = gpiod_get_value(dev_data->desc);
    return sprintf(buf, "%d\n", value);
}

ssize_t value_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    struct gpiodev_private_data *dev_data = dev_get_drvdata(dev);
    long value;
    int ret = kstrtol(buf, 0, &value);
    if (ret)
        return ret;

    gpiod_set_value(dev_data->desc, value);
    return count;
}

ssize_t label_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct gpiodev_private_data *dev_data = dev_get_drvdata(dev);
    return sprintf(buf, "%s\n", dev_data->label);
}

/* Create sysfs attributes */
static DEVICE_ATTR_RW(direction);
static DEVICE_ATTR_RW(value);
static DEVICE_ATTR_RO(label);

static struct attribute *gpio_attrs[] = {
    &dev_attr_direction.attr,
    &dev_attr_value.attr,
    &dev_attr_label.attr,
    NULL
};

static struct attribute_group gpio_attr_group = {
    .attrs = gpio_attrs,
};

static const struct attribute_group *gpio_attr_groups[] = {
    &gpio_attr_group,
    NULL
};

/* ---------- Probe Function ---------- */

static int gpio_sysfs_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct device_node *parent = dev->of_node;
    struct device_node *child;
    const char *name;
    struct gpiodev_private_data *dev_data;
    struct device *dev_sysfs;
    int i = 0, ret;

    for_each_available_child_of_node(parent, child) {
        dev_data = devm_kzalloc(dev, sizeof(*dev_data), GFP_KERNEL);
        if (!dev_data)
            return -ENOMEM;

        if (of_property_read_string(child, "label", &name)) {
            dev_warn(dev, "Missing label information\n");
            snprintf(dev_data->label, sizeof(dev_data->label), "unkngpio%d", i);
        } else {
            strcpy(dev_data->label, name);
            dev_info(dev, "GPIO label = %s\n", dev_data->label);
        }

        dev_data->desc = fwnode_gpiod_get_index(&child->fwnode, "bone", 0, GPIOD_ASIS, dev_data->label);
        if (IS_ERR(dev_data->desc)) {
            ret = PTR_ERR(dev_data->desc);
            if (ret == -ENOENT)
                dev_err(dev, "No GPIO assigned to %s\n", dev_data->label);
            return ret;
        }

        ret = gpiod_direction_output(dev_data->desc, 0);
        if (ret) {
            dev_err(dev, "GPIO direction set failed\n");
            return ret;
        }

        dev_sysfs = device_create_with_groups(gpio_drv_data.class_gpio, dev, 0,
                                              dev_data, gpio_attr_groups, dev_data->label);
        if (IS_ERR(dev_sysfs)) {
            dev_err(dev, "Error in device_create_with_groups\n");
            return PTR_ERR(dev_sysfs);
        }

        dev_set_drvdata(dev_sysfs, dev_data);
        i++;
    }

    gpio_drv_data.total_devices = i;
    pr_info("GPIO sysfs probe successful. Total GPIOs: %d\n", i);
    return 0;
}

/* ---------- Remove Function ---------- */

static void gpio_sysfs_remove(struct platform_device *pdev)
{
        struct device *dev = &pdev->dev;
        dev_info(dev, "GPIO sysfs driver removed\n");

}

/* ---------- Platform Driver Table ---------- */

static const struct of_device_id gpio_device_match[] = {
    { .compatible = "org,bone-gpio-sysfs" },
    {}
};
MODULE_DEVICE_TABLE(of, gpio_device_match);

static struct platform_driver gpiosysfs_platform_driver = {
    .probe = gpio_sysfs_probe,
    .remove = gpio_sysfs_remove,
    .driver = {
        .name = "bone-gpio-sysfs",
        .of_match_table = of_match_ptr(gpio_device_match),
    },
};

/* ---------- Init / Exit ---------- */

static int __init gpio_init(void)
{
    gpio_drv_data.class_gpio = class_create("bone_gpios");
    if (IS_ERR(gpio_drv_data.class_gpio)) {
        pr_err("Error creating class\n");
        return PTR_ERR(gpio_drv_data.class_gpio);
    }

    platform_driver_register(&gpiosysfs_platform_driver);
    pr_info("GPIO sysfs module loaded successfully\n");
    return 0;
}

static void __exit gpio_sysfs_exit(void)
{
    platform_driver_unregister(&gpiosysfs_platform_driver);
    class_destroy(gpio_drv_data.class_gpio);
    pr_info("GPIO sysfs module unloaded\n");
}

module_init(gpio_init);
module_exit(gpio_sysfs_exit);

