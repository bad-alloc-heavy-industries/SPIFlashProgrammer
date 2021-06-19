// SPDX-License-Identifier: BSD-3-Clause
#ifndef TM4C123GH6PM_PLATFORM___HXX
#define TM4C123GH6PM_PLATFORM___HXX

#include <cstdint>
#include <array>

namespace tivaC
{
	// Interrupt number definitions
	enum class IRQn_t
	{
		// ARM Cortex-M4 Specific Interrupt Numbers
		reset = -15, // Reset Vector, invoked on Power up and warm reset
		nonMaskableInt = -14, // Non maskable Interrupt, cannot be stopped or preempted
		hardFault = -13, // Hard Fault, all classes of Fault
		memoryManagement = -12, // Memory Management, MPU mismatch, including Access Violation and No Match
		busFault = -11, // Bus Fault, Pre-Fetch-, Memory Access Fault, other address/memory related Fault
		usageFault = -10, // Usage Fault, i.e. Undef Instruction, Illegal State Transition
		svCall = -5, // System Service Call via SVC instruction
		debugMonitor = -4, // Debug Monitor
		pendSV = -2, // Pendable request for system service
		sysTick = -1, // System Tick Timer

		// TM4C123GH6PM Specific Interrupt Numbers
		gpioA = 0, // gpioA
		gpioB = 1, // gpioB
		gpioC = 2, // gpioC
		gpioD = 3, // gpioD
		gpioE = 4, // gpioE
		uart0 = 5, // uart0
		uart1 = 6, // uart1
		ssi0 = 7, // ssi0
		i2c0 = 8, // i2c0
		pwm0Fault = 9, // pwm0Fault
		pwm0chn0 = 10, // pwm0chn0
		pwm0chn1 = 11, // pwm0chn1
		pwm0chn2 = 12, // pwm0chn2
		qei0 = 13, // qei0
		adc0SS0 = 14, // adc0SS0
		adc0SS1 = 15, // adc0SS1
		adc0SS2 = 16, // adc0SS2
		adc0SS3 = 17, // adc0SS3
		watchdog0 = 18, // watchdog0
		timer0A = 19, // timer0A
		timer0B = 20, // timer0B
		timer1A = 21, // timer1A
		timer1B = 22, // timer1B
		timer2A = 23, // timer2A
		timer2B = 24, // timer2B
		comp0 = 25, // comp0
		comp1 = 26, // comp1
		sysCtrl = 28, // sysCtrl
		flashCtrl = 29, // flashCtrl
		gpioF = 30, // gpioF
		uart2 = 33, // uart2
		ssi1 = 34, // ssi1
		timer3A = 35, // timer3A
		timer3B = 36, // timer3B
		i2c1 = 37, // i2c1
		qei1 = 38, // qei1
		can0 = 39, // can0
		can1 = 40, // can1
		hib = 43, // hib
		usb0 = 44, // usb0
		pwm0chn3 = 45, // pwm0chn3
		udma = 46, // udma
		udmaErr = 47, // udmaErr
		adc1SS0 = 48, // adc1SS0
		adc1SS1 = 49, // adc1SS1
		adc1SS2 = 50, // adc1SS2
		adc1SS3 = 51, // adc1SS3
		ssi2 = 57, // ssi2
		ssi3 = 58, // ssi3
		uart3 = 59, // uart3
		uart4 = 60, // uart4
		uart5 = 61, // uart5
		uart6 = 62, // uart6
		uart7 = 63, // uart7
		i2c2 = 68, // i2c2
		i2c3 = 69, // i2c3
		timer4A = 70, // timer4A
		timer4B = 71, // timer4B
		timer5A = 92, // timer5A
		timer5B = 93, // timer5B
		wtimer0A = 94, // wtimer0A
		wtimer0B = 95, // wtimer0B
		wtimer1A = 96, // wtimer1A
		wtimer1B = 97, // wtimer1B
		wtimer2A = 98, // wtimer2A
		wtimer2B = 99, // wtimer2B
		wtimer3A = 100, // wtimer3A
		wtimer3B = 101, // wtimer3B
		wtimer4A = 102, // wtimer4A
		wtimer4B = 103, // wtimer4B
		wtimer5A = 104, // wtimer5A
		wtimer5B = 105, // wtimer5B
		sysExc = 106, // sysExc
		pwm1chn0 = 134, // pwm1chn0
		pwm1chn1 = 135, // pwm1chn1
		pwm1chn2 = 136, // pwm1chn2
		pwm1chn3 = 137, // pwm1chn3
		pwm1Fault = 138 // 138 pwm1Fault
	};

	// Watchdog periphal structure
	struct watchdog_t final
	{
		volatile uint32_t load;
		volatile uint32_t value;
		volatile uint32_t ctrl; // Watchdog Control
		volatile uint32_t icr; // Watchdog Interrupt Clear
		volatile uint32_t ris; // Watchdog Raw Interrupt Status
		volatile uint32_t mis; // Watchdog Masked Interrupt Status
		const volatile uint32_t reserved0[256];
		volatile uint32_t test;
		const volatile uint32_t reserved1[505];
		volatile uint32_t lock;
	};

	// GPIO peripheral structure
	struct gpio_t final
	{
		volatile uint32_t dataBits[255];
		volatile uint32_t data;
		volatile uint32_t dir; // GPIO Direction
		volatile uint32_t is; // GPIO Interrupt Sense
		volatile uint32_t ibe; // GPIO Interrupt Both Edges
		volatile uint32_t iev; // GPIO Interrupt Event
		volatile uint32_t im; // GPIO Interrupt Mask
		volatile uint32_t ris; // GPIO Raw Interrupt Status
		volatile uint32_t mis; // GPIO Masked Interrupt Status
		volatile uint32_t icr; // GPIO Interrupt Clear
		volatile uint32_t afSel; // GPIO Alternate Function Select
		const volatile uint32_t reserved1[55];
		volatile uint32_t dr2r; // GPIO 2-mA Drive Select
		volatile uint32_t dr4r; // GPIO 4-mA Drive Select
		volatile uint32_t dr8r; // GPIO 8-mA Drive Select
		volatile uint32_t odr; // GPIO Open Drain Select
		volatile uint32_t pur; // GPIO Pull-Up Select
		volatile uint32_t pdr; // GPIO Pull-Down Select
		volatile uint32_t slr; // GPIO Slew Rate Control Select
		volatile uint32_t den; // GPIO Digital Enable
		volatile uint32_t lock;
		volatile uint32_t commit;
		volatile uint32_t amSel; // GPIO Analog Mode Select
		volatile uint32_t portCtrl;
		volatile uint32_t adcCtrl;
		volatile uint32_t dmaCtrl;
	};

	// SSI peripheral structure
	struct ssi_t final
	{
		volatile uint32_t ctrl0; // SSI Control 0
		volatile uint32_t ctrl1; // SSI Control 1
		volatile uint32_t data;
		volatile uint32_t status;
		volatile uint32_t cpsr; // SSI Clock Prescale
		volatile uint32_t im; // SSI Interrupt Mask
		volatile uint32_t ris; // SSI Raw Interrupt Status
		volatile uint32_t mis; // SSI Masked Interrupt Status
		volatile uint32_t icr; // SSI Interrupt Clear
		volatile uint32_t dmaCtrl; // SSI DMA Control
		const volatile uint32_t reserved[1000];
		volatile uint32_t clockConfig;
	};

