
#ifndef __ARCH_H__
#define __ARCH_H__

/*****************************/
/* BSP CONTROLLER BASE       */
/*****************************/
#define DMAC0_REG_BASE   (0x00500000)
#define DMAC1_REG_BASE   (0x00510000)
#define INTC_REG_BASE    (0x00520000)
#define AES_REG_BASE     (0x00530000)

#define AON_RAM_BASE     (0x0d000000)
#define AON_REG_CTRL     (0x0d400000)
#define AON_SCU_CTRL     (0x0d410000)
#define AON_PINMUX_BASE  (0x0d420000)
#define AON_POWERCTRL_BASE (0x0d430000)
#define SPINLOCK_REG_BASE  (0x0d470000)

#define PERIPH_REG_BASE (0x04100000)
#define SPI0_REG_BASE   (0x04110000)
#define SPI1_REG_BASE   (0x04120000)
#define SPIC_AHB_REG_BASE (0x04200000)
#define UART1_REG_BASE  (0x04500000)
#define UART0_REG_BASE  (0x0d460000)
#define I2C0_REG_BASE   (0x0d4a0000)
#define I2C1_REG_BASE   (0x04550000)
#define PWM_REG_BASE    (0x0d4b0000)
#define STM0_REG_BASE   (0x04680000)
#define STM1_REG_BASE   (0x04681000)
#define WDT_REG_BASE    (0x04630000)
#define GPIO0_REG_BASE  (0x0d440000)
#define GPIO1_REG_BASE  (0x04640000)
#define TMR0_REG_BASE   (0x0d450000)
#define HWSPINLOCK_REG_BASE  (0x0d470000)
#define TMR1_REG_BASE   (0x04670000)
#define SADC_REG_BASE   (0x05060000)
#define DDRC_REG_BASE   (0x04820000)
#define PERF_REG_BASE   (0x04830000)
#define MPPC_REG_BASE   (0x04840000)
#define ANA_REG_BASE    (0x05000000)
#define ACW_REG_BASE    (0x05040000)
#define EFUSE_REG_BASE  (0x05050000)
#define SDC0_REG_BASE   (0x05100000)
#define SDC1_REG_BASE   (0x05200000)

#define SMT0_REG_BASE STM0_REG_BASE
#define SMT1_REG_BASE STM1_REG_BASE
#define TMR_REG_BASE TMR1_REG_BASE

#define VEU_REG_BASE             (0x07400000)
#define ISPP_REG_BASE            (0x09300000)
#define MIPI_PCS_WRAP_BASE       (0x09020000)
#define VICAP_REG_BASE           (0x09200000)


#define REG_EPHY_BASE            (0x05080000)
#define REG_MDIO_BASE            (REG_EPHY_BASE + 0x600)
#define GMAC_REG_BASE            (0x05400000)
#define USB_REG_BASE             (0x05300000)

#define CPU_CTRL_REG_BASE        (0x01000000)
#define CPU_SYS_APB_REG_BASE     CPU_CTRL_REG_BASE
#define CPU_RESET_REG_BASE       (0x01010000)
#define TOP_CTRL_REG_BASE        (0x04000000)
#define CLK_REG_BASE             (0x04010000)
#define PMU_REG_BASE             (0x04020000)
#define AHB_MON_TOP_REG_BASE     (0x04030000)
#define AHB_MON_NN_REG_BASE      (0x04070000)

#define NN_CRTL_REG_BASE         (0x07A00000)

#define VEU_SYS_CRTL_REG_BASE    (0x07000000)
#define VEU_SYS_RSTN_REG_BASE    (0x07010000)

#define ISP_SYS_CRTL_REG_BASE    (0x09000000)
#define ISP_SYS_RSTN_REG_BASE    (0x09010000)
#define ISP_REG_BASE             (0x09300000)
#define JPG_REG_BASE             (0x09700000)

#define DDR_SYS_REG_BASE    (0x04850000)
#define DDR_SYS_RSTN_BASE   (0x04860000)


#define SYS_REG_V2P(va)          (va)
#define SYS_REG_P2V(pa)          (pa)

#define REG_CPU_ADDR        (0x0d40004c)
#define REG_COH_ADDR        (0x0d400050)
#define ARM_IN_SLP          (0x1)

typedef enum IRQn
{
    DDR_IRQn           = 4,
    VEU_IRQn           = 5,
    INTC_IRQn          = 6,
    ISPP_IRQn          = 7,
    VPU_IRQn           = 9,
    PWM_IRQn           = 10,
    I2C0_IRQn          = 11,
    PWR_CTRL_IRQn      = 12,
    JPEG_IRQn          = 13,
    NNP_IRQn           = 14,
    LOOPBUF0_IRQn      = 15,
    UART0_IRQn         = 16,
    MIPI_TOP_IRQn      = 17,
    SADC_IRQn          = 19,
    SPI1_IRQn          = 21,
    JPG_LP0_IRQn       = 22,
    I2C1_IRQn          = 23,
    DMAC1_IRQn         = 24,
    WDT_IRQn           = 25,
    SPI0_IRQn          = 28,
    ARM_SW_IRQn        = 29,
    VEU_LP0_IRQn       = 30,
    GPIO0_IRQn         = 31,
    ARC_SW_IRQn        = 32,
    AES_IRQn           = 33,
    SMT0_IRQn          = 34,
    SMT1_IRQn          = 35,
    ACW_IRQn           = 36,
    AP_IRQn            = 37,
    USB_IRQn           = 39,
    GPIO1_IRQn         = 40,
    SDC0_IRQn          = 42,
    SDC1_IRQn          = 43,
    EMAC_IRQn          = 44,
    EPHY_IRQn          = 45,
    UART1_IRQn         = 50,
    PERF_IRQn          = 51,
    TMR0_IRQn          = 54,
    DMAC0_IRQn         = 58,
} IRQn_Type;

enum DMA_HW_HS_MAP
{
    ACODEC_RX = 0,
    ACODEC_TX,
    SPI1_RX,
    SPI1_TX,
    SPI0_RX,
    SPI0_TX,
    UART0_RX,
    UART0_TX,
    UART1_RX,
    UART1_TX,
};

#define TIMER_CLOCK        1000000
#define DMA_FIXED_CHANNEL_NUM       6

#endif /* ARCH_H_ */
