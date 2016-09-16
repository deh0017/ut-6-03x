//****************************************************************************
//
// File Name: SysTick.c
//
// Description: Initialize System Timer (SysTick) and create a delay function
//   This program assumes the clock is running at 80 MHz
//   The RELOAD value must be changed if the clock is running at
//   a different speed
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created June 05, 2016
// Updated August 13, 2016
//
//****************************************************************************

#include "SysTick.h"

//**********SysTick_Init**********
// Activates the system timer of the microcontroller
// Inputs: None
// Outputs: None
void SysTick_Init(void) {
    NVIC_ST_CTRL_R = 0;             // disable SysTick during set up
    NVIC_ST_RELOAD_R = 0x00FFFFFF;  // maximum value to RELOAD register
    NVIC_ST_CURRENT_R = 0;          // overwrite CURRENT to clear it
    NVIC_ST_CTRL_R = 0x00000005;    // enable CLK_SRC bit and ENABLE bit in
                                    // CTRL register
}

//**********SysTick_Delay**********
// Delays the program for a number of milliseconds
// Inputs: ms    the number of milliseconds to delay
// Outputs: None
// Assumes: 80-MHz clock
// Notes: period = (real_time_delay seconds)*(80,000,000 Hz)
void SysTick_Delay(unsigned long ms) {
    unsigned long count;
    // period = (1/1000 seconds)*(80,000,000 Hz) = 80,000
    const unsigned long period = 80000;
    for (count = 0; count < ms; count++) {
        // change the RELOAD value of SysTick
        NVIC_ST_RELOAD_R = (period - 1) & 0x00FFFFFF;
        // overwrite the CURRENT register
        NVIC_ST_CURRENT_R = 0;
        // wait until COUNT is flagged
        while (!(NVIC_ST_CTRL_R & 0x00010000)) {};
    }
}