	// UART periphal structure
	struct uart_t final
	{
		volatile uint32_t data;
		union
		{
			volatile uint32_t status; // UART Receive Status/Error Clear
			volatile uint32_t errClr; // UART Receive Status/Error Clear
		};
		const volatile uint32_t reserved0[4];
		volatile uint32_t flag;
		const volatile uint32_t reserved1;
		volatile uint32_t ilpr; // UART IrDA Low-Power Register
		volatile uint32_t ibrd; // UART Integer Baud-Rate Divisor
		volatile uint32_t fbrd; // UART Fractional Baud-Rate Divisor
		volatile uint32_t lcrh; // UART Line Control
		volatile uint32_t ctrl; // UART Control
		volatile uint32_t ifls; // UART Interrupt FIFO Level Select
		volatile uint32_t im; // UART Interrupt Mask
		volatile uint32_t ris; // UART Raw Interrupt Status
		volatile uint32_t mis; // UART Masked Interrupt Status
		volatile uint32_t icr; // UART Interrupt Clear
		volatile uint32_t dmaCtrl; // UART DMA Control
		const volatile uint32_t reserved2[22];
		volatile uint32_t bit9Addr; // UART 9-Bit Self Address
		volatile uint32_t bit9AMask; // UART 9-Bit Self Address Mask
		const volatile uint32_t reserved3[965];
		volatile uint32_t peripheralProps;
		const volatile uint32_t reserved4;
		volatile uint32_t clockConfig;
	};

	// I2C periphal structure
	struct i2c_t final
	{
		volatile uint32_t controlerPeriphAddr; // I2C Controller Peripheral Address
		union
		{
			volatile uint32_t controlerCtrl;
			volatile uint32_t controlerStatus;
		};
		volatile uint32_t controlerData;
		volatile uint32_t controlerTP; // I2C Controller Timer Period
		volatile uint32_t controlerIM; // I2C Controller Interrupt Mask
		volatile uint32_t controlerRIS; // I2C Controller Raw Interrupt Status
		volatile uint32_t controlerMIS; // I2C Controller Masked Interrupt Status
		volatile uint32_t controlerICR; // I2C Controller Interrupt Clear
		volatile uint32_t controlerConfig1;
		volatile uint32_t controlerClkOCnt; // I2C Controller Clock Low Timeout Count
		const volatile uint32_t reserved0;
		volatile uint32_t controlerBMon; // I2C Controller Bus Monitor
		const volatile uint32_t reserved1[2];
		volatile uint32_t controlerConfig2;
		const volatile uint32_t reserved2[497];

		volatile uint32_t periphOwnAddr1; // I2C Peripheral Own (local device) Address 1
		union
		{
			volatile uint32_t periphCtrl;
			volatile uint32_t periphStatus; // I2C Peripheral Control/Status
		};
		volatile uint32_t periphData;
		volatile uint32_t periphIM; // I2C Peripheral Interrupt Mask
		volatile uint32_t periphRIS; // I2C Peripheral Raw Interrupt Status
		volatile uint32_t periphMIS; // I2C Peripheral Masked Interrupt Status
		volatile uint32_t periphICR; // I2C Peripheral Interrupt Clear
		volatile uint32_t periphOwnAddr2; // I2C Peripheral Own (local device) Address 2
		volatile uint32_t periphAckCtrl; // I2C Peripheral ACK Control
		const volatile uint32_t reserved3[487];

		volatile uint32_t peripheralProps; // I2C Peripheral Properties
		volatile uint32_t peripheralConfig; // I2C Peripheral Configuration
	};

	// PWM periphal structure
	struct pwm_t final
	{
		volatile uint32_t ctrl;
		volatile uint32_t sync;
		volatile uint32_t outputEnable;
		volatile uint32_t outputInvert;
		volatile uint32_t fault;
		volatile uint32_t intEn; // PWM Interrupt Enable
		volatile uint32_t ris; // PWM Raw Interrupt Status
		volatile uint32_t isc; // PWM Interrupt Status and Clear
		volatile uint32_t status;
		volatile uint32_t faultValue;
		volatile uint32_t enUpd; // PWM Enable Update
		const volatile uint32_t reserved0[5];
		volatile uint32_t chn0Ctrl; // PWM0 Control
		volatile uint32_t chn0IntEn; // PWM0 Interrupt and Trigger Enable
		volatile uint32_t chn0RIS; // PWM0 Raw Interrupt Status
		volatile uint32_t chn0ISC; // PWM0 Interrupt Status and Clear
		volatile uint32_t chn0Load;
		volatile uint32_t chn0Count;
		volatile uint32_t chn0CmpA; // PWM0 Compare A
		volatile uint32_t chn0CmpB; // PWM0 Compare B
		volatile uint32_t chn0GenA; // PWM0 Generator A Control
		volatile uint32_t chn0GenB; // PWM0 Generator B Control
		volatile uint32_t chn0DBCtrl; // PWM0 Dead-Band Control
		volatile uint32_t chn0DBRise; // PWM0 Dead-Band Rising-Edge Delay
		volatile uint32_t chn0DBFall; // PWM0 Dead-Band Falling-Edge-Delay
		volatile uint32_t chn0FaultSource0;
		volatile uint32_t chn0FaultSource1;
		volatile uint32_t chn0MinFaultPeriod;
		volatile uint32_t chn1Ctrl; // PWM1 Control
		volatile uint32_t chn1IntEn; // PWM1 Interrupt and Trigger Enable
		volatile uint32_t chn1RIS; // PWM1 Raw Interrupt Status
		volatile uint32_t chn1ISC; // PWM1 Interrupt Status and Clear
		volatile uint32_t chn1Load;
		volatile uint32_t chn1Count;
		volatile uint32_t chn1CmpA; // PWM1 Compare A
		volatile uint32_t chn1CmpB; // PWM1 Compare B
		volatile uint32_t chn1GenA; // PWM1 Generator A Control
		volatile uint32_t chn1GenB; // PWM1 Generator B Control
		volatile uint32_t chn1DBCtrl; // PWM1 Dead-Band Control
		volatile uint32_t chn1DBRise; // PWM1 Dead-Band Rising-Edge Delay
		volatile uint32_t chn1DBFall; // PWM1 Dead-Band Falling-Edge-Delay
		volatile uint32_t chn1FaultSource0;
		volatile uint32_t chn1FaultSource1;
		volatile uint32_t chn1MinFaultPeriod;
		volatile uint32_t chn2Ctrl; // PWM2 Control
		volatile uint32_t chn2IntEn; // PWM2 Interrupt and Trigger Enable
		volatile uint32_t chn2RIS; // PWM2 Raw Interrupt Status
		volatile uint32_t chn2ISC; // PWM2 Interrupt Status and Clear
		volatile uint32_t chn2Load;
		volatile uint32_t chn2Count;
		volatile uint32_t chn2CmpA; // PWM2 Compare A
		volatile uint32_t chn2CmpB; // PWM2 Compare B
		volatile uint32_t chn2GenA; // PWM2 Generator A Control
		volatile uint32_t chn2GenB; // PWM2 Generator B Control
		volatile uint32_t chn2DBCtrl; // PWM2 Dead-Band Control
		volatile uint32_t chn2DBRise; // PWM2 Dead-Band Rising-Edge Delay
		volatile uint32_t chn2DBFall; // PWM2 Dead-Band Falling-Edge-Delay
		volatile uint32_t chn2FaultSource0;
		volatile uint32_t chn2FaultSource1;
		volatile uint32_t chn2MinFaultPeriod;
		volatile uint32_t chn3Ctrl; // PWM3 Control
		volatile uint32_t chn3IntEn; // PWM3 Interrupt and Trigger Enable
		volatile uint32_t chn3RIS; // PWM3 Raw Interrupt Status
		volatile uint32_t chn3ISC; // PWM3 Interrupt Status and Clear
		volatile uint32_t chn3Load;
		volatile uint32_t chn3Count;
		volatile uint32_t chn3CmpA; // PWM3 Compare A
		volatile uint32_t chn3CmpB; // PWM3 Compare B
		volatile uint32_t chn3GenA; // PWM3 Generator A Control
		volatile uint32_t chn3GenB; // PWM3 Generator B Control
		volatile uint32_t chn3DBCtrl; // PWM3 Dead-Band Control
		volatile uint32_t chn3DBRise; // PWM3 Dead-Band Rising-Edge Delay
		volatile uint32_t chn3DBFall; // PWM3 Dead-Band Falling-Edge-Delay
		volatile uint32_t chn3FaultSource0;
		volatile uint32_t chn3FaultSource1;
		volatile uint32_t chn3MinFaultPeriod;
		const volatile uint32_t reserved1[432];
		volatile uint32_t chn0FaultSense;
		const volatile uint32_t chn0FaultStatus0;
		const volatile uint32_t chn0FaultStatus1;
		const volatile uint32_t reserved2[29];
		volatile uint32_t chn1FaultSense;
		const volatile uint32_t chn1FaultStatus0;
		const volatile uint32_t chn1FaultStatus1;
		const volatile uint32_t reserved3[30];
		const volatile uint32_t chn2FaultStatus0;
		const volatile uint32_t chn2FaultStatus1;
		const volatile uint32_t reserved4[30];
		const volatile uint32_t chn3FaultStatus0;
		const volatile uint32_t chn3FaultStatus1;
		const volatile uint32_t reserved5[397];
		volatile uint32_t peripheralProps;
	};

