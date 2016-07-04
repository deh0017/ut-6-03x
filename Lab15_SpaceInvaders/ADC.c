/*******************************************************************************

UTAustinX: UT.6.03x Embedded Systems - Shape the World
Lab 15: SpaceInvaders

Name: ADC.c

Description: Functions that are responsible for Analog-Digital Conversion,
which reads input from the slide pot and convert to numerical result

Compatibility: EK-TM4C123GXL

Phi Luu
David Douglas High School
Portland, OR
July 03, 2016

*******************************************************************************/

// PE2: Slide Pot
// PE1: Switches

#include "ADC.h"
#include "tm4c123gh6pm.h"

//**********ADC0_Init**********
// Sets up the ADC 
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source:  channel 1
// SS3 interrupts: enabled but not promoted to controller
// Inputs: None
// Outputs: None
void ADC0_Init(void) {
    volatile unsigned long delay;
    // GPIO Port E Initialization:
    SYSCTL_RCGC2_R |= 0x10;             // enable port E clock
    delay = SYSCTL_RCGC2_R;             // allow time for clock to start
    GPIO_PORTE_LOCK_R |= 0x4C4F434B;    // unlock GPIO port E
    GPIO_PORTE_CR_R |= 0x06;            // allow change to PE2-PE1
    GPIO_PORTE_DIR_R &= ~0x06;          // PE2-PE1 input
	GPIO_PORTE_AFSEL_R &= ~0x02;        // disable alternate function on PE1
	GPIO_PORTE_AFSEL_R |= 0x04;         // enable alternate function on PE2
    GPIO_PORTE_PCTL_R = 0;              // disable PCTL on port E
    GPIO_PORTE_DEN_R &= ~0x04;          // disable digital I/O on PE2
	GPIO_PORTE_DEN_R |= 0x02;           // enable digital I/O on PE1
	GPIO_PORTE_AMSEL_R &= ~0x02;        // disable analog function on PE1
    GPIO_PORTE_AMSEL_R |= 0x04;         // enable analog function on PE2
    // Analog Initialization:
    SYSCTL_RCGC0_R |= 0x10000;          // enable the ADC clock
    delay = SYSCTL_RCGC0_R;             // allow time for clock to start
    SYSCTL_RCGC0_R &= ~0x300;           // configure for 125K samples/second
    ADC0_SSPRI_R &= ~0x3000;            // Highest Priority = 0
    ADC0_ACTSS_R &= ~0x08;              // disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000;             // use software trigger - SysTick
    ADC0_SSMUX3_R |= 0x01;              // choose Ain channel 1
    ADC0_SSCTL3_R |= 0x06;              // clear TS0, D0; set IE0, END0
    ADC0_ACTSS_R |= 0x08;               // enable sample sequencer 3
}

//**********ADC0_In**********
// Busy-wait Analog to digital conversion
// Inputs: None
// Outputs: 12-bit result of ADC conversion
unsigned long ADC0_In(void) {
    unsigned long result;
    ADC0_PSSI_R |= 0x08;                // initiate SS3
    // wait until conversion is done:
    while ((ADC0_RIS_R & 0x08) == 0) {} 
    result = ADC0_SSFIFO3_R & 0xFFF;    // mask with 12-bit binary
    ADC0_ISC_R |= 0x08;   // clear the flag to start capturing another sample
    return result;        // return the value of result
}
