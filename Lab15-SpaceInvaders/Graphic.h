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
// Updated September 05, 2016
//
//****************************************************************************

typedef struct State Thing;

//**********Check_Collision**********
// Detects whether two Things overlap
// Inputs: Thing1   Thing 1
//         Thing2   Thing 2
// Outputs: 0       False
//          1       True
unsigned char Check_Collision(Thing Thing1, Thing Thing2);

//**********Draw**********
// Draws a Thing on the buffer
// Inputs: aThing    a Thing, such as Ship, Bunker, Enemy, Mothership, and Laser
// Outputs: None
void Draw(Thing aThing);

//**********Draw_AllEnemies**********
// Draws all enemies on the screen
// Inputs: None
// Outputs: None
void Draw_AllEnemies(void);

//**********Move**********
// Changes the coordinate and move a movable Thing on the buffer
// Inputs: *aMovableThing   a movable Thing, such as Ship, Enemy, Mothership,
//                                                   Missile, and Laser
//         toX              x-coordinate that Thing is moving to
//         toY              y-coordinate that Thing is moving to
// Outputs: None
void Move(Thing *aMovableThing, unsigned char toX, unsigned char toY);

//**********Create_Laser**********
// Creates a laser when player fires, as long as under MAXLASER
// Inputs: None
// Outputs: None
void Create_Laser(void);

//**********Create_Missile**********
// Creates a missile when an enemy fires, as long as under MAXMISSILE
// Inputs: None
// Outputs: None
void Create_Missile(void);

//**********Move_Lasers**********
// Moves all lasers 1 pixel downward at a time
// Inputs: None
// Outputs: None
void Move_Lasers(void);

//**********Move_Missiles**********
// Moves all missiles 1 pixel upward at a time
// Inputs: None
// Outputs: None
void Move_Missiles(void);
