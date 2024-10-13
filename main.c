// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/pci.h>

#include "main.h"
#include "debug.h"
#include "aura-gpu-hw.h"
#include "include/types.h"

// Stores i2c_adapters and asic type
static struct aura_adapter adapters[] = {
    [0 ... MAX_AURA_DEVICES] = {NULL, CHIP_LAST}
};

static int __init aura_module_init (
    void
){
    // find at most MAX_AURA_DEVICES devices
    aura_i2c_bios_create(adapters);

    // clears error on all devices
    for (int i = 0; i < MAX_AURA_DEVICES; ++i) {
        if (IS_ERR(adapters[i].adapter))
            CLEAR_ERR(adapters[i].adapter);
    }

    return 0;
}

static void __exit aura_module_exit (
    void
){
    for (int i = 0; i < MAX_AURA_DEVICES; ++i) {
        if (adapters[i].adapter)
            aura_i2c_bios_destroy(adapters[i].adapter);
    }
}

/*
 * Returns the AURA asic type associated to a certain adapter
 */
enum aura_asic_type aura_i2c_adapter_asic_type(struct i2c_adapter* adapter) {
    for (int i = 0; i < MAX_AURA_DEVICES; ++i){
        // If adapter is not null, matches on adapter number
        if (
            adapters[i].adapter &&
            i2c_adapter_id(adapters[i].adapter) == i2c_adapter_id(adapter)
            ) {
            return adapters[i].asic_type;
        }
    }
    AURA_ERR("Asic_type not registered");
    return CHIP_LAST;
}

module_init(aura_module_init);
module_exit(aura_module_exit);

MODULE_AUTHOR("Owen Parry <waldermort@gmail.com>");
MODULE_DESCRIPTION("ASUS AURA SMBus driver");
MODULE_LICENSE("GPL");
