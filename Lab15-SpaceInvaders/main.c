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
// Updated August 13, 2016
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
#include "Random.h"
#include "TExaS.h"
#include "Animation.h"
#include "Nokia5110.h"
#include "Images.h"
#include "Tick.h"
#include "ADC.h"
#include "DAC.h"
#include "tm4c123gh6pm.h"

//**********2. Global Declarations Section**********
// Function Prototypes
void DisableInterrupts(void);
void EnableInterrupts(void);
void SysTick_Handler(void);
void Timer2A_Handler(void);
void Delay(unsigned long ms);

// Global Variables
// bunker
Thing Bunker;
// ship
Thing Ship;
Projectile Missile[84];
unsigned short ADCData;
unsigned char OldShipX;
unsigned char MissileX;
unsigned char MissileY;
// enemy
Thing Enemy[5];
Projectile Laser[84];
unsigned char LaserX;
unsigned char LaserY;
unsigned char EnemyFireChance;
unsigned char EnemyIndex;
// player data
unsigned short score = 0;

//**********3. Subroutines Section**********
//----------NoEnemy----------
// Checks whether all enemies are eliminated
// Inputs: None
// Outputs: 0   if there is enemy
//          1   if there is no enemy
unsigned char NoEnemy(void) {
    unsigned char i;
    for (i = 0; i < ENEMYNUMBER; i++) {
        if (Enemy[i].life == 1) {
            return 0;
        }
    }
    return 1;
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
    // store old ship x-position
    OldShipX = Ship.x;
    // get new ship x-position
    Ship.x = ADCData*(SCREENW - PLAYERW)/4095;
    // if the ship moves
    if (Ship.x != OldShipX) {
        // move the ship on the screen
        Nokia5110_PrintBMP(OldShipX, Ship.y, PlayerShip3, 0);
        Nokia5110_PrintBMP(Ship.x, Ship.y, Ship.image[ShipFrame], 0);
        Nokia5110_DisplayBuffer();
    }
    // if the ship fires
    if (GPIO_PORTE_DATA_R & 0x02) {
        // get the coordinate
        MissileX = Ship.x + PLAYERW/2 - 1;
        MissileY = SCREENH - PLAYERH - 1;
        // if there is no missile on that column
        if (!Missile[MissileX].life) {
            // store the missile coordinate
            Missile[MissileX].y = MissileY;
            Missile[MissileX].life = 1;
            // initiate the missile on the screen
            if (Random()%2 == 0) {
                Nokia5110_PrintBMP(MissileX, MissileY, Missile0, 0);
            }
            else {
                Nokia5110_PrintBMP(MissileX, MissileY, Missile1, 0);
            }
            Nokia5110_DisplayBuffer();
        }
    }
    // randomize the fire chance of the enemies
    EnemyFireChance = Random()%500;
    // randomize which one of them will fire
    EnemyIndex = Random()%ENEMYNUMBER;
    // probability 3/500
    if (EnemyFireChance < 3) {
        // get the laser coordinate
        LaserX = Enemy[EnemyIndex].x + ENEMY20W/2 - 1;
        LaserY = Enemy[EnemyIndex].y + LASERH - 1;
        // if there is no laser on that column
        if (!Laser[LaserX].life) {
            // store the laser coordinate
            Laser[LaserX].y = LaserY;
            Laser[LaserX].life = 1;
            // initiate the laser on the screen
            Nokia5110_PrintBMP(LaserX, LaserY, Laser0, 0);
            Nokia5110_DisplayBuffer();
        }
    }
}

//----------Timer2A_Handler----------
// Timer2A interrupt service routine
// 11-kHz interrupt, handling sound output
// Inputs: None
// Ouputs: None
// Assumes: 80-MHz clock
void Timer2A_Handler(void) {
    // if the ship fires
    if (GPIO_PORTE_DATA_R & 0x02) {
        // play the shooting sound
        DAC_Shoot();
    }
    // if the bunker is destroyed

    // if one sprite is dead

    // if the ship explodes
    if (!Ship.life) {
        // play the explosive sound
        DAC_Explosion();
    }
    // acknowledge timer2A timeout
    TIMER2_ICR_R = 0x00000001;
}

//**********4. Main Function**********
int main(void) {
    // Set up:
    // set system clock to 80 MHz
    TExaS_Init(SSI0_Real_Nokia5110_Scope);
    Random_Init(1);	// enable randomization
    ADC0_Init();    // init PE2 for ADC
    DAC_Init();     // init PB3-PB0 for 4-bit DAC
    // init Nokia 5110 screen
    Nokia5110_Init();
    Nokia5110_ClearBuffer();
    Nokia5110_DisplayBuffer();
    Ship_Init();    // init position, state, and life of the ship
    Bunker_Init();  // init position, state, and life of the bunker
    Enemy_Init();   // init position, state, and life of enemies
    Missile_Init(); // default values for missiles
    Laser_Init();   // default values for lasers
    Nokia5110_DisplayBuffer();  // initialize the startup screen
    SysTick_Init(); // set up SysTick interrupt
    Timer2_Init();  // set up Timer2A interrupt
    EnableInterrupts();         // interrupts begin to tick

    // Loop:
    while (1) {
        // if the game is over
        if (!Ship.life || NoEnemy()) {
            // disable all interrupts
            DisableInterrupts();
            // display the game-over message and the score on the screen
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
            Nokia5110_OutUDec(score);
            Nokia5110_SetCursor(0, 0);
            // terminate the program
            break;
        }
        // move all missiles and lasers 1 pixel at a time
        Missiles_And_Lasers();
    }
}
