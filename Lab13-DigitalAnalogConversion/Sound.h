//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 13: Digital-Analog Conversion
//
// File Name: Sound.h
//
// Description: Create a sinusoid sound wave using 4-bit DAC
// and simulate C, D, E, and G notes of the piano.
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created April 22, 2016
// Updated December 28, 2016
//
//****************************************************************************

//**********Sound_Init**********
// Initializes Systick periodic interrupts and DAC
// Inputs: None
// Outputs: None
void Sound_Init(void);

//**********Sound_Tone**********
// Changes SysTick periodic interrupts to start sound output
// Inputs: period    Interrupt period
//         Unit of period are 12.5ns
//         Maximum is 2^24-1
//         Minimum is determined by length of ISR
// Outputs: None
void Sound_Tone(unsigned long period);

//**********Sound_Off**********
// Stops outputing to DAC
// Inputs: None
// Outputs: None
void Sound_Off(void);

//**********Frequency_Period**********
// convert from the frequency of the SysTick
// to the frequency of the sound wave
// has 4 elements representing the most suitable value
// f_sound = f_SysTick / 16
extern const unsigned long Frequency_Period[4];
