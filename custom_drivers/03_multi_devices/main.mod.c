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
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x92997ed8, "_printk" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0x92526a12, "validate_usercopy_range" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x4b4f78b4, "class_create" },
	{ 0x60e0f127, "cdev_init" },
	{ 0x350a3457, "cdev_add" },
	{ 0xe1cae1ed, "device_create" },
	{ 0x42de25b3, "device_destroy" },
	{ 0xab6bfbc5, "cdev_del" },
	{ 0x4b9f8d27, "class_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x5f754e5a, "memset" },
	{ 0xfdf6ada4, "module_layout" },
};

MODULE_INFO(depends, "");

