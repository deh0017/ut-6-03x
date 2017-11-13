/**
 * @file     dac.h
 * @author   Phi Luu
 * @date     April 22, 2016
 *
 * @brief    UTAustinX: UT.6.03x Embedded Systems - Shape the World
 *           Lab 13: Digital-Analog Conversion
 *
 * @section  DESCRIPTION
 *
 * Initializes DAC and writes data into a 4-bit DAC.
 * Note that 4-bit DAC connected to PB3-PB0.
 */

/**
 * Initializes 4-bit DAC.
 */
void InitDac(void);

/**
 * Writes output to DAC.
 *
 * @param  data  4-bit data, 0 to 15
 */
void OutDac(unsigned long data);
