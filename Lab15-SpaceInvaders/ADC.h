/*****************************************************************************

UTAustinX: UT.6.03x Embedded Systems - Shape the World
Lab 15: SpaceInvaders

File Name: ADC.c

Description: Functions that are responsible for Analog-Digital Conversion,
which reads input from the slide pot and convert to numerical result

Compatibility: EK-TM4C123GXL

Phi Luu
Portland, Oregon, United States
Created May 20, 2016
Updated July 17, 2016

*****************************************************************************/

//**********ADC0_Init**********
// Sets up the ADC
// Max sample rate: <= 125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source: channel 1
// SS3 interrupts: enabled but not promoted to controller
// Inputs: None
// Outputs: None
void ADC0_Init(void);

//**********ADC0_In**********
// Busy-wait Analog to digital conversion
// Inputs: None
// Outputs: 12-bit result of ADC conversion
unsigned long ADC0_In(void);
