/**
 * UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Lab 13: Digital-Analog Conversion
 *
 * File Name: dac.h
 *
 * Description: Create a sinusoid sound wave using 4-bit DAC
 * and simulate C, D, E, and G notes of the piano.
 *
 * Compatibility: EK-TM4C123GXL
 *
 * Phi Luu
 * Portland, Oregon, United States
 * Created April 22, 2016
 * Updated May 30, 2017
 */

// 4-bit DAC connected to PB3-PB0

/**
 * Initializes 4-bit DAC
 */
void DAC_Init(void);

/**
 * Writes output to DAC
 *
 * @param  data  4-bit data, 0 to 15
 */
void DAC_Out(unsigned long data);
