#ifndef __LPC4370_H__
#define __LPC4370_H__

#include <stdint.h>

typedef struct
{
	volatile uint32_t IR;
	volatile uint32_t TCR;
	volatile uint32_t TC;
	volatile uint32_t PR;
	volatile uint32_t RC;
	volatile uint32_t MCR;
	volatile uint32_t MR0;
	volatile uint32_t MR1;
	volatile uint32_t MR2;
	volatile uint32_t CCR;
	volatile uint32_t CR0;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t EMR;
	volatile uint32_t reserved[12];
	volatile uint32_t CTCR;
} lpcTimer_t;

/*
 * SCU stands for System Controller Unit
 * SFS stands for Special Function Select
 */
typedef struct
{
	volatile uint32_t SFS_Port0[32];
	volatile uint32_t SFS_Port1[32];
	volatile uint32_t SFS_Port2[32];
	volatile uint32_t SFS_Port3[32];
	volatile uint32_t SFS_Port4[32];
	volatile uint32_t SFS_Port5[32];
	volatile uint32_t SFS_Port6[32];
	volatile uint32_t SFS_Port7[32];
	volatile uint32_t SFS_Port8[32];
	volatile uint32_t SFS_Port9[32];
	volatile uint32_t SFS_PortA[32];
	volatile uint32_t SFS_PortB[32];
	volatile uint32_t SFS_PortC[32];
	volatile uint32_t SFS_PortD[32];
	volatile uint32_t SFS_PortE[32];
	volatile uint32_t SFS_PortF[32];
	volatile uint32_t SFS_PortClk[32];
	volatile uint32_t SFS_USB;
	volatile uint32_t SFS_I2C0;
	volatile uint32_t ENAIO0;
	volatile uint32_t ENAIO1;
	volatile uint32_t ENAIO2;
	volatile uint32_t reserved0[0x1C];
	volatile uint32_t EMC_DLYCLK;
	volatile uint32_t reserved1[0x1F];
	volatile uint32_t SD_DELAY;
	volatile uint32_t reserved2[0x1F];
	volatile uint32_t PINT_SEL0;
	volatile uint32_t PINT_SEL1;
} lpcSCU_t;

typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t CCR; /* aka CPSR */
	volatile uint32_t reserved[3];
	volatile uint32_t INT;
} lpcSPI_t;

#define Timer0				((lpcTimer_t *)0x40084000)
#define Timer1				((lpcTimer_t *)0x40085000)
#define SCU					((lpcSCU_t *)0x40086000)
#define Timer2				((lpcTimer_t *)0x400C3000)
#define Timer3				((lpcTimer_t *)0x400C4000)

#define GPIO_PORT0_BYTES	((volatile uint8_t *)0x400F4000)
#define GPIO_PORT0_WORDS	((volatile uint32_t *)0x400F5000)
#define GPIO_PORT0_DIR		*((volatile uint32_t *)0x400F6000)
#define GPIO_PORT0_MASK		*((volatile uint32_t *)0x400F6080)
#define GPIO_PORT0_PIN		*((volatile uint32_t *)0x400F6100)
#define GPIO_PORT0_MPIN		*((volatile uint32_t *)0x400F6180)
#define GPIO_PORT0_SET		*((volatile uint32_t *)0x400F6200)
#define GPIO_PORT0_CLR		*((volatile uint32_t *)0x400F6280)
#define GPIO_PORT0_TGL		*((volatile uint32_t *)0x400F6300)

#define GPIO_PORT1_BYTES	((volatile uint8_t *)0x400F4020)
#define GPIO_PORT1_WORDS	((volatile uint32_t *)0x400F5080)
#define GPIO_PORT1_DIR		*((volatile uint32_t *)0x400F6004)
#define GPIO_PORT1_MASK		*((volatile uint32_t *)0x400F6084)
#define GPIO_PORT1_PIN		*((volatile uint32_t *)0x400F6104)
#define GPIO_PORT1_MPIN		*((volatile uint32_t *)0x400F6184)
#define GPIO_PORT1_SET		*((volatile uint32_t *)0x400F6204)
#define GPIO_PORT1_CLR		*((volatile uint32_t *)0x400F6284)
#define GPIO_PORT1_TGL		*((volatile uint32_t *)0x400F6304)

