#ifndef _UAPI_AURA_GPU_HW_I2C_H
#define _UAPI_AURA_GPU_HW_I2C_H

#include <linux/i2c.h>
#include "include/types.h"
#include "main.h"

void aura_i2c_bios_create (
    struct aura_adapter adapters[MAX_AURA_DEVICES]
);

void aura_i2c_bios_destroy (
    struct i2c_adapter *i2c_adapter
);

#endif
