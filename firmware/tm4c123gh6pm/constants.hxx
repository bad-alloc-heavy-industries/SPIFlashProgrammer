// SPDX-License-Identifier: BSD-3-Clause
#ifndef TM4C123GH6PM_CONSTANTS__HXX
#define TM4C123GH6PM_CONSTANTS__HXX

#include <cstdint>

namespace vals
{
	namespace gpio
	{
		// The GPIO commit register is unlocked and may be modified
		constexpr static const uint32_t lockUnlocked{0x00000000};
		// The GPIO commit register is locked and may not be modified
		constexpr static const uint32_t lockLocked{0x00000001};
		// Unlocks the GPIO commit register
		constexpr static const uint32_t lockKey{0x4C4F434B};
	} // namespace gpio

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

		// GPIO perhipheral ready constants
		constexpr static const uint32_t periphReadyGPIOA{0x00000001};
		constexpr static const uint32_t periphReadyGPIOB{0x00000002};
		constexpr static const uint32_t periphReadyGPIOC{0x00000004};
		constexpr static const uint32_t periphReadyGPIOD{0x00000008};
		constexpr static const uint32_t periphReadyGPIOE{0x00000010};
		constexpr static const uint32_t periphReadyGPIOF{0x00000020};
	} // namespace sysCtrl
} // namespace vals

#endif /*TM4C123GH6PM_CONSTANTS__HXX*/
