/**
 * @file     sprites.h
 * @author   Phi Luu
 * @date     May 20, 2016
 *
 * @brief    UTAustinX: UT.6.03x Embedded Systems - Shape the World
 *           Lab 15: SpaceInvaders
 *
 * @section  DESCRIPTION
 *
 * This file contains functions that are responsible for sprites in the game.
 */

#include "data.h"

// Sprites data
typedef struct Sprite {
    int x;
    int y;
    int width;
    int height;
    int exists;
    int score;
    int bitmapn;
    int exploding_at;
    const unsigned char *bitmap[MAX_BITMAPS];
} Sprite;

extern Sprite score_digits[MAX_SCORE_DIGITS];
extern Sprite player;
extern Sprite powerbar;
extern Sprite hearts;
extern Sprite aliens[ALIEN_COLS][ALIEN_ROWS];
extern Sprite mothership;
extern Sprite bunkers[MAX_BUNKERS];
extern Sprite player_missiles[PMISSILES];
extern Sprite alien_missiles[AMISSILES];
extern Sprite *sprite_list[MAX_SCORE_DIGITS + ALIEN_ROWS * ALIEN_COLS
                           + PMISSILES + AMISSILES + MAX_HEARTS + MAX_POWERBAR];

extern const int NUM_SPRITES;

// Global game parameters
extern int alien_speed;
extern int alien_init_speed;
extern int alien_direction;
extern int score;
extern int level;
extern int lives;
extern int power_up;
extern int power_level;
extern long adc_data;

/**
 * Initializes all sprites of the game.
 */
void InitSprites(void);

/**
 * Draws all active sprites.
 */
void DrawBoard(void);

/**
 * Resets board to start of level state.
 */
void ResetBoard(void);

/**
 * Returns the number of active aliens.
 */
int CountAliens(void);

/**
 * Returns the number of active alien missiles.
 */
int CountAlienMissiles(void);

/**
 * Returns the number of active player missiles.
 */
int CountPlayerMissiles(void);

/**
 * Randomly spawns alien missiles.
 */
void CreateAlienMissiles(void);

/**
 * Updates active alien missiles.
 */
void MoveAlienMissiles(void);

/**
 * Creates a player-initiated laser.
 */
void CreatePlayerMissile(void);

/**
 * Updates active player missiles.
 */
void MovePlayerMissile(void);

/**
 * Update player's ship position.
 */
void MovePlayer(void);

/**
 * Update alien positions.
 */
void MoveAliens(void);

/**
 * Updates mothership position.
 */
void MoveMothership(void);

/**
 * Changes alien direction at the end of a row.
 */
void ChangeAlienDirection(int direction);

/**
 * Updates sprites within appropriate interrupting intervals.
 */
void MoveSprites(void);

/**
 * Checks if aliens are present on screen.
 */
int AliensExist(void);

/**
 * Starts the destruction of an alien ship.
 */
void ExplodeAlien(Sprite *alien);

/**
 * Starts the destruction of the player's ship.
 */
void ExplodePlayer(void);

/**
 * Updates on-screen score and digit sprites.
 */
void UpdateScore(int value);

/**
 * Updates the appearance of a bunker as it takes damage.
 */
void DamageBunker(Sprite *bunker);

/**
 * Starts the destruction of a sprite.
 */
void DestroySprite(Sprite *s);

/**
 * Shows the level on the screen before starting the round.
 */
void ShowLevel(void);

/**
 * Checks if two sprites collide.
 * Two sprite collide if their coordinates overlap.
 */
int SpriteCollision(Sprite *s1, Sprite *s2);

/**
 * Tests whether the player is hit by an alien missile.
 */
void CheckPlayerHit(Sprite *missile);

/**
 * Tests whether an alien is hit by a player missile.
 */
void CheckAlienHit(Sprite *missile);

/**
 * Tests whether a bunker is hit by an alien missile.
 */
void CheckBunkerHit(Sprite *missile);

/**
 * Tests whether a player missile and an alien missile collide.
 */
void CheckMissileCollision(Sprite *missile);
