// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/pci.h>

#include "main.h"
#include "debug.h"
#include "aura-gpu-hw.h"
#include "include/types.h"

static struct aura_adapter adapters[] = {
        {NULL, CHIP_LAST},
        {NULL, CHIP_LAST},
        {NULL, CHIP_LAST},
        {NULL, CHIP_LAST},
};
//static struct i2c_adapter *adapter = NULL;

static int __init aura_module_init (
    void
){
    adapters[0].adapter = aura_i2c_bios_create();
    if (IS_ERR_OR_NULL(adapters[0].adapter))
        CLEAR_ERR(adapters[0].adapter);

    return 0;
}

static void __exit aura_module_exit (
    void
){
    if (adapters[0].adapter)
        aura_i2c_bios_destroy(adapters[0].adapter);
}

enum aura_asic_type aura_i2c_adapter_asic_type(struct i2c_adapter* adapter) {
    return adapters[0].asic_type;
}

module_init(aura_module_init);
module_exit(aura_module_exit);

MODULE_AUTHOR("Owen Parry <waldermort@gmail.com>");
MODULE_DESCRIPTION("ASUS AURA SMBus driver");
MODULE_LICENSE("GPL");
