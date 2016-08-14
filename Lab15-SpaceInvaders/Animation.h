//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 15: SpaceInvaders
//
// File Name: Animation.h
//
// Description: Graphic Processor
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created May 20, 2016
// Updated August 13, 2016
//
//****************************************************************************

// A data structure for multi-frame things,
// such as Ship, Bunker, and Enemy
struct State {
  unsigned char x;      // x coordinate
  unsigned char y;      // y coordinate
  // show different images based on the object's state
  const unsigned char* image[4];
  unsigned char life;   // zero = dead, non-zero = alive
};
typedef struct State Thing;
extern Thing Ship;
extern Thing Bunker;
extern Thing Enemy[];

// A data structure for single-frame things,
// such as Missile and Laser
struct MiniState {
	unsigned char y;    // y coordinate
	unsigned char life; // zero = dead, non-zero = alive
};
typedef struct MiniState Projectile;
extern Projectile Missile[84];
extern Projectile Laser[84];

extern const unsigned char ENEMYNUMBER; // the number of sprites
// current frame of the ship
extern unsigned char ShipFrame; // 0 = undamaged
                                // 1 = moderate generic damaged
                                // 2 = heavy generic damaged
                                // 3 = destroyed
// current frame of the bunker
extern unsigned char BunkerFrame;   // 0 = undamaged
                                    // 1 = moderate generic damaged
                                    // 2 = heavy generic damaged
                                    // 3 = destroyed

//**********Ship_Init**********
// Initializes coordinate, frames, and current frame of the ship
// Write the .bmp file into Nokia5110 buffer
// Inputs: None
// Ouputs: None
void Ship_Init(void);

//**********Bunker_Init**********
// Initializes coordinate, frames, and current frame of the bunker
// Write the .bmp file into Nokia5110 buffer
// Inputs: None
// Outputs: None
void Bunker_Init(void);

//**********Enemy_Init**********
// Initializes coordinate, frames, and current frame of the enemy
// Write each .bmp file into Nokia5110 buffer
// Inputs: None
// Outputs: None
void Enemy_Init(void);

//**********Missile_Init**********
// Initializes y-coordinate and life of the missile
// Inputs: None
// Outputs: None
void Missile_Init(void);

//**********Laser_Init**********
// Initializes y-coordinate and life of the laser
// Inputs: None
// Outputs: None
void Laser_Init(void);

//**********Missiles_And_Lasers**********
// Process the animation of missiles and lasers on the field
// Also process the all the collisions on the field
// Inputs: None
// Outputs: None
void Missiles_And_Lasers(void);
