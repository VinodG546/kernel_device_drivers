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
	{ 0x6e8af1d8, "param_set_int" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0xe8819b6c, "param_get_int" },
	{ 0x1b3db703, "param_array_ops" },
	{ 0x1b3db703, "param_ops_int" },
	{ 0x1b3db703, "param_ops_charp" },
	{ 0xd272d446, "__fentry__" },
	{ 0x70eca2ca, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x6e8af1d8,
	0xe8213e80,
	0xd272d446,
	0x90a48d82,
	0xe8819b6c,
	0x1b3db703,
	0x1b3db703,
	0x1b3db703,
	0xd272d446,
	0x70eca2ca,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"param_set_int\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"__ubsan_handle_out_of_bounds\0"
	"param_get_int\0"
	"param_array_ops\0"
	"param_ops_int\0"
	"param_ops_charp\0"
	"__fentry__\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "E65A45E5C63A4D0CA5754A3");