#define GPIO_PORT2_BYTES	((volatile uint8_t *)0x400F4040)
#define GPIO_PORT2_WORDS	((volatile uint32_t *)0x400F5100)
#define GPIO_PORT2_DIR		*((volatile uint32_t *)0x400F6008
#define GPIO_PORT2_MASK		*((volatile uint32_t *)0x400F6088)
#define GPIO_PORT2_PIN		*((volatile uint32_t *)0x400F6108)
#define GPIO_PORT2_MPIN		*((volatile uint32_t *)0x400F6188)
#define GPIO_PORT2_SET		*((volatile uint32_t *)0x400F6208)
#define GPIO_PORT2_CLR		*((volatile uint32_t *)0x400F6288)
#define GPIO_PORT2_TGL		*((volatile uint32_t *)0x400F6308)

#define GPIO_PORT3_BYTES	((volatile uint8_t *)0x400F4060)
#define GPIO_PORT3_WORDS	((volatile uint32_t *)0x400F5180)
#define GPIO_PORT3_DIR		*((volatile uint32_t *)0x400F600C)
#define GPIO_PORT3_MASK		*((volatile uint32_t *)0x400F608C)
#define GPIO_PORT3_PIN		*((volatile uint32_t *)0x400F610C)
#define GPIO_PORT3_MPIN		*((volatile uint32_t *)0x400F618C)
#define GPIO_PORT3_SET		*((volatile uint32_t *)0x400F620C)
#define GPIO_PORT3_CLR		*((volatile uint32_t *)0x400F628C)
#define GPIO_PORT3_TGL		*((volatile uint32_t *)0x400F630C)

#define GPIO_PORT4_BYTES	((volatile uint8_t *)0x400F4080)
#define GPIO_PORT4_WORDS	((volatile uint32_t *)0x400F5200)
#define GPIO_PORT4_DIR		*((volatile uint32_t *)0x400F6010)
#define GPIO_PORT4_MASK		*((volatile uint32_t *)0x400F6090)
#define GPIO_PORT4_PIN		*((volatile uint32_t *)0x400F6110)
#define GPIO_PORT4_MPIN		*((volatile uint32_t *)0x400F6190)
#define GPIO_PORT4_SET		*((volatile uint32_t *)0x400F6210)
#define GPIO_PORT4_CLR		*((volatile uint32_t *)0x400F6290)
#define GPIO_PORT4_TGL		*((volatile uint32_t *)0x400F6310)

#define GPIO_PORT5_BYTES	((volatile uint8_t *)0x400F40A0)
#define GPIO_PORT5_WORDS	((volatile uint32_t *)0x400F5280)
#define GPIO_PORT5_DIR		*((volatile uint32_t *)0x400F6014)
#define GPIO_PORT5_MASK		*((volatile uint32_t *)0x400F6094)
#define GPIO_PORT5_PIN		*((volatile uint32_t *)0x400F6114)
#define GPIO_PORT5_MPIN		*((volatile uint32_t *)0x400F6194)
#define GPIO_PORT5_SET		*((volatile uint32_t *)0x400F6214)
#define GPIO_PORT5_CLR		*((volatile uint32_t *)0x400F6294)
#define GPIO_PORT5_TGL		*((volatile uint32_t *)0x400F6314)

#define GPIO_PORT6_BYTES	((volatile uint8_t *)0x400F40C0)
#define GPIO_PORT6_WORDS	((volatile uint32_t *)0x400F5300)
#define GPIO_PORT6_DIR		*((volatile uint32_t *)0x400F6018)
#define GPIO_PORT6_MASK		*((volatile uint32_t *)0x400F6098)
#define GPIO_PORT6_PIN		*((volatile uint32_t *)0x400F6118)
#define GPIO_PORT6_MPIN		*((volatile uint32_t *)0x400F6198)
#define GPIO_PORT6_SET		*((volatile uint32_t *)0x400F6218)
#define GPIO_PORT6_CLR		*((volatile uint32_t *)0x400F6298)
#define GPIO_PORT6_TGL		*((volatile uint32_t *)0x400F6318)

#define GPIO_PORT7_BYTES	((volatile uint8_t *)0x400F40E0)
#define GPIO_PORT7_WORDS	((volatile uint32_t *)0x400F5380)
#define GPIO_PORT7_DIR		*((volatile uint32_t *)0x400F601C)
#define GPIO_PORT7_MASK		*((volatile uint32_t *)0x400F609C)
#define GPIO_PORT7_PIN		*((volatile uint32_t *)0x400F611C)
#define GPIO_PORT7_MPIN		*((volatile uint32_t *)0x400F619C)
#define GPIO_PORT7_SET		*((volatile uint32_t *)0x400F621C)
#define GPIO_PORT7_CLR		*((volatile uint32_t *)0x400F629C)
#define GPIO_PORT7_TGL		*((volatile uint32_t *)0x400F631C)

