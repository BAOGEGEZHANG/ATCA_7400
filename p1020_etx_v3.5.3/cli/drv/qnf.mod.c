#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xd9ef8a65, "module_layout" },
	{ 0xadf42bd5, "__request_region" },
	{ 0x4cf30b72, "cdev_del" },
	{ 0xd43418e7, "pci_bus_read_config_byte" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x8a97ae3b, "cdev_init" },
	{ 0x3ec8886f, "param_ops_int" },
	{ 0x788fe103, "iomem_resource" },
	{ 0x1e9fcce7, "dev_set_drvdata" },
	{ 0x19d7638c, "kobject_set_name" },
	{ 0x52b78c0e, "filp_close" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xada04ff9, "pci_set_master" },
	{ 0x5f754e5a, "memset" },
	{ 0x461ebfa0, "__copy_tofrom_user" },
	{ 0x27e1a049, "printk" },
	{ 0x42224298, "sscanf" },
	{ 0x20030ecd, "ioremap" },
	{ 0xa39b4cf2, "udelay" },
	{ 0x54b67bae, "cdev_add" },
	{ 0xb8aa2342, "__check_region" },
	{ 0x929586ce, "pci_bus_read_config_dword" },
	{ 0x9bce482f, "__release_region" },
	{ 0xe1c080b6, "pci_unregister_driver" },
	{ 0x1d2e87c6, "do_gettimeofday" },
	{ 0xacbc1903, "pci_bus_write_config_byte" },
	{ 0x37a0cba, "kfree" },
	{ 0x9d669763, "memcpy" },
	{ 0xedc03953, "iounmap" },
	{ 0x638bbccc, "__pci_register_driver" },
	{ 0x6be3163a, "pci_enable_device" },
	{ 0xf7108f2a, "dev_get_drvdata" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x6deea7e0, "filp_open" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

