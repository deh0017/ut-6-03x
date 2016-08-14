//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 15: SpaceInvaders
//
// File Name: Images.h
//
// Description: Contain all images data and their sizes
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created May 20, 2016
// Updated August 13, 2016
//
//****************************************************************************

//**********Capture image dimensions out of BMP**********
// bunker
#define BUNKERW         ((unsigned char)Bunker0[18])
#define BUNKERH         ((unsigned char)Bunker0[22])
// enemy
#define ENEMY30W        ((unsigned char)SmallEnemy30PointA[18])
#define ENEMY30H        ((unsigned char)SmallEnemy30PointA[22])
#define ENEMY20W        ((unsigned char)SmallEnemy20PointA[18])
#define ENEMY20H        ((unsigned char)SmallEnemy20PointA[22])
#define ENEMY10W        ((unsigned char)SmallEnemy10PointA[18])
#define ENEMY10H        ((unsigned char)SmallEnemy10PointA[22])
// mothership
#define ENEMYBONUSW     ((unsigned char)SmallEnemyBonus0[18])
#define ENEMYBONUSH     ((unsigned char)SmallEnemyBonus0[22])
// laser
#define LASERW          ((unsigned char)Laser0[18])
#define LASERH          ((unsigned char)Laser0[22])
// missile
#define MISSILEW        ((unsigned char)Missile0[18])
#define MISSILEH        ((unsigned char)Missile0[22])
// player's ship
#define PLAYERW         ((unsigned char)PlayerShip0[18])
#define PLAYERH         ((unsigned char)PlayerShip0[22])

//**********Images**********
// enemy ship that arm and mouths are closed
// width = 16 x height = 10
extern const unsigned char SmallEnemy30PointA[];

// enemy ship that arms and mouth are open
// width = 16 x height = 10
extern const unsigned char SmallEnemy30PointB[];

// enemy ship that arms are together
// width = 16 x height = 10
extern const unsigned char SmallEnemy20PointA[];

// enemy ship that arms are apart
// width = 16 x height = 10
extern const unsigned char SmallEnemy20PointB[];

// enemy ship that arms are down
// width = 16 x height = 10
extern const unsigned char SmallEnemy10PointA[];

// enemy ship that arms are up
// width = 16 x height = 10
extern const unsigned char SmallEnemy10PointB[];

// the player's ship (undamaged)
// width = 18 x height = 8
extern const unsigned char PlayerShip0[];

// the player's ship (moderate generic damage)
// width = 18 x height = 8
extern const unsigned char PlayerShip1[];

// the player's ship (heavy generic damage)
// width = 18 x height = 8
extern const unsigned char PlayerShip2[];

// blank space used to cover a ship that is moved
// width = 18 x height = 8
extern const unsigned char PlayerShip3[];

// small, fast bonus enemy that occasionally appears
// after enough enemies have been killed to make a room for it
// width = 20 x height = 7
extern const unsigned char SmallEnemyBonus0[];

// small shield floating in space to cover the player's (undamaged)
// width = 18 x height = 5
extern const unsigned char Bunker0[];

// small shield floating in space to cover the player's ship
// (moderate generic damage)
// width = 18 x height = 5
extern const unsigned char Bunker1[];

// small shield floating in space to cover the player's ship
// (heavy generic damage)
// width = 18 x height = 5
extern const unsigned char Bunker2[];

// blank space used to cover a bunker that is destroyed
// width = 18 x height = 5
extern const unsigned char Bunker3[];

// large explosion that can be used upon the demise of the player's ship
// (first frame)
// width = 18 x height = 8
extern const unsigned char BigExplosion0[];

// large explosion that can be used upon the demise of the player's ship
// (second frame)
// width = 18 x height = 8
extern const unsigned char BigExplosion1[];

// small explosion best used for the demise of a sprite or a mothership
// width = 16 x height = 10
extern const unsigned char SmallExplosion0[];

// blank space following the small explosion for the demise of
// a sprite or a mothership
// width = 16 x height = 10
extern const unsigned char SmallExplosion1[];

// a missile in flight (left skewed)
// width = 4 x height = 9
extern const unsigned char Missile0[];

// a missile in flight (right skewed)
// width = 4 x height = 9
extern const unsigned char Missile1[];

// blank space to cover a missile after it hits something
// width = 4 x height = 9
extern const unsigned char Missile2[];

// a laser burst in flight
// width = 2 x height = 9
extern const unsigned char Laser0[];

// blank space to cover a laser after it hits something
// width = 2 x height = 9
extern const unsigned char Laser1[];
