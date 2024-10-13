#ifndef _UAPI_AURA_GPU_MAIN_H
#define _UAPI_AURA_GPU_MAIN_H

#define MAX_AURA_DEVICES 4

#include "include/types.h"

/*
 * Returns the AURA asic type associated to a certain adapter
 */
enum aura_asic_type aura_i2c_adapter_asic_type(struct i2c_adapter *adapter);

#endif