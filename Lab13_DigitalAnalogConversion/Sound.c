/*****************************************************************************

UTAustinX: UT.6.03x Embedded Systems - Shape the World
Lab 13: Digital-Analog Conversion

File Name: Sound.c

Description: Create a sinusoid sound wave using 4-bit DAC
and simulate C, D, E, and G notes of the piano.

Compatibility: EK-TM4C123GXL

Phi Luu
Portland, Oregon, United States
Created April 22, 2016
Updated July 17, 2016

*****************************************************************************/

#include "Sound.h"
#include "DAC.h"
#include "Piano.h"
#include "..//tm4c123gh6pm.h"

// a smooth digital value for analog sine wave
const unsigned char SineWave[16] = {
    8, 10, 13, 14, 15, 14, 13, 10,
    8, 5, 2, 1, 0, 1, 2, 5
};

// index variable of SineWave
// varies from 0-15
unsigned char Index = 0;

// convert from the frequency of the SysTick
// to the frequency of the sound wave
// has 4 elements representing the most suitable value
// f_sound = f_SysTick / 16
const unsigned long Frequency_Period[4] = {
    9550, 8508, 7570, 6366
};

//**********Sound_Init**********
// Initializes Systick periodic interrupts and DAC
// Inputs: None
// Outputs: None
void Sound_Init(void) {
    // DAC Initialization
    DAC_Init();     // call the DAC_Init() function in DAC.c
    Index = 0;      // init value of Index is 0
    // SysTick Initialization
    NVIC_ST_CTRL_R = 0;           // disable SysTick during setup
    NVIC_ST_RELOAD_R = 0x00FFFFFF;// maximize the RELOAD value
    NVIC_ST_CURRENT_R = 0;        // any write to CURRENT clears it
    NVIC_ST_CTRL_R = 0x00000005;  // enable SysTick with core clock
    // priority 1
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000;
    NVIC_ST_CTRL_R = 0x0007;      // enable, source clock, and interrupts
}

//**********Sound_Tone**********
// Changes SysTick periodic interrupts to start sound output
// Inputs: period    Interrupt period
//         Unit of period are 12.5ns
//         Maximum is 2^24-1
//         Minimum is determined by length of ISR
// Outputs: None
void Sound_Tone(unsigned long period) {
    // reset the RELOAD value for different frequencies
    NVIC_ST_RELOAD_R = (period - 1) & 0x00FFFFFF;
}

//**********Sound_Off**********
// Stops outputing to DAC
// Inputs: None
// Outputs: None
void Sound_Off(void) {
    GPIO_PORTB_DATA_R &= ~0x0F;     // clear PB3-PB0
}

//**********SysTick_Handler**********
// SysTick interrupt service routine
// Executed every (12.5 ns)*(period)
// Inputs: None
// Outputs: None
// Assumes: 80-MHz clock
void SysTick_Handler(void) {
    if (Freq_Index != 4) {
    // only executes if there is non-zero input
        Index = (Index + 1) & 0x0F; // cycles from 0-15
        DAC_Out(SineWave[Index]);   // output 1 data each interrupt
    }
}
