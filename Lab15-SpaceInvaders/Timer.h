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
// Updated December 31, 2016
//
//****************************************************************************

//***
// Initialize 30-Hz software-triggered interrupt
//
// @assumption      80-MHz clock
//***
void Init_SysTick(void);

//***
// Initialize 11-kHz software-triggered interrupt
//
// @assumption      80-MHz clock
//***
void Init_Timer2(void);

//***
// Delays a number of times based on the clock speed of the LaunchPad
//
// @param   ms   number of milliseconds needed to delay
//
// @assumption   80-MHz clock
//***
void Delay(unsigned long ms);
