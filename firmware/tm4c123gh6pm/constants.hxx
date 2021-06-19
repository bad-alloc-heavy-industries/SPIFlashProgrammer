// SPDX-License-Identifier: BSD-3-Clause
#ifndef TM4C123GH6PM_CONSTANTS___HXX
#define TM4C123GH6PM_CONSTANTS___HXX

#include <cstdint>

namespace vals
{
	template<typename T> [[gnu::optimize("O0")]] inline void readDiscard(T &value) noexcept
		{ [[maybe_unused]] T result = value; }

	namespace gpio
	{
		// GPIO lock register constants
		constexpr static const uint32_t lockUnlocked{0x00000000};
		constexpr static const uint32_t lockLocked{0x00000001};
		constexpr static const uint32_t lockKey{0x4C4F434B};

		namespace portB
		{
			// PB0 alternative functions
			constexpr static const uint32_t portCtrlPin0Mask{0x0000000F};
			constexpr static const uint32_t portCtrlPin0USBID{0x00000000};
			constexpr static const uint32_t portCtrlPin0UART1Rx{0x00000001};
			constexpr static const uint32_t portCtrlPin0Timer2CCP0{0x00000007};

			// PB1 alternative functions
			constexpr static const uint32_t portCtrlPin1Mask{0x000000F0};
			constexpr static const uint32_t portCtrlPin1USBVBus{0x00000000};
			constexpr static const uint32_t portCtrlPin1UART1Tx{0x00000010};
			constexpr static const uint32_t portCtrlPin1Timer2CCP1{0x00000070};

			// PB2 alternative functions
			constexpr static const uint32_t portCtrlPin2Mask{0x00000F00};
			constexpr static const uint32_t portCtrlPin2I2C0Clk{0x00000300};
			constexpr static const uint32_t portCtrlPin2Timer3CCP0{0x00000700};

			// PB3 alternative functions
			constexpr static const uint32_t portCtrlPin3Mask{0x0000F000};
			constexpr static const uint32_t portCtrlPin3I2C0Data{0x00003000};
			constexpr static const uint32_t portCtrlPin3Timer3CCP1{0x00007000};

			// PB4 alternative functions
			constexpr static const uint32_t portCtrlPin4Mask{0x000F0000};
			constexpr static const uint32_t portCtrlPin4SSI2Clk{0x00020000};
			constexpr static const uint32_t portCtrlPin4M0PWM2{0x00040000};
			constexpr static const uint32_t portCtrlPin4Timer1CCP0{0x00070000};
			constexpr static const uint32_t portCtrlPin4CAN0Rx{0x00080000};

			// PB5 alternative functions
			constexpr static const uint32_t portCtrlPin5Mask{0x00F00000};
			constexpr static const uint32_t portCtrlPin5SSI2CS{0x00200000};
			constexpr static const uint32_t portCtrlPin5M0PWM3{0x00400000};
			constexpr static const uint32_t portCtrlPin5Timer1CCP1{0x00700000};
			constexpr static const uint32_t portCtrlPin5CAN0Tx{0x00800000};

			// PB6 alternative functions
			constexpr static const uint32_t portCtrlPin6Mask{0x0F000000};
			constexpr static const uint32_t portCtrlPin6SSI2Rx{0x02000000};
			constexpr static const uint32_t portCtrlPin6M0PWM0{0x04000000};
			constexpr static const uint32_t portCtrlPin6Timer0CCP0{0x07000000};

			// PB7 alternative functions
			constexpr static const uint32_t portCtrlPin7Mask{0xF0000000};
			constexpr static const uint32_t portCtrlPin7SSI2Tx{0x20000000};
			constexpr static const uint32_t portCtrlPin7M0PWM1{0x40000000};
			constexpr static const uint32_t portCtrlPin7Timer0CCP1{0x70000000};
		} // namespace portB

		namespace portD
		{
			// PD0 alternative functions
			constexpr static const uint32_t portCtrlPin0Mask{0x0000000F};
			constexpr static const uint32_t portCtrlPin0AnalogIn7{0x00000000};
			constexpr static const uint32_t portCtrlPin0SSI3Clk{0x00000001};
			constexpr static const uint32_t portCtrlPin0SSI1Clk{0x00000002};
			constexpr static const uint32_t portCtrlPin0I2C3Clk{0x00000003};
			constexpr static const uint32_t portCtrlPin0M0PWM6{0x00000004};
			constexpr static const uint32_t portCtrlPin0M1PWM0{0x00000005};
			constexpr static const uint32_t portCtrlPin0WideTimer2CCP0{0x00000007};

