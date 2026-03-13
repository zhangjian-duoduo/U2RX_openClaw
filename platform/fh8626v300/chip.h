/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-04-09     songyh    the first version
 *
 */

#ifndef __CHIP_H__
#define __CHIP_H__

#include "fh8626v300/arch.h"

#define PAD_REG_START_OFF 0x100

/* TOP_CTRL_REG_BASE */
#define REG_PMU_CHIP_ID                  (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0000)
#define REG_PMU_IP_VER                   (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0004)
#define REG_PMU_FW_VER                   (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0008)
#define REG_PMU_SPC_IO_STATUS            (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x000c)
#define REG_PMU_WDT_CTRL                 (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0010)
#define REG_PMU_DBG_STAT0                (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0014)
#define REG_PMU_DBG_STAT1                (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0018)
#define REG_PMU_PMU_USER0                (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0024)
#define REG_PMU_RESERVED2                (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0028)
#define REG_PMU_EPHY_REG0                (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x002c)
#define REG_PMU_EPHY_REG1                (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0030)
#define REG_PMU_PRDCID_CTRL0             (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0034)
#define REG_PMU_PRDCID_CTRL1             (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0038)
#define REG_PMU_SYNOP_TESTPORT           (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x003c)
#define REG_PMU_TOP_LPC_EN               (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0040)
#define REG_PMU_TOP_LPC_FORCE            (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0044)
#define REG_PMU_TOP_LPC_NUM              (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0048)
#define REG_PMU_TOP_LP_STATE             (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x004c)
#define REG_PMU_TOP_LP_GATE              (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0050)
#define REG_PMU_TOP_LPC_GATE             (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0054)
#define REG_PMU_ACW_DA_DAT_SEL           (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x005c)
#define REG_PMU_PAD_POWER_SEL            (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0060)
#define REG_PMU_RESERVED0                (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0064)
#define REG_PMU_RESERVED1                (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0068)
#define REG_PMU_SDC_MISC                 (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x006c)
#define REG_PMU_PRDCID_CTRL2             (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0070)
#define REG_PMU_EFHY_REG0 REG_PMU_EPHY_REG0

#define REG_PMU_SD1_FUNC_SEL             (SYS_REG_P2V(ANA_REG_BASE) + 0x0404)

#define REG_PMU_SCU_PLL_WREN             (SYS_REG_P2V(TOP_CTRL_REG_BASE) + 0x0058)

/* CPU_CTRL_REG_BASE */
#define REG_PMU_EPHY_PARAM               (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x0054)
#define REG_PMU_LPC_EN_CFG               (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x0000)
#define REG_PMU_LPC_NUM_CFG              (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x0004)
#define REG_PMU_LPC_STATE                (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x0008)
#define REG_PMU_LPC_GATE                 (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x000c)
#define REG_PMU_SUB_CPUARC_INTC_MASK     (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x0010)
#define REG_PMU_CPU_SYS_MISC             (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x0018)
#define REG_PMU_QOS_CTRL                 (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x001c)
#define REG_PMU_DMA_HDSHAKE_EN           (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x0020)
#define REG_PMU_LPC_STATE1               (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x0024)
#define REG_PMU_CPU_DBG                  (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x0030)
#define REG_PMU_WREN                     (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x0040)
#define REG_PMU_DDR_SIZE                 (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x0048)
#define REG_PMU_CHIP_INFO                (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x0050)
#define REG_PMU_DMA_SEL                  REG_PMU_DMA_HDSHAKE_EN
#define REG_DMA_HDSHAKE_EN               REG_PMU_DMA_HDSHAKE_EN

#define PMU_ARM_INT_MASK                 (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x00B4)
#define PMU_ARM_INT_RAWSTAT              (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x00C4)
#define PMU_ARM_INT_STAT                 (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x00BC)

#define PMU_A625_INT_MASK                (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x00C0)
#define PMU_A625_INT_RAWSTAT             (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x00B8)
#define PMU_A625_INT_STAT                (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x00C8)

#define REG_PMU_CPU_SYS_MISC1            (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x00cc)
#define REG_PMU_CPU_SYS_MISC2            (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x00d0)
#define REG_PMU_CPU_SYS_MISC3            (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x00d4)
#define REG_PMU_CPU_SYS_MISC4            (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x00d8)

