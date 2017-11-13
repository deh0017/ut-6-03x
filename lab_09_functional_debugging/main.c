/**
 * @file     main.c
 * @author   Phi Luu
 * @date     March 25, 2016
 *
 * @brief    UTAustinX: UT.6.03x Embedded Systems - Shape the World
 *           Lab 09: Functional Debugging
 *
 * @section  DESCRIPTION
 *
 * A brief introduction to debug the circuit using high-precision oscilloscopes.
 *     Learn how to debug using Logic Analyzer
 *     When either of SW1 and SW2 is pressed, the LED should flash red 10 Hz
 *     Otherwise, turn of the LED
 *     When the LED is flashing red,
 *     dumb data of PF4-PF0 to data[] array for visual observing
 */

#include "TExaS.h"
#include "tm4c123gh6pm.h"

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
#define PF4                   (*((volatile unsigned long *)0x40025040))
#define PF3                   (*((volatile unsigned long *)0x40025020))
#define PF2                   (*((volatile unsigned long *)0x40025010))
#define PF1                   (*((volatile unsigned long *)0x40025008))
#define PF0                   (*((volatile unsigned long *)0x40025004))
#define GPIO_PORTF_DR2R_R     (*((volatile unsigned long *)0x40025500))
#define GPIO_PORTF_DR4R_R     (*((volatile unsigned long *)0x40025504))
#define GPIO_PORTF_DR8R_R     (*((volatile unsigned long *)0x40025508))
#define GPIO_LOCK_KEY         0x4C4F434B // unlock the GPIO_CR register
#define SYSCTL_RCGC2_R        (*((volatile unsigned long *)0x400FE108))
#define NVIC_ST_CURRENT_R     (*((volatile unsigned long *)0xE000E018))

// Global variables
unsigned long time_counter[50]; // array contains time
unsigned long data[50];         // array contains data

// Function prototypes
void SystickInit(void);
void PortFInit(void);
void Delay(void);
void EnableInterrupts(void); // enable interrupts

int main(void) {
    // Setup
    unsigned long i = 0, last = NVIC_ST_CURRENT_R;
    unsigned long now;

    // activate grader and set system clock to 16 MHz
    TExaS_Init(SW_PIN_PF40, LED_PIN_PF1);
    PortFInit();        // initialize PF1 to output
    SystickInit();      // initialize SysTick, runs at 16 MHz
    EnableInterrupts(); // enable interrupts for the grader

    // Loop
    while (1) {
        if (!PF4 || !PF0) { // if either SW1 or SW2 is pressed
            PF1 ^= 0x02;    // toggle the red LED

            if (i < 50) {
                now = NVIC_ST_CURRENT_R;
                time_counter[i] = (last - now) & 0x00FFFFFF; // 24-bit time difference
                data[i] = GPIO_PORTF_DATA_R & 0x13;          // record changes
                last = now;
                i++;
            }
        }
        Delay();
    }
}

/**
 * Initializes port F pins for input and output
 */
void PortFInit(void) {
    volatile unsigned long delay;

    SYSCTL_RCGC2_R |= 0x00000020;      // 1) activate clock for Port F
    delay = SYSCTL_RCGC2_R;            // allow time for clock to start
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY; // 2) unlock GPIO Port F
    GPIO_PORTF_CR_R |= 0x1F;           // allow changes to PF4-PF0
    GPIO_PORTF_AMSEL_R = 0x00;         // 3) disable analog function
    GPIO_PORTF_PCTL_R = 0x00;          // 4) PCTL GPIO on PF4-PF0
    GPIO_PORTF_DIR_R |= 0x0E;          // 5) PF4, PF0 inputs, PF3-PF1 outputs
    GPIO_PORTF_AFSEL_R = 0x00;         // 6) disable alternate function
    GPIO_PORTF_PUR_R |= 0x11;          // enable pull-up resistor on PF0 and PF4
    GPIO_PORTF_DEN_R |= 0x1F;          // 7) enable digital I/O on PF4-PF0
}

/**
 * Initializes the SysTick timer
 */
void SystickInit(void) {
    NVIC_ST_CTRL_R = 0;            // disable SysTick during setup
    NVIC_ST_RELOAD_R = 0x00FFFFFF; // maximum reload value
    NVIC_ST_CURRENT_R = 0;         // any write to current clears it
    NVIC_ST_CTRL_R = 0x00000005;   // enable SysTick with core clock
}

/**
 * Delays the program 50 millisecond (0.05 seconds)
 *
 * @assumption    80-MHz clock
 */
void Delay(void) {
    unsigned long volatile count;

    count = 72727; // 50 millisecond

    while (count) {
        count--;
    }
}
