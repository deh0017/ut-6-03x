//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 15: SpaceInvaders
//
// File Name: Tick.h
//
// Description: Contains SysTick, Timer2A, and the regular Delay initialization
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created May 20, 2016
// Updated August 13, 2016
//
//****************************************************************************

//**********SysTick_Init**********
// Initializes 30-Hz software-triggered interrupt
// Inputs: None
// Outputs: None
// Assumes: 80-MHz clock
void SysTick_Init(void);			        // SysTick initialization

//**********Timer2_Init**********
// Initializes 11-kHz software-triggered interrupt
// Inputs: None
// Outputs: None
// Assumes: 80-MHz clock
void Timer2_Init(void);

//**********Delay**********
// Delays a number of times based on the clock speed of the LaunchPad
// Inputs: number of milliseconds needed to delay
// Outputs: None
// Assumes: 80-MHz clock
void Delay(unsigned long ms);
