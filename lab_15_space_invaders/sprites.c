/**
 * @file     sprites.c
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

#include "tm4c123gh6pm.h"
#include "random.h"
#include "nokia5110.h"
#include "timer.h"
#include "led.h"
#include "sound.h"
#include "sprites.h"

const int NUM_SPRITES = sizeof(sprite_list) / sizeof(sprite_list[0]);

// frames to make aliens move their body when they move
const unsigned char *ALIENS_FRAME_A[ALIEN_ROWS] = {
    BMP_SMALL_ENEMY_30_POINTS_A,
    BMP_SMALL_ENEMY_20_POINTS_A,
    BMP_SMALL_ENEMY_10_POINTS_A
};

const unsigned char *ALIENS_FRAME_B[ALIEN_ROWS] = {
    BMP_SMALL_ENEMY_30_POINTS_B,
    BMP_SMALL_ENEMY_20_POINTS_B,
    BMP_SMALL_ENEMY_10_POINTS_B
};

const int ALIEN_POINTS[ALIEN_ROWS] = { 30, 20, 10 };

// global game parameters
int alien_init_speed = 16;
int level = 1;
int power_up = 0;
int power_level = MAX_POWER;

/**
 * Initializes all sprites of the game.
 */
void InitSprites(void) {
    int i, j;
    int n = 0; // sprite_list index

    // lives
    hearts.width = BMP_HEART_0[BMP_WIDTH_INDEX];
    hearts.height = BMP_HEART_0[BMP_HEIGHT_INDEX];
    hearts.x = SCREEN_WIDTH - hearts.width - 1;
    hearts.y = hearts.height;
    hearts.bitmap[0] = BMP_HEART_0;
    hearts.bitmap[1] = BMP_HEART_1;
    hearts.bitmap[2] = BMP_HEART_2;
    hearts.bitmap[3] = BMP_HEART_3;
    hearts.bitmapn = 0;
    hearts.exists = 1;
    sprite_list[n++] = &hearts; // add hearts to list

    // score digits
    for (i = 0; i < MAX_SCORE_DIGITS; i++) {
        score_digits[i].height = BMP_DIGIT[0][BMP_HEIGHT_INDEX];
        score_digits[i].width = BMP_DIGIT[0][BMP_WIDTH_INDEX];
        score_digits[i].x = (MAX_SCORE_DIGITS - i - 1) * score_digits[i].width;
        score_digits[i].y = score_digits[i].height;
        score_digits[i].bitmap[0] = BMP_DIGIT[0];
        score_digits[i].bitmapn = 0;
        score_digits[i].exists = 0;
        sprite_list[n++] = &score_digits[i]; // add score digits to list
    }
    score_digits[0].exists = 1;

    // missiles
    for (i = 0; i < PMISSILES; i++) {
        player_missiles[i].height = BMP_LASER[BMP_HEIGHT_INDEX];
        player_missiles[i].width = BMP_LASER[BMP_WIDTH_INDEX];
        player_missiles[i].bitmap[0] = BMP_LASER;
        player_missiles[i].bitmap[1] = BMP_LASER;
        player_missiles[i].bitmap[2] = BMP_LASER;
        player_missiles[i].bitmap[3] = BMP_LASER;
        player_missiles[i].bitmapn = 0;
        player_missiles[i].exploding_at = 0xFF;
        sprite_list[n++] = &player_missiles[i]; // add player missiles to list
    }

    for (i = 0; i < AMISSILES; i++) {
        alien_missiles[i].height = BMP_MISSILE_0[BMP_HEIGHT_INDEX];
        alien_missiles[i].width = BMP_MISSILE_0[BMP_WIDTH_INDEX];
        alien_missiles[i].bitmap[0] = BMP_MISSILE_0;
        alien_missiles[i].bitmap[1] = BMP_MISSILE_1;
        alien_missiles[i].bitmap[2] = BMP_MISSILE_0;
        alien_missiles[i].bitmap[3] = BMP_MISSILE_1;
        alien_missiles[i].bitmapn = 0;
        alien_missiles[i].exploding_at = 0xFF;
        sprite_list[n++] = &alien_missiles[i]; // add alien missiles to list
    }

    // player's ship
    player.height = BMP_PLAYER_SHIP_0[BMP_HEIGHT_INDEX];
    player.width = BMP_PLAYER_SHIP_0[BMP_WIDTH_INDEX];
    player.x = (SCREEN_WIDTH - player.width) / 2 - 1;
    player.y = SCREEN_HEIGHT - 1;
    player.bitmap[0] = BMP_PLAYER_SHIP_0;
    player.bitmap[1] = BMP_PLAYER_EXPLOSION_0;
    player.bitmap[2] = BMP_PLAYER_EXPLOSION_1;
    player.bitmap[3] = BMP_PLAYER_EXPLOSION_0;
    player.bitmapn = 0;
    player.exploding_at = 1;
    sprite_list[n++] = &player; // add player's ship to list

    // bunkers
    for (i = 0; i < MAX_BUNKERS; i++) {
        bunkers[i].height = BMP_BUNKER_0[BMP_HEIGHT_INDEX];
        bunkers[i].width = BMP_BUNKER_0[BMP_WIDTH_INDEX];
        bunkers[i].x =
            (SCREEN_WIDTH / MAX_BUNKERS - bunkers[i].width + i * SCREEN_WIDTH)
            / 2;
        bunkers[i].y = SCREEN_HEIGHT - player.height - 1;
        bunkers[i].bitmap[0] = BMP_BUNKER_0;
        bunkers[i].bitmap[1] = BMP_BUNKER_1;
        bunkers[i].bitmap[2] = BMP_BUNKER_2;
        bunkers[i].bitmap[3] = BMP_BUNKER_2;
        bunkers[i].bitmapn = 0;
        bunkers[i].exploding_at = 0xFF;
        sprite_list[n++] = &bunkers[i]; // add bunkers to list
    }

    // overlaid powerbar on player's ship
    powerbar.height = player.height;
    powerbar.width = player.width;
    powerbar.x = player.x;
    powerbar.y = player.y;
    powerbar.bitmap[0] = BMP_POWERBAR_1;
    powerbar.bitmap[1] = BMP_POWERBAR_2;
    powerbar.bitmap[2] = BMP_POWERBAR_3;
    powerbar.bitmap[3] = BMP_POWERBAR_4;
    powerbar.bitmapn = power_level - 1;
    powerbar.exists = 1;
    sprite_list[n++] = &powerbar; // add powerbar to list

    // mothership
    mothership.height = BMP_SMALL_ENEMY_BONUS[BMP_HEIGHT_INDEX];
    mothership.width = BMP_SMALL_ENEMY_BONUS[BMP_WIDTH_INDEX];
    mothership.bitmap[0] = BMP_SMALL_ENEMY_BONUS;
    mothership.bitmap[1] = BMP_SMALL_EXPLOSION_0;
    mothership.bitmap[2] = BMP_SMALL_EXPLOSION_0;
    mothership.bitmap[3] = BMP_SMALL_EXPLOSION_0;
    mothership.bitmapn = 0;
    mothership.exploding_at = 1;
    mothership.score = 100;
    sprite_list[n++] = &mothership; // add mothership to list

    // aliens
    for (i = 0; i < ALIEN_COLS; i++) {
        for (j = 0; j < ALIEN_ROWS; j++) {
            aliens[i][j].height = ALIENS_FRAME_A[j][BMP_HEIGHT_INDEX];
            aliens[i][j].width = ALIENS_FRAME_A[j][BMP_WIDTH_INDEX];
            aliens[i][j].bitmap[0] = ALIENS_FRAME_A[j];
            aliens[i][j].bitmap[1] = ALIENS_FRAME_B[j];
            aliens[i][j].bitmap[2] = BMP_SMALL_EXPLOSION_0;
            aliens[i][j].bitmap[3] = BMP_SMALL_EXPLOSION_0;
            aliens[i][j].bitmapn = 0;
            aliens[i][j].exploding_at = 2;
            aliens[i][j].score = ALIEN_POINTS[j];
            sprite_list[n++] = &aliens[i][j]; // add aliens to list
        }
    }
}