			// PD1 alternative functions
			constexpr static const uint32_t portCtrlPin1Mask{0x000000F0};
			constexpr static const uint32_t portCtrlPin1AnalogIn6{0x00000000};
			constexpr static const uint32_t portCtrlPin1SSI3CS{0x00000010};
			constexpr static const uint32_t portCtrlPin1SSI1CS{0x00000020};
			constexpr static const uint32_t portCtrlPin1I2CData{0x00000030};
			constexpr static const uint32_t portCtrlPin1M0PWM7{0x00000040};
			constexpr static const uint32_t portCtrlPin1M1PWM1{0x00000050};
			constexpr static const uint32_t portCtrlPin1WideTimer2CCP1{0x00000070};

			// PD2 alternative functions
			constexpr static const uint32_t portCtrlPin2Mask{0x00000F00};
			constexpr static const uint32_t portCtrlPin2AnalogIn5{0x00000000};
			constexpr static const uint32_t portCtrlPin2SSI3Rx{0x00000100};
			constexpr static const uint32_t portCtrlPin2SSI1Rx{0x00000200};
			constexpr static const uint32_t portCtrlPin2M0Fault0{0x00000400};
			constexpr static const uint32_t portCtrlPin2WideTimer3CCP0{0x00000700};
			constexpr static const uint32_t portCtrlPin2USBExtPwrEnable{0x00000800};

			// PD3 alternative functions
			constexpr static const uint32_t portCtrlPin3Mask{0x0000F000};
			constexpr static const uint32_t portCtrlPin3AnalogIn4{0x00000000};
			constexpr static const uint32_t portCtrlPin3SSI3Tx{0x00001000};
			constexpr static const uint32_t portCtrlPin3SSI1Tx{0x00002000};
			constexpr static const uint32_t portCtrlPin3QEI0Index{0x00006000};
			constexpr static const uint32_t portCtrlPin3WideTimer3CCP1{0x00007000};
			constexpr static const uint32_t portCtrlPin3USBExPwrFault{0x00008000};

			// PD4 alternative functions
			constexpr static const uint32_t portCtrlPin4Mask{0x000F0000};
			constexpr static const uint32_t portCtrlPin4USBDataMinus{0x00000000};
			constexpr static const uint32_t portCtrlPin4UART6Rx{0x00010000};
			constexpr static const uint32_t portCtrlPin4WideTimer4CCP0{0x00070000};

			// PD5 alternative functions
			constexpr static const uint32_t portCtrlPin5Mask{0x00F00000};
			constexpr static const uint32_t portCtrlPin5USBDataPlus{0x00000000};
			constexpr static const uint32_t portCtrlPin5UART6Tx{0x00100000};
			constexpr static const uint32_t portCtrlPin5WideTimer4CCP1{0x00700000};

			// PD6 alternative functions
			constexpr static const uint32_t portCtrlPin6Mask{0x0F000000};
			constexpr static const uint32_t portCtrlPin6UART2Rx{0x01000000};
			constexpr static const uint32_t portCtrlPin6M0Fault0{0x04000000};
			constexpr static const uint32_t portCtrlPin6QEI0PhaseA{0x06000000};
			constexpr static const uint32_t portCtrlPin6WideTimer5CCP0{0x07000000};

			// PD7 alternative functions
			constexpr static const uint32_t portCtrlPin7Mask{0xF0000000};
			constexpr static const uint32_t portCtrlPin7UART2Tx{0x10000000};
			constexpr static const uint32_t portCtrlPin7QEI0PhaseB{0x60000000};
			constexpr static const uint32_t portCtrlPin7WideTimer5CCP1{0x70000000};
			constexpr static const uint32_t portCtrlPin7NMI{0x80000000};
		} // namespace portD

