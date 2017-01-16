/**
 * UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Lab 15: SpaceInvaders
 *
 * File Name: Graphic.h
 *
 * Description: Graphic Processor
 *
 * Compatibility: EK-TM4C123GXL
 *
 * Phi Luu
 * Portland, Oregon, United States
 * Created May 20, 2016
 * Updated January 15, 2017
 */

typedef struct State Thing;

/**
 * Detects whether two Things overlap
 *
 * @param   Thing1  a Thing
 * @param   Thing2  another Thing
 *
 * @return          0 if not collide
 * @return          1 if collide
 */
unsigned char Check_Collision(Thing Thing1, Thing Thing2);

/**
 * Draws a Thing on the buffer
 *
 * @param  aThing  a Thing, such as Ship, Bunker, Enemy, Mothership, and Laser
 */
void          Draw(Thing aThing);

/**
 * Draws all enemies on the screen
 */
void          Draw_AllEnemies(void);

/**
 * Changes the coordinate and moves a movable Thing on the buffer
 *
 * @param  aMovableThing  a movable Thing, such as Ship, Enemy, Mothership, Missile, and Laser
 * @param  toX            x-coordinate that Thing is moving to
 * @param  toY            y-coordinate that Thing is moving to
 */
void          Move(Thing *aMovableThing, unsigned char toX,
                   unsigned char toY);

/**
 * Destroys a Thing and plays an explosion sequence if available
 *
 * @param  aThing  a Thing, such as Ship, Enemy, Mothership, Missile, and Laser
 */
void          Destroy(Thing *aThing);

/**
 * Damages a Thing and destroys if neccessary
 *
 * @param  aThing  a Thing, such as Ship, Bunker, Enemy, Mothership, Missile, and Laser
 */
void          Damage(Thing *aThing);

/**
 * Creates a laser when player fires, as long as under MAXLASER
 */
void          Create_Laser(void);

/**
 * Creates a missile when an enemy fires, as long as under MAXMISSILE
 */
void          Create_Missile(void);

/**
 * Checks the collision between a laser and other Things
 *
 * @param   aLaser  a laser in Laser[]
 *
 * @return          0 if missed
 * @return          1 if hit
 */
unsigned char Laser_Is_Hit(Thing *aLaser);

/**
 * Checks the collision between a missile and other Things
 *
 * @param   aMissile  a missile in Missile[]
 *
 * @return            0 if missed
 * @return            1 if hit
 */
unsigned char Missile_Is_Hit(Thing *aMissile);

/**
 * Moves all lasers 1 pixel downward at a time
 */
void          Move_Lasers(void);

/**
 * Moves all missiles 1 pixel upward at a time
 */
void          Move_Missiles(void);
