//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 13: Digital-Analog Conversion
//
// File Name: Piano.h
//
// Description: Create a sinusoid sound wave using 4-bit DAC
// and simulate C, D, E, and G notes of the piano.
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created April 22, 2016
// Updated August 13, 2016
//
//****************************************************************************

// Port E bits 3-0 have 4 piano keys

//**********Piano_Init**********
// Initializes piano key inputs
// Inputs: None
// Outputs: None
void Piano_Init(void);

//**********Piano_In**********
// Reads the input from PE3-PE0
// Inputs: None
// Outputs: None
// Notes: Returns to Freq_Index with values from 0 to 4 depending on keys
//    0 if key 0 = note C = PE0 is pressed
//    1 if key 1 = note D = PE1 is pressed
//    2 if key 2 = note E = PE2 is pressed
//    3 if key 3 = note G = PE3 is pressed
void Piano_In(void);

//**********Freq_Index**********
// global variable which is accessable from other functions
// store the index of the corresponding input
extern unsigned short Freq_Index;
