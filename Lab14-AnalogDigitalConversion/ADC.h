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
// Updated December 31, 2016
//
//****************************************************************************

//***
// Set up the ADC
// Max sample rate: <= 125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source: channel 1
// SS3 interrupts: enabled but not promoted to controller
//***
void ADC0_Init(void);

//***
// Busy-wait Analog-Digital Conversion
//
// @return   12-bit result of ADC conversion
//***
unsigned long ADC0_In(void);
