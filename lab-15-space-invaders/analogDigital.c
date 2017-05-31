/**
 * UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Lab 15: SpaceInvaders
 *
 * File Name: analogDigital.c
 *
 * Description:
 *    Functions that are responsible for Analog-Digital Conversion,
 * which reads input from the slide pot and convert to numerical result.
 *    Functions that are responsible for Digital-Analog Conversion,
 * which forms sinusoid waves to create different sounds.
 *
 * Compatibility: EK-TM4C123GXL
 *
 * Phi Luu
 * Portland, Oregon, United States
 * Created May 20, 2016
 * Updated May 30, 2017
 */

#include "analogDigital.h"
#include "data.h"
#include "tm4c123gh6pm.h"

/**
 * Sets up the ADC
 * Max sample rate: <= 125,000 samples/second
 * SS3 triggering event: software trigger
 * SS3 1st sample source: channel 1
 * SS3 interrupts: enabled but not promoted to controller
 */
void ADC0_Init(void) {
    volatile unsigned long delay;
    unsigned char PORTEPINS  = POTPIN | LSWITCHPIN | RSWITCHPIN;
    unsigned char SWITCHPINS = LSWITCHPIN | RSWITCHPIN;

    // GPIO Port E Initialization:
    SYSCTL_RCGC2_R     |= 0x10;           // enable port E clock
    delay               = SYSCTL_RCGC2_R; // allow time for clock to start
    GPIO_PORTE_LOCK_R  |= 0x4C4F434B;     // unlock GPIO port E
    GPIO_PORTE_CR_R    |= PORTEPINS;      // allow change to PE2-PE0
    GPIO_PORTE_DIR_R   &= ~PORTEPINS;     // PE2-PE0 input
    GPIO_PORTE_AFSEL_R &= ~SWITCHPINS;    // disable alternate function on PE1-PE0
    GPIO_PORTE_AFSEL_R |= POTPIN;         // enable alternate function on PE2
    GPIO_PORTE_PCTL_R   = 0;              // disable PCTL on port E
    GPIO_PORTE_DEN_R   &= ~POTPIN;        // disable digital I/O on PE2
    GPIO_PORTE_DEN_R   |= SWITCHPINS;     // enable digital I/O on PE1-PE0
    GPIO_PORTE_AMSEL_R &= ~SWITCHPINS;    // disable analog function on PE1-PE0
    GPIO_PORTE_AMSEL_R |= POTPIN;         // enable analog function on PE2
    // Analog Initialization:
    SYSCTL_RCGC0_R |= 0x10000;            // enable the ADC clock
    delay           = SYSCTL_RCGC0_R;     // allow time for clock to start
    SYSCTL_RCGC0_R &= ~0x300;             // configure for 125K samples/second
    ADC0_SSPRI_R   &= ~0x3000;            // Highest Priority = 0
    ADC0_ACTSS_R   &= ~0x08;              // disable sample sequencer 3
    ADC0_EMUX_R    &= ~0xF000;            // use software trigger - SysTick
    ADC0_SSMUX3_R  |= 0x01;               // choose Ain channel 1
    ADC0_SSCTL3_R  |= 0x06;               // clear TS0, D0; set IE0, END0
    ADC0_ACTSS_R   |= 0x08;               // enable sample sequencer 3
}

/**
 * Busy-wait Analog-Digital conversion (ADC)
 *
 * @return    12-bit result of ADC conversion
 */
unsigned long ADC0_In(void) {
    unsigned long result;

    ADC0_PSSI_R |= 0x08; // initiate SS3

    // wait until conversion is done:
    while ((ADC0_RIS_R & 0x08) == 0) {}
    result      = ADC0_SSFIFO3_R & 0xFFF; // mask with 12-bit binary
    ADC0_ISC_R |= 0x08;                   // clear the flag to start capturing another sample
    return result;
}

/**
 * Initializes 4-bit DAC on PB3-PB0 and two LEDs on PB5-PB4
 */
void DAC_Init(void) {
    volatile unsigned long delay;
    unsigned char PORTBPINS = DACPINS | LLEDPIN | RLEDPIN;

    SYSCTL_RCGC2_R     |= 0x02;           // unlock port B clock
    delay               = SYSCTL_RCGC2_R; // allow time for clock to start
    GPIO_PORTB_LOCK_R  |= 0x4C4F434B;     // unlock GPIO Port B
    GPIO_PORTB_CR_R    |= PORTBPINS;      // allow change to PB5-PB0
    GPIO_PORTB_AMSEL_R &= ~PORTBPINS;     // disable analog function on PB5-PB0
    GPIO_PORTB_PCTL_R   = 0;              // clear PCTL register on Port B
    GPIO_PORTB_DIR_R   |= PORTBPINS;      // PB5-PB0 outputs
    GPIO_PORTB_AFSEL_R &= ~PORTBPINS;     // disable alternate function on PB5-PB0
    GPIO_PORTB_DEN_R   |= PORTBPINS;      // enable digital I/O on PB5-PB0
}

/**
 * Outputs to DAC
 *
 * @param  data  4-bit data
 */
void DAC_Out(unsigned short data) {
    GPIO_PORTB_DACOUT_R = data;
}

unsigned short DACIndex = 0;

/**
 * Plays a sound when the ship shoots
 */
void PlaySound_Shoot(void) {
    DACIndex = (DACIndex + 1) % SHOOTSIZE;
    DAC_Out(shoot[DACIndex]);
}

/**
 * Plays a sound when the ship is destroyed
 */
void PlaySound_Explosion(void) {
    DACIndex = (DACIndex + 1) % EXPLOSIONSIZE;
    DAC_Out(explosion[DACIndex]);
}

// ***
// Play a sound when a sprite is killed
// ***
void PlaySound_InvaderKilled(void) {
    DACIndex = (DACIndex + 1) % INVADERKILLEDSIZE;
    DAC_Out(invaderkilled[DACIndex]);
}

/**
 * Plays a sound when a sprite moves
 */
void PlaySound_FastInvader1(void) {
    DACIndex = (DACIndex + 1) % FASTINVADER1SIZE;
    DAC_Out(fastinvader1[DACIndex]);
}

// /**
// * Plays a sound when a sprite moves
// */
// void PlaySound_FastInvader2(void) {
// DACIndex = (DACIndex + 1)%FASTINVADER2SIZE;
// DAC_Out(fastinvader2[DACIndex]);
// }
//
// /**
// * Plays a sound when a sprite moves
// */
// void PlaySound_FastInvader3(void) {
// DACIndex = (DACIndex + 1)%FASTINVADER3SIZE;
// DAC_Out(fastinvader3[DACIndex]);
// }
//
// /**
// * Plays a sound when a sprite moves
// */
// void PlaySound_FastInvader4(void) {
// DACIndex = (DACIndex + 1)%FASTINVADER4SIZE;
// DAC_Out(fastinvader4[DACIndex]);
// }

/**
 * Plays a sounds when the mothership arrives
 */
void PlaySound_HighPitch(void) {
    DACIndex = (DACIndex + 1) % HIGHPITCHSIZE;
    DAC_Out(highpitch[DACIndex]);
}

/**
 * Plays a sound when the mothership is destroyed
 */
void PlaySound_SmallExplosion(void) {
    DACIndex = (DACIndex + 1) % SMALLEXPLOSIONSIZE;
    DAC_Out(smallexplosion[DACIndex]);
}
