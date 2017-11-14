/**
 * @file     move.c
 * @author   Phi Luu
 * @date     May 20, 2016
 *
 * @brief    UTAustinX: UT.6.03x Embedded Systems - Shape the World
 *           Lab 15: SpaceInvaders
 *
 * @section  DESCRIPTION
 *
 * This file contains functions that are responsible for getting inputs from
 * potentiometer and switches and determining the player's movement based on
 * the inputs received.
 */

#include "tm4c123gh6pm.h"
#include "data.h"
#include "move.h"

/**
 * Sets up the ADC.
 * Max sample rate: <= 125,000 samples/second
 * SS3 triggering event: software trigger
 * SS3 1st sample source: channel 1
 * SS3 interrupts: enabled but not promoted to controller
 */
void InitAdc0(void) {
    volatile unsigned long delay;

    // GPIO Port E Initialization
    SYSCTL_RCGC2_R |= 0x10;                    // enable port E clock (bit 4)
    delay = SYSCTL_RCGC2_R;                    // allow time for clock to start
    GPIO_PORTE_LOCK_R |= 0x4C4F434B;           // unlock GPIO port E
    GPIO_PORTE_CR_R |= POT_PIN;                // allow change to POT_PIN
    GPIO_PORTE_DIR_R &= ~POT_PIN;              // set POT_PIN as input
    GPIO_PORTE_AFSEL_R |= POT_PIN;             // enable alternate function on POT_PIN
    GPIO_PORTE_PCTL_R = 0;                     // disable PCTL on port E
    GPIO_PORTE_DEN_R &= ~POT_PIN;              // disable digital I/O on POT_PIN
    GPIO_PORTE_AMSEL_R |= POT_PIN;             // enable analog function on POT_PIN

    // analog Initialization
    SYSCTL_RCGC0_R |= 0x10000;                 // enable the ADC clock at bit 16
    delay = SYSCTL_RCGC0_R;                    // allow time for clock to start
    SYSCTL_RCGC0_R &= ~SYSCTL_RCGC0_ADC0SPD_M; // configure for 125K samples/second
    SYSCTL_RCGC0_R |= SYSCTL_RCGC0_ADC0SPD_125K;
    ADC0_SSPRI_R = 0x3210L;                    // SS3 has lowest priority
    ADC0_EMUX_R &= ~0xF000;                    // use software trigger - SysTick
    ADC0_SSCTL3_R = 0x06;                      // clear TS0, D0; set IE0, END0
    ADC0_PSSI_R &= ~0x08;                      // clear S/W request for SS3 sample
    ADC0_ACTSS_R |= 0x08;                      // enable sample sequencer 3
    ADC0_SSMUX3_R |= 0x01;                     // choose Ain channel 1
    ADC0_ISC_R |= 0x08;                        // clear SS3 ADC reading ready flag
}

/**
 * Busy-wait Analog-Digital conversion (ADC) that samples analog signals and
 * returns the value read from the channel.
 */
unsigned long ReadAdc0(void) {
    unsigned long result;

    // request SS3 reading
    ADC0_PSSI_R |= 0x08;

    // wait until conversion is done
    while ((ADC0_RIS_R & 0x08) == 0) {}

    // mask with 12-bit binary
    result = ADC0_SSFIFO3_R & 0xFFF;

    // clear the flag to start capturing another sample
    ADC0_ISC_R |= 0x08;

    return result;
}

/**
 * Initializes the game buttons for normal shooting and powerbar shooting.
 */
void InitButton(void) {
    /* Fire button is positive logic switch on PE0 */
    volatile unsigned long delay;

    SYSCTL_RCGC2_R |= 0x10;                           // enable GPIO Port E clock (bit 4)
    delay = SYSCTL_RCGC2_R;
    GPIO_PORTE_DIR_R &= ~(SWITCH1_PIN | SWITCH2_PIN); // set two switches as input
    GPIO_PORTE_DEN_R |= SWITCH1_PIN | SWITCH2_PIN;    // Enable digital input on two switches
    GPIO_PORTE_PDR_R |= SWITCH1_PIN | SWITCH2_PIN;    // Enable pull-down resistor on two switches
    GPIO_PORTE_AFSEL_R &= SWITCH1_PIN | SWITCH2_PIN;  // Disable two switches alternate function
    GPIO_PORTE_AMSEL_R &= SWITCH1_PIN | SWITCH2_PIN;  // Disable two switches analog functions
    GPIO_PORTE_PCTL_R = 0;                            // Clear PCTL content for two switches
}
