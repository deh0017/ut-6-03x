/**
 * @file     adc.h
 * @author   Phi Luu
 * @date     May 07, 2016
 *
 * @brief    UTAustinX: UT.6.03x Embedded Systems - Shape the World
 *           Lab 14: Analog-Digital Conversion
 *
 * @section  DESCRIPTION
 *
 * Provides functions that initializes ADC0 SS3 to be triggered by software and
 * trigger a conversion, wait for it to finish, and return the result
 */

/**
 * Sets up the ADC.
 * Max sample rate: <= 125,000 samples/second
 * SS3 triggering event: software trigger
 * SS3 1st sample source: channel 1
 * SS3 interrupts: enabled but not promoted to controller
 */
void InitAdcChannel0(void);

/**
 * Busy-wait Analog-Digital Conversion (ADC)
 *
 * @return    12-bit result of ADC
 */
unsigned long AdcChannel0In(void);
