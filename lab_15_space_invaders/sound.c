/**
 * @file     sound.c
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

#include "tm4c123gh6pm.h"
#include "data.h"
#include "sound.h"
#include "led.h"

int sound_enabled[MAX_SOUND] = { 0 };

// current DAC step
unsigned long dac_step[MAX_SOUND] = { 0 };

// maximum DAC step
const unsigned long DAC_STEP_MAX[MAX_SOUND] = {
    sizeof(WAV_EXPLOSION) / sizeof(WAV_EXPLOSION[0]),
    sizeof(WAV_FAST_INVADER) / sizeof(WAV_FAST_INVADER[0]),
    sizeof(WAV_INVADER_KILLED) / sizeof(WAV_INVADER_KILLED[0]),
    sizeof(WAV_SHOOT) / sizeof(WAV_SHOOT[0]),
    sizeof(WAV_UFO_HIGH_PITCH) / sizeof(WAV_UFO_HIGH_PITCH[0])
};

const unsigned char *WAV_DAC[] = {
    WAV_EXPLOSION,
    WAV_FAST_INVADER,
    WAV_INVADER_KILLED,
    WAV_SHOOT,
    WAV_UFO_HIGH_PITCH
};

/**
 * Initializes 4-bit DAC on PB3-PB0 and two LEDs on PB5-PB4.
 */
void InitDac(void) {
    volatile unsigned long delay;

    SYSCTL_RCGC2_R |= 0x02;          // unlock port B clock
    delay = SYSCTL_RCGC2_R;          // allow time for clock to start
    GPIO_PORTB_LOCK_R |= 0x4C4F434B; // unlock GPIO Port B
    GPIO_PORTB_CR_R |= DAC_PINS;     // allow change to PB3-PB0
    GPIO_PORTB_AMSEL_R &= ~DAC_PINS; // disable analog function on PB3-PB0
    GPIO_PORTB_PCTL_R = 0;           // clear PCTL register on Port B
    GPIO_PORTB_DIR_R |= DAC_PINS;    // PB3-PB0 outputs
    GPIO_PORTB_AFSEL_R &= ~DAC_PINS; // disable alternate function on PB3-PB0
    GPIO_PORTB_DEN_R |= DAC_PINS;    // enable digital I/O on PB3-PB0
    GPIO_PORTB_DR8R_R |= DAC_PINS;   // enable driver register for PB3-PB0
}

/**
 * Outputs a 4-bit data to DAC.
 */
void WriteDac(unsigned long data) {
    data = (data & (DAC_PINS)) | (GPIO_PORTB_DATA_R & ~(DAC_PINS));
    GPIO_PORTB_DACOUT_R = data;
}

/**
 * Initializes the DAC hardware and SysTick interrupt to play 5.5-kHz sound.
 */
void InitSound(void) {
    InitDac();
    NVIC_ST_CTRL_R = 0;
    // NVIC_ST_RELOAD_R = 7526; // 11 KHz sample rate
    NVIC_ST_RELOAD_R = 15052; // 5.5 KHz sample rate
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_CTRL_R = 7;
}

/**
 * Plays the specified sound name (look at the enum).
 */
void PlaySound(enum SoundName sound) {
    sound_enabled[sound] = 1;

    if (sound == INVADER_SOUND) {
        // turn on LED 1 on PB4 if the sound is the alien moving sound
        SetLed1(1);
    } else if (sound == SHOOT_SOUND) {
        // turn on LED 2 on PB5 if the sound is the shooting sound
        SetLed2(1);
    }

    DAC_step[sound] = 0;
}

/**
 * Stops playing a sound
 */
void StopSound(int sound) {
    sound_enabled[sound] = 0;

    if (sound == INVADER_SOUND) {
        // turn off LED 1 on PB4 if the sound is the alien moving sound
        SetLed1(0);
    } else if (sound == SHOOT_SOUND) {
        // turn off LED 2 on PB5 if the sound is the shooting sound
        SetLed2(0);
    }
}