#define SPI					((lpcSPI_t *)0x40100000)

#define TIMER_IR_EVT3			0x00000080
#define TIMER_IR_EVT2			0x00000040
#define TIMER_IR_EVT1			0x00000020
#define TIMER_IR_EVT0			0x00000010
#define TIMER_IR_CH3			0x00000008
#define TIMER_IR_CH2			0x00000004
#define TIMER_IR_CH1			0x00000002
#define TIMER_IR_CH0			0x00000001

#define TIMER_TCR_CRST			0x00000002
#define TIMER_TCR_CEN			0x00000001

#define TIMER_MCR_MR0IE			0x00000001
#define TIMER_MCR_MR0RE			0x00000002
#define TIMER_MCR_MR0SE			0x00000004
#define TIMER_MCR_MR1IE			0x00000008
#define TIMER_MCR_MR1RE			0x00000010
#define TIMER_MCR_MR1SE			0x00000020
#define TIMER_MCR_MR2IE			0x00000040
#define TIMER_MCR_MR2RE			0x00000080
#define TIMER_MCR_MR2SE			0x00000100
#define TIMER_MCR_MR3IE			0x00000200
#define TIMER_MCR_MR3RE			0x00000400
#define TIMER_MCR_MR3SE			0x00000800

#define TIMER_CTCR_MODE_TIMER	0x00000000
#define TIMER_CTCR_MODE_COUNTR	0x00000001
#define TIMER_CTCR_MODE_COUNTF	0x00000002
#define TIMER_CTCR_MODE_COUNTE	0x00000003
#define TIMER_CTCR_SEL_CAP0		0x00000000
#define TIMER_CTCR_SEL_CAP1		0x00000004
#define TIMER_CTCR_SEL_CAP2		0x00000008
#define TIMER_CTCR_SEL_CAP3		0x0000000C

#define SCU_SFS_EHD_4			0x00000000
#define SCU_SFS_EHD_8			0x00000100
#define SCU_SFS_EHD_14			0x00000200
#define SCU_SFS_EHD_20			0x00000300
#define SCU_SFS_DIF				0x00000080
#define SCU_SFS_EIB				0x00000040
#define SCU_SFS_EHS				0x00000020
#define SCU_SFS_DPU				0x00000010
#define SCU_SFS_EPD				0x00000008
#define SCU_SFS_MODE_0			0x00000000
#define SCU_SFS_MODE_1			0x00000001
#define SCU_SFS_MODE_2			0x00000002
#define SCU_SFS_MODE_3			0x00000003
#define SCU_SFS_MODE_4			0x00000004
#define SCU_SFS_MODE_5			0x00000005
#define SCU_SFS_MODE_6			0x00000006
#define SCU_SFS_MODE_7			0x00000007

#define SPI_CR_DSS_8			0x00000800 /* 8-bit data */
#define SPI_CR_DSS_9			0x00000900 /* 9-bit data */
#define SPI_CR_DSS_10			0x00000A00 /* 10-bit data */
#define SPI_CR_DSS_11			0x00000B00 /* 11-bit data */
#define SPI_CR_DSS_12			0x00000C00 /* 12-bit data */
#define SPI_CR_DSS_13			0x00000D00 /* 13-bit data */
#define SPI_CR_DSS_14			0x00000E00 /* 14-bit data */
#define SPI_CR_DSS_15			0x00000F00 /* 15-bit data */
#define SPI_CR_DSS_16			0x00000000 /* 16-bit data */
#define SPI_CR_MASTER			0x00000020
#define SPI_CR_SPO				0x00000010
#define SPI_CR_SPH				0x00000008
#define SPI_CR_DSS_EN			0x00000004

#define SPI_SR_ABRT				0x00000008 /* SPI slave abort */
#define SPI_SR_MODF				0x00000010 /* SPI mode fault */
#define SPI_SR_ROVR				0x00000020 /* SPI read overrun */
#define SPI_SR_WCOL				0x00000040 /* SPI write collision */
#define SPI_SR_SPIF				0x00000080 /* SPI transfer completed */

#endif /*__LPC4370_H__*/