#define REG_PMU_CPU_SYS_STAT1            (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x00dc)
#define REG_PMU_CPU_SYS_STAT2            (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x00e0)
#define REG_PMU_CPU_SYS_STAT3            (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x00e4)
#define REG_PMU_CPU_SYS_STAT4            (SYS_REG_P2V(CPU_CTRL_REG_BASE) + 0x00e8)

/* CLK_REG_BASE */
#define REG_PMU_PLL1_CTRL                (SYS_REG_P2V(CLK_REG_BASE) + 0x0000)
#define REG_PMU_PLL2_CTRL                (SYS_REG_P2V(CLK_REG_BASE) + 0x0004)
#define REG_PMU_CLK_SEL0                 (SYS_REG_P2V(CLK_REG_BASE) + 0x0030)
#define REG_PMU_CLK_SEL1                 (SYS_REG_P2V(CLK_REG_BASE) + 0x0034)
#define REG_PMU_CLK_SEL2                 (SYS_REG_P2V(CLK_REG_BASE) + 0x0038)
#define REG_PMU_CLK_SEL3                 (SYS_REG_P2V(CLK_REG_BASE) + 0x003C)
#define REG_PMU_CLK_SEL4                 (SYS_REG_P2V(CLK_REG_BASE) + 0x0040)
#define REG_PMU_CLK_SEL5                 (SYS_REG_P2V(CLK_REG_BASE) + 0x0044)
#define REG_PMU_CLK_SEL6                 (SYS_REG_P2V(CLK_REG_BASE) + 0x0048)
#define REG_PMU_CLK_GATE0                (SYS_REG_P2V(CLK_REG_BASE) + 0x0070)
#define REG_PMU_CLK_GATE1                (SYS_REG_P2V(CLK_REG_BASE) + 0x0074)
#define REG_PMU_CLK_GATE2                (SYS_REG_P2V(CLK_REG_BASE) + 0x0078)
#define REG_PMU_PRE_DIV_GATE             (SYS_REG_P2V(CLK_REG_BASE) + 0x007C)
#define REG_PMU_CLK_DIV0                 (SYS_REG_P2V(CLK_REG_BASE) + 0x0090)
#define REG_PMU_CLK_DIV1                 (SYS_REG_P2V(CLK_REG_BASE) + 0x0094)
#define REG_PMU_CLK_DIV2                 (SYS_REG_P2V(CLK_REG_BASE) + 0x0098)
#define REG_PMU_CLK_DIV3                 (SYS_REG_P2V(CLK_REG_BASE) + 0x009c)
#define REG_PMU_CLK_DIV4                 (SYS_REG_P2V(CLK_REG_BASE) + 0x00a0)
#define REG_PMU_CLK_DIV5                 (SYS_REG_P2V(CLK_REG_BASE) + 0x00a4)
#define REG_PMU_CLK_DIV6                 (SYS_REG_P2V(CLK_REG_BASE) + 0x00a8)
#define REG_PMU_CLK_DIV7                 (SYS_REG_P2V(CLK_REG_BASE) + 0x00ac)
#define REG_PMU_CLK_DIV8                 (SYS_REG_P2V(CLK_REG_BASE) + 0x00b0)
#define REG_PMU_CLK_DIV9                 (SYS_REG_P2V(CLK_REG_BASE) + 0x00b4)

#define REG_PMU_SWRST_MAIN_CTRL          (SYS_REG_P2V(CLK_REG_BASE) + 0x00e0)
#define REG_PMU_SWRST_AHB_CTRL           (SYS_REG_P2V(CLK_REG_BASE) + 0x00f0)
#define REG_PMU_SWRST_APB_CTRL           (SYS_REG_P2V(CLK_REG_BASE) + 0x00f8)
#define REG_PMU_SWRSTN_NSR               (SYS_REG_P2V(CLK_REG_BASE) + 0x0100)

/* CPU_RSTN_REG_BASE */
#define REG_PMU_CPU_GATE                 (SYS_REG_P2V(CPU_RESET_REG_BASE) + 0x0000)
#define REG_PMU_CPU_SWRST                (SYS_REG_P2V(CPU_RESET_REG_BASE) + 0x0004)
#define REG_PMU_CPU_SWRSTN_NSR           (SYS_REG_P2V(CPU_RESET_REG_BASE) + 0x0008)

