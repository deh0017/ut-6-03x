//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 15: SpaceInvaders
//
// File Name: DAC.h
//
// Description: Functions that are responsible for Digital-Analog Conversion,
// which forms sinusoid waves to create different sounds
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created May 20, 2016
// Updated August 13, 2016
//
//****************************************************************************

#define DAC_PINS        (*((volatile unsigned long *)0x4000503C))

//**********DAC_Init**********
// Initializes 4-bit DAC on PB3-PB0 and two LEDs on PB5-PB4
// Input: None
// Output: None
void DAC_Init(void);

//**********DAC_Out**********
// Outputs to DAC
// Inputs: 4-bit data
// Outputs: None
void DAC_Out(unsigned short data);

//**********DAC_Shoot**********
// Plays a sound when the ship shoots
// Inputs: None
// Outputs: None
void DAC_Shoot(void);

//**********DAC_Invaderkilled**********
// Plays a sound when a sprite is killed
// Inputs: None
// Outputs: None
void DAC_Invaderkilled(void);

//**********DAC_Explosion**********
// Plays a sound when the ship is destroyed
// Inputs: None
// Outputs: None
void DAC_Explosion(void);

//**********DAC_Fastinvader1**********
// Plays a sound when a sprite moves
// Inputs: None
// Outputs: None
void DAC_Fastinvader1(void);

//**********DAC_Fastinvader1**********
// Plays a sound when a sprite moves
// Inputs: None
// Outputs: None
void DAC_Fastinvader2(void);

//**********DAC_Fastinvader1**********
// Plays a sound when a sprite moves
// Inputs: None
// Outputs: None
void DAC_Fastinvader3(void);

//**********DAC_Fastinvader1**********
// Plays a sound when a sprite moves
// Inputs: None
// Outputs: None
void DAC_Fastinvader4(void);

//**********DAC_Highpitch**********
// Plays a sounds when the mothership arrives
// Inputs: None
// Outputs: None
void DAC_Highpitch(void);

//**********DAC_Smallexplosion**********
// Plays a sound when the mothership is destroyed
// Inputs: None
// Outputs: None
void DAC_Smallexplosion(void);
