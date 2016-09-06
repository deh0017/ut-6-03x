//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 15: SpaceInvaders
//
// File Name: AnalogDigital.c
//
// Description:
//      Functions that are responsible for Analog-Digital Conversion,
// which reads input from the slide pot and convert to numerical result.
//      Functions that are responsible for Digital-Analog Conversion,
// which forms sinusoid waves to create different sounds.
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created May 20, 2016
// Updated September 05, 2016
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
// Busy-wait Analog to digital conversion
// Inputs: None
// Outputs: 12-bit result of ADC conversion
unsigned long ADC0_In(void);

//**********DAC_Init**********
// Initializes 4-bit DAC on PB3-PB0 and two LEDs on PB5-PB4
// Input: None
// Output: None
void DAC_Init(void);

//**********PlaySound_Shoot**********
// Plays a sound when the ship shoots
// Inputs: None
// Outputs: None
void PlaySound_Shoot(void);

//**********PlaySound_Explosion**********
// Plays a sound when the ship is destroyed
// Inputs: None
// Outputs: None
void PlaySound_Explosion(void);

//**********PlaySound_InvaderKilled**********
// Plays a sound when a sprite is killed
// Inputs: None
// Outputs: None
void PlaySound_InvaderKilled(void);

//**********PlaySound_FastInvader1**********
// Plays a sound when a sprite moves
// Inputs: None
// Outputs: None
void PlaySound_FastInvader1(void);

// //**********PlaySound_FastInvader2**********
// // Plays a sound when a sprite moves
// // Inputs: None
// // Outputs: None
// void PlaySound_FastInvader2(void);

// //**********PlaySound_Fastinvader3**********
// // Plays a sound when a sprite moves
// // Inputs: None
// // Outputs: None
// void PlaySound_FastInvader3(void);

// //**********PlaySound_Fastinvader4**********
// // Plays a sound when a sprite moves
// // Inputs: None
// // Outputs: None
// void PlaySound_FastInvader4(void);

//**********PlaySound_HighPitch**********
// Plays a sounds when the mothership arrives
// Inputs: None
// Outputs: None
void PlaySound_HighPitch(void);

//**********PlaySound_SmallExplosion**********
// Plays a sound when the mothership is destroyed
// Inputs: None
// Outputs: None
void PlaySound_SmallExplosion(void);