/**
 * Draws all active sprites.
 */
void DrawBoard(void) {
    int i;

    Nokia5110_ClearBuffer();

    // only draw a sprite if it exists
    for (i = 0; i < NUM_SPRITES; i++) {
        if (sprite_list[i]->exists) {
            Nokia5110_PrintBMP(sprite_list[i]->x,
                               sprite_list[i]->y,
                               sprite_list[i]->bitmap[sprite_list[i]->bitmapn],
                               0);
        }
    }

    Nokia5110_DisplayBuffer();
}

/**
 * Resets board to start of level state.
 */
void ResetBoard(void) {
    int i, j;

    // reset bunker
    for (i = 0; i < MAX_BUNKERS; i++) {
        bunkers[i].exists = 1;
    }

    // reset alien
    DestroySprite(&mothership);
    alien_direction = -1;
    alien_speed = alien_init_speed;
    for (i = 0; i < ALIEN_COLS; i++) {
        int y = 0;
        for (j = 0; j < ALIEN_ROWS; j++) {
            y += aliens[i][j].height;
            aliens[i][j].exists = 1;
            aliens[i][j].x = i * (aliens[i][j].width + 1) + SCREEN_WIDTH
                             - (ALIEN_COLS * (aliens[i][j].width + 1)) - 1;
            aliens[i][j].y = y++;
        }
    }

    // clear any missiles
    for (i = 0; i < PMISSILES; i++) {
        DestroySprite(&player_missiles[i]);
    }

    for (i = 0; i < AMISSILES; i++) {
        DestroySprite(&alien_missiles[i]);
    }
}

