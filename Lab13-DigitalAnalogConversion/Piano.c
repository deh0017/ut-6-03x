/*****************************************************************************

UTAustinX: UT.6.03x Embedded Systems - Shape the World
Lab 13: Digital-Analog Conversion

File Name: Piano.c

Description: Create a sinusoid sound wave using 4-bit DAC
and simulate C, D, E, and G notes of the piano.

Compatibility: EK-TM4C123GXL

Phi Luu
Portland, Oregon, United States
Created April 22, 2016
Updated July 22, 2016

*****************************************************************************/

// Port E bits 3-0 have 4 piano keys

#include "Piano.h"
#include "tm4c123gh6pm.h"

// store the index value of the corresponding input
unsigned short Freq_Index;

//**********Piano_Init**********
// Initializes piano key inputs
// Inputs: None
// Outputs: None
void Piano_Init(void) {
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x10;           // unlock port E clock
    delay = SYSCTL_RCGC2_R;           // allow time for clock to start
    GPIO_PORTE_LOCK_R |= 0x4C4F434B;  // unlock GPIO Port E
    GPIO_PORTE_CR_R |= 0x0F;          // allow change to PE3-PE0
    GPIO_PORTE_AMSEL_R &= ~0x0F;      // disable analog function on PE3-PE0
    GPIO_PORTE_PCTL_R = 0x00;         // clear PCTL register on PE3-PE0
    GPIO_PORTE_DIR_R &= ~0x0F;        // PE3-PE0 inputs
    GPIO_PORTE_AFSEL_R &= ~0x0F;      // disable alternate function on PE3-PE0
    GPIO_PORTE_PUR_R &= ~0x0F;        // disable pull-up resistors on PE3-PE0
    GPIO_PORTE_DEN_R |= 0x0F;         // enable digital pins on PE3-PE0
}

//**********Piano_In**********
// Reads the input from PE3-PE0
// Inputs: None
// Outputs: None
// Notes: Returns to Freq_Index with values from 0 to 4 depending on keys
//    0 if key 0 = note C = PE0 is pressed
//    1 if key 1 = note D = PE1 is pressed
//    2 if key 2 = note E = PE2 is pressed
//    3 if key 3 = note G = PE3 is pressed
void Piano_In(void) {
    if (GPIO_PORTE_DATA_R == 0x01) {      // C Note,
        Freq_Index = 0;           // store "Key 0 is pressed"
    }
    else if (GPIO_PORTE_DATA_R == 0x02) { // D Note,
        Freq_Index = 1;           // store "Key 1 is pressed"
    }
    else if (GPIO_PORTE_DATA_R == 0x04) { // E Note,
        Freq_Index = 2;           // store "Key 2 is pressed"
    }
    else if (GPIO_PORTE_DATA_R == 0x08) { // G Note,
        Freq_Index = 3;           // store "Key 3 is pressed"
    }
    else {                                // otherwise,
        Freq_Index = 4;           // store "No key is pressed"
    }
}
