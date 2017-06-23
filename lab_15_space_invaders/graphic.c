/**
 * UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Lab 15: SpaceInvaders
 *
 * File Name: graphic.c
 *
 * Description: Graphic Processor
 *
 * Compatibility: EK-TM4C123GXL
 *
 * Phi Luu
 * Portland, Oregon, United States
 * Created May 20, 2016
 * Updated May 30, 2017
 */

#include <string.h>
#include "data.h"
#include "graphic.h"
#include "nokia5110.h"
#include "random.h"
#include "timer.h"

/**
 * Detects whether two Things overlap
 *
 * @param   Thing1  a Thing
 * @param   Thing2  another Thing
 *
 * @return          0 if not collide
 * @return          1 if collide
 */
unsigned char Check_Collision(Thing Thing1, Thing Thing2) {
    if ((Thing1.x < Thing2.x + Thing2.width)
            && (Thing1.x + Thing1.width > Thing2.x)
            && (Thing1.y > Thing2.y - Thing2.height)
            && (Thing1.y - Thing1.height < Thing2.y)) {
        return 1;
    }
    return 0;
}

/**
 * Draws a Thing on the buffer
 *
 * @param  aThing  a Thing, such as Ship, Bunker, Enemy, Mothership, and Laser
 */
void Draw(Thing aThing) {
    Nokia5110_PrintBMP(aThing.x, aThing.y, aThing.image[aThing.frame], 0);
}

/**
 * Draws all enemies on the screen
 */
void Draw_AllEnemies(void) {
    unsigned char i;

    for (i = 0; i < MAXENEMY; i++) {
        Draw(Enemy[i]);
    }
}

/**
 * Changes the coordinate and moves a movable Thing on the buffer
 *
 * @param  aMovableThing  a movable Thing, such as Ship, Enemy, Mothership, Missile, and Laser
 * @param  toX            x-coordinate that Thing is moving to
 * @param  toY            y-coordinate that Thing is moving to
 */
void Move(Thing *aMovableThing, unsigned char toX, unsigned char toY) {
    Nokia5110_PrintBMP(aMovableThing->x, aMovableThing->y,
                       aMovableThing->blank, 0);
    aMovableThing->x = toX;
    aMovableThing->y = toY;
    Draw(*aMovableThing);
}

/**
 * Destroys a Thing and plays an explosion sequence if available
 *
 * @param  aThing  a Thing, such as Ship, Enemy, Mothership, Missile, and Laser
 */