	// QEI periphal structure
	struct qei_t final
	{
		volatile uint32_t ctrl;
		volatile uint32_t status;
		volatile uint32_t position;
		volatile uint32_t maxPosition;
		volatile uint32_t load;
		volatile uint32_t timer;
		volatile uint32_t count; // QEI Velocity Counter
		volatile uint32_t speed; // QEI Velocity
		volatile uint32_t intEn; // QEI Interrupt Enable
		volatile uint32_t ris; // QEI Raw Interrupt Status
		volatile uint32_t isc; // QEI Interrupt Status and Clear
	};

	// Timer periphal structure
	struct timer_t final
	{
		volatile uint32_t config; // GPTM Configuration
		volatile uint32_t timerAMode; // GPTM Timer A Mode
		volatile uint32_t timerBMode; // GPTM Timer B Mode
		volatile uint32_t ctrl;
		volatile uint32_t sync;
		const volatile uint32_t reserved0;
		volatile uint32_t imr; // GPTM Interrupt Mask
		volatile uint32_t ris; // GPTM Raw Interrupt Status
		volatile uint32_t mis; // GPTM Masked Interrupt Status
		volatile uint32_t icr; // GPTM Interrupt Clear
		volatile uint32_t timerAIntervalLoad;
		volatile uint32_t timerBIntervalLoad;
		volatile uint32_t timerAMatch;
		volatile uint32_t timerBMatch;
		volatile uint32_t timerAPrescale;
		volatile uint32_t timerBPrescale;
		volatile uint32_t timerAPrescaleMatch;
		volatile uint32_t timerBPrescaleMatch;
		volatile uint32_t timerA;
		volatile uint32_t timerB;
		volatile uint32_t timerAValue;
		volatile uint32_t timerBValue;
		volatile uint32_t rtcPreDiv; // GPTM RTC Predivide
		volatile uint32_t timerAPrescaleSnap; // GPTM Timer A Prescale Snapshot
		volatile uint32_t timerBPrescaleSnap; // GPTM Timer B Prescale Snapshot
		volatile uint32_t timerAPrescaleValue;
		volatile uint32_t timerBPrescaleValue;
		const volatile uint32_t reserved1[981];
		volatile uint32_t peripheralProps;
	};

	// ADC periphal structure
	struct adc_t final
	{
		volatile uint32_t actSampSeq; // ADC Active Sample Sequencer
		volatile uint32_t ris; // ADC Raw Interrupt Status
		volatile uint32_t im; // ADC Interrupt Mask
		volatile uint32_t isc; // ADC Interrupt Status and Clear
		volatile uint32_t overflowStatus;
		volatile uint32_t eventMux;
		volatile uint32_t underflowStatus;
		volatile uint32_t trigSrcSel; // ADC Trigger Source Select
		volatile uint32_t sampSeqPriority; // ADC Sample Sequencer Priority
		volatile uint32_t sampPhaseCtrl; // ADC Sample Phase Control
		volatile uint32_t procSampSeqInitiate; // ADC Processor Sample Sequence Initiate
		const volatile uint32_t reserved0;
		volatile uint32_t sampAvgCtrl; // ADC Sample Averaging Control
		volatile uint32_t digiCompISC; // ADC Digital Comparator Interrupt Status and Clear
		volatile uint32_t ctrl;
		const volatile uint32_t reserved1;
		volatile uint32_t sampSeqMux0; // ADC Sample Sequence Input Multiplexer Select 0
		volatile uint32_t sampSeqCtrl0;
		volatile uint32_t sampSeqFIFO0;
		volatile uint32_t sampSeqFIFOStatus0;
		volatile uint32_t sampSeqOperation0;
		volatile uint32_t sampSeqDigiComp0; // ADC Sample Sequence 0 Digital Comparator Select
		const volatile uint32_t reserved2[2];
		volatile uint32_t sampSeqMux1; // ADC Sample Sequence Input Multiplexer Select 1
		volatile uint32_t sampSeqCtrl1;
		volatile uint32_t sampSeqFIFO1;
		volatile uint32_t sampSeqFIFOStatus1;
		volatile uint32_t sampSeqOperation1;
		volatile uint32_t sampSeqDigiComp1; // ADC Sample Sequence 1 Digital Comparator Select
		const volatile uint32_t reserved3[2];
		volatile uint32_t sampSeqMux2; // ADC Sample Sequence Input Multiplexer Select 2
		volatile uint32_t sampSeqCtrl2;
		volatile uint32_t sampSeqFIFO2;
		volatile uint32_t sampSeqFIFOStatus2;
		volatile uint32_t sampSeqOperation2;
		volatile uint32_t sampSeqDigiComp2; // ADC Sample Sequence 2 Digital Comparator Select
		const volatile uint32_t reserved4[2];
		volatile uint32_t sampSeqMux3; // ADC Sample Sequence Input Multiplexer Select 3
		volatile uint32_t sampSeqCtrl3;
		volatile uint32_t sampSeqFIFO3;
		volatile uint32_t sampSeqFIFOStatus3;
		volatile uint32_t sampSeqOperation3;
		volatile uint32_t sampSeqDigiComp3; // ADC Sample Sequence 3 Digital Comparator Select
		const volatile uint32_t reserved5[786];
		volatile uint32_t digiCompRIC; // ADC Digital Comparator Reset Initial Conditions
		const volatile uint32_t reserved6[63];
		volatile uint32_t digiCompCtrl0; // ADC Digital Comparator Control 0
		volatile uint32_t digiCompCtrl1; // ADC Digital Comparator Control 1
		volatile uint32_t digiCompCtrl2; // ADC Digital Comparator Control 2
		volatile uint32_t digiCompCtrl3; // ADC Digital Comparator Control 3
		volatile uint32_t digiCompCtrl4; // ADC Digital Comparator Control 4
		volatile uint32_t digiCompCtrl5; // ADC Digital Comparator Control 5
		volatile uint32_t digiCompCtrl6; // ADC Digital Comparator Control 6
		volatile uint32_t digiCompCtrl7; // ADC Digital Comparator Control 7
		const volatile uint32_t reserved7[8];
		volatile uint32_t digiCompCMP0; // ADC Digital Comparator Range 0
		volatile uint32_t digiCompCMP1; // ADC Digital Comparator Range 1
		volatile uint32_t digiCompCMP2; // ADC Digital Comparator Range 2
		volatile uint32_t digiCompCMP3; // ADC Digital Comparator Range 3
		volatile uint32_t digiCompCMP4; // ADC Digital Comparator Range 4
		volatile uint32_t digiCompCMP5; // ADC Digital Comparator Range 5
		volatile uint32_t digiCompCMP6; // ADC Digital Comparator Range 6
		volatile uint32_t digiCompCMP7; // ADC Digital Comparator Range 7
		const volatile uint32_t reserved8[88];
		volatile uint32_t peripheralProps;
		volatile uint32_t peripheralConfig;
		volatile uint32_t clockConfig;
	};

