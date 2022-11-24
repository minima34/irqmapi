#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xb622add4, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xf93fb492, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0xb3c7d0fb, __VMLINUX_SYMBOL_STR(__platform_driver_probe) },
	{ 0x65345022, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xf20dabd8, __VMLINUX_SYMBOL_STR(free_irq) },
	{ 0x1ea1e740, __VMLINUX_SYMBOL_STR(class_unregister) },
	{ 0x7cc53847, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0xa3c6fa2, __VMLINUX_SYMBOL_STR(mutex_destroy) },
	{ 0x72e45ac7, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x10a6bb3c, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0xa202a8e5, __VMLINUX_SYMBOL_STR(kmalloc_order_trace) },
	{ 0x71ff0ddf, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x618014ef, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0xecc4df08, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0xe99c823a, __VMLINUX_SYMBOL_STR(__register_chrdev) },
	{ 0xd6b8e852, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x9c97fde8, __VMLINUX_SYMBOL_STR(platform_get_irq) },
	{ 0x5c1a1b97, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x25bbaf54, __VMLINUX_SYMBOL_STR(of_irq_count) },
	{ 0xb35dea8f, __VMLINUX_SYMBOL_STR(__arch_copy_to_user) },
	{ 0xdcb764ad, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x84bc974b, __VMLINUX_SYMBOL_STR(__arch_copy_from_user) },
	{ 0x88db9f48, __VMLINUX_SYMBOL_STR(__check_object_size) },
	{ 0xfadf2436, __VMLINUX_SYMBOL_STR(memstart_addr) },
	{ 0x50e8c70f, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0xd716155e, __VMLINUX_SYMBOL_STR(mutex_trylock) },
	{ 0x322f0473, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x1fdc7df2, __VMLINUX_SYMBOL_STR(_mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Cirqmap");
MODULE_ALIAS("of:N*T*CirqmapC*");

MODULE_INFO(srcversion, "C10F88B979F5126D9B907D0");
