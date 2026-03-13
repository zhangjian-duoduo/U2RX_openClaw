/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-01-08     songyh    the first version
 *
 */

#ifndef __BOARD_DEF_H__
#define __BOARD_DEF_H__

#define FH_SDIO_IGNORE_CD

#define CONFIG_PHY_RESET_GPIO       10
#define CONFIG_GPIO_EMACPHY_RXDV    54
#define CONFIG_IRCUT_ON_GPIO        0
#define CONFIG_IRCUT_OFF_GPIO       1
#define CONFIG_CIS_RESET_GPIO       13

#define NUM_OF_GPIO                 (64)

#ifndef FH_DDR_START
#define FH_DDR_START        0x10000000
#define FH_DDR_END          0x50000000
#define FH_DDR_SIZE         (GET_REG(REG_PMU_DDR_SIZE) << 17)

#ifdef FH_USING_PSRAM
#define FH_RTT_OS_MEM_SIZE  0x00300000
#else
#define FH_RTT_OS_MEM_SIZE  0x01800000
#endif

#ifdef FH_USING_DMA_MEM
#define FH_DMA_MEM_SIZE     0x20000 /* 128k */
#else
#define FH_DMA_MEM_SIZE     0       /* 0k */
#endif

#define FH_RTT_OS_MEM_START     FH_DDR_START
#define FH_RTT_OS_MEM_END       (FH_DDR_START + FH_RTT_OS_MEM_SIZE)
#define FH_SDK_MEM_START        (FH_RTT_OS_MEM_END + FH_DMA_MEM_SIZE)
/*
 * Reserved 1MB for VBUS
 */
#define FH_SHAREMEM_SIZE        (0)        /* used for share memory for bootloader & OS */
#if defined(FH_USING_XBUS)

#define FH_ARCOS_SIZE           (0x400000)

#if defined (FH_USING_DDRBOOT) || defined (FH_USING_LOAD_NBG)
#define FH_NBG_SIZE             (0x500000)
#else
#define FH_NBG_SIZE             (0)
#endif

#define FH_NN_MEM_START         (FH_DDR_START + FH_DDR_SIZE - FH_ARCOS_SIZE - FH_NBG_SIZE)
#define FH_NN_HDR_OFFEST        (0X40)

#define FH_SDK_MEM_END          (FH_DDR_START + FH_DDR_SIZE - FH_ARCOS_SIZE - FH_NBG_SIZE - FH_SHAREMEM_SIZE)
#else
#define FH_SDK_MEM_END          (FH_DDR_START + FH_DDR_SIZE - FH_SHAREMEM_SIZE)
#endif

#define FH_SDK_MEM_SIZE         (FH_SDK_MEM_END - FH_SDK_MEM_START)

/* #define MMC_USE_INTERNAL_BUF */
#define MMC_INTERNAL_DMA_BUF_SIZE (32 * 1024)
#endif
#ifdef WIFI_USING_SDIOWIFI
#if (WIFI_SDIO == 0)
#define HW_WIFI_POWER_GPIO 25 /* wifi power on */
#elif (WIFI_SDIO == 1)
#define HW_WIFI_POWER_GPIO 58 /* wifi power on */
#endif
#define WIFI_ENABLE_GPIO 28

#ifdef WIFI_USING_CYPRESS
/* #define RECV_BY_POLL */
#define RECV_BY_SDIO_IRQ
#define WIFI_IRQ_GPIO 55 /* not used when RECV_BY_SDIO_IRQ or RECV_BY_POLL defined */
#endif

#define HW_WIFI_POWER_GPIO_ON_LEVEL 0
#endif
#endif /* BOARD_H_ */
