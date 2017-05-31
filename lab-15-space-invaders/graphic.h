/**
 * UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Lab 15: SpaceInvaders
 *
 * File Name: graphic.h
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

typedef struct State Thing;

/**
 * Draws a Thing on the buffer
 *
 * @param  aThing  a Thing, such as Ship, Bunker, Enemy, Mothership, and Laser
 */
void Draw(Thing aThing);

/**
 * Draws all enemies on the screen
 */
void Draw_AllEnemies(void);

/**
 * Changes the coordinate and moves a movable Thing on the buffer
 *
 * @param  aMovableThing  a movable Thing, such as Ship, Enemy, Mothership, Missile, and Laser
 * @param  toX            x-coordinate that Thing is moving to
 * @param  toY            y-coordinate that Thing is moving to
 */
void Move(Thing *aMovableThing, unsigned char toX,
          unsigned char toY);

/**
 * Destroys a Thing and plays an explosion sequence if available
 *
 * @param  aThing  a Thing, such as Ship, Enemy, Mothership, Missile, and Laser
 */
void Destroy(Thing *aThing);

/**
 * Damages a Thing and destroys if neccessary
 *
 * @param  aThing  a Thing, such as Ship, Bunker, Enemy, Mothership, Missile, and Laser
 */
void Damage(Thing *aThing);

/**
 * Creates a laser when player fires, as long as under MAXLASER
 */
void Create_Laser(void);

/**
 * Creates a missile when an enemy fires, as long as under MAXMISSILE
 */
void Create_Missile(void);

/**
 * Checks the collisions which involve all lasers
 */
void Check_Laser_Collisions(void);

/**
 * Checks the collisions which involve all missiles
 */
void Check_Missile_Collisions(void);


/**
 * Counts the number of active lasers on the field
 *
 * @return      the number of active lasers on the field
 */
unsigned char Count_Laser(void);

/**
 * Counts the number of active missiles on the field
 *
 * @return      the number of active missiles on the field
 */
unsigned char Count_Missile(void);

/**
 * Moves all lasers 1 pixel downward at a time
 */
void          Move_Lasers(void);

/**
 * Moves all missiles 1 pixel upward at a time
 */
void          Move_Missiles(void);
