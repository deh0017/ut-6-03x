/*****************************************************************************

UTAustinX: UT.6.03x Embedded Systems - Shape the World
Lab 15: SpaceInvaders

File Name: Animation.c

Description: Graphic Processor

Compatibility: EK-TM4C123GXL

Phi Luu
Portland, Oregon, United States
Created May 20, 2016
Updated July 17, 2016

*****************************************************************************/

#include "Animation.h"
#include "Random.h"
#include "Images.h"
#include "Nokia5110.h"
#include "Tick.h"

const unsigned char* EnemyFrameA[3] = {
    SmallEnemy30PointA,     // 30 points, frame A
    SmallEnemy20PointA,     // 20 points, frame A
    SmallEnemy10PointA      // 10 points, frame A
};
const unsigned char* EnemyFrameB[3] = {
    SmallEnemy30PointB,     // 30 points, frame B
    SmallEnemy20PointB,     // 20 points, frame B
    SmallEnemy10PointB      // 10 points, frame B
};
const unsigned char* MissileStyle[2] = {
    Missile0,               // left
    Missile1,               // right
};

const unsigned char ENEMYNUMBER = 5;    // the number of sprites

// current frame of the ship
unsigned char ShipFrame;    // 0 = undamaged
                            // 1 = moderate generic damaged
                            // 2 = heavy generic damaged
                            // 3 = destroyed
// current frame of the bunker
unsigned char BunkerFrame;  // 0 = undamaged
                            // 1 = moderate generic damaged
                            // 2 = heavy generic damaged
                            // 3 = destroyed
// current frame of the enemy
unsigned char EnemyFrame;   // 0 = frame A
                            // 1 = frame B

//**********Ship_Init**********
// Initializes coordinate, frames, and current frame of the ship
// Write the .bmp file into Nokia5110 buffer
// Inputs: None
// Ouputs: None
void Ship_Init(void) {
    Ship.x = (SCREENW - PLAYERW)/2;
    Ship.y = (SCREENH - 1);
    Ship.image[0] = PlayerShip0;    // undamaged
    Ship.image[1] = PlayerShip1;    // moderate generic damaged
    Ship.image[2] = PlayerShip2;    // heavy generic damaged
    Ship.image[3] = BigExplosion0;  // destroyed
    Ship.life = 3;
    ShipFrame = 0;
    // output the image of the player's ship on the buffer
    Nokia5110_PrintBMP(Ship.x, Ship.y, Ship.image[ShipFrame], 0);
}

//**********Bunker_Init**********
// Initializes coordinate, frames, and current frame of the bunker
// Write the .bmp file into Nokia5110 buffer
// Inputs: None
// Outputs: None
void Bunker_Init(void) {
    Bunker.x = (SCREENW - PLAYERW)/2;
    Bunker.y = SCREENH - PLAYERH;
    Bunker.image[0] = Bunker0;      // undamaged
    Bunker.image[1] = Bunker1;      // moderate generic damaged
    Bunker.image[2] = Bunker2;      // heavy generic damaged
    Bunker.image[3] = Bunker3;      // destroyed
    Bunker.life = 3;
    BunkerFrame = 0;
    // output the image of the bunker on the buffer
    Nokia5110_PrintBMP(Bunker.x, Bunker.y, Bunker.image[BunkerFrame], 0);
}

//**********Enemy_Init**********
// Initializes coordinate, frames, and current frame of the enemy
// Write each .bmp file into Nokia5110 buffer
// Inputs: None
// Outputs: None
void Enemy_Init(void) {
    unsigned char i;
    EnemyFrame = 0;
    for (i = 0; i < ENEMYNUMBER; i++) {
        Enemy[i].x = ENEMY20W*i;
        Enemy[i].y = ENEMY20H - 1;
        Enemy[i].image[0] = EnemyFrameA[i%3];
        Enemy[i].image[1] = EnemyFrameB[i%3];
        Enemy[i].life = 1;
        // output the image of the i-th enemy on the buffer
        Nokia5110_PrintBMP(Enemy[i].x, Enemy[i].y,
                           Enemy[i].image[EnemyFrame], 0);
    }
}

