/**
 * UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Lab 13: Digital-Analog Conversion
 *
 * File Name: Sound.h
 *
 * Description: Create a sinusoid sound wave using 4-bit DAC
 * and simulate C, D, E, and G notes of the piano.
 *
 * Compatibility: EK-TM4C123GXL
 *
 * Phi Luu
 * Portland, Oregon, United States
 * Created April 22, 2016
 * Updated January 15, 2017
 */

// convert from the frequency of the SysTick
// to the frequency of the sound wave
// has 4 elements representing the most suitable value
// f_sound = f_SysTick / 16
extern const unsigned long Frequency_Period[4];

/**
 * Initializes Systick periodic interrupts and DAC
 */
void Sound_Init(void);

/**
 * Changes SysTick periodic interrupts to start sound output
 *
 * @param  period  interrupt period
 *                 1 unit of period = 12.5 ns
 *                 maximum is 2^24 - 1
 *                 minimum is determined by the length of ISR
 */
void Sound_Tone(unsigned long period);

/**
 * Stops outputing to DAC
 */
void Sound_Off(void);