	// Comparator periphal structure
	struct comp_t final
	{
		volatile uint32_t mis; // Analog Comparator Masked Interrupt Status
		volatile uint32_t ris; // Analog Comparator Raw Interrupt Status
		volatile uint32_t intEn; // Analog Comparator Interrupt Enable
		const volatile uint32_t reserved0;
		volatile uint32_t refCtrl; // Analog Comparator Reference Voltage Control
		const volatile uint32_t reserved1[3];
		volatile uint32_t status0;
		volatile uint32_t ctrl0;
		const volatile uint32_t reserved2[6];
		volatile uint32_t status1;
		volatile uint32_t ctrl1;
		const volatile uint32_t reserved3[990];
		volatile uint32_t peripheralProps;
	};

	// CAN periphal structure
	struct can_t final
	{
		volatile uint32_t ctrl;
		volatile uint32_t status;
		volatile uint32_t errorCnt;
		volatile uint32_t bit; // CAN Bit Timing
		volatile uint32_t itr; // CAN Interrupt
		volatile uint32_t test; // CAN Test
		volatile uint32_t brpe; // CAN Baud Rate Prescaler Extension
		const volatile uint32_t reserved0;
		volatile uint32_t if1CmdRq; // CAN IF1 Command Request

		union
		{
			volatile uint32_t if1Command;
			volatile uint32_t if1CmdMask;
		};
		volatile uint32_t if1Mask1;
		volatile uint32_t if1Mask2;
		volatile uint32_t if1Arb1; // CAN IF1 Arbitration 1
		volatile uint32_t if1Arb2; // CAN IF1 Arbitration 2
		volatile uint32_t if1MsgCtrl;
		volatile uint32_t if1DataA1; // CAN IF1 Data A1
		volatile uint32_t if1DataA2; // CAN IF1 Data A2
		volatile uint32_t if1DataB1; // CAN IF1 Data B1
		volatile uint32_t if1DataB2; // CAN IF1 Data B2
		const volatile uint32_t reserved1[13];
		volatile uint32_t if2CmdReq; // CAN IF2 Command Request

		union
		{
			volatile uint32_t if2Command;
			volatile uint32_t if2CmdMask;
		};
		volatile uint32_t if2Mask1; // CAN IF2 Mask 1
		volatile uint32_t if2Mask2; // CAN IF2 Mask 2
		volatile uint32_t if2Arb1; // CAN IF2 Arbitration 1
		volatile uint32_t if2Arb2; // CAN IF2 Arbitration 2
		volatile uint32_t if2MsgCtrl;
		volatile uint32_t if2DataA1; // CAN IF2 Data A1
		volatile uint32_t if2DataA2; // CAN IF2 Data A2
		volatile uint32_t if2DataB1; // CAN IF2 Data B1
		volatile uint32_t if2DataB2; // CAN IF2 Data B2
		const volatile uint32_t reserved2[21];
		volatile uint32_t txReq1; // CAN Transmission Request 1
		volatile uint32_t txReq2; // CAN Transmission Request 2
		const volatile uint32_t reserved3[6];
		volatile uint32_t newDataA1; // CAN New Data 1
		volatile uint32_t newDataA2; // CAN New Data 2
		const volatile uint32_t reserved4[6];
		volatile uint32_t message1Int; // CAN Message 1 Interrupt Pending
		volatile uint32_t message2Int; // CAN Message 2 Interrupt Pending
		const volatile uint32_t reserved5[6];
		volatile uint32_t message1Valid; // CAN Message 1 Valid
		volatile uint32_t message2Valid; // CAN Message 2 Valid
	};

	// USB periphal structure
	struct usb_t final
	{
		struct ep0Config_t final
		{
			volatile uint8_t txFuncAddr;
			const volatile uint8_t reserved0;
			volatile uint8_t txHubAddr;
			volatile uint8_t txHubPort;
			const volatile uint32_t reserved1;
		};

		struct epConfig_t final
		{
			volatile uint8_t txFuncAddr;
			const volatile uint8_t reserved0;
			volatile uint8_t txHubAddr;
			volatile uint8_t txHubPort;
			const volatile uint8_t reserved1;
			volatile uint8_t rxFuncAddr;
			volatile uint8_t rxHubAddr;
			volatile uint8_t rxHubPort;
		};

		struct ep0Ctrl_t final
		{
			const volatile uint32_t reserved0[16];
			const volatile uint16_t reserved1;
			volatile uint8_t statusCtrlL;
			volatile uint8_t statusCtrlH;
			const volatile uint32_t reserved2;
			volatile uint8_t rxCount;
			const volatile uint8_t reserved3;
			volatile uint8_t type;
			volatile uint8_t nackLimit;
			const volatile uint32_t reserved4;
		};

		struct epCtrl_t final
		{
			volatile uint16_t txDataMax;
			volatile uint8_t txStatusCtrlL;
			volatile uint8_t txStatusCtrlH;
			volatile uint16_t rxDataMax;
			volatile uint8_t rxStatusCtrlL;
			volatile uint8_t rxStatusCtrlH;
			volatile uint16_t rxCount;
			volatile uint8_t txType;
			volatile uint8_t txInterval;
			volatile uint8_t rxType;
			volatile uint8_t rxInterval;
			const volatile uint16_t reserved;
		};

		union fifo_t final
		{
			uint32_t u32;
			uint16_t u16;
			uint8_t u8;
		};

		volatile uint8_t address; // Bus address for the device
		volatile uint8_t power;
		volatile uint16_t txIntStatus;
		volatile uint16_t rxIntStatus;
		volatile uint16_t txIntEnable;
		volatile uint16_t rxIntEnable;
		volatile uint8_t intStatus;
		volatile uint8_t intEnable;
		volatile uint16_t frameValue;
		volatile uint8_t epIndex;
		volatile uint8_t test;
		const volatile uint32_t reserved0[4];
		std::array<volatile fifo_t, 8> epFIFO;
		const volatile uint32_t reserved1[8];
		volatile uint8_t deviceCtrl;
		const volatile uint8_t reserved2;
		volatile uint8_t txFIFOSize;
		volatile uint8_t rxFIFOSize;
		volatile uint16_t txFIFOAddr;
		volatile uint16_t rxFIFOAddr;
		const volatile uint32_t reserved3[4];
		const volatile uint16_t reserved4;
		volatile uint8_t connectTiming;
		volatile uint8_t VPLEN; // USB OTG VBUS Pulse Timing
		const volatile uint8_t reserved5;
		volatile uint8_t fullSpeedEOF; // USB Full-Speed Last Transaction to End of Frame Timing
		volatile uint8_t lowSpeedEOF; // USB Low-Speed Last Transaction to End of Frame Timing
		const volatile uint8_t reserved6;
		ep0Config_t ep0Config;
		epConfig_t epConfigs[7];
		ep0Ctrl_t ep0Ctrl;
		epCtrl_t epCtrls[7];
		const volatile uint32_t reserved7[97];
		volatile uint16_t ep1ReqPacketCount;
		const volatile uint16_t reserved8;
		volatile uint16_t ep2ReqPacketCount;
		const volatile uint16_t reserved9;
		volatile uint16_t ep3ReqPacketCount;
		const volatile uint16_t reserved10;
		volatile uint16_t ep4ReqPacketCount;
		const volatile uint16_t reserved11;
		volatile uint16_t ep5ReqPacketCount;
		const volatile uint16_t reserved12;
		volatile uint16_t ep6ReqPacketCount;
		const volatile uint16_t reserved13;
		volatile uint16_t ep7ReqPacketCount;
		const volatile uint16_t reserved14;
		const volatile uint32_t reserved15[8];
		volatile uint16_t rxPacketDoubleBuffEnable;
		volatile uint16_t txPacketDoubleBuffEnable;
		const volatile uint32_t reserved16[47];
		volatile uint32_t extPwrCtrl; // USB External Power Control
		volatile uint32_t extPwrCtrlRIS; // USB External Power Control Raw Interrupt Status
		volatile uint32_t extPwrCtrlIM; // USB External Power Control Interrupt Mask
		volatile uint32_t extPwrCtrlISC; // USB External Power Control Interrupt Status and Clear
		volatile uint32_t deviceResumeRIS; // USB Device RESUME Raw Interrupt Status
		volatile uint32_t deviceResumeIM; // USB Device RESUME Interrupt Mask
		volatile uint32_t deviceResumeISC; // USB Device RESUME Interrupt Status and Clear
		volatile uint32_t gpCtrlStatus; // USB General-Purpose Control and Status
		const volatile uint32_t reserved17[4];
		volatile uint32_t vbusDroopCtrl; // USB VBUS Droop Control
		volatile uint32_t vbusDroopCtrlRIS; // USB VBUS Droop Control Raw Interrupt Status
		volatile uint32_t vbusDroopCtrlIM; // USB VBUS Droop Control Interrupt Mask
		volatile uint32_t vbusDroopCtrlISC; // USB VBUS Droop Control Interrupt Status and Clear
		const volatile uint32_t reserved18;
		volatile uint32_t idValidRIS; // USB ID Valid Detect Raw Interrupt Status
		volatile uint32_t idValidIM; // USB ID Valid Detect Interrupt Mask
		volatile uint32_t idValidISC; // USB ID Valid Detect Interrupt Status and Clear
		volatile uint32_t dmaSel; // USB DMA Select
		const volatile uint32_t reserved19[731];
		volatile uint32_t peripheralProps;
	};

