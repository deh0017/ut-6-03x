/**
 * @file     main.c
 * @author   Phi Luu
 * @date     March 05, 2016
 *
 * @brief    UTAustinX: UT.6.03x Embedded Systems - Shape the World
 *           Lab 02: Hello LaunchPad
 *
 * @section  DESCRIPTION
 *
 * If the left switch SW1 is...
 *     not pressed: the LED toggles blue-red.
 *     pressed: the LED toggles blue-green.
 */

/**
 * LaunchPad built-in hardware
 *
 * SW1 (left switch) is negative logic PF4 on the LaunchPad
 * SW2 (right switch) is negative logic PF0 on the LaunchPad
 * Red LED connected to PF1 on the LaunchPad
 * Blue LED connected to PF2 on the LaunchPad
 * Green LED connected to PF3 on the LaunchPad
 */

#include "TExaS.h" // grader library

// Constant declarations to access port registers
// using symbolic names instead of addresses
#define GPIO_PORTF_DATA_R     (*((volatile unsigned long*)0x400253FC))
#define GPIO_PORTF_DIR_R      (*((volatile unsigned long*)0x40025400))
#define GPIO_PORTF_AFSEL_R    (*((volatile unsigned long*)0x40025420))
#define GPIO_PORTF_PUR_R      (*((volatile unsigned long*)0x40025510))
#define GPIO_PORTF_DEN_R      (*((volatile unsigned long*)0x4002551C))
#define GPIO_PORTF_LOCK_R     (*((volatile unsigned long*)0x40025520))
#define GPIO_PORTF_CR_R       (*((volatile unsigned long*)0x40025524))
#define GPIO_PORTF_AMSEL_R    (*((volatile unsigned long*)0x40025528))
#define GPIO_PORTF_PCTL_R     (*((volatile unsigned long*)0x4002552C))
#define SYSCTL_RCGC2_R        (*((volatile unsigned long*)0x400FE108))

// Global variables
unsigned long switch_input; // input from PF4

// Function prototypes
void PortFInit(void);
void Delay(void);
void EnableInterrupts(void);

int main(void) {
    // Setup
    // initialize the TExaS grader lab 2
    TExaS_Init(SW_PIN_PF40, LED_PIN_PF321);
    PortFInit();        // initialize port F
    EnableInterrupts(); // the grader uses interrupts

    // Loop
    while (1) {
        switch_input = GPIO_PORTF_DATA_R & 0x10; // read PF4 into switch

        if (!switch_input) {                     // zero means SW1 is pressed
            GPIO_PORTF_DATA_R = 0x08;            // LED is green
        } else {                                 // 0x10 means SW1 is not pressed
            GPIO_PORTF_DATA_R = 0x02;            // LED is red
        }
        Delay();                                 // wait 0.1 sec
        GPIO_PORTF_DATA_R = 0x04;                // LED is blue
        Delay();                                 // wait 0.1 sec
    }
}

/**
 * Initializes port F pins for input and output.
 * PF4 and PF0 are input SW1 and SW2 respectively.
 * PF3, PF2, PF1 are outputs to the LED.
 * These five pins are built in the LaunchPad.
 */
void PortFInit(void) {
    volatile unsigned long delay;

    SYSCTL_RCGC2_R |= 0x00000020;   // 1) unlock F clock
    delay = SYSCTL_RCGC2_R;         // allow time for clock to start
    GPIO_PORTF_LOCK_R = 0x4C4F434B; // 2) unlock Port F
    GPIO_PORTF_CR_R = 0x1F;         // allow changes to PF4-0
    GPIO_PORTF_AMSEL_R = 0x00;      // 3) disable analog function
    GPIO_PORTF_PCTL_R = 0x00000000; // 4) clear bit PCTL
    GPIO_PORTF_DIR_R = 0x0E;        // 5) PF4, PF0 input; PF3, PF2, PF1 output
    GPIO_PORTF_AFSEL_R = 0x00;      // 6) no alternate function
    GPIO_PORTF_PUR_R = 0x11;        // enable pullup resistors on PF4, PF0
    GPIO_PORTF_DEN_R = 0x1F;        // 7) enable digital pins PF4-PF0
}

/**
 * Waits 0.1 sec.
 *
 * @assumption   80-MHz clock
 */
void Delay(void) {
    unsigned long volatile time;

    time = 727240 * 200 / 91; // 0.1sec

    while (time) {
        time--;
    }
}