		namespace portF
		{
			// PF0 alternative functions
			constexpr static const uint32_t portCtrlPin0Mask{0x0000000F};
			constexpr static const uint32_t portCtrlPin0UART1RTS{0x00000001};
			constexpr static const uint32_t portCtrlPin0SSI1Rx{0x00000002};
			constexpr static const uint32_t portCtrlPin0CAN0Rx{0x00000003};
			constexpr static const uint32_t portCtrlPin0M1PWM4{0x00000005};
			constexpr static const uint32_t portCtrlPin0QEI0PhaseA{0x00000006};
			constexpr static const uint32_t portCtrlPin0Timer0CCP0{0x00000007};
			constexpr static const uint32_t portCtrlPin0NMI{0x00000008};
			constexpr static const uint32_t portCtrlPin0Comp0Out{0x00000009};
			// PF1 alternative functions
			constexpr static const uint32_t portCtrlPin1Mask{0x000000F0};
			constexpr static const uint32_t portCtrlPin1UART1CTS{0x00000010};
			constexpr static const uint32_t portCtrlPin1SSI1Tx{0x00000020};
			constexpr static const uint32_t portCtrlPin1M1PWM5{0x00000050};
			constexpr static const uint32_t portCtrlPin1QEI0PhaseB{0x00000060};
			constexpr static const uint32_t portCtrlPin1Timer0CCP1{0x00000070};
			constexpr static const uint32_t portCtrlPin1Comp1Out{0x00000090};
			constexpr static const uint32_t portCtrlPin1TraceData1{0x000000E0};
			// PF2 alternative functions
			constexpr static const uint32_t portCtrlPin2Mask{0x00000F00};
			constexpr static const uint32_t portCtrlPin2SSI1Clk{0x00000200};
			constexpr static const uint32_t portCtrlPin2M0Fault0{0x00000400};
			constexpr static const uint32_t portCtrlPin2M1PWM6{0x00000500};
			constexpr static const uint32_t portCtrlPin2Timer1CCP0{0x00000700};
			constexpr static const uint32_t portCtrlPin2TraceData0{0x00000E00};
			// PF3 alternative functions
			constexpr static const uint32_t portCtrlPin3Mask{0x0000F000};
			constexpr static const uint32_t portCtrlPin3SSI1CS{0x00002000};
			constexpr static const uint32_t portCtrlPin3CAN0Tx{0x00003000};
			constexpr static const uint32_t portCtrlPin3M1PWM7{0x00005000};
			constexpr static const uint32_t portCtrlPin3Timer1CCP1{0x00007000};
			constexpr static const uint32_t portCtrlPin3TraceClk{0x0000E000};
			// PF4 alternative functions
			constexpr static const uint32_t portCtrlPin4Mask{0x000F0000};
			constexpr static const uint32_t portCtrlPin4M1Fault0{0x00050000};
			constexpr static const uint32_t portCtrlPin4QEI0Index{0x00060000};
			constexpr static const uint32_t portCtrlPin4Timer2CCP0{0x00070000};
			constexpr static const uint32_t portCtrlPin4USB0ExtPwrEn{0x00080000};
		} // namespace portF
	} // namespace gpio

	namespace ssi
	{
		// SSI controller ctrl0 register constants
		constexpr static const uint32_t ctrl0ClockRateMask{0x0000FF00};
		constexpr static const uint32_t ctrl0ClockPhaseLeading{0x00000000};
		constexpr static const uint32_t ctrl0ClockPhaseTrailing{0x00000080};
		constexpr static const uint32_t ctrl0ClockPolarityLow{0x00000000};
		constexpr static const uint32_t ctrl0ClockPolarityHigh{0x00000040};
		constexpr static const uint32_t ctrl0FormatMask{0x00000030};
		constexpr static const uint32_t ctrl0FormatMotorola{0x00000000};
		constexpr static const uint32_t ctrl0FormatTI{0x00000010};
		constexpr static const uint32_t ctrl0FormatMicrowire{0x00000020};
		constexpr static const uint32_t ctrl0DataMask{0x0000000F};
		constexpr static const uint32_t ctrl0Data4Bit{0x00000003};
		constexpr static const uint32_t ctrl0Data5Bit{0x00000004};
		constexpr static const uint32_t ctrl0Data6Bit{0x00000005};
		constexpr static const uint32_t ctrl0Data7Bit{0x00000006};
		constexpr static const uint32_t ctrl0Data8Bit{0x00000007};
		constexpr static const uint32_t ctrl0Data9Bit{0x00000008};
		constexpr static const uint32_t ctrl0Data10Bit{0x00000009};
		constexpr static const uint32_t ctrl0Data11Bit{0x0000000A};
		constexpr static const uint32_t ctrl0Data12Bit{0x0000000B};
		constexpr static const uint32_t ctrl0Data13Bit{0x0000000C};
		constexpr static const uint32_t ctrl0Data14Bit{0x0000000D};
		constexpr static const uint32_t ctrl0Data15Bit{0x0000000E};
		constexpr static const uint32_t ctrl0Data16Bit{0x0000000F};
		constexpr static const uint8_t ctrl0ClockRateShift{8};