/* ISP_SYS_RSTN_BASE */
#define REG_PMU_ISP_GATE                 (SYS_REG_P2V(ISP_SYS_RSTN_REG_BASE) + 0x0000)
#define REG_PMU_ISP_SWRST                (SYS_REG_P2V(ISP_SYS_RSTN_REG_BASE) + 0x0004)

/* VEU_SYS_RSTN_BASE */
#define REG_PMU_VEU_GATE                 (SYS_REG_P2V(VEU_SYS_RSTN_REG_BASE) + 0x0000)
#define REG_PMU_VEU_SWRST                (SYS_REG_P2V(VEU_SYS_RSTN_REG_BASE) + 0x0004)

#define REG_DDR_PLL0_CTRL0      (SYS_REG_P2V(DDR_SYS_RSTN_BASE) + 0x0000)
#define REG_DDR_PLL0_CTRL1      (SYS_REG_P2V(DDR_SYS_RSTN_BASE) + 0x0004)
#define REG_DDR_CLK_CTRL        (SYS_REG_P2V(DDR_SYS_RSTN_BASE) + 0x0008)
#define REG_PMU_DDR_SCU_WREN     (SYS_REG_P2V(DDR_SYS_REG_BASE) + 0x0014)

/* AON_REG_BASE */
#define REG_ARC_BOOT0            (SYS_REG_P2V(AON_REG_CTRL) + 0x00)
#define REG_ARC_BOOT1            (SYS_REG_P2V(AON_REG_CTRL) + 0x04)
#define REG_ARC_BOOT2            (SYS_REG_P2V(AON_REG_CTRL) + 0x08)
#define REG_ARC_BOOT3            (SYS_REG_P2V(AON_REG_CTRL) + 0x0c)
#define REG_IP_CTRL              (SYS_REG_P2V(AON_REG_CTRL) + 0x10)
#define REG_BUS_CTRL             (SYS_REG_P2V(AON_REG_CTRL) + 0x14)
#define REG_AON_LPC_CTRL         (SYS_REG_P2V(AON_REG_CTRL) + 0x18)
#define REG_AON2_TOP_LPC_CTRL    (SYS_REG_P2V(AON_REG_CTRL) + 0x1c)
#define REG_UART0_AUTO_CG        (SYS_REG_P2V(AON_REG_CTRL) + 0x20)
#define REG_I2C0_AUTO_CG         (SYS_REG_P2V(AON_REG_CTRL) + 0x24)
#define REG_PWM_STM_AUTO_CG      (SYS_REG_P2V(AON_REG_CTRL) + 0x28)
#define REG_ARC_INT_MASK         (SYS_REG_P2V(AON_REG_CTRL) + 0x30)
#define REG_AON_SCU_PLL_WREN     (SYS_REG_P2V(AON_REG_CTRL) + 0x34)
#define REG_PTSLO                (SYS_REG_P2V(AON_REG_CTRL) + 0xa0)
#define REG_PTSHI                (SYS_REG_P2V(AON_REG_CTRL) + 0xa4)
#define REG_PMU_USER0            (SYS_REG_P2V(AON_REG_CTRL) + 0x7c)
#define REG_AUTO_CG_STATE        (SYS_REG_P2V(AON_REG_CTRL) + 0x80)
#define REG_DEBUG_STAT           (SYS_REG_P2V(AON_REG_CTRL) + 0x84)
#define REG_AON_LP_STATE         (SYS_REG_P2V(AON_REG_CTRL) + 0x8c)
#define REG_AON_LP_GATE          (SYS_REG_P2V(AON_REG_CTRL) + 0x90)
#define REG_SUB_CPU_INTC_MASK    REG_ARC_INT_MASK
#define REG_PMU_PTSLO            REG_PTSLO
#define REG_PMU_PTSHI            REG_PTSHI

#define REG_PMU_A625BOOT0        REG_ARC_BOOT0
#define REG_PMU_A625BOOT1        REG_ARC_BOOT1
#define REG_PMU_A625BOOT2        REG_ARC_BOOT2
#define REG_PMU_A625BOOT3        REG_ARC_BOOT3
#define REG_PMU_A625_START_CTRL  REG_IP_CTRL

