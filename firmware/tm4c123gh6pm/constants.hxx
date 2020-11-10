// SPDX-License-Identifier: BSD-3-Clause
#ifndef TM4C123GH6PM_CONSTANTS__HXX
#define TM4C123GH6PM_CONSTANTS__HXX

#include <cstdint>

// The GPIO commit register is unlocked and may be modified
constexpr static const uint32_t gpioLockUnlocked{0x00000000};
// The GPIO commit register is locked and may not be modified
constexpr static const uint32_t gpioLockLocked{0x00000001};
// Unlocks the GPIO commit register
constexpr static const uint32_t gpioLockKey{0x4C4F434B};

// GPIO AHB enable constants
constexpr static const uint32_t sysCtrlGPIOAHBCtrlPortA{0x00000001};
constexpr static const uint32_t sysCtrlGPIOAHBCtrlPortB{0x00000002};
constexpr static const uint32_t sysCtrlGPIOAHBCtrlPortC{0x00000004};
constexpr static const uint32_t sysCtrlGPIOAHBCtrlPortD{0x00000008};
constexpr static const uint32_t sysCtrlGPIOAHBCtrlPortE{0x00000010};
constexpr static const uint32_t sysCtrlGPIOAHBCtrlPortF{0x00000020};

// GPIO run-mode clock gating control constants
constexpr static const uint32_t sysCtrlRunClockGateCtrlGPIOA{0x00000001};
constexpr static const uint32_t sysCtrlRunClockGateCtrlGPIOB{0x00000002};
constexpr static const uint32_t sysCtrlRunClockGateCtrlGPIOC{0x00000004};
constexpr static const uint32_t sysCtrlRunClockGateCtrlGPIOD{0x00000008};
constexpr static const uint32_t sysCtrlRunClockGateCtrlGPIOE{0x00000010};
constexpr static const uint32_t sysCtrlRunClockGateCtrlGPIOF{0x00000020};

// GPIO perhipheral ready constants
constexpr static const uint32_t sysCtrlPeriphReadyGPIOA{0x00000001};
constexpr static const uint32_t sysCtrlPeriphReadyGPIOB{0x00000002};
constexpr static const uint32_t sysCtrlPeriphReadyGPIOC{0x00000004};
constexpr static const uint32_t sysCtrlPeriphReadyGPIOD{0x00000008};
constexpr static const uint32_t sysCtrlPeriphReadyGPIOE{0x00000010};
constexpr static const uint32_t sysCtrlPeriphReadyGPIOF{0x00000020};

#endif /*TM4C123GH6PM_CONSTANTS__HXX*/