/**
 * Returns the number of active aliens.
 */
int CountAliens(void) {
    int i, j;
    int count = mothership.exists;

    for (i = 0; i < ALIEN_COLS; i++) {
        for (j = 0; j < ALIEN_ROWS; j++) {
            if (aliens[i][j].exists
                && (aliens[i][j].bitmapn < aliens[i][j].exploding_at)) {
                count++;
            }
        }
    }

    return count;
}

/**
 * Returns the number of active alien missiles.
 */
int CountAlienMissiles(void) {
    int i;
    int count = 0;

    for (i = 0; i < AMISSILES; i++) {
        count += alien_missiles[i].exists;
    }

    return count;
}

/**
 * Returns the number of active player missiles.
 */
int CountPlayerMissiles(void) {
    int i;
    int count = 0;

    for (i = 0; i < PMISSILES; i++) {
        count += player_missiles[i].exists;
    }

    return count;
}

/**
 * Randomly spawns alien missiles.
 */
void CreateAlienMissiles(void) {
    int i, j;
    // random value, reduced by number of aliens present, reduced by number of alien missiles already spawned
    int threshold = RAND_MAX - RAND_BASE_REDUCTION - CountAliens()
                    * RAND_ALIEN_WEIGHT - CountAlienMissiles()
                    * RAND_MISSILE_WEIGHT;

    for (i = 0; i < ALIEN_COLS; i++) {
        for (j = ALIEN_ROWS - 1; j >= 0; j--) {
            if (aliens[i][j].exists) {
                if (aliens[i][j].bitmapn < 2) {
                    if ((int)Random() <= threshold) {
                        int k = 0;
                        while (alien_missiles[k].exists) {
                            k++;
                        }
                        alien_missiles[k].x = aliens[i][j].x
                                              + (aliens[i][j].width
                                                 - alien_missiles[k].width)
                                              / 2;
                        alien_missiles[k].y = aliens[i][j].y;
                        alien_missiles[k].exists = 1;
                    }
                }
                break;
            }
        }

        if (CountAlienMissiles() == AMISSILES) {
            break;
        }
    }
}

/**
 * Updates active alien missiles.
 */
void MoveAlienMissiles(void) {
    int i;

    for (i = 0; i < AMISSILES; i++) {
        if (alien_missiles[i].exists) {
            if (alien_missiles[i].y == SCREEN_HEIGHT - 2) {
                DestroySprite(&alien_missiles[i]);
            } else {
                alien_missiles[i].y++;
                alien_missiles[i].bitmapn = 1 - alien_missiles[i].bitmapn;
            }
        }
        // detect alien missile collision with player/bunkers
        if (alien_missiles[i].exists) {
            // no need to check missile collisions as player missile will check
            // this and is updated faster
            CheckBunkerHit(&alien_missiles[i]);
            CheckPlayerHit(&alien_missiles[i]);
        }
    }
}

/**
 * Creates a player-initiated laser.
 */
