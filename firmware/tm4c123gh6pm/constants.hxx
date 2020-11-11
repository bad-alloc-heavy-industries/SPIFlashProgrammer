// SPDX-License-Identifier: BSD-3-Clause
#ifndef TM4C123GH6PM_CONSTANTS__HXX
#define TM4C123GH6PM_CONSTANTS__HXX

#include <cstdint>

namespace vals
{
	namespace gpio
	{
		// GPIO lock register constants
		constexpr static const uint32_t lockUnlocked{0x00000000};
		constexpr static const uint32_t lockLocked{0x00000001};
		constexpr static const uint32_t lockKey{0x4C4F434B};

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
		}
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

	namespace sysCtrl
	{
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
	} // namespace sysCtrl
} // namespace vals

#endif /*TM4C123GH6PM_CONSTANTS__HXX*/
