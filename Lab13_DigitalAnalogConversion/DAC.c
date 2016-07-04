/*******************************************************************************

UTAustinX: UT.6.03x Embedded Systems - Shape the World
Lab 13: Digital-Analog Conversion

Name: DAC.c

Description: Create a sinusoid sound wave using 4-bit DAC
and simulate C, D, E, and G notes of the piano.

Compatibility: EK-TM4C123GXL

Phi Luu
David Douglas High School
Portland, OR
July 03, 2016

*******************************************************************************/

// 4-bit DAC connected to PB3-PB0

#include "DAC.h"
#include "tm4c123gh6pm.h"

//**********DAC_Init**********
// Initializes 4-bit DAC 
// Inputs: None
// Outputs: None
void DAC_Init(void) {
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x02;           // unlock port B clock
    delay = SYSCTL_RCGC2_R;           // allow time for clock to start
    GPIO_PORTB_LOCK_R |= 0x4C4F434B;  // unlock GPIO Port B
    GPIO_PORTB_CR_R |= 0x0F;          // allow change to PB3-PB0
    GPIO_PORTB_AMSEL_R &= ~0x0F;      // disable analog function on PB3-PB0
    GPIO_PORTB_PCTL_R = 0x00;         // clear PCTL register on PB3-PB0
    GPIO_PORTB_DIR_R |= 0x0F;         // PB3-PB0 outputs
    GPIO_PORTB_AFSEL_R &= ~0x0F;      // disable alternate function on PB3-PB0
    GPIO_PORTB_DEN_R |= 0x0F;         // enable digital pins on PB3-PB0
    GPIO_PORTB_DR8R_R |= 0x0F;        // enable 8-mA drive select on PB3-PB0
}

//**********DAC_Out**********
// Writes output to DAC
// Inputs: data    4-bit data, 0 to 15 
// Outputs: None
void DAC_Out(unsigned long data) {
    // write every single value from the SineWave array
    GPIO_PORTB_DATA_R = data; 
}
