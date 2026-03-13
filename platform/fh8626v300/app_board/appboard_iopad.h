/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-01-08     wangyl307    the first version, for fh8856&zy2
 * 2019-01-14     wangyl307    add pads fix funcs and defs for zy
 *
 */

#ifndef __APP_IOPAD_H__
#define __APP_IOPAD_H__

char *fh_pinctrl_selected_devices[] = {
    /* CONFIG_PINCTRL_SELECT */
    "SADC_IN0", "SADC_IN1", "SPI0_4BIT",
    "STM0", "STM1", "UART0", "UART1", "GPIO28", "GPIO40",
    "GPIO49", "GPIO62", "GPIO63", "MIPI",
#ifdef WIFI_USING_SDIOWIFI
#if (WIFI_SDIO == 0)
"SD0_WIFI", "GPIO25"
#else
"SD0_NO_WP"
#endif
#else
"SD0_NO_WP"
#endif
};

char *fh_pinctrl_selected_aon_devices[] = {
    /* CONFIG_PINCTRL_SELECT */
    "I2C0", "SENSOR0_VS", "SENSOR1_VS", "SENSOR_CLK",
    "GPIO14",

    "GPIO6", "GPIO7", "GPIO8", "GPIO9"
};

#endif