		// SSI controller ctrl1 register constants
		constexpr static const uint32_t control1EOT{0x00000010};
		constexpr static const uint32_t control1PeripheralOutDisable{0x00000008};
		constexpr static const uint32_t control1ModeController{0x00000000};
		constexpr static const uint32_t control1ModePeripheral{0x00000004};
		constexpr static const uint32_t control1EnableOperations{0x00000002};
		constexpr static const uint32_t control1LoopEnable{0x00000001};

		// SSI controller status register constants
		constexpr static const uint32_t statusBusy{0x00000010};
		constexpr static const uint32_t statusRxFIFOFull{0x00000008};
		constexpr static const uint32_t statusRxFIFONotEmpty{0x00000004};
		constexpr static const uint32_t statusTxFIFONotFull{0x00000002};
		constexpr static const uint32_t statusTxFIFOEmpty{0x00000001};

		// SSI controller clock configuration constants
		constexpr static const uint32_t clockConfigSysClk{0};
		constexpr static const uint32_t clockConfigPIOsc{5};
	} // namespace ssi

	namespace usb
	{
		enum class endpointDir_t : uint8_t
		{
			controllerOut = 0x00U,
			controllerIn = 0x80U
		};

		constexpr static const uint8_t endpointDirMask{0x7F};
		constexpr inline uint8_t endpoint(const endpointDir_t dir, const uint8_t number) noexcept
			{ return uint8_t(dir) | (number & endpointDirMask); }

		constexpr inline uint8_t indexFor(const uint8_t endpoint) noexcept
			{ return uint8_t(endpoint << 1) & uint8_t(endpoint >> 7); }

		// Functional address register constants
		constexpr static const uint8_t addressMask{0x7FU};
		constexpr static const uint8_t addressClrMask{0x80U};

		// Power and signaling control register constants
		constexpr static const uint8_t powerIsochronousUpdate{0x80U};
		constexpr static const uint8_t powerSoftDisconnectMask{0xBFU};
		constexpr static const uint8_t powerSoftDisconnect{0x00U};
		constexpr static const uint8_t powerSoftConnect{0x40U};
		constexpr static const uint8_t powerReset{0x08U};
		constexpr static const uint8_t powerResume{0x04U};
		constexpr static const uint8_t powerSuspend{0x02U};
		constexpr static const uint8_t powerPowerDownPhy{0x01U};
		constexpr static const uint8_t powerMask{0x30U};

		// Transmit interrupt enable register constants
		constexpr static const uint16_t txItrEnableMask{0xFF00U};
		constexpr static const uint16_t txItrEnableEP0{0x0001}; // This also enables the rx interrupt for EP0
		constexpr static const uint16_t txItrEnableEP1{0x0002};
		constexpr static const uint16_t txItrEnableEP2{0x0004};
		constexpr static const uint16_t txItrEnableEP3{0x0008};
		constexpr static const uint16_t txItrEnableEP4{0x0010};
		constexpr static const uint16_t txItrEnableEP5{0x0020};
		constexpr static const uint16_t txItrEnableEP6{0x0040};
		constexpr static const uint16_t txItrEnableEP7{0x0080};

		// Receive interrupt enable register constants
		constexpr static const uint16_t rxItrEnableMask{0xFF01U};
		constexpr static const uint16_t rxItrEnableEP1{0x0002U};
		constexpr static const uint16_t rxItrEnableEP2{0x0004U};
		constexpr static const uint16_t rxItrEnableEP3{0x0008U};
		constexpr static const uint16_t rxItrEnableEP4{0x0010U};
		constexpr static const uint16_t rxItrEnableEP5{0x0020U};
		constexpr static const uint16_t rxItrEnableEP6{0x0040U};
		constexpr static const uint16_t rxItrEnableEP7{0x0080U};

