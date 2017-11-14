/**
 * @file     led.c
 * @author   Phi Luu
 * @date     May 20, 2016
 *
 * @brief    UTAustinX: UT.6.03x Embedded Systems - Shape the World
 *           Lab 15: SpaceInvaders
 *
 * @section  DESCRIPTION
 *
 * This file contains functions that are responsible for outputing LED signals.
 */

#include "tm4c123gh6pm.h"
#include "data.h"
#include "led.h"

/**
 * Initializes LEDs on PB5 and PB4.
 */
void InitLed(void) {
    volatile unsigned long delay;

    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;         // Enable GPIO Port B clock
    delay = SYSCTL_RCGC2_R;
    GPIO_PORTB_DIR_R |= (LED1_PIN | LED2_PIN);    // PB4,5 are output
    GPIO_PORTB_AFSEL_R &= ~(LED1_PIN | LED2_PIN); // Disable alternate function
    GPIO_PORTB_AMSEL_R &= ~(LED1_PIN | LED2_PIN); // Disable analog functions
    GPIO_PORTB_PCTL_R = 0;                        // Clear PCTL content
    GPIO_PORTB_DEN_R |= (LED1_PIN | LED2_PIN);    // Enable digital
}

/**
 * Sets LED 1 on PB4 to be on/off depending on the value.
 */
void SetLed1(int value) {
    if (value == 0) {
        GPIO_PORTB_DATA_R &= ~LED1_PIN;
    } else {
        GPIO_PORTB_DATA_R |= LED1_PIN;
    }
}

/**
 * Sets LED 2 on PB5 to be on/off depending on the value.
 */
void SetLed2(int value) {
    if (value == 0) {
        GPIO_PORTB_DATA_R &= ~LED2_PIN;
    } else {
        GPIO_PORTB_DATA_R |= LED2_PIN;
    }
}
