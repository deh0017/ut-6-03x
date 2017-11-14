/**
 * @file     timer.h
 * @author   Phi Luu
 * @date     May 20, 2016
 *
 * @brief    UTAustinX: UT.6.03x Embedded Systems - Shape the World
 *           Lab 15: SpaceInvaders
 *
 * @section  DESCRIPTION
 *
 * This file contains functions that are responsible initializing interrupt
 * service routines and delay functions.
 */

// Timer 2A flag
extern unsigned long timer2a_count;
extern unsigned long semaphore2a;

/**
 * Initializes Timer 2 used for main control loop.
 */
void InitTimer2(unsigned long period);

/**
 * General-purpose 100ms delay that delays count times of each 100ms.
 */
void Delay100ms(unsigned long count);
