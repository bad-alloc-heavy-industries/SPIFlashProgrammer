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

	namespace sysCtrl
	{
		// Oscillator config register 1 constants
		constexpr static const uint32_t runClockCfg1Mask{0xF021D00E};
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
		constexpr static const uint32_t runClockCfg2Mask{0xF021D00E};
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

#endif /*TM4C123GH6PM_CONSTANTS__HXX*/
