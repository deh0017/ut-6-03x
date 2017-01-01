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
// Updated September 31, 2016
//
//****************************************************************************

typedef struct State Thing;

//***
// Detect whether two Things overlap
//
// @param   Thing1      a Thing
//          Thing2      another Thing
//
// @return              0 if not collide
// @return              1 if collide
//***
unsigned char Check_Collision(Thing Thing1, Thing Thing2);

//***
// Draw a Thing on the buffer
//
// @param   aThing   a Thing, such as Ship, Bunker, Enemy, Mothership, and Laser
//***
void Draw(Thing aThing);

//***
// Draw all enemies on the screen
//***
void Draw_AllEnemies(void);

//***
// Change the coordinate and move a movable Thing on the buffer
//
// @param   *aMovableThing   a movable Thing, such as Ship, Enemy, Mothership,
//                             Missile, and Laser
// @param   toX              x-coordinate that Thing is moving to
// @param   toY              y-coordinate that Thing is moving to
//***
void Move(Thing *aMovableThing, unsigned char toX, unsigned char toY);

//***
// Destroy a Thing and plays an explosion sequence if available
//
// @param   *aThing   a Thing, such as Ship, Enemy, Mothership, Missile, and Laser
//***
void Destroy(Thing *aThing);

//***
// Damage a Thing and destroys it if neccessary
//
// @param   *aThing   a Thing, such as Ship, Bunker, Enemy, Mothership,
//                     Missile, and Laser
//***
void Damage(Thing *aThing);

//***
// Create a laser when player fires, as long as under MAXLASER
//***
void Create_Laser(void);

//***
// Create a missile when an enemy fires, as long as under MAXMISSILE
//***
void Create_Missile(void);

//***
// Check the collision between a laser other things
//
// @param   *aLaser   a laser in Laser[]
//
// @return            0 if not hit
// @return            1 if hit
//***
unsigned char Laser_Is_Hit(Thing *aLaser);

//***
// Check the collision between a missile other things
//
// @param   *aMissile   a missile in Missile[]
//
// @return              0 if not hit
// @return              1 if hit
//***
unsigned char Missile_Is_Hit(Thing *aMissile);

//***
// Move all lasers 1 pixel downward at a time
//***
void Move_Lasers(void);

//***
// Move all missiles 1 pixel upward at a time
//***
void Move_Missiles(void);
