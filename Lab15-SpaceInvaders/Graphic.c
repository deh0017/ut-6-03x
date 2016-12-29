//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 15: SpaceInvaders
//
// File Name: Animation.c
//
// Description: Graphic Processor
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created May 20, 2016
// Updated December 28, 2016
//
//****************************************************************************

#include <string.h>
#include "Data.h"
#include "Graphic.h"
#include "Nokia5110.h"
#include "Random.h"
#include "Timer.h"

//**********Check_Collision**********
// Detects whether two Things overlap
// Inputs: Thing1   Thing 1
//         Thing2   Thing 2
// Outputs: 0       False
//          1       True
unsigned char Check_Collision(Thing Thing1, Thing Thing2) {
    if (Thing1.x < Thing2.x + Thing2.width
        && Thing1.x + Thing1.width > Thing2.x
        && Thing1.y > Thing2.y - Thing2.height
        && Thing1.y - Thing1.height < Thing2.y) {
        return 1;
    }
    return 0;
}

//**********Draw**********
// Draws a Thing on the buffer
// Inputs: aThing    a Thing, such as Ship, Bunker, Enemy, Mothership, and Laser
// Outputs: None
void Draw(Thing aThing) {
    Nokia5110_PrintBMP(aThing.x, aThing.y, aThing.image[aThing.frame], 0);
}

//**********Draw_AllEnemies**********
// Draws all enemies on the screen
// Inputs: None
// Outputs: None
void Draw_AllEnemies(void) {
    unsigned char i;

    for (i = 0; i < MAXENEMY; i++) {
        Draw(Enemy[i]);
    }
}

//**********Move**********
// Changes the coordinate and move a movable Thing on the buffer
// Inputs: *aMovableThing   a movable Thing, such as Ship, Enemy, Mothership,
//                                                   Missile, and Laser
//         toX              x-coordinate that Thing is moving to
//         toY              y-coordinate that Thing is moving to
// Outputs: None
void Move(Thing *aMovableThing, unsigned char toX, unsigned char toY) {
    Nokia5110_PrintBMP(aMovableThing->x, aMovableThing->y,
                       aMovableThing->blank, 0);
    aMovableThing->x = toX;
    aMovableThing->y = toY;
    Draw(*aMovableThing);
}

//**********Destroy**********
// Destroys a Thing and plays an explosion sequence if available
// Inputs: *aThing    a Thing, such as Ship, Enemy, Mothership, Missile,
//                                and Laser
// Outputs: None
void Destroy(Thing *aThing) {
    aThing->life = 0;
    // multi-frame explosion
    if (strncmp(aThing->name, "ship", 4) == 0
        || strncmp(aThing->name, "enemy", 5) == 0
        || strncmp(aThing->name, "mothership", 10) == 0) {
        Nokia5110_PrintBMP(aThing->x, aThing->y, aThing->explode[0], 0);
        Nokia5110_DisplayBuffer();
        Delay(DELTAEXPLODE);
        Nokia5110_PrintBMP(aThing->x, aThing->y, aThing->explode[1], 0);
        Nokia5110_DisplayBuffer();
    }
    // single-frame explosion
    else if (strncmp(aThing->name, "laser", 5) == 0) {
        nLaser--;
        Nokia5110_PrintBMP(aThing->x, aThing->y, Laser1, 0);
        Nokia5110_DisplayBuffer();
    } else {
        nMissile--;
        Nokia5110_PrintBMP(aThing->x, aThing->y, Missile2, 0);
        Nokia5110_DisplayBuffer();
    }
}

//**********Damage**********
// Damages a Thing and destroys it if neccessary
// Inputs: *aThing    a Thing, such as Ship, Bunker, Enemy, Mothership,
//                                     Missile, and Laser
// Outputs: None
void Damage(Thing *aThing) {
    // destroy if there is only 1 life left
    if (aThing->life == 1) {
        Destroy(&*aThing);
    }
    // otherwise, change to the worse state of being
    else {
        aThing->life--;
        aThing->frame++;
        Draw(*aThing);
        Nokia5110_DisplayBuffer();
    }
}

