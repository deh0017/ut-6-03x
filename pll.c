/**
 * File Name: pll.c
 *
 * Description: Phase-Locked Loop (PLL)
 * Configures the crystal frequency to 16 MHz and sets the clock speed to run
 * at 80 MHz. For more information, see
 *     page 256 (for crystal values)
 *     page 224 (for divisor values) of the data sheet
 *
 * Author: Phi Luu
 * Location: Portland, Oregon, United States
 * Created: June 05, 2016
 * Updated: June 23, 2017
 */

#include "pll.h"

/**
 * Sets the crystal frequency to 16 MHz and the clock speed to 80 MHz.
 */
void SetPll_Xtal_16_Clock_80(void) {
    // set SYSDIV2 value corresponding to 80-MHz clock, page 224
    // DIVISOR = SYSDIV2 + 1
    const unsigned char SYSDIV2 = 4;

    // configure the system to use RCC2 for advanced features
    // such as 400 MHz PLL and non-integer System Clock Divisor
    SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;

    // bypass PLL while initializing
    SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;

    // select the crystal value and oscillator source
    // clear XTAL field
    SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M;
    // configure for 16 MHz crystal, page 256
    SYSCTL_RCC_R += SYSCTL_RCC_XTAL_16MHZ;
    // clear oscillator source field
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;
    // configure for main oscillator source
    SYSCTL_RCC2_R += SYSCTL_RCC2_OSCSRC2_MO;

    // activate PLL by clearing PWRDN
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;

    // set the desired system divider and
    // the system divider least significant bit
    // use 400 MHz PLL
    SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;
    // clear system clock divider field
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_SYSDIV2;
    // configure for 400/(SYSDIV2 + 1) MHz clock
    SYSCTL_RCC2_R += SYSDIV2 << 22;

    // wait for the PLL to lock by polling PLLLRIS
    while ((SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS) == 0) {}

    // enable use of PLL by clearing BYPASS
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;
}
