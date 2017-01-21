/**
 * UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Lab 15: SpaceInvaders
 *
 * File Name: Timer.c
 *
 * Description: Contains SysTick, Timer2A, and the regular Delay initialization
 *
 * Compatibility: EK-TM4C123GXL
 *
 * Phi Luu
 * Portland, Oregon, United States
 * Created May 20, 2016
 * Updated January 15, 2017
 */

#include "Timer.h"
#include "tm4c123gh6pm.h"

/**
 * Initialize 120-Hz software-triggered interrupt
 *
 * @assumption      80-MHz clock
 */
void Init_SysTick(void) {
    NVIC_ST_CTRL_R    = 0;       // disable SysTick during set up
    NVIC_ST_RELOAD_R  = 666667;  // 120-Hz interrupt 666667
    NVIC_ST_CURRENT_R = 0;       // overwrite to CURRENT to clear it
    // priority 1
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000;
    // enable, source clock, and interrupts
    NVIC_ST_CTRL_R = 0x0007;
}

/**
 * Initialize 11-kHz software-triggered interrupt
 *
 * @assumption      80-MHz clock
 */
void Init_Timer2(void) {
    unsigned long volatile delay;

    SYSCTL_RCGCTIMER_R |= 0x04;         // 0) activate timer2
    delay               = SYSCTL_RCGCTIMER_R;
    TIMER2_CTL_R        = 0x00000000;   // 1) disable timer2A during setup
    TIMER2_CFG_R        = 0x00000000;   // 2) configure for 32-bit mode
    TIMER2_TAMR_R       = 0x00000002;   // 3) configure for periodic mode
    TIMER2_TAILR_R      = 7272;         // 4) 11-kHz interrupt 7272
    TIMER2_TAPR_R       = 0;            // 5) bus clock resolution
    TIMER2_ICR_R        = 0x00000001;   // 6) clear timer2A timeout flag
    TIMER2_IMR_R        = 0x00000001;   // 7) arm timeout interrupt
    NVIC_PRI5_R         = (NVIC_PRI5_R & 0x00FFFFFF)
                          | 0x80000000; // 8) priority 4
    // interrupts enabled in the main program after all devices initialized
    // vector number 39, interrupt number 23
    NVIC_EN0_R   = 1 << 23;             // 9) enable IRQ 23 in NVIC
    TIMER2_CTL_R = 0x00000001;          // 10) enable timer2A
}

/**
 * Delays a number of times based on the clock speed of the LaunchPad
 *
 * @param  ms  number of milliseconds to delay
 *
 * @assumption 80-MHz clock
 */
void Delay(unsigned long ms) {
    unsigned long count;

    while (ms) {
        count = 16000; // 16000 = 1ms for 80 MHz

        while (count) {
            count--;
        }
        ms--;
    }
}
