#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xd4441043, "gpiod_get_direction" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x222e7ce2, "sysfs_streq" },
	{ 0x1cc394e9, "gpiod_direction_input" },
	{ 0x59064f38, "gpiod_direction_output" },
	{ 0xf0379d3f, "gpiod_get_value" },
	{ 0xaa19e4aa, "_kstrtol" },
	{ 0x2a542d2b, "gpiod_set_value" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xcb710431, "_dev_info" },
	{ 0x4b4f78b4, "class_create" },
	{ 0x92997ed8, "_printk" },
	{ 0x3f08ccd3, "__platform_driver_register" },
	{ 0x4d2a23d3, "of_get_next_available_child" },
	{ 0xac05ba89, "devm_kmalloc" },
	{ 0x6863a2ba, "of_property_read_string" },
	{ 0x97255bdf, "strlen" },
	{ 0x9d669763, "memcpy" },
	{ 0xdb7c5c1b, "fwnode_gpiod_get_index" },
	{ 0xa7e15789, "_dev_err" },
	{ 0x85cb0352, "device_create_with_groups" },
	{ 0x5e35dc80, "__fortify_panic" },
	{ 0xdb5b2376, "_dev_warn" },
	{ 0xc358aaf8, "snprintf" },
	{ 0x30861af3, "platform_driver_unregister" },
	{ 0x4b9f8d27, "class_destroy" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xfdf6ada4, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Corg,bone-gpio-sysfs");
MODULE_ALIAS("of:N*T*Corg,bone-gpio-sysfsC*");
