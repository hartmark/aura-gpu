/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _UAPI_INCLUDE_TYPES_H
#define _UAPI_INCLUDE_TYPES_H
#include <linux/module.h>
#include <linux/pci.h>

#include "../asic/asic-types.h"

#ifndef ERROR_TYPE
typedef int error_t;

struct aura_adapter {
    struct i2c_adapter* adapter;
    enum aura_asic_type asic_type;
};

#define ERROR_TYPE error_t
#endif

#endif