void CreatePlayerMissile(void) {
    int missiles = CountPlayerMissiles();

    if (missiles < PMISSILES) {
        int k, empty;
        int max_y = 0;
        for (k = 0; k < PMISSILES; k++) {
            if (player_missiles[k].exists) {
                if (player_missiles[k].y > max_y) {
                    max_y = player_missiles[k].y;
                }
            } else empty = k;
        }
        if ((max_y
             < SCREEN_HEIGHT - player.height - player_missiles[empty].height
             - 1)
            || (power_up
                && (max_y
                    < SCREEN_HEIGHT - player.height
                    - player_missiles[empty].height + 1))) {
            player_missiles[empty].x = player.x
                                       + (player.width
                                          - player_missiles[empty].width) / 2
                                       + 1;
            player_missiles[empty].y = player.y - player.height
                                       + player_missiles[empty].height - 1;
            player_missiles[empty].exists = 1;
            PlaySound(SHOOT_SOUND);
            SetLed2(1);
        }
    }
}

/**
 * Updates active player missiles.
 */
void MovePlayerMissile(void) {
    int i;

    for (i = 0; i < PMISSILES; i++) {
        // move player missile
        if (player_missiles[i].exists) {
            // detect player missile collision with aliens/bunkers
            CheckBunkerHit(&player_missiles[i]);
            CheckMissileCollision(&player_missiles[i]);
            CheckAlienHit(&player_missiles[i]);

            if (player_missiles[i].y == player_missiles[i].height) {
                DestroySprite(&player_missiles[i]);
            } else {
                player_missiles[i].y--;
                player_missiles[i].bitmapn = 1 - player_missiles[i].bitmapn;
            }
        }
    }
}

/**
 * Update player's ship position.
 */
void MovePlayer(void) {
    // Read ADC value
    static int last_fire_button = 0;
    int x = adc_data - (player.width >> 1);

    if (player.bitmapn == player.exploding_at) {
        player.bitmapn = 2;
    }

    if ((player.bitmapn == 2) && !(timer2a_count % 32)) {
        player.bitmapn = 3;
    }

    if ((player.bitmapn == 3) && !(timer2a_count % 32)) {
        DestroySprite(&player);
        DestroySprite(&powerbar);
    } else {
        // Convert to screen position
        if (x < 0) {
            x = 0;
        } else if (x > (SCREEN_WIDTH - player.width)) {
            x = (SCREEN_WIDTH - player.width);
        }

        player.x = x;

        if ((GPIO_PORTE_DATA_R & SWITCH2_PIN) || power_up) {
            if (power_level == MAX_POWER) {
                power_up = 1;
            }

            if (power_up) {
                CreatePlayerMissile();
            }
        } else if (GPIO_PORTE_DATA_R & SWITCH1_PIN) {
            // Read fire button and compare with last time
            if (!last_fire_button) {
                // create missile
                CreatePlayerMissile();
                last_fire_button = 1;
            }
        } else {
            last_fire_button = 0;
        }
    }

    // update embedded power bar
    powerbar.x = player.x;
    // power bar is incremented every 2048th tick while power up is charging
    // ~40 seconds to charge
    if (!power_up && !(timer2a_count & 0x3FF)) {
        if (power_level < MAX_POWER) {
            power_level++;
        }
        powerbar.bitmapn = power_level - 1;
        powerbar.exists = 1;
    }

    // power bar is decremented every 256th tick while power up is active ~4 seconds active
    if (power_up && !(timer2a_count & 0x3F)) {
        if (power_level > 0) {
            power_level--;
        } else {
            power_up = 0;
        }
        powerbar.bitmapn = power_level - 1;
        powerbar.exists = (power_level > 0);
    }
}

/**
 * Update alien positions.
 */
void MoveAliens(void) {
    int i, j, start, end, step;

    if (alien_direction == -1) {
        start = 0;
        end = ALIEN_COLS;
        step = 1;
    } else {
        start = ALIEN_COLS - 1;
        end = -1;
        step = -1;
    }

    // for each living alien,
    // 1. move it to its new positions.
    // detect end of row, reverse direction and drop downsampled
    // 2. change the animation, progress alien explosions and delete exploded
    // aliens
    // 3. possibly fire an alien missile
    for (i = start; i != end; i += step) {
        for (j = 0; j < ALIEN_ROWS; j++) {
            if (aliens[i][j].exists) {
                if ((alien_direction == -1) && (aliens[i][j].x == 0)) {
                    ChangeAlienDirection(1);
                }
                if ((alien_direction == 1)
                    && (aliens[i][j].x
                        == SCREEN_WIDTH - aliens[i][j].width - 1)) {
                    ChangeAlienDirection(-1);
                }
                switch (aliens[i][j].bitmapn) {
                    case 0:
                        aliens[i][j].bitmapn = 1;
                        break;
                    case 1:
                        aliens[i][j].bitmapn = 0;
                        break;
                    case 2:
                        aliens[i][j].bitmapn = 3;
                        break;
                    case 3:
                        DestroySprite(&aliens[i][j]);
                        break;
                    default:
                        break; // Should never get here!!
                }
                aliens[i][j].x += alien_direction;
            }
        }
    }

    CreateAlienMissiles();
    PlaySound(INVADER_SOUND);
}

