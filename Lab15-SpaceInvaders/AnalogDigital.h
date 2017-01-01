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
// Updated September 31, 2016
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
// Busy-wait Analog to digital conversion
//
// @return      12-bit result of ADC conversion
//***
unsigned long ADC0_In(void);

//***
// Initialize 4-bit DAC on PB3-PB0 and two LEDs on PB5-PB4
//***
void DAC_Init(void);

//***
// Output to DAC
//***
void DAC_Out(unsigned short data);

//***
// Play a sound when the ship shoots
//***
void PlaySound_Shoot(void);

//***
// Play a sound when the ship is destroyed
//***
void PlaySound_Explosion(void);

//***
// Play a sound when a sprite is killed
//***
void PlaySound_InvaderKilled(void);

//***
// Play a sound when a sprite moves
//***
void PlaySound_FastInvader1(void);

// //***
// // Play a sound when a sprite moves
// //***
// void PlaySound_FastInvader2(void);
//
// //***
// // Play a sound when a sprite moves
// //***
// void PlaySound_FastInvader3(void);
//
// //***
// // Play a sound when a sprite moves
// //***
// void PlaySound_FastInvader4(void);

//***
// Play a sounds when the mothership arrives
//***
void PlaySound_HighPitch(void);

//***
// Play a sound when the mothership is destroyed
//***
void PlaySound_SmallExplosion(void);