	// EEPROM periphal structure
	struct eeprom_t final
	{
		volatile uint32_t size; // EEPROM Size Information
		volatile uint32_t block; // EEPROM Current Block
		volatile uint32_t offset; // EEPROM Current Offset
		const volatile uint32_t reserved0;
		volatile uint32_t rdrw; // EEPROM Read-Write
		volatile uint32_t rdrwInc; // EEPROM Read-Write with Increment
		volatile uint32_t done; // EEPROM Done Status
		volatile uint32_t ctrlStatus; // EEPROM Support Control and Status
		volatile uint32_t unlock; // EEPROM Unlock
		const volatile uint32_t reserved1[3];
		volatile uint32_t prot; // EEPROM Protection
		volatile uint32_t passwd0; // EEPROM Password
		volatile uint32_t passwd1; // EEPROM Password
		volatile uint32_t passwd2; // EEPROM Password
		volatile uint32_t itr; // EEPROM Interrupt
		const volatile uint32_t reserved2[3];
		volatile uint32_t hide; // EEPROM Block Hide
		const volatile uint32_t reserved3[11];
		volatile uint32_t dbgMassErase; // EEPROM Debug Mass Erase
		const volatile uint32_t reserved4[975];
		volatile uint32_t peripheralProps; // EEPROM Peripheral Properties
	};

	// SysExc peripheral structure
	struct sysExc_t final
	{
		volatile uint32_t ris; // System Exception Raw Interrupt Status
		volatile uint32_t im; // System Exception Interrupt Mask
		volatile uint32_t mis; // System Exception Masked Interrupt Status
		volatile uint32_t ic; // System Exception Interrupt Clear
	};

	// Hibernation peripheral structure
	struct hib_t final
	{
		volatile uint32_t rtcCounter;
		volatile uint32_t rtcMatch0;
		const volatile uint32_t reserved0;
		volatile uint32_t rtcLoad;
		volatile uint32_t ctrl;
		volatile uint32_t im; // Hibernation Interrupt Mask
		volatile uint32_t ris; // Hibernation Raw Interrupt Status
		volatile uint32_t mis; // Hibernation Masked Interrupt Status
		volatile uint32_t ic; // Hibernation Interrupt Clear
		volatile uint32_t rtcTrim;
		volatile uint32_t rtcSubSeconds;
		const volatile uint32_t reserved1;
		volatile uint32_t data;
	};

	// Flash Control peripheral structure
	struct flashCtrl_t final
	{
		volatile uint32_t flashMemAddr;
		volatile uint32_t flashMemData;
		volatile uint32_t flashMemCtrl;
		volatile uint32_t flashCtrlRIS; // Flash Controller Raw Interrupt Status
		volatile uint32_t flashCtrlIM; // Flash Controller Interrupt Mask
		volatile uint32_t flashCtrlMISC; // Flash Controller Masked Interrupt Status and Clear
		const volatile uint32_t reserved0[2];
		volatile uint32_t flashMemCtrl2;
		const volatile uint32_t reserved1[3];
		volatile uint32_t flashWriteBufferValid;
		const volatile uint32_t reserved2[51];
		volatile uint32_t flashWriteBufferN;
		const volatile uint32_t reserved3[943];
		volatile uint32_t flashSize;
		volatile uint32_t sramSize;
		const volatile uint32_t reserved4;

		volatile uint32_t romSWMap;
		const volatile uint32_t reserved5[72];
		volatile uint32_t romCtrl; // ROM Control
		const volatile uint32_t reserved6[55];
		volatile uint32_t bootConfig; // Boot Configuration
		const volatile uint32_t reserved7[3];
		volatile uint32_t userReg0; // User Register 0
		volatile uint32_t userReg1; // User Register 1
		volatile uint32_t userReg2; // User Register 2
		volatile uint32_t userReg3; // User Register 3
		const volatile uint32_t reserved8[4];
		volatile uint32_t flashMemoryProtReadEn0; // Flash Memory Protection Read Enable 0
		volatile uint32_t flashMemoryProtReadEn1; // Flash Memory Protection Read Enable 1
		volatile uint32_t flashMemoryProtReadEn2; // Flash Memory Protection Read Enable 2
		volatile uint32_t flashMemoryProtReadEn3; // Flash Memory Protection Read Enable 3
		const volatile uint32_t reserved9[124];
		volatile uint32_t flashMemoryProtProgEn0; // Flash Memory Protection Program Enable 0
		volatile uint32_t flashMemoryProtProgEn1; // Flash Memory Protection Program Enable 1
		volatile uint32_t flashMemoryProtProgEn2; // Flash Memory Protection Program Enable 2
		volatile uint32_t flashMemoryProtProgEn3; // Flash Memory Protection Program Enable 3
	};