		// Interrupt status register constants
		constexpr static const uint8_t itrStatusSuspend{0x01U};
		constexpr static const uint8_t itrStatusResume{0x02U};
		constexpr static const uint8_t itrStatusHostBabble{0x04U};
		constexpr static const uint8_t itrStatusDeviceReset{0x04U};
		constexpr static const uint8_t itrStatusConnect{0x10U};
		constexpr static const uint8_t itrStatusDisconnect{0x20U};
		constexpr static const uint8_t itrStatusSessionRequest{0x40U};
		constexpr static const uint8_t itrStatusVBusError{0x80U};
		constexpr static const uint8_t itrStatusSOF{0x08U};

		// Interrupt enable register constants
		constexpr static const uint8_t itrEnableHostMask{0x01U};
		constexpr static const uint8_t itrEnableDeviceMask{0xD0U};
		constexpr static const uint8_t itrEnableSuspend{0x01U};
		constexpr static const uint8_t itrEnableResume{0x02U};
		constexpr static const uint8_t itrEnableHostBabble{0x04U};
		constexpr static const uint8_t itrEnableDeviceReset{0x04U};
		constexpr static const uint8_t itrEnableSOF{0x08U};
		constexpr static const uint8_t itrEnableConnect{0x10U};
		constexpr static const uint8_t itrEnableDisconnect{0x20U};
		constexpr static const uint8_t itrEnableSessionRequest{0x40U};
		constexpr static const uint8_t itrEnableVBusError{0x80U};

		// TX|RX FIFO sizing register constants
		constexpr static const uint8_t fifoSizeSingleBuffered{0x00};
		constexpr static const uint8_t fifoSizeDoubleBuffered{0x10};
		constexpr static const uint8_t fifoSizeMask{0x0F};
		constexpr static const uint8_t fifoSize8{0x00};
		constexpr static const uint8_t fifoSize16{0x01};
		constexpr static const uint8_t fifoSize32{0x02};
		constexpr static const uint8_t fifoSize64{0x03};
		constexpr static const uint8_t fifoSize128{0x04};
		constexpr static const uint8_t fifoSize256{0x05};
		constexpr static const uint8_t fifoSize512{0x06};
		constexpr static const uint8_t fifoSize1024{0x07};
		constexpr static const uint8_t fifoSize2048{0x08};

		constexpr inline static uint8_t fifoMapMaxSize(const uint16_t size, const uint8_t doubleBuffered) noexcept
		{
			if (!doubleBuffered)
			{
				if (size <= 8)
					return fifoSize8;
				else if (size <= 16)
					return fifoSize16;
				else if (size <= 32)
					return fifoSize32;
				else if (size <= 64)
					return fifoSize64;
				else if (size <= 128)
					return fifoSize128;
				else if (size <= 256)
					return fifoSize256;
				else if (size <= 512)
					return fifoSize512;
				else if (size <= 1024)
					return fifoSize1024;
				else
					return fifoSize2048;
			}
			else
			{
				return [](const uint16_t size) noexcept
				{
					if (size <= 8)
						return fifoSize16;
					else if (size <= 16)
						return fifoSize32;
					else if (size <= 32)
						return fifoSize64;
					else if (size <= 64)
						return fifoSize128;
					else if (size <= 128)
						return fifoSize256;
					else if (size <= 256)
						return fifoSize512;
					else if (size <= 512)
						return fifoSize1024;
					else
						return fifoSize2048;
				}(size) | fifoSizeDoubleBuffered;
			}
		}

		constexpr inline static uint16_t fifoAddr(const uint16_t address) noexcept
			{ return address >> 3; } // Divide by 8

		// RX double bufering enable register constants
		constexpr static const uint16_t rxPacketDoubleBuffEnableEP1{0x00000002};
		constexpr static const uint16_t rxPacketDoubleBuffEnableEP2{0x00000004};
		constexpr static const uint16_t rxPacketDoubleBuffEnableEP3{0x00000008};
		constexpr static const uint16_t rxPacketDoubleBuffEnableEP4{0x00000010};
		constexpr static const uint16_t rxPacketDoubleBuffEnableEP5{0x00000020};
		constexpr static const uint16_t rxPacketDoubleBuffEnableEP6{0x00000040};
		constexpr static const uint16_t rxPacketDoubleBuffEnableEP7{0x00000080};

