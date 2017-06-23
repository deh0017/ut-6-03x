/**
 * UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Lab 7: Heart Block
 *
 * File Name: main.c
 *
 * Description:
 *     SW1 is released: Green light turns on and waits
 *     SW1 is held: Green light goes off
 *     SW1 is released after being pressed: Red light goes on for 250ms,
 *      then it turns off, and Green light turns on again.
 *
 * Compatibility: EK-TM4C123GXL
 *
 * Author: Phi Luu
 * Location: Portland, Oregon, United States
 * Created: March 09, 2016
 * Updated: June 22, 2017
 */

#include "TExaS.h" // lab grader functions

// Constant declarations to access port registers
// using symbolic names instead of addresses
#define GPIO_PORTF_DATA_R     (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R      (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R    (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R      (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R      (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R     (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R       (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R    (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R     (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R        (*((volatile unsigned long *)0x400FE108))

// Function prototypes
void PortFInit(void);             // port F initial function
void Delay1ms(unsigned long msec); // delay function
void EnableInterrupts(void);       // enable interrupts
void WaitForAsLow(void);           // sensor-off waiting function
void WaitForAsHigh(void);          // sensor-on waiting function
void SetVt(void);                  // trigger setting function
void ClearVt(void);                // trigger clearing function
void SetReady(void);               // indicator setting function
void ClearReady(void);             // indicator clearing function

int  main(void) {
    // Setup
    // activate grader and set system clock to 80 MHz
    TExaS_Init(SW_PIN_PF40, LED_PIN_PF31, ScopeOn);
    PortFInit();       // port F initalization
    EnableInterrupts(); // enable interrupts for the grader

    // Loop
    while (1) {
        SetReady();      // ready signal goes high
        WaitForAsHigh(); // wait for switch to be pressed
        ClearReady();    // ready signal goes low
        Delay1ms(10);    // wait 10ms
        WaitForAsLow();  // wait for switch to be released
        Delay1ms(250);   // wait 250ms
        SetVt();         // trigger signal goes high
        Delay1ms(250);   // wait 250ms
        ClearVt();       // trigger signal goes low
    }
}

/**
 * Initializes port F pins for input and output
 */
void PortFInit(void) {
    volatile unsigned long delay;

    SYSCTL_RCGC2_R    |= 0x00000020;     // 1) F clock
    delay              = SYSCTL_RCGC2_R; // delay to allow clock to stabilize
    GPIO_PORTF_LOCK_R  = 0x4C4F434B;     // 2) unlock Port F
    GPIO_PORTF_CR_R   |= 0x1A;           // allow changes to PF4, PF3, PF1
    GPIO_PORTF_AMSEL_R = 0x00;           // 3) disable analog function
    GPIO_PORTF_PCTL_R  = 0x00;           // 4) GPIO clear bit PCTL
    GPIO_PORTF_DIR_R  |= 0x0A;           // 5) PF4 is input. PF3, PF1 are outputs
    GPIO_PORTF_AFSEL_R = 0x00;           // 6) no alternate function
    GPIO_PORTF_PUR_R  |= 0x10;           // enable pullup resistor on PF4
    GPIO_PORTF_DEN_R  |= 0x1A;           // 7) enable digital pins PF4, PF3, PF1
}

/**
 * Reads AS input from SW1 and waits for signal to be low
 */
void WaitForAsLow(void) {
    unsigned long as;                  // variable for input from SW1

    do {
        as = GPIO_PORTF_DATA_R & 0x10; // read the status from SW1 repeatedly
    } while (!as);                     // as long as SW1 is pressed
}

/**
 * Reads AS input from SW1 and waits for signal to be high
 */
void WaitForAsHigh(void) {
    unsigned long as;                  // variable for input from SW1

    do {
        as = GPIO_PORTF_DATA_R & 0x10; // read the status from SW1 repeatedly
    } while (as);                      // as long as SW1 is released
}

/**
 * Sets VT high and turns the red LED (PF1) on
 */
void SetVt(void) {
    GPIO_PORTF_DATA_R ^= 0x02; // turn the red LED on
}

/**
 * Clears VT (all bits in port F)
 */
void ClearVt(void) {
    GPIO_PORTF_DATA_R = 0; // turn the light off
}

/**
 * Sets ready high and turns the green LED (PF3) on
 */
void SetReady(void) {
    GPIO_PORTF_DATA_R = 0x08; // turn the green LED on
}

/**
 * Clears ready (all bits in port F)
 */
void ClearReady(void) {
    GPIO_PORTF_DATA_R = 0; // turn the light off
}

/**
 * Delays in units of milliseconds
 *
 * @param  msec  number of milliseconds to delay
 *
 * @assumption   80-MHz clock
 */
void Delay1ms(unsigned long msec) {
    unsigned long count; // declare count down variable

    while (msec) {
        count = 15913;   // 15913 for simulation and 11934 for the real board

        while (count) {
            count--;     // when counting down is finished, it takes 0.999875ms
        }
        msec--;          // count down the number of milliseconds you want
    }
}