	// System Control peripheral structure
	struct sysCtrl_t final
	{
		volatile uint32_t deviceID0;
		volatile uint32_t deviceID1;
		volatile uint32_t deviceCaps0;
		const volatile uint32_t reserved0;
		volatile uint32_t deviceCaps1;
		volatile uint32_t deviceCaps2;
		volatile uint32_t deviceCaps3;
		volatile uint32_t deviceCaps4;
		volatile uint32_t deviceCaps5;
		volatile uint32_t deviceCaps6;
		volatile uint32_t deviceCaps7;
		volatile uint32_t deviceCaps8;
		volatile uint32_t borCtrl;
		const volatile uint32_t reserved1[3];
		volatile uint32_t swResetCtrl0;
		volatile uint32_t swResetCtrl1;
		volatile uint32_t swResetCtrl2;
		const volatile uint32_t reserved2;
		volatile uint32_t ris; // Raw Interrupt Status
		volatile uint32_t imc; // Interrupt Mask Control
		volatile uint32_t misc; // Masked Interrupt Status and Clear
		volatile uint32_t resetCause;
		volatile uint32_t runClockConfig1;
		const volatile uint32_t reserved3[2];
		volatile uint32_t gpioAHBCtrl; // GPIO High-Performance Bus Control
		volatile uint32_t runClockConfig2;
		const volatile uint32_t reserved4[2];
		volatile uint32_t mainOscCtrl;
		const volatile uint32_t reserved5[32];
		volatile uint32_t runClockGateCtrl0;
		volatile uint32_t runClockGateCtrl1;
		volatile uint32_t runClockGateCtrl2;
		const volatile uint32_t reserved6;
		volatile uint32_t sleepClockGateCtrl0;
		volatile uint32_t sleepClockGateCtrl1;
		volatile uint32_t sleepClockGateCtrl2;
		const volatile uint32_t reserved7;
		volatile uint32_t deepSleepClockCtrl0;
		volatile uint32_t deepSleepClockCtrl1;
		volatile uint32_t deepSleepClockCtrl2;
		const volatile uint32_t reserved8[6];
		volatile uint32_t deepSleepClockConfig;
		const volatile uint32_t reserved9;
		volatile uint32_t sysProps;
		volatile uint32_t precisionIntOscCal; // Precision Internal Oscillator Calibration
		volatile uint32_t precisionIntOscStats; // Precision Internal Oscillator Statistics
		const volatile uint32_t reserved10[2];
		volatile uint32_t pllFreq0;
		volatile uint32_t pllFreq1;
		volatile uint32_t pllStatus;
		const volatile uint32_t reserved11[7];
		volatile uint32_t sleepPowerConfig;
		volatile uint32_t deepSleepPowerConfig;
		volatile uint32_t deviceCaps9;
		const volatile uint32_t reserved12[3];
		volatile uint32_t nvmInfo;
		const volatile uint32_t reserved13[4];
		volatile uint32_t ldoSleepCtrl;
		const volatile uint32_t reserved14;
		volatile uint32_t ldoDeepSleepCtrl;
		const volatile uint32_t reserved15[80];
		volatile uint32_t wdtPresent;
		volatile uint32_t timerPresent;
		volatile uint32_t gpioPresent;
		volatile uint32_t udmaPresent;
		const volatile uint32_t reserved16;
		volatile uint32_t hibPresent;
		volatile uint32_t uartPresent;
		volatile uint32_t ssiPresent;
		volatile uint32_t i2cPresent;
		const volatile uint32_t reserved17;
		volatile uint32_t usbPresent;
		const volatile uint32_t reserved18[2];
		volatile uint32_t canPresent;
		volatile uint32_t adcPresent;
		volatile uint32_t compPresent;
		volatile uint32_t pwmPresent;
		volatile uint32_t qeiPresent;
		const volatile uint32_t reserved19[4];
		volatile uint32_t eepromPresent;
		volatile uint32_t wtimerPresent;
		const volatile uint32_t reserved20[104];
		volatile uint32_t wdtSwReset;
		volatile uint32_t timerSwReset;
		volatile uint32_t gpioSwReset;
		volatile uint32_t udmaSwReset;
		const volatile uint32_t reserved21;
		volatile uint32_t hidSwReset;
		volatile uint32_t uartSwReset;
		volatile uint32_t ssiSwReset;
		volatile uint32_t i2cSwReset;
		const volatile uint32_t reserved22;
		volatile uint32_t usbSwReset;
		const volatile uint32_t reserved23[2];
		volatile uint32_t canSwReset;
		volatile uint32_t adcSwReset;
		volatile uint32_t compSwReset;
		volatile uint32_t pwmSwReset;
		volatile uint32_t qeiSwReset;
		const volatile uint32_t reserved24[4];
		volatile uint32_t eepromSwReset;
		volatile uint32_t wtimerSwReset;
		const volatile uint32_t reserved25[40];
		volatile uint32_t runClockGateCtrlWDT;
		volatile uint32_t runClockGateCtrlTimer;
		volatile uint32_t runClockGateCtrlGPIO;
		volatile uint32_t runClockGateCtrlUDMA;
		const volatile uint32_t reserved26;
		volatile uint32_t runClockGateCtrlHib;
		volatile uint32_t runClockGateCtrlUART;
		volatile uint32_t runClockGateCtrlSSI;
		volatile uint32_t runClockGateCtrlI2C;
		const volatile uint32_t reserved27;
		volatile uint32_t runClockGateCtrlUSB;
		const volatile uint32_t reserved28[2];
		volatile uint32_t runClockGateCtrlCAN;
		volatile uint32_t runClockGateCtrlADC;
		volatile uint32_t runClockGateCtrlComp;
		volatile uint32_t runClockGateCtrlPWM;
		volatile uint32_t runClockGateCtrlQEI;
		const volatile uint32_t reserved29[4];
		volatile uint32_t runClockGateCtrlEEPROM;
		volatile uint32_t runClockGateCtrlWTimer;
		const volatile uint32_t reserved30[40];
		volatile uint32_t sleepClockGateCtrlWDT;
		volatile uint32_t sleepClockGateCtrlTimer;
		volatile uint32_t sleepClockGateCtrlGPIO;
		volatile uint32_t sleepClockGateCtrlUDMA;
		const volatile uint32_t reserved31;
		volatile uint32_t sleepClockGateCtrlHib;
		volatile uint32_t sleepClockGateCtrlUART;
		volatile uint32_t sleepClockGateCtrlSSI;
		volatile uint32_t sleepClockGateCtrlI2C;
		const volatile uint32_t reserved32;
		volatile uint32_t sleepClockGateCtrlUSB;
		const volatile uint32_t reserved33[2];
		volatile uint32_t sleepClockGateCtrlCAN;
		volatile uint32_t sleepClockGateCtrlADC;
		volatile uint32_t sleepClockGateCtrlComp;
		volatile uint32_t sleepClockGateCtrlPWM;
		volatile uint32_t sleepClockGateCtrlQEI;
		const volatile uint32_t reserved34[4];
		volatile uint32_t sleepClockGateCtrlEEPROM;
		volatile uint32_t sleepClockGateCtrlWTimer;
		const volatile uint32_t reserved35[40];
		volatile uint32_t deepSleepGateCtrlWDT;
		volatile uint32_t deepSleepGateCtrlTimer;
		volatile uint32_t deepSleepGateCtrlGPIO;
		volatile uint32_t deepSleepGateCtrlUDMA;
		const volatile uint32_t reserved36;
		volatile uint32_t deepSleepGateCtrlHib;
		volatile uint32_t deepSleepGateCtrlUART;
		volatile uint32_t deepSleepGateCtrlSSI;
		volatile uint32_t deepSleepGateCtrlI2C;
		const volatile uint32_t reserved37;
		volatile uint32_t deepSleepGateCtrlUSB;
		const volatile uint32_t reserved38[2];
		volatile uint32_t deepSleepGateCtrlCAN;
		volatile uint32_t deepSleepGateCtrlADC;
		volatile uint32_t deepSleepGateCtrlComp;
		volatile uint32_t deepSleepGateCtrlPWM;
		volatile uint32_t deepSleepGateCtrlQEI;
		const volatile uint32_t reserved39[4];
		volatile uint32_t deepSleepGateCtrlEEPROM;
		volatile uint32_t deepSleepGateCtrlWTimer;
		const volatile uint32_t reserved40[104];
		volatile uint32_t periphReadyWDT;
		volatile uint32_t periphReadyTimier;
		volatile uint32_t periphReadyGPIO;
		volatile uint32_t periphReadyUDMA;
		const volatile uint32_t reserved41;
		volatile uint32_t periphReadyHib;
		volatile uint32_t periphReadyUART;
		volatile uint32_t periphReadySSI;
		volatile uint32_t periphReadyI2C;
		const volatile uint32_t reserved42;
		volatile uint32_t periphReadyUSB;
		const volatile uint32_t reserved43[2];
		volatile uint32_t periphReadyCAN;
		volatile uint32_t periphReadyADC;
		volatile uint32_t periphReadyComp;
		volatile uint32_t periphReadyPWM;
		volatile uint32_t periphReadyQEI;
		const volatile uint32_t reserved44[4];
		volatile uint32_t periphReadyEEPROM;
		volatile uint32_t periphReadyWTimer;
	};