		// TX double bufering enable register constants
		constexpr static const uint16_t txPacketDoubleBuffEnableEP1{0x00000002};
		constexpr static const uint16_t txPacketDoubleBuffEnableEP2{0x00000004};
		constexpr static const uint16_t txPacketDoubleBuffEnableEP3{0x00000008};
		constexpr static const uint16_t txPacketDoubleBuffEnableEP4{0x00000010};
		constexpr static const uint16_t txPacketDoubleBuffEnableEP5{0x00000020};
		constexpr static const uint16_t txPacketDoubleBuffEnableEP6{0x00000040};
		constexpr static const uint16_t txPacketDoubleBuffEnableEP7{0x00000080};

		// Endpoint status register constants
		// For both modes
		constexpr static const uint8_t ep0StatusCtrlLRxReady{0x01U};
		constexpr static const uint8_t epStatusCtrlLRxReady{0x01U};
		constexpr static const uint8_t ep0StatusCtrlLTxReady{0x02U};
		constexpr static const uint8_t epStatusCtrlLTxReady{0x01U};
		constexpr static const uint8_t ep0StatusCtrlLStalled{0x04U};
		constexpr static const uint8_t epStatusCtrlLStalled{0x20U};
		constexpr static const uint8_t epStatusCtrLTxUnderRun{0x04U};

		// For host mode
		constexpr static const uint8_t epStatusCtrlLSetup{0x08U};
		constexpr static const uint8_t epStatusCtrlLError{0x10U};
		constexpr static const uint8_t epStatusCtrlLRequestPacket{0x20U};
		constexpr static const uint8_t epStatusCtrlLStatus{0x40U};
		constexpr static const uint8_t epStatusCtrlLNAKTimedOut{0x80U};

		// For device mode
		constexpr static const uint8_t epStatusCtrlLDataEnd{0x08U};
		constexpr static const uint8_t epStatusCtrlLSetupEnd{0x10U};
		constexpr static const uint8_t epStatusCtrlLStall{0x20U};
		constexpr static const uint8_t epStatusCtrlLRxReadyClr{0x40U};
		constexpr static const uint8_t epStatusCtrlLSetupEndClr{0x80U};

		constexpr static const uint8_t epTxStatusCtrlHModeBulkIntr{0x00U};
		constexpr static const uint8_t epTxStatusCtrlHModeIsochronous{0x40U};
		constexpr static const uint8_t epTxStatusCtrlHMask{0x03U};

		constexpr static const uint8_t epRxStatusCtrlHMask{0x01U};
		constexpr static const uint8_t epRxStatusCtrlHDTSWriteEn{0x04U};

		// General-Purpose Control and Status register constants
		constexpr static const uint32_t gpCtrlStatusOTGModeHostMask{0xFFFFFFFDU};
		constexpr static const uint32_t gpCtrlStatusOTGModeHost{0x00000000U};
		constexpr static const uint32_t gpCtrlStatusOTGModeDevice{0x00000002U};
		constexpr static const uint32_t gpCtrlStatusHostModeMask{0xFFFFFFFEU};
		constexpr static const uint32_t gpCtrlStatusHostMode{0x00000000U};
		constexpr static const uint32_t gpCtrlStatusDeviceMode{0x00000001U};
	} // namespace usb

