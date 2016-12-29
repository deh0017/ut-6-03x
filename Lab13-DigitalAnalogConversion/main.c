//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 13: Digital-Analog Conversion
//
// File Name: main.c
//
// Description: Create a sinusoid sound wave using 4-bit DAC
// and simulate C, D, E, and G notes of the piano.
// Port B bits 3-0 have the 4-bit DAC
// Port E bits 3-0 have 4 piano keys
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created April 22, 2016
// Updated December 28, 2016
//
//****************************************************************************

//**********Required Hardware I/O Connections**********
// PD3 must be connected to DAC output
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// 4 switches connected to PE3-PE0

//**********1. Pre-processor Section**********
#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"

//**********2. Global Declarations Section**********
// Function Prototypes:
void DisableInterrupts(void);       // Disable interrupts
void EnableInterrupts(void);        // Enable interrupts


//**********3. Main Function**********
int main(void) {
    // set up:
    // for the real board grader to work
    // activate grader and set system clock to 80 MHz
    TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210, ScopeOn);
    Sound_Init();           // initialize SysTick and DAC
    Piano_Init();           // initialize Switches
    EnableInterrupts();     // enable interrupts

    // loop:
    while (1) {
        Piano_In();         // read input from switches
        // if the index is out of range (no switch is pressed)
        if (Freq_Index == 4) {
            Sound_Off();   // turn off the sound
        }
        // otherwise
        else {
            // adjust the frequency similarly to C, D, E, G
            Sound_Tone(Frequency_Period[Freq_Index]);
        }
    }
}