	// UDMA peripheral structure
	struct udma_t final
	{
		volatile uint32_t status;
		volatile uint32_t config;
		volatile uint32_t ctrlBasePtr;
		volatile uint32_t altBasePtr;
		volatile uint32_t waitStatus;
		volatile uint32_t swReq; // DMA Channel Software Request
		volatile uint32_t useBurstSet;
		volatile uint32_t useBurstClr;
		volatile uint32_t reqMaskSet;
		volatile uint32_t reqMaskClr;
		volatile uint32_t enableSet;
		volatile uint32_t enableClr;
		volatile uint32_t altSet;
		volatile uint32_t altClr;
		volatile uint32_t prioritySet;
		volatile uint32_t priorityClr;
		const volatile uint32_t reserved0[3];
		volatile uint32_t errorClr;
		const volatile uint32_t reserved1[300];
		volatile uint32_t chnAssignment;
		volatile uint32_t chnIS; // DMA Channel Interrupt Status
		const volatile uint32_t reserved2[2];
		volatile uint32_t chnMapSel0; // DMA Channel Map Select 0
		volatile uint32_t chnMapSel1; // DMA Channel Map Select 1
		volatile uint32_t chnMapSel2; // DMA Channel Map Select 2
		volatile uint32_t chnMapSel3; // DMA Channel Map Select 3
	};

	struct sysTick_t final
	{
		volatile uint32_t ctrl;
		volatile uint32_t load;
		volatile uint32_t value;
		volatile uint32_t calibration;
	};

	struct nvic_t final
	{
		std::array<volatile uint32_t, 8> intrSetEnable;
		const volatile uint32_t reserved0[24];
		std::array<volatile uint32_t, 8> intrClrEnable;
		const volatile uint32_t reserved1[24];
		std::array<volatile uint32_t, 8> intrSetPending;
		const volatile uint32_t reserved2[24];
		std::array<volatile uint32_t, 8> intrClrPending;
		const volatile uint32_t reserved3[24];
		std::array<const volatile uint32_t, 8> intrActive;

		constexpr void enableInterrupt(const uint32_t intrNumber) noexcept
			{ intrSetEnable[intrNumber >> 5] = 1 << (intrNumber & 0x1FU); }
		constexpr void disableInterrupt(const uint32_t intrNumber) noexcept
			{ intrClrEnable[intrNumber >> 5] = 1 << (intrNumber & 0x1FU); }
	};

	constexpr static const uintptr_t watchdog0Base{0x40000000U};
	constexpr static const uintptr_t watchdog1Base{0x40001000U};
	constexpr static const uintptr_t gpioABaseAPB{0x40004000U};
	constexpr static const uintptr_t gpioBBaseAPB{0x40005000U};
	constexpr static const uintptr_t gpioCBaseAPB{0x40006000U};
	constexpr static const uintptr_t gpioDBaseAPB{0x40007000U};
	constexpr static const uintptr_t ssi0Base{0x40008000U};
	constexpr static const uintptr_t ssi1Base{0x40009000U};
	constexpr static const uintptr_t ssi2Base{0x4000A000U};
	constexpr static const uintptr_t ssi3Base{0x4000B000U};
	constexpr static const uintptr_t uart0Base{0x4000C000U};
	constexpr static const uintptr_t uart1Base{0x4000D000U};
	constexpr static const uintptr_t uart2Base{0x4000E000U};
	constexpr static const uintptr_t uart3Base{0x4000F000U};
	constexpr static const uintptr_t uart4Base{0x40010000U};
	constexpr static const uintptr_t uart5Base{0x40011000U};
	constexpr static const uintptr_t uart6Base{0x40012000U};
	constexpr static const uintptr_t uart7Base{0x40013000U};
	constexpr static const uintptr_t i2c0Base{0x40020000U};
	constexpr static const uintptr_t i2c1Base{0x40021000U};
	constexpr static const uintptr_t i2c2Base{0x40022000U};
	constexpr static const uintptr_t i2c3Base{0x40023000U};
	constexpr static const uintptr_t gpioEBaseAPB{0x40024000U};
	constexpr static const uintptr_t gpioFBaseAPB{0x40025000U};
	constexpr static const uintptr_t pwm0Base{0x40028000U};
	constexpr static const uintptr_t pwm1Base{0x40029000U};
	constexpr static const uintptr_t qei0Base{0x4002C000U};
	constexpr static const uintptr_t qei1Base{0x4002D000U};
	constexpr static const uintptr_t timer0Base{0x40030000U};
	constexpr static const uintptr_t timer1Base{0x40031000U};
	constexpr static const uintptr_t timer2Base{0x40032000U};
	constexpr static const uintptr_t timer3Base{0x40033000U};
	constexpr static const uintptr_t timer4Base{0x40034000U};
	constexpr static const uintptr_t timer5Base{0x40035000U};
	constexpr static const uintptr_t wtimer0Base{0x40036000U};
	constexpr static const uintptr_t wtimer1Base{0x40037000U};
	constexpr static const uintptr_t adc0Base{0x40038000U};
	constexpr static const uintptr_t adc1Base{0x40039000U};
	constexpr static const uintptr_t compBase{0x4003C000U};
	constexpr static const uintptr_t can0Base{0x40040000U};
	constexpr static const uintptr_t can1Base{0x40041000U};
	constexpr static const uintptr_t wtimer2Base{0x4004C000U};
	constexpr static const uintptr_t wtimer3Base{0x4004D000U};
	constexpr static const uintptr_t wtimer4Base{0x4004E000U};
	constexpr static const uintptr_t wtimer5Base{0x4004F000U};
	constexpr static const uintptr_t usbBase{0x40050000U};
	constexpr static const uintptr_t gpioABaseAHB{0x40058000U};
	constexpr static const uintptr_t gpioBBaseAHB{0x40059000U};
	constexpr static const uintptr_t gpioCBaseAHB{0x4005A000U};
	constexpr static const uintptr_t gpioDBaseAHB{0x4005B000U};
	constexpr static const uintptr_t gpioEBaseAHB{0x4005C000U};
	constexpr static const uintptr_t gpioFBaseAHB{0x4005D000U};
	constexpr static const uintptr_t eepromBase{0x400AF000U};
	constexpr static const uintptr_t sysExcBase{0x400F9000U};
	constexpr static const uintptr_t hibBase{0x400FC000U};
	constexpr static const uintptr_t flashCtrlBase{0x400FD000U};
	constexpr static const uintptr_t sysCtrlBase{0x400FE000U};
	constexpr static const uintptr_t udmaBase{0x400FF000U};

	constexpr static const uintptr_t sysTickBase{0xE000E010};
	constexpr static const uintptr_t nvicBase{0xE000E100};
} // namespace tivaC

