#ifndef __LPC4370_H__
#define __LPC4370_H__

#include <stdint.h>

typedef struct
{
	volatile uint32_t reserved0[64];
	volatile uint32_t capLength;
	volatile uint32_t hcsParams;
	volatile uint64_t hccParams;
	volatile uint32_t dciVersion;
	volatile uint32_t dccParams;
	volatile uint32_t reserved1[6];
	volatile uint32_t usbCmd;
	volatile uint32_t usbIF;
	volatile uint32_t usbIE;
	volatile uint32_t frameIndex;
	volatile uint32_t reserved2;
	volatile uint32_t deviceAddr;
	volatile uint32_t periodicListBase;
	volatile uint32_t listAddr;
	volatile uint32_t ttCtrl;
	volatile uint32_t burstSize;
	volatile uint32_t txFillTuning;
	volatile uint32_t reserved3[3];
	volatile uint32_t vFrameLen;
	volatile uint32_t epNAK;
	volatile uint32_t epNAKEnable;
	volatile uint32_t reserved4;
	volatile uint32_t portSC1;
	volatile uint32_t reserved5[2];
	volatile uint32_t otgSC;
	volatile uint32_t usbMode;
	volatile uint32_t epSetupStat;
	volatile uint32_t epPrime;
	volatile uint32_t epFlush;
	volatile uint32_t epStat;
	volatile uint32_t epComplete;
	union
	{
		volatile uint32_t epCtrl[6];
		struct
		{
			volatile uint32_t epCtrl0;
			volatile uint32_t epCtrl1;
			volatile uint32_t epCtrl2;
			volatile uint32_t epCtrl3;
			volatile uint32_t epCtrl4;
			volatile uint32_t epCtrl5;
		};
	};
} lpcUSB_t;

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

#define USB0				((lpcUSB_t *)0x40006000)
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

#define NVIC_SETIE0			*((volatile uint32_t *)0xE000E100)
#define NVIC_SETIE1			*((volatile uint32_t *)0xE000E104)
#define NVIC_CLRIE0			*((volatile uint32_t *)0xE000E180)
#define NVIC_CLRIE1			*((volatile uint32_t *)0xE000E184)
#define NVIC_SETPND0		*((volatile uint32_t *)0xE000E200)
#define NVIC_SETPND1		*((volatile uint32_t *)0xE000E204)
#define NVIC_CLRPND0		*((volatile uint32_t *)0xE000E280)
#define NVIC_CLRPND1		*((volatile uint32_t *)0xE000E284)
#define NVIC_ACTIVE0		*((volatile uint32_t *)0xE000E300)
#define NVIC_ACTIVE1		*((volatile uint32_t *)0xE000E304)
#define NVIC_PRI0			*((volatile uint32_t *)0xE000E400)
#define NVIC_PIR1			*((volatile uint32_t *)0xE000E404)
#define NVIC_PRI2			*((volatile uint32_t *)0xE000E408)
#define NVIC_PIR3			*((volatile uint32_t *)0xE000E40C)
#define NVIC_PRI4			*((volatile uint32_t *)0xE000E410)
#define NVIC_PIR5			*((volatile uint32_t *)0xE000E414)
#define NVIC_PRI6			*((volatile uint32_t *)0xE000E418)
#define NVIC_PIR7			*((volatile uint32_t *)0xE000E41C)
#define NVIC_SW_TRIG		*((volatile uint32_t *)0xE000EF00)

#define USB_CL_LENGTH_MASK		0x000000FF
#define USB_CL_VERSION_MASK		0x00FFFF00

#define USB_HCS_PORTS_MASK		0x0000000F
#define USB_HCS_PPCSUP_MASK		0x00000010
#define USB_HCS_PCC_MASK		0x00000F00
#define USB_HCS_CC_MASK			0x0000F000
#define USB_HCS_PISUP_MASK		0x00010000
#define USB_HCS_PTT_MASK		0x00F00000
#define USB_HCS_TT_MASK			0x0F000000

#define USB_HCC_ADDR64_CAP		0x00000001
#define USB_HCC_PFL_FLAG		0x00000002
#define USB_HCC_ASP_CAP			0x00000004
#define USB_HCC_ISOSCH_THRESH	0x000000F0
#define USB_HCC_ECC_PTR			0x0000FF00

#define USB_DCI_VERSION_MASK	0x0000FFFF

#define USB_DCC_DEN_MASK		0x0000001F
#define USB_DCC_DEV_CAP			0x00000080
#define USB_DCC_HOST_CAP		0x00000100

#define USB_DCMD_RUN			0x00000001
#define USB_DCMD_STOP_MASK		0xFFFFFFFE
#define USB_DCMD_RST			0x00000002
#define USB_DCMD_SUTW			0x00002000
#define USB_DCMD_ATDTW			0x00004000

#define USB_HCMD_RUN			0x00000001
#define USB_HCMD_STOP_MASK		0xFFFFFFFE
#define USB_HCMD_RST			0x00000002
#define USB_HCMD_FS01			0x0000000C
#define USB_HCMD_PSE			0x00000010
#define USB_HCMD_ASE			0x00000020
#define USB_HCMD_IAA			0x00000040
#define USB_HCMD_ASP_MODE_1		0x00000100
#define USB_HCMD_ASP_MODE_3		0x00000300
#define USB_HCMD_ASP_EN			0x00000800
#define USB_HCMD_FS2			0x00008000