/**
 * Updates mothership position.
 */
void MoveMothership(void) {
    if (mothership.exists) {
        if (mothership.x == SCREEN_WIDTH - mothership.width - 1) {
            DestroySprite(&mothership);
        } else if (mothership.bitmapn == 1) {
            mothership.bitmapn = 2;
        } else if (mothership.bitmapn == 2) {
            mothership.bitmapn = 3;
        } else if (mothership.bitmapn == 3) {
            DestroySprite(&mothership);
        } else {
            mothership.x++;
        }
        // if no other noise is playing use the mothership sound
        PlaySound(MOTHERSHIP_SOUND);
    } else {
        int i, j;
        int min_y = 47;

        // find topmost alien
        for (i = 0; i < ALIEN_COLS; i++) {
            for (j = 0; j < ALIEN_ROWS; j++) {
                if (aliens[i][j].exists) {
                    if (aliens[i][j].y < min_y) {
                        min_y = aliens[i][j].y - aliens[i][j].height;
                    }
                }
            }
        }

        // If there is room for a mothership above the topmost alien
        if (min_y >= mothership.height) {
            // then there is a 3% chance each tick to spawn a mothership
            if (Random() < (RAND_MAX / 32)) {
                mothership.exists = 1;
                mothership.x = 0;
                mothership.y = mothership.height;
            }
        }
    }
}

/**
 * Changes alien direction at the end of a row.
 */
void ChangeAlienDirection(int direction) {
    int i, j;
    int max_y = 0;

    alien_direction = direction;
    for (i = 0; i < ALIEN_COLS; i++) {
        for (j = 0; j < ALIEN_ROWS; j++) {
            if (aliens[i][j].exists
                && (aliens[i][j].bitmapn < aliens[i][j].exploding_at)) {
                aliens[i][j].y++;

                if (aliens[i][j].y > max_y) {
                    max_y = aliens[i][j].y;
                }
            }
        }
    }

    if (max_y >= bunkers[0].y - bunkers[0].height) {
        for (i = 0; i < MAX_BUNKERS; i++) {
            DamageBunker(&bunkers[i]);
        }
    }

    if (max_y > player.y - player.height) {
        ExplodePlayer();
        // move aliens to top row again
        DestroySprite(&mothership);
        for (i = 0; i < ALIEN_COLS; i++) {
            int y = 0;
            for (j = 0; j < ALIEN_ROWS; j++) {
                y += aliens[i][j].height;
                aliens[i][j].x = i * (aliens[i][j].width + 1) + SCREEN_WIDTH
                                 - (ALIEN_COLS * (aliens[i][j].width + 1))
                                 - 1;
                aliens[i][j].y = y++;
            }
        }
    }
}

/**
 * Updates sprites within appropriate interrupting intervals.
 */
void MoveSprites(void) {
    // player position is updated every tick
    MovePlayer();

    // player missiles are updated every 3rd tick
    if (!(timer2a_count % 2)) {
        MovePlayerMissile();
    }

    // alien positions are updated every ? tick
    if (!(timer2a_count % alien_speed)) {
        MoveAliens();
    }

    // mothership is updated every 8th tick
    if (!(timer2a_count % 4)) {
        MoveMothership();
    }

    // enemy missiles are updated every 8th tick
    if (!(timer2a_count % 6)) {
        MoveAlienMissiles();
    }
}

/**
 * Checks if aliens are present on screen.
 */
int AliensExist(void) {
    int i, j;

    if (mothership.exists) {
        return 1;
    }

    for (i = 0; i < ALIEN_COLS; i++) {
        for (j = 0; j < ALIEN_ROWS; j++) {
            if (aliens[i][j].exists) {
                return 1;
            }
        }
    }

    return 0;
}

/**
 * Starts the destruction of an alien ship.
 */
