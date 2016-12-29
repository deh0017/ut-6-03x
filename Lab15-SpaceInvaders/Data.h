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
// Updated December 28, 2016
//
//****************************************************************************

//**********Required Hardware I/O Connections**********
#define POTPIN					(0x04)  // slide pot connected to PE2
#define LSWITCHPIN				(0x02)  // left switch connected to PE1
#define RSWITCHPIN				(0x01)  // right switch connected to PE0
#define LLEDPIN					(0x20)  // left LED connected to PB5
#define RLEDPIN					(0x10)  // right LED connected to PB4
#define DACPINS					(0x0F)  // DAC connected to PB3-PB0

//**********DAC Data**********
#define GPIO_PORTB_DACOUT_R		(*((volatile unsigned long *)0x4000503C))
#define SHOOTSIZE				((unsigned short)4080)
#define EXPLOSIONSIZE			((unsigned short)2000)
#define INVADERKILLEDSIZE		((unsigned short)3377)
#define FASTINVADER1SIZE		((unsigned short)982)
// #define FASTINVADER2SIZE        ((unsigned short)1042)
// #define FASTINVADER3SIZE        ((unsigned short)1054)
// #define FASTINVADER4SIZE        ((unsigned short)1098)
#define HIGHPITCHSIZE			((unsigned short)1802)
#define SMALLEXPLOSIONSIZE		((unsigned short)1500)


//**********Game Data**********
#define BUNKERW					((unsigned char)Bunker0[18])
#define BUNKERH					((unsigned char)Bunker0[22])
#define ENEMYW					((unsigned char)SmallEnemy20PointA[18])
#define ENEMYH					((unsigned char)SmallEnemy20PointA[22])
#define MOTHERSHIPW				((unsigned char)SmallEnemyBonus0[18])
#define MOTHERSHIPH				((unsigned char)SmallEnemyBonus0[22])
#define LASERW					((unsigned char)Laser0[18])
#define LASERH					((unsigned char)Laser0[22])
#define MISSILEW				((unsigned char)Missile0[18])
#define MISSILEH				((unsigned char)Missile0[22])
#define SHIPW					((unsigned char)PlayerShip0[18])
#define SHIPH					((unsigned char)PlayerShip0[22])
#define MAXENEMY				((unsigned char)5)      // maximum 5 enemies available
#define MAXLASER				((unsigned char)10)     // player can fire 10 lasers
#define MAXMISSILE				((unsigned char)4)      // enemy can fire 4 missiles
#define MOTHERSHIPSCORE			((unsigned short)1000)
#define DELTAEXPLODE			((unsigned char)100)    // time between two explosion frame

struct State {
    const char *			name;
    unsigned char			width;      // bitmap width
    unsigned char			height;     // bitmap height
    unsigned char			x;          // x position
    unsigned char			y;          // y position
    unsigned char			life;       // number of lives with 0 = dead
    unsigned short			score;
    unsigned char			frame;      // current frame in image[]
    const unsigned char *	image[3];   // contains different state frames
    const unsigned char *	explode[2]; // contains "explode" frame
    const unsigned char *	blank;      // contains a blank cover
};
typedef struct State Thing;
extern Thing Ship;
extern Thing Bunker;
extern Thing Enemy[MAXENEMY];       // maximum 5 enemy per row
extern Thing Mothership;
extern Thing Laser[MAXLASER];       // player can shoot maximum 10 laser
extern Thing Missile[MAXMISSILE];   // enemy can shoot maximum 4 missiles
extern unsigned char nLaser;        // current number of lasers
extern unsigned char nMissile;      // current number of missiles
//----------Init_Thing----------
// Initializes ship, bunker, enemy, mothership, missile, and laser
// Inputs: None
// Outputs: None
void Init_Thing(void);


//**********Image Data**********
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

// blank space used to cover an enemy that is moved
extern const unsigned char SmallEnemyMoved[];

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
// (undamaged)
// width = 20 x height = 7
extern const unsigned char SmallEnemyBonus0[];

// small, fast bonus enemy that occasionally appears
// after enough enemies have been killed to make a room for it
// (moderate generic damaged)
// width = 20 x height = 7
extern const unsigned char SmallEnemyBonus1[];

// small, fast bonus enemy that occasionally appears
// after enough enemies have been killed to make a room for it
// (heavy generic damaged)
// width = 20 x height = 7
extern const unsigned char SmallEnemyBonus2[];

// blank space used to cover a mothership that is moved
// after enough enemies have been killed to make a room for it
// width = 20 x height = 7
extern const unsigned char SmallEnemyBonusMoved[];

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
// (first frame)
// width = 16 x height = 10
extern const unsigned char SmallExplosion0[];

// blank space following the small explosion for the demise of
// a sprite or a mothership
// (second frame)
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


//**********Sound**********
// shoot.wav converted to array
extern const unsigned char shoot[SHOOTSIZE];

// explosion.wav converted to array
extern const unsigned char explosion[EXPLOSIONSIZE];

// invaderkilled.wav converted to array
extern const unsigned char invaderkilled[INVADERKILLEDSIZE];

// fastinvader1.wav converted to array
extern const unsigned char fastinvader1[FASTINVADER1SIZE];

// // fastinvader2.wav converted to array
// extern const unsigned char fastinvader2[FASTINVADER2SIZE];

// // fastinvader3.wav converted to array
// extern const unsigned char fastinvader3[FASTINVADER3SIZE];

// // fastinvader4.wav converted to array
// extern const unsigned char fastinvader4[FASTINVADER4SIZE];

// ufo_highpitch.wav converted to array
extern const unsigned char highpitch[HIGHPITCHSIZE];

// smallexplosion.wav converted to array
extern const unsigned char smallexplosion[SMALLEXPLOSIONSIZE];