#define REG_AON_CLK_SEL0         (SYS_REG_P2V(AON_SCU_CTRL) + 0x00)
#define REG_AON_CLK_SEL1         (SYS_REG_P2V(AON_SCU_CTRL) + 0x04)
#define REG_AON_CLK_SEL2         (SYS_REG_P2V(AON_SCU_CTRL) + 0x08)
#define REG_AON_CLK_GATE0        (SYS_REG_P2V(AON_SCU_CTRL) + 0x20)
#define REG_AON_CLK_DIV0         (SYS_REG_P2V(AON_SCU_CTRL) + 0x28)
#define REG_AON_CLK_DIV1         (SYS_REG_P2V(AON_SCU_CTRL) + 0x2c)
#define REG_AON_CLK_DIV2         (SYS_REG_P2V(AON_SCU_CTRL) + 0x30)
#define REG_AON_SWRST_MAIN_CTRL0 (SYS_REG_P2V(AON_SCU_CTRL) + 0x50)
#define REG_AON_SWRST_MAIN_CTRL0_NSR (SYS_REG_P2V(AON_SCU_CTRL) + 0x54)
#define REG_AON_SWRST_APB_CTRL   (SYS_REG_P2V(AON_SCU_CTRL) + 0x58)
#define REG_AON_SCU_RST_CLEAR    (SYS_REG_P2V(AON_SCU_CTRL) + 0x100)
#define REG_AON_SCU_RST_STATUS   (SYS_REG_P2V(AON_SCU_CTRL) + 0x104)
#define REG_AON_RC_CTRL          (SYS_REG_P2V(AON_SCU_CTRL) + 0x120)

#define PMU_DWI2S_CLK_SEL_REG   (REG_PMU_CLK_SEL3)
#define PMU_DWI2S_CLK_SEL_SHIFT (24)
#define PMU_DWI2S_CLK_DIV_REG   (REG_PMU_CLK_DIV6)
#define PMU_DWI2S_CLK_DIV_SHIFT (0)

/* SWRST_MAIN_CTRL */
#define CPU_RSTN_BIT             (0)
#define UTMI_RSTN_BIT            (1)
#define DDRPHY_RSTN_BIT          (2)
#define DDRC_RSTN_BIT            (3)
#define GPIO0_DB_RSTN_BIT        (4)
#define GPIO1_DB_RSTN_BIT        (5)
#define PIXEL_RSTN_BIT           (6)
#define PWM_RSTN_BIT             (7)
#define SPI0_RSTN_BIT            (8)
#define SPI1_RSTN_BIT            (9)
#define I2C0_RSTN_BIT            (10)
#define I2C1_RSTN_BIT            (11)
#define ACODEC_RSTN_BIT          (12)
#define I2S_RSTN_BIT             (13)
#define UART0_RSTN_BIT           (14)
#define UART1_RSTN_BIT           (15)
#define SADC_RSTN_BIT            (16)
#define VEU_ADAPT_RSTN_BIT       (17)
#define TMR_RSTN_BIT             (18)
#define UART2_RSTN_BIT           (15)
#define SPI2_RSTN_BIT            (20)
#define JPEG_ADAPT_RSTN_BIT      (21)
#define ARC_RSTN_BIT             (22)
#define EFUSE_RSTN_BIT           (23)
#define JPEG_RSTN_BIT            (24)
#define VEU_RSTN_BIT             (25)
#define NN_RSTN_BIT              (26)
#define ISP_RSTN_BIT             (27)
#define BGM_RSTN_BIT             (28)
#define PTS_RSTN_BIT             (29)
#define EPHY_RSTN_BIT            (30)
#define SYS_RSTN_BIT             (31)

/* SWRST_AHB_CTRL */
#define EMC_HRSTN_BIT            (0)
#define SDC1_HRSTN_BIT           (1)
#define SDC0_HRSTN_BIT           (2)
#define AES_HRSTN_BIT            (3)
#define DMAC0_HRSTN_BIT          (4)
#define INTC_HRSTN_BIT           (5)
#define JPEG_HRSTN_BIT           (6)
#define JPEG_ADAPT_HRSTN_BIT     (7)
#define VEU_HRSTN_BIT            (8)
#define VCU_HRSTN_BIT            (9)
#define NN_HRSTN_BIT             (10)
#define ISP_HRSTN_BIT            (11)
#define USB_HRSTN_BIT            (12)
#define HRSTN_BIT                (13)
#define EMAC_HRSTN_BIT           (17)
#define DDRC_HRSTN_BIT           (19)
#define BGM_HRSTN_BIT            (22)


