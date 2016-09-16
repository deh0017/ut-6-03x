//****************************************************************************
//
// File Name: PLL.c
//
// Description: Phase-Locked Loop (PLL)
//     configures the crystal frequency to 16 MHz and
//     sets the clock speed to run at 80 MHz
// for more information, see
// page 256 (for crystal values) and
// page 224 (for divisor values) of the data sheet
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created June 05, 2016
// Updated August 13, 2016
//
//****************************************************************************

#include "PLL.h"

//**********PLL_XTAL_16_CLOCK_80**********
// Sets the crystal frequency to 16 MHz and the clock speed to 80 MHz
// Inputs: None
// Outputs: None
void PLL_XTAL_16_CLOCK_80(void) {
    // set SYSDIV2 value corresponding to 80-MHz clock, page 224
    // DIVISOR = SYSDIV2 + 1
    const unsigned char SYSDIV2 = 4;
    // 0) configure the system to use RCC2 for advanced features
    //    such as 400 MHz PLL and non-integer System Clock Divisor
    SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;

    // 1) bypass PLL while initializing
    SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;

    // 2) select the crystal value and oscillator source
    // clear XTAL field
    SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M;
    // configure for 16 MHz crystal, page 256
    SYSCTL_RCC_R += SYSCTL_RCC_XTAL_16MHZ;
    // clear oscillator source field
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;
    // configure for main oscillator source
    SYSCTL_RCC2_R += SYSCTL_RCC2_OSCSRC2_MO;

    // 3) activate PLL by clearing PWRDN
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;

    // 4) set the desired system divider and
    // the system divider least significant bit
    // use 400 MHz PLL
    SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;
    // clear system clock divider field
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_SYSDIV2;
    // configure for 400/(SYSDIV2 + 1) MHz clock
    SYSCTL_RCC2_R += SYSDIV2 << 22;

    // 5) wait for the PLL to lock by polling PLLLRIS
    while((SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS) == 0) {};

    // 6) enable use of PLL by clearing BYPASS
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;
}
