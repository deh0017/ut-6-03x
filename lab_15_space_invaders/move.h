/**
 * @file     move.h
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

/**
 * Sets up the ADC.
 * Max sample rate: <= 125,000 samples/second
 * SS3 triggering event: software trigger
 * SS3 1st sample source: channel 1
 * SS3 interrupts: enabled but not promoted to controller
 */
void InitAdc0(void);

/**
 * Busy-wait Analog-Digital conversion (ADC) that samples analog signals and
 * returns the value read from the channel.
 */
unsigned long ReadAdc0(void);

/**
 * Initializes the game buttons for normal shooting and powerbar shooting.
 */
void InitButton(void);