/* SWRST_APB_CTRL */
#define ACODEC_PRSTN_BIT           (0)
#define UART1_PRSTN_BIT            (2)
#define UART0_PRSTN_BIT            (3)
#define SPI0_PRSTN_BIT             (4)
#define SPI1_PRSTN_BIT             (5)
#define GPIO0_PRSTN_BIT            (6)
#define UART2_PRSTN_BIT            (7)
#define I2C2_PRSTN_BIT             (8)
#define I2C0_PRSTN_BIT             (9)
#define I2C1_PRSTN_BIT             (10)
#define TMR_PRSTN_BIT              (11)
#define PWM_PRSTN_BIT              (12)
#define MIPIW_PRSTN_BIT            (13)
#define MIPIC_PRSTN_BIT            (14)
#define RTC_PRSTN_BIT              (15)
#define SADC_PRSTN_BIT             (16)
#define EFUSE_PRSTN_BIT            (17)
#define SPI2_PRSTN_BIT             (18)
#define WDT_PRSTN_BIT              (19)
#define GPIO1_PRSTN_BIT            (20)

/*USB*/
#define USB_UTMI_RST_BIT      (0x1<<1)
#define USB_PHY_RST_BIT       (0x11)
#define USB_SLEEP_MODE_BIT    (0x1<<24)
#define USB_IDDQ_PWR_BIT      (0x1<<10)
#define USB_VBUS_PWR_GPIO     (47)

#define REG_PMU_USB_CFG                  (SYS_REG_P2V(ANA_REG_BASE) + 0x0000)
#define REG_PMU_USB_SYS                  (SYS_REG_P2V(ANA_REG_BASE) + 0x0004)
#define REG_PMU_USB_SYS1                 (SYS_REG_P2V(ANA_REG_BASE) + 0x0008)
#define REG_PMU_USB_TUNE                 (SYS_REG_P2V(ANA_REG_BASE) + 0x000c)
#define REG_PMU_GMAC_REG                 (SYS_REG_P2V(ANA_REG_BASE) + 0x0100)
#define REG_PMU_EPHY_SEL                 (SYS_REG_P2V(ANA_REG_BASE) + 0x0180)


/*SDIO*/
#define SIMPLE_0     (0)
#define SIMPLE_22    (1)
#define SIMPLE_45    (2)
#define SIMPLE_67    (3)
#define SIMPLE_90    (4)
#define SIMPLE_112   (5)
#define SIMPLE_135   (6)
#define SIMPLE_157   (7)
#define SIMPLE_180   (8)
#define SIMPLE_202   (9)
#define SIMPLE_225   (10)
#define SIMPLE_247   (11)
#define SIMPLE_270   (12)
#define SIMPLE_292   (13)
#define SIMPLE_315   (14)
#define SIMPLE_337   (15)


#define SDIO0_CLK_DRV_DEGREE (SIMPLE_180)
#define SDIO0_CLK_SAM_DEGREE (SIMPLE_0)
#define SDIO1_CLK_DRV_DEGREE (SIMPLE_180)
#define SDIO1_CLK_SAM_DEGREE (SIMPLE_0)
#define SDIO_PHASE_BIT_MUX  0xf

/* inside fhy below */
#define CLK_SCAN_BIT_POS                (28)
#define INSIDE_PHY_ENABLE_BIT_POS       (24)
#define MAC_REF_CLK_DIV_MASK            (0x0f)
#define MAC_REF_CLK_DIV_BIT_POS         (24)
#define MAC_PAD_RMII_CLK_MASK           (0x0f)
#define MAC_PAD_RMII_CLK_BIT_POS        (24)
#define MAC_PAD_MAC_REF_CLK_BIT_POS     (28)
#define ETH_REF_CLK_OUT_GATE_BIT_POS    (25)
#define ETH_RMII_CLK_OUT_GATE_BIT_POS   (28)
#define IN_OR_OUT_PHY_SEL_BIT_POS       (26)
#define INSIDE_CLK_GATE_BIT_POS         (0)
#define INSIDE_PHY_SHUTDOWN_BIT_POS     (31)
#define INSIDE_PHY_RST_BIT_POS          (30)
#define INSIDE_PHY_TRAINING_BIT_POS     (27)
#define INSIDE_PHY_TRAINING_MASK        (0x0f)
#define TRAINING_EFUSE_ACTIVE_BIT_POS     4
#define HASH_DMA_HANDSHAKE (14)

#define EXTERNAL_PHY_RESET_GPIO    (29)

/* inside fhy done */
#endif