void ExplodeAlien(Sprite *alien) {
    int alien_count = CountAliens() - mothership.exists;

    alien->bitmapn = alien->exploding_at;
    UpdateScore(alien->score);
    PlaySound(INVADER_KILLED_SOUND);
    if (alien != &mothership) {
        switch (alien_count - 1) {
            case 1:
            case 2:
                alien_speed = alien_speed / 2 + 1;
                break;
            case ALIEN_COLS * ALIEN_ROWS * 1 / 3:
            case ALIEN_COLS * ALIEN_ROWS * 1 / 2:
            case ALIEN_COLS * ALIEN_ROWS * 3 / 4:
                if (alien_speed > 2) {
                    alien_speed--;
                }
                break;
            default:
                break;
        }
    }
}

/**
 * Starts the destruction of the player's ship.
 */
void ExplodePlayer(void) {
    player.bitmapn = player.exploding_at;
    hearts.bitmapn = 4 - lives;
    PlaySound(EXPLOSION_SOUND);
}

/**
 * Updates on-screen score and digit sprites.
 */
void UpdateScore(int value) {
    int i = 0;
    int number;

    score += value;
    number = score;

    while ((number > 0) && (i < MAX_SCORE_DIGITS)) {
        score_digits[i].exists = 1;
        score_digits[i++].bitmap[0] = BMP_DIGIT[number % 10];
        number /= 10;
    }
}

/**
 * Updates the appearance of a bunker as it takes damage.
 */
void DamageBunker(Sprite *bunker) {
    if (bunker->exists) {
        if (bunker->bitmapn < 2) {
            bunker->bitmapn++;
        } else {
            DestroySprite(bunker);
        }
    }
}

/**
 * Starts the destruction of a sprite.
 */
void DestroySprite(Sprite *s) {
    s->exists = 0;
    s->bitmapn = 0;
}

/**
 * Shows the level on the screen before starting the round.
 */
void ShowLevel(void) {
    Nokia5110_ClearBuffer();
    Nokia5110_DisplayBuffer();
    Nokia5110_SetCursor(5, 2);
    Nokia5110_OutUDec(level);
    Nokia5110_SetCursor(2, 2);
    Nokia5110_OutString("Level");
    Delay100ms(25); // delay 2.5 seconds
}

/**
 * Checks if two sprites collide.
 * Two sprite collide if their coordinates overlap.
 */
int SpriteCollision(Sprite *s1, Sprite *s2) {
    if ((s1->x < s2->x + s2->width) && (s1->x + s1->width > s2->x)
        && (s1->y > s2->y - s2->height) && (s1->y - s1->height < s2->y)) {
        return 1;
    }
    return 0;
}

/**
 * Tests whether the player is hit by an alien missile.
 */
void CheckPlayerHit(Sprite *missile) {
    if (player.bitmapn < player.exploding_at) {
        if (SpriteCollision(missile, &player)) {
            ExplodePlayer();
            DestroySprite(missile);
        }
    }
}

/**
 * Tests whether an alien is hit by a player missile.
 */
void CheckAlienHit(Sprite *missile) {
    int i, j;

    for (i = 0; i < ALIEN_COLS; i++) {
        for (j = 0; j < ALIEN_ROWS; j++) {
            if (aliens[i][j].exists
                && (aliens[i][j].bitmapn < aliens[i][j].exploding_at)) {
                if (SpriteCollision(missile, &aliens[i][j])) {
                    ExplodeAlien(&aliens[i][j]);
                    DestroySprite(missile);
                    return;
                }
            }
        }
    }

    if (mothership.exists && (mothership.bitmapn < mothership.exploding_at)) {
        if (SpriteCollision(missile, &mothership)) {
            ExplodeAlien(&mothership);
            DestroySprite(missile);
        }
    }
}

/**
 * Tests whether a bunker is hit by an alien missile.
 */
void CheckBunkerHit(Sprite *missile) {
    int i;

    for (i = 0; i < MAX_BUNKERS; i++) {
        if (bunkers[i].exists) {
            if (SpriteCollision(missile, &bunkers[i])) {
                DamageBunker(&bunkers[i]);
                DestroySprite(missile);
                return;
            }
        }
    }
}

/**
 * Tests whether a player missile and an alien missile collide.
 */
void CheckMissileCollision(Sprite *missile) {
    int i;

    for (i = 0; i < AMISSILES; i++) {
        if (alien_missiles[i].exists) {
            if (SpriteCollision(missile, &alien_missiles[i])) {
                DestroySprite(missile);
                DestroySprite(&alien_missiles[i]);
                return;
            }
        }
    }
}
