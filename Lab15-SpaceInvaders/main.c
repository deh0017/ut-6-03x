//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 15: SpaceInvaders
//
// File Name: main.c
//
// Description: Mandatory file which contains interrupts routines and
// the main thread
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created May 20, 2016
// Updated September 05, 2016
//
//****************************************************************************

//**********Required Hardware I/O Connections**********
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V
// Fire switch connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)

//**********1. Pre-processor Section**********
#include "AnalogDigital.h"
#include "Data.h"
#include "Graphic.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "Timer.h"
#include "tm4c123gh6pm.h"

//**********2. Global Declarations Section**********
// Function Prototypes
void EnableInterrupts(void);
void DisableInterrupts(void);
void Delay(unsigned long ms);
unsigned char Enemy_Exist(void);
void SysTick_Handler(void);
void Timer2A_Handler(void);

// Global Variables
unsigned short ADCData;
unsigned char NewShipX;
unsigned char prevSwitchState = 0;
unsigned char EnemyFire;

//**********3. Subroutines Section**********
//----------NoEnemy----------
// Checks if all enemies are eliminated
// Inputs: None
// Outputs: 0   if there is no enemy
//          1   if there is enemy
unsigned char Enemy_Exist(void) {
    unsigned char i;
    if (Mothership.life) {
        return 1;
    }
    for (i = 0; i < MAXENEMY; i++) {
        if (Enemy[i].life) {
            return 1;
        }
    }
    return 0;
}

//----------Display_GameOver----------
// Displays the closure of the game
// Inputs: None
// Outputs: None
void Display_GameOver(void) {
    Nokia5110_Clear();
    Nokia5110_SetCursor(1, 1);
    Nokia5110_OutString("GAME OVER");
    Nokia5110_SetCursor(1, 2);
    Nokia5110_OutString("Nice try,");
    Nokia5110_SetCursor(1, 3);
    Nokia5110_OutString("Earthling!");
    Nokia5110_SetCursor(3, 4);
    Nokia5110_OutString("Score:");
    Nokia5110_SetCursor(1, 5);
    Nokia5110_OutUDec(Ship.score);
    Nokia5110_SetCursor(0, 0);
}

//----------SysTick_Handler----------
// SysTick interrupt service routine
// 30-Hz interrupt, handling input and graphic data
// Inputs: None
// Outputs: None
// Assumes: 80-MHz clock
void SysTick_Handler(void) {
    // read raw input from the slide pot
    ADCData = ADC0_In();
    // get new ship x-position (limit from 0 - 66)
    NewShipX = 66 - ADCData*(SCREENW - SHIPW)/4095;
    // move the ship if there is a change in x-coordinate
    if (NewShipX != Ship.x) {
        Move(&Ship, NewShipX, Ship.y);
        Nokia5110_DisplayBuffer();
    }
    // create a missile and flash a LED if the ship fires
    if ((GPIO_PORTE_DATA_R & LSWITCHPIN) != prevSwitchState
            && (GPIO_PORTE_DATA_R & LSWITCHPIN)) {
        Create_Laser();
        Nokia5110_DisplayBuffer();
        GPIO_PORTB_DATA_R ^= LLEDPIN;
    }
    prevSwitchState = GPIO_PORTE_DATA_R & LSWITCHPIN;
    // create a laser and flash a LED if an enemy fires
    if (Random()%500 < 3) {
        EnemyFire = 1;
        Create_Missile();
        Nokia5110_DisplayBuffer();
        GPIO_PORTB_DATA_R ^= RLEDPIN;
    }
    else {
        EnemyFire = 0;
    }
}

//----------Timer2A_Handler----------
// Timer2A interrupt service routine
// 11-kHz interrupt, handling sound output
// Inputs: None
// Ouputs: None
// Assumes: 80-MHz clock
void Timer2A_Handler(void) {
    // ship fires sound
    if (GPIO_PORTE_DATA_R & 0x02) {
        PlaySound_Shoot();
    }
    // enemy fires sound
    if (EnemyFire) {
    }
    // ship destroyed sound
    if (!Ship.life) {
        PlaySound_Explosion();
    }
    // bunker destroyed sound

    // enemy destroyed sound

    // mothership destroyed sound

    // acknowledge timer2A timeout
    TIMER2_ICR_R = 0x00000001;
}

//**********4. Main Function**********
int main(void) {
    // Set up:
    TExaS_Init(NoLCD_NoScope);  // set system clock to 80 MHz
    Random_Init(1);	            // enable randomization
    ADC0_Init();                // PE2 ADC input, PE1-PE0 switch input
    DAC_Init();                 // PB5-PB4 LED output, PB3-PB0 DAC output
    Nokia5110_Init();           // Nokia5110 screen
    Nokia5110_ClearBuffer();
    Init_Thing();
    Draw(Ship);
    Draw(Bunker);
    Draw_AllEnemies();
    Nokia5110_DisplayBuffer();
    Init_SysTick();             // set up SysTick interrupt
    Init_Timer2();              // set up Timer2A interrupt
    EnableInterrupts();         // interrupts begin to tick
    // Loops:
    // As long as the ship and the enemy are live, the game still runs
    while (Ship.life && Enemy_Exist()) {
        Move_Lasers();
        Move_Missiles();
    }
    // Otherwise, game over
    DisableInterrupts();
    Display_GameOver();
}
