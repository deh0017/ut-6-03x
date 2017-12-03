/**
 * @file     sound.h
 * @author   Phi Luu
 * @date     May 20, 2016
 *
 * @brief    UTAustinX: UT.6.03x Embedded Systems - Shape the World
 *           Lab 15: SpaceInvaders
 *
 * @section  DESCRIPTION
 *
 * This file contains functions that are responsible for outputing sound waves
 * using Digital-Analog Conversion.
 */

typedef enum SoundName {
    EXPLOSION_SOUND = 0,
    INVADER_SOUND = 1,
    INVADER_KILLED_SOUND = 2,
    SHOOT_SOUND = 3,
    MOTHERSHIP_SOUND = 4,
    MAX_SOUND
} SoundName;

extern int sound_enabled[MAX_SOUND];

// current DAC step
extern unsigned long dac_step[MAX_SOUND];

// maximum DAC step
extern unsigned long dac_step_max[MAX_SOUND];

extern const unsigned char* WAV_DAC[];

/**
 * Initializes 4-bit DAC on PB3-PB0 and two LEDs on PB5-PB4.
 */
void InitDac(void);

/**
 * Outputs a 4-bit data to DAC.
 */
void WriteDac(unsigned long data);

/**
 * Initializes the DAC hardware and SysTick interrupt to play 5.5-kHz sound.
 */
void InitSound(void);

/**
 * Plays the specified sound name (look at the enum).
 */
void PlaySound(enum SoundName sound);

/**
 * Stops playing a sound
 */
void StopSound(int sound);
