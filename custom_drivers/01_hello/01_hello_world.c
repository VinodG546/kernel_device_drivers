#include <linux/kernel.h>
#include <linux/module.h>

static int __init hello_init(void);
static void __exit hello_exit(void);

static int __init hello_init(void)
{
    pr_info("init function %s\n", __func__);
    return 0;
}

static void __exit hello_exit(void)
{
    pr_info("%s exit function\n", __func__);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinod");
MODULE_DESCRIPTION("Simple Hello World Module");  // fixes modpost warning

