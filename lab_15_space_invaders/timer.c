/**
 * @file     timer.c
 * @author   Phi Luu
 * @date     May 20, 2016
 *
 * @brief    UTAustinX: UT.6.03x Embedded Systems - Shape the World
 *           Lab 15: SpaceInvaders
 *
 * @section  DESCRIPTION
 *
 * This file contains functions that are responsible initializing interrupt
 * service routines and delay functions.
 */

#include "tm4c123gh6pm.h"
#include "timer.h"

/**
 * Initializes Timer 2 used for main control loop.
 */
void InitTimer2(unsigned long period) {
    unsigned long volatile delay;

    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R2; // 0a) activate timer2
    delay = SYSCTL_RCGCTIMER_R;
    timer2a_count = 0;
    semaphore2a = 0;
    TIMER2_CTL_R = 0x00000000;               // 1a) disable timer2 during setup
    TIMER2_CFG_R = TIMER_CFG_32_BIT_TIMER;   // 2a) configure timer2 for 32-bit mode
    TIMER2_TAMR_R |= TIMER_TAMR_TAMR_PERIOD; // 3a) periodic count down is default but needed by simulator
    TIMER2_TAILR_R = period - 1;             // 4a) timer2A reload value
    TIMER2_TAPR_R = 0;                       // 5a) timer2A bus clock resolution
    TIMER2_ICR_R |= TIMER_ICR_TATOCINT;      // 6a) clear timer2A timeout flag
    TIMER2_IMR_R |= TIMER_IMR_TATOIM;        // 7a) arm timer2A timeout interrupt
    TIMER2_CTL_R |= TIMER_CTL_TAEN;          // 10a) enable timer2A
    // interrupts are enabled in the main program after all devices initialized
    // vector number 39, interrupt number 23
    NVIC_PRI5_R = (NVIC_PRI5_R & (~NVIC_PRI5_INT23_M))
                  | (4UL << NVIC_PRI5_INT23_S); // 8) priority 4
    NVIC_EN0_R = 1 << 23;                       // 9) enable IRQ 23 in NVIC
}

/**
 * General-purpose 100ms delay that delays count times of each 100ms.
 */
void Delay100ms(unsigned long count) {
    unsigned long time100ms;

    while (count > 0) {
        time100ms = 727240; // 100ms at 80 MHz

        while (time100ms > 0) {
            time100ms--;
        }

        count--;
    }
}
