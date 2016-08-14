//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 14: Analog-Digital Conversion
//
// File Name: ADC.h
//
// Description: Provide functions that initialize ADC0 SS3 to be triggered by
// software and trigger a conversion, wait for it to finish,
// and return the result
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created May 07, 2016
// Updated August 13, 2016
//
//****************************************************************************

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
// Busy-wait Analog-Digital Conversion
// Inputs: None
// Outputs: 12-bit result of ADC conversion
unsigned long ADC0_In(void);