//**********Missile_Init**********
// Initializes y-coordinate and life of the missile
// Inputs: None
// Outputs: None
void Missile_Init(void) {
    unsigned char i;
    for (i = 0; i < SCREENW; i++) {
        Missile[i].y = 0;
        Missile[i].life = 0;
    }
}

//**********Laser_Init**********
// Initializes y-coordinate and life of the laser
// Inputs: None
// Outputs: None
void Laser_Init(void) {
    unsigned char i;
    for (i = 0; i < SCREENW; i++) {
        Laser[i].y = 0;
        Laser[i].life = 0;
    }
}

//**********Missiles_And_Lasers**********
// Process the animation of missiles and lasers
// Also process the all the collisions
// Inputs: None
// Outputs: None
void Missiles_And_Lasers(void) {
    unsigned char i, j;
    for (i = 0; i < SCREENW; i++) {
        // process the missile-laser collision
        for (j = i - 1; j < i + 4; j++) {
            if (Missile[i].life && Laser[j].life
                    && Missile[i].y - Laser[j].y <= 5) {
                // set the life to be non-existent
                Missile[i].life = 0;
                Laser[j].life = 0;
                // erase the pair out of the field
                Nokia5110_PrintBMP(i, Missile[i].y, Missile2, 0);
                Nokia5110_PrintBMP(j, Laser[j].y, Laser1, 0);
                Nokia5110_DisplayBuffer();
                break;
            }
        }
        // move the missile
        if (Missile[i].life) {
            // move 1 pixel at a time
            Nokia5110_PrintBMP(i, Missile[i].y + 1, Missile2, 0);
            Nokia5110_PrintBMP(i, Missile[i].y, MissileStyle[Random()%2], 0);
            Nokia5110_DisplayBuffer();
            // process missile-enemy collision

            // process missile-bunker collision
            if (Bunker.life && i >= 30 && i <= 51) {
                // reduce the bunker's life
                BunkerFrame++;
                Bunker.life--;
                // set the missile to be non-existent
                Missile[i].life = 0;
                // change the bunker's image and erase the missile
                Nokia5110_PrintBMP(i, Missile[i].y, Missile2, 0);
                Nokia5110_PrintBMP(Bunker.x, Bunker.y,
                                   Bunker.image[BunkerFrame], 0);
                Nokia5110_DisplayBuffer();
            }
            // update the y-coordinate of the missile
            Missile[i].y--;

            // delay makes the game easier
            Delay(50);
        }
        // move the laser
        if (Laser[i].life) {
            // move 1 pixel at a time
            Nokia5110_PrintBMP(i, Laser[i].y - 1, Laser1, 0);
            Nokia5110_PrintBMP(i, Laser[i].y, Laser0, 0);
            Nokia5110_DisplayBuffer();
            // process laser-ship collision
            if (Ship.life && Ship.y - Laser[i].y <= 6
                    && i >= Ship.x - 1 && i <= Ship.x + 18) {
                // reduce the ship's life
                ShipFrame++;
                Ship.life--;
                // set the laser to be non-existent
                Laser[i].life = 0;
                // change the ship's frame and erase the laser
                Nokia5110_PrintBMP(i, Laser[i].y, Laser1, 0);
                Nokia5110_PrintBMP(Ship.x, Ship.y, Ship.image[ShipFrame], 0);
                Nokia5110_DisplayBuffer();
                // after the ship is destroyed
                if (!Ship.life) {
                    // display the second frame of the explosion
                    Delay(250);
                    Nokia5110_PrintBMP(Ship.x, Ship.y, BigExplosion1, 0);
                    Nokia5110_DisplayBuffer();
                    Delay(500);
                }
            }
            // process laser-bunker collision
            if (Bunker.life && Bunker.y - Laser[i].y <= 3
                    && i >= 30 && i <= 51) {
                // reduce the bunker's life
                BunkerFrame++;
                Bunker.life--;
                // set the laser to be non-existent
                Laser[i].life = 0;
                // change the bunker's frame and erase the laser
                Nokia5110_PrintBMP(i, Laser[i].y, Laser1, 0);
                Nokia5110_PrintBMP(Bunker.x, Bunker.y,
                                   Bunker.image[BunkerFrame], 0);
                Nokia5110_DisplayBuffer();
            }
            // update the y-coordinate of the laser
            Laser[i].y++;

            // delay makes the game easier
            Delay(50);
        }
    }
}
