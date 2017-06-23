/**
 * UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Lab 13: Digital-Analog Conversion
 *
 * File Name: main.c
 *
 * Description: Creates a sinusoid sound wave using 4-bit DAC and simulate
 * C, D, E, and G notes of the piano.
 * Port B bits 3-0 have the 4-bit DAC.
 * Port E bits 3-0 have 4 piano keys.
 *
 * Author: Phi Luu
 * Location: Portland, Oregon, United States
 * Created: April 22, 2016
 * Updated: June 23, 2017
 */

/**
 * Required hardware I/O connections
 *
 * PD3 must be connected to DAC output
 * 8*R resistor DAC bit 0 on PB0 (least significant bit)
 * 4*R resistor DAC bit 1 on PB1
 * 2*R resistor DAC bit 2 on PB2
 * 1*R resistor DAC bit 3 on PB3 (most significant bit)
 * 4 switches connected to PE3-PE0
 */

#include "tm4c123gh6pm.h"
#include "sound.h"
#include "piano.h"
#include "TExaS.h"

// Function prototypes:
void EnableInterrupts(void); // Enable interrupts

int  main(void) {
    // Setup
    // for the real board grader to work
    // activate grader and set system clock to 80 MHz
    TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210, ScopeOn);
    InitSound();        // initialize SysTick and DAC
    InitPiano();        // initialize Switches
    EnableInterrupts(); // enable interrupts

    // Loop
    while (1) {
        PianoIn(); // read input from switches

        // if the index is out of range (no switch is pressed)
        if (freq_index == 4) {
            StopSound(); // turn off the sound
        }
        // otherwise
        else {
            // adjust the frequency similarly to C, D, E, G
            PlaySound(FREQ_SYSTICK_TO_WAVE[freq_index]);
        }
    }
}
