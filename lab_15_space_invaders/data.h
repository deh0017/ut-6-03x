/**
 * @file     data.h
 * @author   Phi Luu
 * @date     May 20, 2016
 *
 * @brief    UTAustinX: UT.6.03x Embedded Systems - Shape the World
 *           Lab 15: SpaceInvaders
 *
 * @section  DESCRIPTION
 *
 * Contains images, sound, and global variables. Initializes sprite's data
 */

// Required hardware I/O connections
#define POT_PIN                0x04 // slide pot connected to PE2
#define SWITCH1_PIN            0x01 // right switch connected to PE0
#define SWITCH2_PIN            0x02 // left switch connected to PE1
#define LED1_PIN               0x10 // right LED connected to PB4
#define LED2_PIN               0x20 // left LED connected to PB5
#define DAC_PINS               0x0F // DAC connected to PB3-PB0
#define CLOCK_SPEED            80000000L

// Random limits
#define RAND_MAX               255 // Maximum value returned by rand function
#define RAND_BASE_REDUCTION    220 // MAX - REDUCTION = base chance for alien to fire a missile (255 - 220 = 35 = ~14%)
#define RAND_MISSILE_WEIGHT    5   // reduced further by missiles already fired (5 * 3 = 15 = ~6%)
#define RAND_ALIEN_WEIGHT      2   // reduced further by existing aliens as more can randomly fire (15 * 2 = 30 = ~12%)

// bitmap and screen limits
#define BMP_WIDTH_INDEX        18
#define BMP_HEIGHT_INDEX       22
#define SCREEN_WIDTH           84
#define SCREEN_HEIGHT          48

// Sprite limits
#define ALIEN_ROWS             3  // number of aliens in each column
#define ALIEN_COLS             5  // number of aliens in each row
#define PMISSILES              10 // number of concurrent missiles a player can fire
#define AMISSILES              4  // number of concurrent missiles aliens can fire
#define MAX_BITMAPS            4
#define MAX_SCORE_DIGITS       6
#define MAX_BUNKERS            2  // number of bunkers
#define MAX_HEARTS             3
#define MAX_POWERBAR           1
#define MAX_POWER              4

/**
 * bitmaps of sprites
 */
extern const unsigned char BMP_BUNKER_0[];
extern const unsigned char BMP_BUNKER_1[];
extern const unsigned char BMP_BUNKER_2[];
extern const unsigned char BMP_SPLASH_SCREEN[];
extern const unsigned char BMP_HEART_0[];
extern const unsigned char BMP_HEART_1[];
extern const unsigned char BMP_HEART_2[];
extern const unsigned char BMP_HEART_3[];
extern const unsigned char BMP_PLAYER_EXPLOSION_0[];
extern const unsigned char BMP_PLAYER_EXPLOSION_1[];
extern const unsigned char BMP_SMALL_EXPLOSION_0[];
extern const unsigned char BMP_SMALL_EXPLOSION_1[];
extern const unsigned char BMP_SMALL_ENEMY_BONUS[];
extern const unsigned char BMP_SMALL_ENEMY_30_POINTS_B[];
extern const unsigned char BMP_SMALL_ENEMY_30_POINTS_A[];
extern const unsigned char BMP_SMALL_ENEMY_20_POINTS_B[];
extern const unsigned char BMP_SMALL_ENEMY_20_POINTS_A[];
extern const unsigned char BMP_SMALL_ENEMY_10_POINTS_B[];
extern const unsigned char BMP_SMALL_ENEMY_10_POINTS_A[];
extern const unsigned char BMP_PLAYER_SHIP_0[];
extern const unsigned char BMP_POWERBAR_1[];
extern const unsigned char BMP_POWERBAR_2[];
extern const unsigned char BMP_POWERBAR_3[];
extern const unsigned char BMP_POWERBAR_4[];
extern const unsigned char BMP_MISSILE_1[];
extern const unsigned char BMP_MISSILE_0[];
extern const unsigned char BMP_LASER[];
extern const unsigned char BMP_DIGIT[10][0x8b];

/**
 * 5.5-kHz wav sound files
 */
extern const unsigned char WAV_SHOOT[];
extern const unsigned char WAV_EXPLOSION[];
extern const unsigned char WAV_FAST_INVADER[];
extern const unsigned char WAV_INVADER_KILLED[];
extern const unsigned char WAV_UFO_HIGH_PITCH[];
