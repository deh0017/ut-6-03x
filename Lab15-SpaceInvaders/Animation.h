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
// Updated August 28, 2016
//
//****************************************************************************

// a data structure for Ship, Bunker, and Enemy
// and a queue for Missle and Laser
struct State {
    unsigned char x;        // x coordinate
    unsigned char y;        // y coordinate
    // show different images based on the object's state
    const unsigned char* image[4];
    unsigned char life;     // zero = dead, non-zero = alive
};
typedef struct State Thing;
extern Thing Ship;
extern Thing Bunker;
extern Thing Enemy[];
extern Thing Missile[100];
extern Thing Laser[100];
extern unsigned char MissileFront;
extern unsigned char MissileRear;
extern unsigned char LaserFront;
extern unsigned char LaserRear;
extern const unsigned char ENEMYNUMBER;     // the number of sprites
// current frame of the ship
extern unsigned char ShipFrame;     // 0 = undamaged
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

//**********Ship_Draw**********
// Draws the image of the player's ship on the buffer
// Inputs: None
// Outputs: None
void Ship_Draw(void);

//**********Bunker_Init**********
// Initializes coordinate, frames, and current frame of the bunker
// Write the .bmp file into Nokia5110 buffer
// Inputs: None
// Outputs: None
void Bunker_Init(void);

//**********Bunker_Draw**********
// Draws the image of the bunker on the buffer
// Inputs: None
// Outputs: None
void Bunker_Draw(void);

//**********Enemy_Draw**********
// Draws the image of a specific enemy on the buffer
// Inputs: None
// Outputs: None
void Enemy_Draw(unsigned char i);

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

//**********Missile_Move**********
// Move missiles 1 pixel at a time
// Inputs: None
// Outputs: None
void Missile_Move(void);

//**********Laser_Init**********
// Initializes y-coordinate and life of the laser
// Inputs: None
// Outputs: None
void Laser_Init(void);

//**********Laser_Move**********
// Move lasers 1 pixel at a time
// Inputs: None
// Outputs: None
void Laser_Move(void);

//**********Missiles_And_Lasers**********
// Process the animation of missiles and lasers on the field
// Also process the all the collisions on the field
// Inputs: None
// Outputs: None
void Missiles_And_Lasers(void);