	namespace sysCtrl
	{
		// Oscillator config register 1 constants
		constexpr static const uint32_t runClockCfg1Mask{0xF021D00EU};
		constexpr static const uint32_t runClockCfg1ACG{0x08000000};
		constexpr static const uint32_t runClockCfg1SysClockDivMask{0x07800000};
		constexpr static const uint32_t runClockCfg1NoSysClkDiv{0x00000000};
		constexpr static const uint32_t runClockCfg1UseSysClkDiv{0x00400000};
		constexpr static const uint32_t runClockCfg1NoPWMClkDiv{0x00000000};
		constexpr static const uint32_t runClockCfg1UsePWMClkDiv{0x00100000};
		constexpr static const uint32_t runClockCfg1PWMClkDivMask{0x000E0000};
		constexpr static const uint32_t runClockCfg1PWMClkDiv2{0x00000000};
		constexpr static const uint32_t runClockCfg1PWMClkDiv4{0x00020000};
		constexpr static const uint32_t runClockCfg1PWMClkDiv8{0x00040000};
		constexpr static const uint32_t runClockCfg1PWMClkDiv16{0x00060000};
		constexpr static const uint32_t runClockCfg1PWMClkDiv32{0x00080000};
		constexpr static const uint32_t runClockCfg1PWMClkDiv64{0x000A0000};
		constexpr static const uint32_t runClockCfg1PLLPowerUp{0x00000000};
		constexpr static const uint32_t runClockCfg1PLLPowerDown{0x00002000};
		constexpr static const uint32_t runClockCfg1PLLNoBypass{0x00000000};
		constexpr static const uint32_t runClockCfg1PLLBypass{0x00000800};
		constexpr static const uint32_t runClockCfg1MainOscXtalMask{0x000007C0};
		constexpr static const uint32_t runClockCfg1MainOscXtal4MHz{0x00000180};
		constexpr static const uint32_t runClockCfg1MainOscXtal4_09MHz{0x000001C0};
		constexpr static const uint32_t runClockCfg1MainOscXtal4_91MHz{0x00000200};
		constexpr static const uint32_t runClockCfg1MainOscXtal5MHz{0x00000240};
		constexpr static const uint32_t runClockCfg1MainOscXtal5_12MHz{0x00000280};
		constexpr static const uint32_t runClockCfg1MainOscXtal6MHz{0x000002C0};
		constexpr static const uint32_t runClockCfg1MainOscXtal6_14MHz{0x00000300};
		constexpr static const uint32_t runClockCfg1MainOscXtal7_37MHz{0x00000340};
		constexpr static const uint32_t runClockCfg1MainOscXtal8MHz{0x00000380};
		constexpr static const uint32_t runClockCfg1MainOscXtal8_19MHz{0x000003C0};
		constexpr static const uint32_t runClockCfg1MainOscXtal10MHz{0x00000400};
		constexpr static const uint32_t runClockCfg1MainOscXtal12MHz{0x00000440};
		constexpr static const uint32_t runClockCfg1MainOscXtal12_2MHz{0x00000480};
		constexpr static const uint32_t runClockCfg1MainOscXtal13_5MHz{0x000004C0};
		constexpr static const uint32_t runClockCfg1MainOscXtal14_3MHz{0x00000500};
		constexpr static const uint32_t runClockCfg1MainOscXtal16MHz{0x00000540};
		constexpr static const uint32_t runClockCfg1MainOscXtal16_3MHz{0x00000580};
		constexpr static const uint32_t runClockCfg1MainOscXtal18MHz{0x000005C0};
		constexpr static const uint32_t runClockCfg1MainOscXtal20MHz{0x00000600};
		constexpr static const uint32_t runClockCfg1MainOscXtal24MHz{0x00000640};
		constexpr static const uint32_t runClockCfg1MainOscXtal25MHz{0x00000680};
		constexpr static const uint32_t runClockCfg1OscSourceMask{0x00000030};
		constexpr static const uint32_t runClockCfg1OscSourceMainOsc{0x00000000};
		constexpr static const uint32_t runClockCfg1OscSourceIntOsc{0x00000010};
		constexpr static const uint32_t runClockCfg1OscSourceIntOscDiv4{0x00000020};
		constexpr static const uint32_t runClockCfg1OscSource30kHzOsc{0x00000030};
		constexpr static const uint32_t runClockCfg1MainOscEnable{0x00000000};
		constexpr static const uint32_t runClockCfg1MainOscDisable{0x00000001};
		constexpr static const uint32_t runClockCfg1MainOscEnableMask{0xFFFFFFFE};
		constexpr static const uint8_t runClockCfg1SysClockDivShift{23};

		constexpr static inline uint32_t runClockCfg1SysClockDiv(const uint8_t divider) noexcept
		{
			return (uint32_t{divider} << runClockCfg1SysClockDivShift) &
				runClockCfg1SysClockDivMask;
		}

		// Oscillator config register 2 constants
		constexpr static const uint32_t runClockCfg2Mask{0x20EF978FU};
		constexpr static const uint32_t runClockCfg2UseRCC2{0x80000000};
		constexpr static const uint32_t runClockCfg2PLLPreDivEnable{0x00000000};
		constexpr static const uint32_t runClockCfg2PLLPreDivDisable{0x40000000};
		constexpr static const uint32_t runClockCfg2SysClockDivMask{0x1F800000};
		constexpr static const uint32_t runClockCfgSysClockDivLSBSet{0x00400000};
		constexpr static const uint32_t runClockCfgSysClockDivLSBClr{0x00000000};
		constexpr static const uint32_t runClockCfg2PLLUSBPowerUp{0x00000000};
		constexpr static const uint32_t runClockCfg2PLLUSBPowerDown{0x00004000};
		constexpr static const uint32_t runClockCfg2PLLPowerUp{0x00000000};
		constexpr static const uint32_t runClockCfg2PLLPowerDown{0x00002000};
		constexpr static const uint32_t runClockCfg2PLLNoBypass{0x00000000};
		constexpr static const uint32_t runClockCfg2PLLBypass{0x00000800};
		constexpr static const uint32_t runClockCfg2OscSourceMask{0x00000070};
		constexpr static const uint32_t runClockCfg2OscSourceMainOsc{0x00000000};
		constexpr static const uint32_t runClockCfg2OscSourceIntOsc{0x00000010};
		constexpr static const uint32_t runClockCfg2OscSourceIntOscDiv4{0x00000020};
		constexpr static const uint32_t runClockCfg2OscSource30khzOsc{0x00000030};
		constexpr static const uint32_t runClockCfg2OscSource32_768HzOsc{0x00000070};
		constexpr static const uint8_t runClockCfg2SysClockDivShift{23};