#define USB_CMD_ITC				0x00FF0000
#define USB_CMD_ITC_IMM			0x00000000
#define USB_CMD_ITC_1			0x00010000
#define USB_CMD_ITC_2			0x00020000
#define USB_CMD_ITC_8			0x00080000
#define USB_CMD_ITC_16			0x00100000
#define USB_CMD_ITC_32			0x00200000
#define USB_CMD_ITC_64			0x00400000

#define USB_HCMD_FS_MASK		0x0000800C
#define USB_HCMD_FS_1024		0x00000000
#define USB_HCMD_FS_512			0x00000004
#define USB_HCMD_FS_256			0x00000008
#define USB_HCMD_FS_128			0x0000000C
#define USB_HCMD_FS_64			0x00008000
#define USB_HCMD_FS_32			0x00008004
#define USB_HCMD_FS_16			0x00008008
#define USB_HCMD_FS_8			0x0000800C

#define USB_INT_UI				0x00000001
#define USB_INT_UEI				0x00000002
#define USB_INT_PCI				0x00000004
#define USB_HINT_FRI			0x00000008
#define USB_HINT_AAI			0x00000020
#define USB_DINT_URI			0x00000040
#define USB_INT_SRI				0x00000080
#define USB_DINT_SLI			0x00000100
#define USB_HINT_HCH			0x00001000
#define USB_HINT_RCL			0x00002000
#define USB_HINT_PS				0x00004000
#define USB_HINT_AS				0x00008000
#define USB_DINT_NAKI			0x00010000
#define USB_HINT_UAI			0x00040000
#define USB_HINT_UPI			0x00080000

#define USB_INT_UIE				0x00000001
#define USB_INT_UEIE			0x00000002
#define USB_INT_PCIE			0x00000004
#define USB_HINT_FRIE			0x00000008
#define USB_HINT_AAIE			0x00000020
#define USB_DINT_URIE			0x00000040
#define USB_INT_SRIE			0x00000080
#define USB_DINT_SLIE			0x00000100
#define USB_DINT_NAKIE			0x00010000
#define USB_HINT_UAIE			0x00040000
#define USB_HINT_UPIE			0x00080000

#define USB_FRINDEX_MICRO		0x00000007
#define USB_DFRINDEX_LASTTX		0x00003FF8
#define USB_HFRINDEX_LISTIDX	0x00001FF8

#define USB_DEVICEADDR_MASK		0xFE000000
/* This allows the USB controller to have the USB address written in advance of being set for SET_ADDRESS */
#define USB_DEVICEADDR_ADV		0x01000000

#define USB_PERBASE_ADDR		0xFFFFF000
#define USB_DEPBASE_ADDR		0xFFFFF800
#define USB_HASYBASE_ADDR		0xFFFFFFE0

#define USB_TTHA_MASK			0x7F000000
#define USB_BURSTSIZE_RXLEN		0x000000FF
#define USB_BURSTSIZE_TXLEN		0x0000FF00

#define USB_BINTERVAL_MASK		0x0000000F

#define USB_SC1_FPR				0x00000040
#define USB_SC1_PFSC_ANY		0x00000000
#define USB_SC1_PFSC_FSONLY		0x01000000
#define USB_SC1_PSPD_FS			0x00000000
#define USB_SC1_PSPD_LS			0x04000000
#define USB_SC1_PSPD_HS			0x08000000

#define USB_MODE_IDLE			0x00000000
#define USB_MODE_DEVICE			0x00000002
#define USB_MODE_HOST			0x00000003
#define USB_MODE_BIGEND			0x00000004
#define USB_MODE_SLOD			0x00000008
#define USB_MODE_SD				0x00000010

#define USB_EP0_RX_MASK			0x00000001
#define USB_EP1_RX_MASK			0x00000002
#define USB_EP2_RX_MASK			0x00000004
#define USB_EP3_RX_MASK			0x00000008
#define USB_EP4_RX_MASK			0x00000010
#define USB_EP5_RX_MASK			0x00000020
#define USB_EP_RX_MASK			0x0000003F
#define USB_EP0_TX_MASK			0x00010000
#define USB_EP1_TX_MASK			0x00020000
#define USB_EP2_TX_MASK			0x00040000
#define USB_EP3_TX_MASK			0x00080000
#define USB_EP4_TX_MASK			0x00100000
#define USB_EP5_TX_MASK			0x00200000
#define USB_EP_TX_MASK			0x003F0000
#define USB_EP_ALL_MASK			0x003F003F

#define USB_EP_RX_OK_MASK		0xFFFFFFFE
#define USB_EP_RX_STALLED		0x00000001
#define USB_EP_RX_TYPE_CTRL		0x00000000
#define USB_EP_RX_TYPE_ISO		0x00000004
#define USB_EP_RX_TYPE_BULK		0x00000008
#define USB_EP_RX_TYPE_INTR		0x0000000C
#define USB_EP_RXI				0x00000020
#define USB_EP_RXR				0x00000040
#define USB_EP_RXE				0x00000080

#define USB_EP_TX_OK_MASK		0xFFFEFFFF
#define USB_EP_TX_STALLED		0x00010000
#define USB_EP_TX_TYPE_CTRL		0x00000000
#define USB_EP_TX_TYPE_ISO		0x00040000
#define USB_EP_TX_TYPE_BULK		0x00080000
#define USB_EP_TX_TYPE_INTR		0x000C0000
#define USB_EP_TXI				0x00200000
#define USB_EP_TXR				0x00400000
#define USB_EP_TXE				0x00800000

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

