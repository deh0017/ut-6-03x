/**
 * @file     led.h
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

/**
 * Initializes LEDs on PB5 and PB4.
 */
void InitLed(void);

/**
 * Sets LED 1 on PB4 to be on/off depending on the value.
 */
void SetLed1(int value);

/**
 * Sets LED 1 on PB5 to be on/off depending on the value.
 */
void SetLed2(int value);