static auto &watchdog0{*reinterpret_cast<tivaC::watchdog_t *>(tivaC::watchdog0Base)};
static auto &watchdog1{*reinterpret_cast<tivaC::watchdog_t *>(tivaC::watchdog1Base)};
static auto &gpioAAPB{*reinterpret_cast<tivaC::gpio_t *>(tivaC::gpioABaseAPB)};
static auto &gpioBAPB{*reinterpret_cast<tivaC::gpio_t *>(tivaC::gpioBBaseAPB)};
static auto &gpioCAPB{*reinterpret_cast<tivaC::gpio_t *>(tivaC::gpioCBaseAPB)};
static auto &gpioDAPB{*reinterpret_cast<tivaC::gpio_t *>(tivaC::gpioDBaseAPB)};
static auto &ssi0{*reinterpret_cast<tivaC::ssi_t *>(tivaC::ssi0Base)};
static auto &ssi1{*reinterpret_cast<tivaC::ssi_t *>(tivaC::ssi1Base)};
static auto &ssi2{*reinterpret_cast<tivaC::ssi_t *>(tivaC::ssi2Base)};
static auto &ssi3{*reinterpret_cast<tivaC::ssi_t *>(tivaC::ssi3Base)};
static auto &uart0{*reinterpret_cast<tivaC::uart_t *>(tivaC::uart0Base)};
static auto &uart1{*reinterpret_cast<tivaC::uart_t *>(tivaC::uart1Base)};
static auto &uart2{*reinterpret_cast<tivaC::uart_t *>(tivaC::uart2Base)};
static auto &uart3{*reinterpret_cast<tivaC::uart_t *>(tivaC::uart3Base)};
static auto &uart4{*reinterpret_cast<tivaC::uart_t *>(tivaC::uart4Base)};
static auto &uart5{*reinterpret_cast<tivaC::uart_t *>(tivaC::uart5Base)};
static auto &uart6{*reinterpret_cast<tivaC::uart_t *>(tivaC::uart6Base)};
static auto &uart7{*reinterpret_cast<tivaC::uart_t *>(tivaC::uart7Base)};
static auto &i2c0{*reinterpret_cast<tivaC::i2c_t *>(tivaC::i2c0Base)};
static auto &i2c1{*reinterpret_cast<tivaC::i2c_t *>(tivaC::i2c1Base)};
static auto &i2c2{*reinterpret_cast<tivaC::i2c_t *>(tivaC::i2c2Base)};
static auto &i2c3{*reinterpret_cast<tivaC::i2c_t *>(tivaC::i2c3Base)};
static auto &gpioEAPB{*reinterpret_cast<tivaC::gpio_t *>(tivaC::gpioEBaseAPB)};
static auto &gpioFAPB{*reinterpret_cast<tivaC::gpio_t *>(tivaC::gpioFBaseAPB)};
static auto &pwm0{*reinterpret_cast<tivaC::pwm_t *>(tivaC::pwm0Base)};
static auto &pwm1{*reinterpret_cast<tivaC::pwm_t *>(tivaC::pwm1Base)};
static auto &qei0{*reinterpret_cast<tivaC::qei_t *>(tivaC::qei0Base)};
static auto &qei1{*reinterpret_cast<tivaC::qei_t *>(tivaC::qei1Base)};
static auto &timer0{*reinterpret_cast<tivaC::timer_t *>(tivaC::timer0Base)};
static auto &timer1{*reinterpret_cast<tivaC::timer_t *>(tivaC::timer1Base)};
static auto &timer2{*reinterpret_cast<tivaC::timer_t *>(tivaC::timer2Base)};
static auto &timer3{*reinterpret_cast<tivaC::timer_t *>(tivaC::timer3Base)};
static auto &timer4{*reinterpret_cast<tivaC::timer_t *>(tivaC::timer4Base)};
static auto &timer5{*reinterpret_cast<tivaC::timer_t *>(tivaC::timer5Base)};
static auto &wtimer0{*reinterpret_cast<tivaC::timer_t *>(tivaC::wtimer0Base)};
static auto &wtimer1{*reinterpret_cast<tivaC::timer_t *>(tivaC::wtimer1Base)};
static auto &adc0{*reinterpret_cast<tivaC::adc_t *>(tivaC::adc0Base)};
static auto &adc1{*reinterpret_cast<tivaC::adc_t *>(tivaC::adc1Base)};
static auto &comp{*reinterpret_cast<tivaC::comp_t *>(tivaC::compBase)};
static auto &can0{*reinterpret_cast<tivaC::can_t *>(tivaC::can0Base)};
static auto &can1{*reinterpret_cast<tivaC::can_t *>(tivaC::can1Base)};
static auto &wtimer2{*reinterpret_cast<tivaC::timer_t *>(tivaC::wtimer2Base)};
static auto &wtimer3{*reinterpret_cast<tivaC::timer_t *>(tivaC::wtimer3Base)};
static auto &wtimer4{*reinterpret_cast<tivaC::timer_t *>(tivaC::wtimer4Base)};
static auto &wtimer5{*reinterpret_cast<tivaC::timer_t *>(tivaC::wtimer5Base)};
static auto &usbCtrl{*reinterpret_cast<tivaC::usb_t *>(tivaC::usbBase)};
static auto &gpioA{*reinterpret_cast<tivaC::gpio_t *>(tivaC::gpioABaseAHB)};
static auto &gpioB{*reinterpret_cast<tivaC::gpio_t *>(tivaC::gpioBBaseAHB)};
static auto &gpioC{*reinterpret_cast<tivaC::gpio_t *>(tivaC::gpioCBaseAHB)};
static auto &gpioD{*reinterpret_cast<tivaC::gpio_t *>(tivaC::gpioDBaseAHB)};
static auto &gpioE{*reinterpret_cast<tivaC::gpio_t *>(tivaC::gpioEBaseAHB)};
static auto &gpioF{*reinterpret_cast<tivaC::gpio_t *>(tivaC::gpioFBaseAHB)};
static auto &eeprom{*reinterpret_cast<tivaC::eeprom_t *>(tivaC::eepromBase)};
static auto &sysExc{*reinterpret_cast<tivaC::sysExc_t *>(tivaC::sysExcBase)};
static auto &hib{*reinterpret_cast<tivaC::hib_t *>(tivaC::hibBase)};
static auto &flashCtrl{*reinterpret_cast<tivaC::flashCtrl_t *>(tivaC::flashCtrlBase)};
static auto &sysCtrl{*reinterpret_cast<tivaC::sysCtrl_t *>(tivaC::sysCtrlBase)};
static auto &udma{*reinterpret_cast<tivaC::udma_t *>(tivaC::udmaBase)};

static auto &sysTick{*reinterpret_cast<tivaC::sysTick_t *>(tivaC::sysTickBase)};
static auto &nvic{*reinterpret_cast<tivaC::nvic_t *>(tivaC::nvicBase)};

template<typename T> struct readFIFO_t;
template<typename T> struct writeFIFO_t;

template<> struct readFIFO_t<uint8_t> final
{
	void operator ()(volatile const tivaC::usb_t::fifo_t &fifo, void *const buffer) noexcept
	{
		auto &data{*reinterpret_cast<uint8_t *>(buffer)};
		data = fifo.u8;
	}
};

template<> struct readFIFO_t<uint16_t> final
{
	void operator ()(volatile const tivaC::usb_t::fifo_t &fifo, void *const buffer) noexcept
	{
		auto &data{*reinterpret_cast<uint16_t *>(buffer)};
		data = fifo.u16;
	}
};

template<> struct readFIFO_t<uint32_t> final
{
	void operator ()(volatile const tivaC::usb_t::fifo_t &fifo, void *const buffer) noexcept
	{
		auto &data{*reinterpret_cast<uint32_t *>(buffer)};
		data = fifo.u32;
	}
};

template<> struct writeFIFO_t<uint8_t> final
{
	void operator ()(volatile tivaC::usb_t::fifo_t &fifo, const void *const buffer) noexcept
	{
		const auto &data{*reinterpret_cast<const uint8_t *>(buffer)};
		fifo.u8 = data;
	}
};

template<> struct writeFIFO_t<uint16_t> final
{
	void operator ()(volatile tivaC::usb_t::fifo_t &fifo, const void *const buffer) noexcept
	{
		const auto &data{*reinterpret_cast<const uint16_t *>(buffer)};
		fifo.u16 = data;
	}
};

template<> struct writeFIFO_t<uint32_t> final
{
	void operator ()(volatile tivaC::usb_t::fifo_t &fifo, const void *const buffer) noexcept
	{
		const auto &data{*reinterpret_cast<const uint32_t *>(buffer)};
		fifo.u32 = data;
	}
};

#endif /*TM4C123GH6PM_PLATFORM___HXX*/