//**********Create_Laser**********
// Creates a laser when player fires, as long as under MAXLASER
// Inputs: None
// Outputs: None
void Create_Laser(void) {
    unsigned char i;

    if (nLaser >= MAXLASER) {
        return;
    }
    nLaser++;
    for (i = 0; i < MAXLASER; i++) {
        if (!Laser[i].life) {
            // get the coordinate
            Laser[i].x = Ship.x + SHIPW / 2 - 1;
            Laser[i].y = SCREENH - SHIPH - 1;
            Laser[i].life = 1;
            // draw on the buffer
            Draw(Laser[i]);
            return;
        }
    }
}

//**********Create_Missile**********
// Creates a missile when an enemy fires, as long as under MAXMISSILE
// Inputs: None
// Outputs: None
void Create_Missile(void) {
    unsigned char i;
    unsigned char iEnemy = Random() % MAXENEMY;

    if (nMissile >= MAXMISSILE || !Enemy[iEnemy].life) {
        return;
    }
    nMissile++;
    for (i = 0; i < MAXMISSILE; i++) {
        if (!Missile[i].life) {
            // get the coordinate
            Missile[i].x = Enemy[iEnemy].x + ENEMYW / 2 - 1;
            Missile[i].y = Enemy[iEnemy].y + LASERH - 1;
            Missile[i].life = 1;
            // draw on the buffer
            Missile[i].frame = Random() % 2;
            Draw(Missile[i]);
            return;
        }
    }
}

//**********Laser_Is_Hit**********
// Checks the collision between a laser other things
// Inputs: *aLaser     a laser in Laser[]
// Outputs: 0    no hit
//          1    hit
unsigned char Laser_Is_Hit(Thing *aLaser) {
    unsigned char i;

    // laser with bunker
    if (Bunker.life && Check_Collision(*aLaser, Bunker)) {
        Destroy(&*aLaser);
        Damage(&Bunker);
        return 1;
    }
    // laser with enemy
    for (i = 0; i < MAXENEMY; i++) {
        if (Enemy[i].life && Check_Collision(*aLaser, Enemy[i])) {
            Destroy(&*aLaser);
            Destroy(&Enemy[i]);
            return 1;
        }
    }
    // laser with mothership
    if (Mothership.life && Check_Collision(*aLaser, Mothership)) {
        Destroy(&*aLaser);
        Damage(&Mothership);
        return 1;
    }
    return 0;
}

//**********Missile_Is_Hit**********
// Checks the collision between a missile other things
// Inputs: *aMissile     a missile in Missile[]
// Outputs: 0    no hit
//          1    hit
unsigned char Missile_Is_Hit(Thing *aMissile) {
    unsigned char i;

    // missile with lasers
    for (i = 0; i < MAXLASER; i++) {
        if (Laser[i].life && Check_Collision(*aMissile, Laser[i])) {
            Destroy(&*aMissile);
            Destroy(&Laser[i]);
            return 1;
        }
    }
    // missile with bunker
    if (Bunker.life && Check_Collision(*aMissile, Bunker)) {
        Destroy(&*aMissile);
        Damage(&Bunker);
        return 1;
    }
    // missile with ship
    if (Ship.life && Check_Collision(*aMissile, Ship)) {
        Destroy(&*aMissile);
        Damage(&Ship);
        return 1;
    }
    return 0;
}

//**********Move_Lasers**********
// Moves all lasers 1 pixel upward at a time
// Inputs: None
// Outputs: None
void Move_Lasers(void) {
    unsigned char i;

    for (i = 0; i < MAXLASER; i++) {
        if (Laser[i].life && !Laser_Is_Hit(&Laser[i])) {
            Move(&Laser[i], Laser[i].x, Laser[i].y - 1);
            if (Laser[i].y < Laser[i].height - 1) {
                Destroy(&Laser[i]);         // destroy if out of screen
            }
            Nokia5110_DisplayBuffer();
            Delay(DELTAEXPLODE);
        }
    }
}

//**********Move_Missiles**********
// Moves all missiles 1 pixel downward at a time
// Inputs: None
// Outputs: None
void Move_Missiles(void) {
    unsigned char i;

    for (i = 0; i < MAXMISSILE; i++) {
        if (Missile[i].life && !Missile_Is_Hit(&Missile[i])) {
            Missile[i].frame = Random() % 2;
            Move(&Missile[i], Missile[i].x, Missile[i].y + 1);
            if (Missile[i].y > SCREENH - 1) {
                Destroy(&Missile[i]);       // destroy if out of sreen
            }
            Nokia5110_DisplayBuffer();
            Delay(DELTAEXPLODE);
        }
    }
}
