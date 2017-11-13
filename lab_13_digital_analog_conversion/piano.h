/**
 * @file     piano.h
 * @author   Phi Luu
 * @date     April 22, 2016
 *
 * @brief    UTAustinX: UT.6.03x Embedded Systems - Shape the World
 *           Lab 13: Digital-Analog Conversion
 *
 * @section  DESCRIPTION
 *
 * Takes inputs from switches and decides what note to output to the buzzer.
 * Port E, bits 3-0 have 4 piano keys.
 */

/**
 * Global variable which is accessable from other functions
 * store the index of the corresponding input
 */
extern unsigned short freq_index;

/**
 * Initializes piano key inputs.
 */
void InitPiano(void);

/**
 * Reads the input from PE3-PE0.
 *
 * @notes    Returns to freq_index with values from 0 to 4 depending on key pressed
 *            0 if key 0 = note C = PE0 is pressed
 *            0 if key 0 = note C = PE0 is pressed
 *            1 if key 1 = note D = PE1 is pressed
 *            2 if key 2 = note E = PE2 is pressed
 *            3 if key 3 = note G = PE3 is pressed
 */
void PianoIn(void);