		constexpr static inline uint32_t runClockCfg2SysClockDiv(const uint8_t divider) noexcept
		{
			return (uint32_t{divider} << runClockCfg2SysClockDivShift) &
				runClockCfg2SysClockDivMask;
		}

		// Main oscillator config register constants
		constexpr static const uint32_t mainOscCtrlNoCrystal{0x00000004};
		constexpr static const uint32_t mainOscCtrlOscFailReboot{0x00000000};
		constexpr static const uint32_t mainOscCtrlOscFailInterrupt{0x00000002};
		constexpr static const uint32_t mainOscCtrlClockMonitorEnable{0x00000001};
		constexpr static const uint32_t mainOscCtrlClockMonitorDisable{0x00000000};

		// PLL status constants
		constexpr static const uint32_t pllStatusLocked{0x00000001};

		// GPIO AHB enable constants
		constexpr static const uint32_t gpioAHBCtrlPortA{0x00000001};
		constexpr static const uint32_t gpioAHBCtrlPortB{0x00000002};
		constexpr static const uint32_t gpioAHBCtrlPortC{0x00000004};
		constexpr static const uint32_t gpioAHBCtrlPortD{0x00000008};
		constexpr static const uint32_t gpioAHBCtrlPortE{0x00000010};
		constexpr static const uint32_t gpioAHBCtrlPortF{0x00000020};

		// GPIO run-mode clock gating control constants
		constexpr static const uint32_t runClockGateCtrlGPIOA{0x00000001};
		constexpr static const uint32_t runClockGateCtrlGPIOB{0x00000002};
		constexpr static const uint32_t runClockGateCtrlGPIOC{0x00000004};
		constexpr static const uint32_t runClockGateCtrlGPIOD{0x00000008};
		constexpr static const uint32_t runClockGateCtrlGPIOE{0x00000010};
		constexpr static const uint32_t runClockGateCtrlGPIOF{0x00000020};

		// SSI (SPI) run-mode clock gating control constants
		constexpr static const uint32_t runClockGateCtrlSSI0{0x00000001};
		constexpr static const uint32_t runClockGateCtrlSSI1{0x00000002};
		constexpr static const uint32_t runClockGateCtrlSSI2{0x00000004};
		constexpr static const uint32_t runClockGateCtrlSSI3{0x00000008};

		// USB run-mode clock gating control constants
		constexpr static const uint32_t runClockGateCtrlUSB{0x00000001};

		// GPIO perhipheral ready constants
		constexpr static const uint32_t periphReadyGPIOA{0x00000001};
		constexpr static const uint32_t periphReadyGPIOB{0x00000002};
		constexpr static const uint32_t periphReadyGPIOC{0x00000004};
		constexpr static const uint32_t periphReadyGPIOD{0x00000008};
		constexpr static const uint32_t periphReadyGPIOE{0x00000010};
		constexpr static const uint32_t periphReadyGPIOF{0x00000020};

		// SSI (SPI) perhipheral ready constants
		constexpr static const uint32_t periphReadySSI0{0x00000001};
		constexpr static const uint32_t periphReadySSI1{0x00000002};
		constexpr static const uint32_t periphReadySSI2{0x00000004};
		constexpr static const uint32_t periphReadySSI3{0x00000008};

		// USB perhipheral ready constants
		constexpr static const uint32_t periphReadyUSB{0x00000001};
	} // namespace sysCtrl
} // namespace vals

#endif /*TM4C123GH6PM_CONSTANTS___HXX*/
