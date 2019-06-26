/*
 * (C) Copyright 2011 Linaro
 * Ryan Harkin, <ryan.harkin@linaro.org>
 *
 * Configuration for Versatile Express. Parts were derived from other ARM
 *   configurations.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __VEXPRESS_CA9X4_H
#define __VEXPRESS_CA9X4_H

#define CONFIG_VEXPRESS_ORIGINAL_MEMORY_MAP
#include "vexpress_common.h"

#define CONFIG_EXTRA_ENV_SETTINGS \
		CONFIG_PLATFORM_ENV_SETTINGS \
                BOOTENV \
		"console=ttyAMA0,38400n8\0" \
		"dram=512M\0" \
		"root=/dev/mmcblk0p2 rw\0" \
		"flashargs=setenv bootargs root=${root} console=${console} mem=${dram} vmalloc=256M\0" \
		"bootflash=run flashargs; " \
			"cp ${ramdisk_addr} ${ramdisk_addr_r} ${maxramdisk}; " \
                        "fatload mmc 0 0x60000000 zImage; " \
                        "fatload mmc 0 0x60500000 vexpress-v2p-ca9.dtb; " \
			"bootz 0x60000000 - 0x60500000\0"

#endif /* VEXPRESS_CA9X4_H */