void Destroy(Thing *aThing) {
    aThing->life = 0;

    // multi-frame explosion
    if ((strncmp(aThing->name, "ship", 4) == 0)
            || (strncmp(aThing->name, "enemy", 5) == 0)
            || (strncmp(aThing->name, "mothership", 10) == 0)) {
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

/**
 * Damages a Thing and destroys if neccessary
 *
 * @param  aThing  a Thing, such as Ship, Bunker, Enemy, Mothership, Missile, and Laser
 */
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

/**
 * Creates a laser when player fires, as long as under MAXLASER
 */
void Create_Laser(void) {
    unsigned char i;

    if (nLaser >= MAXLASER) {
        return;
    }
    nLaser++;

    for (i = 0; i < MAXLASER; i++) {
        if (!Laser[i].life) {
            // get the coordinate
            Laser[i].x    = Ship.x + SHIPW / 2 - 1;
            Laser[i].y    = SCREENH - SHIPH - 1;
            Laser[i].life = 1;
            // draw on the buffer
            Draw(Laser[i]);
            return;
        }
    }
}

/**
 * Creates a missile when an enemy fires, as long as under MAXMISSILE
 */
void Create_Missile(void) {
    unsigned char i;
    unsigned char iEnemy = Random() % MAXENEMY;

    if ((nMissile >= MAXMISSILE) || !Enemy[iEnemy].life) {
        return;
    }
    nMissile++;

    for (i = 0; i < MAXMISSILE; i++) {
        if (!Missile[i].life) {
            // get the coordinate
            Missile[i].x    = Enemy[iEnemy].x + ENEMYW / 2 - 1;
            Missile[i].y    = Enemy[iEnemy].y + LASERH - 1;
            Missile[i].life = 1;
            // draw on the buffer
            Missile[i].frame = Random() % 2;
            Draw(Missile[i]);
            return;
        }
    }
}

/**
 * Checks the collisions which involve all lasers
 */
void Check_Laser_Collisions(void) {
    unsigned char i;

    for (i = 0; i < MAXLASER; i++) {
        if (Laser[i].life) {
            // laser with bunker
            if (Bunker.life && Check_Collision(Laser[i], Bunker)) {
                Destroy(&Laser[i]);
                Damage(&Bunker);
                return;
            }

            // laser with mothership
            if (Mothership.life && Check_Collision(Laser[i], Mothership)) {
                Destroy(&Laser[i]);
                Damage(&Mothership);
							  Ship.score += Mothership.score;
                return;
            }

            // laser with enemy
            for (i = 0; i < MAXENEMY; i++) {
                if (Enemy[i].life && Check_Collision(Laser[i], Enemy[i])) {
                    Destroy(&Laser[i]);
                    Destroy(&Enemy[i]);
									  Ship.score += Enemy[i].score;
                    return;
                }
            }
        }
    }
}

/**
 * Checks the collisions which involve all missiles
 */
void Check_Missile_Collisions(void) {
    unsigned char i;

    for (i = 0; i < MAXMISSILE; i++) {
        if (Missile[i].life) {
            // missile with bunker
            if (Bunker.life && Check_Collision(Missile[i], Bunker)) {
                Destroy(&Missile[i]);
                Damage(&Bunker);
                return;
            }

            // missile with ship
            if (Ship.life && Check_Collision(Missile[i], Ship)) {
                Destroy(&Missile[i]);
                Damage(&Ship);
                return;
            }

            // missile with lasers
            for (i = 0; i < MAXLASER; i++) {
                if (Laser[i].life && Check_Collision(Missile[i], Laser[i])) {
                    Destroy(&Missile[i]);
                    Destroy(&Laser[i]);
                    return;
                }
            }
        }
    }
}

/**
 * Counts the number of active lasers on the field
 *
 * @return      the number of active lasers on the field
 */
unsigned char Count_Laser(void) {
    unsigned char i;
    unsigned char count = 0;

    for (i = 0; i < MAXLASER; i++) {
        if (Laser[i].life) {
            count++;
        }
    }
    return count;
}

/**
 * Counts the number of active missiles on the field
 *
 * @return      the number of active missiles on the field
 */
unsigned char Count_Missile(void) {
    unsigned char i;
    unsigned char count = 0;

    for (i = 0; i < MAXMISSILE; i++) {
        if (Missile[i].life) {
            count++;
        }
    }
    return count;
}

/**
 * Moves all lasers 1 pixel downward at a time
 */
void Move_Lasers(void) {
    unsigned char i;

    for (i = 0; i < MAXLASER; i++) {
        if (Laser[i].life) {
            Move(&Laser[i], Laser[i].x, Laser[i].y - 1);

            if (Laser[i].y < Laser[i].height - 1) {
                Destroy(&Laser[i]); // destroy if out of screen
            }
            Nokia5110_DisplayBuffer();
        }
    }
}

/**
 * Moves all missiles 1 pixel upward at a time
 */
void Move_Missiles(void) {
    unsigned char i;

    for (i = 0; i < MAXMISSILE; i++) {
        if (Missile[i].life) {
            Missile[i].frame = Random() % 2;
            Move(&Missile[i], Missile[i].x, Missile[i].y + 1);

            if (Missile[i].y > SCREENH - 1) {
                Destroy(&Missile[i]); // destroy if out of sreen
            }
            Nokia5110_DisplayBuffer();
        }
    }
}
