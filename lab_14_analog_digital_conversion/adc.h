/**
 * UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Lab 14: Analog-Digital Conversion
 *
 * File Name: adc.h
 *
 * Description: Provides functions that initialize ADC0 SS3 to be triggered by
 * software and trigger a conversion, wait for it to finish,
 * and return the result.
 *
 * Author: Phi Luu
 * Location: Portland, Oregon, United States
 * Created: May 07, 2016
 * Updated: June 23, 2017
 */

/**
 * Sets up the ADC.
 * Max sample rate: <= 125,000 samples/second
 * SS3 triggering event: software trigger
 * SS3 1st sample source: channel 1
 * SS3 interrupts: enabled but not promoted to controller
 */
void          InitAdcChannel0(void);

/**
 * Busy-wait Analog-Digital Conversion (ADC)
 *
 * @return    12-bit result of ADC
 */
unsigned long AdcChannel0In(void);
