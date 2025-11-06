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
	{ 0xa5782ab4, "cdev_add" },
	{ 0x3cd2d66b, "class_create" },
	{ 0x2035aec5, "device_create" },
	{ 0x9126ce86, "request_threaded_irq" },
	{ 0x0bc5fb0d, "unregister_chrdev_region" },
	{ 0x0537a61b, "cdev_del" },
	{ 0x690fb1ed, "class_destroy" },
	{ 0x9dd4105e, "free_irq" },
	{ 0x9c0551c6, "tasklet_kill" },
	{ 0x5d1ebb98, "device_destroy" },
	{ 0x9c0551c6, "__tasklet_schedule" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x9f222e1e, "alloc_chrdev_region" },
	{ 0xc130cdce, "cdev_init" },
	{ 0xd268ca91, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xa5782ab4,
	0x3cd2d66b,
	0x2035aec5,
	0x9126ce86,
	0x0bc5fb0d,
	0x0537a61b,
	0x690fb1ed,
	0x9dd4105e,
	0x9c0551c6,
	0x5d1ebb98,
	0x9c0551c6,
	0xd272d446,
	0xe8213e80,
	0xd272d446,
	0x9f222e1e,
	0xc130cdce,
	0xd268ca91,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"cdev_add\0"
	"class_create\0"
	"device_create\0"
	"request_threaded_irq\0"
	"unregister_chrdev_region\0"
	"cdev_del\0"
	"class_destroy\0"
	"free_irq\0"
	"tasklet_kill\0"
	"device_destroy\0"
	"__tasklet_schedule\0"
	"__fentry__\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"alloc_chrdev_region\0"
	"cdev_init\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "C95C19BBB9138458994E8E2");
