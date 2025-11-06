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
	{ 0x5e505530, "kthread_should_stop" },
	{ 0x9f222e1e, "alloc_chrdev_region" },
	{ 0xc130cdce, "cdev_init" },
	{ 0xa5782ab4, "cdev_add" },
	{ 0x3cd2d66b, "class_create" },
	{ 0x2035aec5, "device_create" },
	{ 0xef959adb, "kthread_create_on_node" },
	{ 0x9a9aee9e, "wake_up_process" },
	{ 0x0bc5fb0d, "unregister_chrdev_region" },
	{ 0x0537a61b, "cdev_del" },
	{ 0x690fb1ed, "class_destroy" },
	{ 0x8697f569, "kthread_stop" },
	{ 0x5d1ebb98, "device_destroy" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xf46d5bf3, "mutex_lock" },
	{ 0xf46d5bf3, "mutex_unlock" },
	{ 0x67628f51, "msleep" },
	{ 0xd268ca91, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x5e505530,
	0x9f222e1e,
	0xc130cdce,
	0xa5782ab4,
	0x3cd2d66b,
	0x2035aec5,
	0xef959adb,
	0x9a9aee9e,
	0x0bc5fb0d,
	0x0537a61b,
	0x690fb1ed,
	0x8697f569,
	0x5d1ebb98,
	0xd272d446,
	0xe8213e80,
	0xd272d446,
	0xf46d5bf3,
	0xf46d5bf3,
	0x67628f51,
	0xd268ca91,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"kthread_should_stop\0"
	"alloc_chrdev_region\0"
	"cdev_init\0"
	"cdev_add\0"
	"class_create\0"
	"device_create\0"
	"kthread_create_on_node\0"
	"wake_up_process\0"
	"unregister_chrdev_region\0"
	"cdev_del\0"
	"class_destroy\0"
	"kthread_stop\0"
	"device_destroy\0"
	"__fentry__\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"mutex_lock\0"
	"mutex_unlock\0"
	"msleep\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "127627C451464286E597865");
