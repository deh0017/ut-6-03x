/*****************************************************************************

UTAustinX: UT.6.03x Embedded Systems - Shape the World
Lab 12: Tuning Fork

File Name: main.c

Description: Using SysTick periodic interrupt, create a frequency of 440Hz
just as you do when striking a tuning fork.

Compatibility: EK-TM4C123GXL

Phi Luu
Portland, Oregon, United States
Created April 16, 2016
Updated July 22, 2016

*****************************************************************************/

//**********Required Hardware I/O Connections**********
// Switch connected to PA3
// Radio Jack connected to PA2

//**********1. Pre-processor Section**********
#include "TExaS.h"

// Constant declarations to access port registers
// using symbolic names instead of addresses
// Port A
#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_PUR_R        (*((volatile unsigned long *)0x40004510))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_LOCK_R       (*((volatile unsigned long *)0x40004520))
#define GPIO_PORTA_CR_R         (*((volatile unsigned long *)0x40004524))
#define GPIO_PORTA_AMSEL_R      (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile unsigned long *)0x4000452C))
// System Clock
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
// SysTick Timer
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))
#define NVIC_SYS_PRI3_R         (*((volatile unsigned long *)0xE000ED20))

//**********2. Global Declarations Section**********
// Function Prototypes
void DisableInterrupts(void);   // Disable interrupts
void EnableInterrupts(void);    // Enable interrupts
void WaitForInterrupt(void);    // low power mode

// Global Variables
unsigned short Switch = 0;      // input from the switch is stored here

//**********3. Subroutines Section**********
//----------Sound_Init----------
// Initializes Port A and SysTick interrupts
// Inputs: None
// Outputs: None
void Sound_Init(void) {
    // Port A Initialization:
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000001;   // unlock port A clock
    delay = SYSCTL_RCGC2_R;         // allow time for clock to start
    GPIO_PORTA_LOCK_R |= 0x4C4F434B;// unlock GPIO Port A
    GPIO_PORTA_CR_R |= 0x0C;        // allow changes to PA3-PA2
    GPIO_PORTA_AMSEL_R &= ~0x0C;    // disable analog function on PA3-PA2
    GPIO_PORTA_PCTL_R = 0;          // clear PCTL register on PA3-PA2
    GPIO_PORTA_DIR_R |= 0x04;       // PA2 output, PA3 input
    GPIO_PORTA_AFSEL_R &= ~0x0C;    // disable alternate function on PA3-PA2
    GPIO_PORTA_PUR_R &= ~0x0C;      // disable pull-up resistors on PA3-PA2
    GPIO_PORTA_DEN_R |= 0x0C;       // enable digital I/O on PA3-PA2

    // SysTick Initialization for periodic interrupts:
    NVIC_ST_CTRL_R = 0;             // disable SysTick during set up
    NVIC_ST_RELOAD_R = 90908;       // reload value for 440Hz
    NVIC_ST_CURRENT_R = 0;          // clear current
    NVIC_SYS_PRI3_R &= 0x00FFFFFF;  // priority 0
    NVIC_ST_CTRL_R = 0x07;          // enable with core clock and interrupts
}

//----------SysTick_Handler----------
// Interrupt Service Routine. Called every 880 Hz
// Inputs: None
// Outputs: None
void SysTick_Handler(void) {
    GPIO_PORTA_DATA_R ^= 0x04;      // toggle PA2 (Audio Jack)
}

//**********4. Main Function**********
int main(void) {
    // set up:
    unsigned long NumberOfPresses = 0;   // store the number of presses
    unsigned long LastInput = 0; // store the last input of Switch here
    // use this to avoid miscounting when the switch is HOLD
    // if Switch != LastInput, then it is considered ONE MORE PRESS
    // if the current input and the last input are the same, then
    // it is a HOLD, and it is NOT counted as ONE MORE PRESS

    // activate grader and set system clock to 80 MHz
    TExaS_Init(SW_PIN_PA3, HEADPHONE_PIN_PA2, ScopeOn);
    Sound_Init();         // initial steps
    EnableInterrupts();   // enable after all initialization are done

    // loop:
    while(1) {
        // perform other tasks - Read the input over and over again:
        Switch = GPIO_PORTA_DATA_R & 0x08;  // read the switch from PA3
        if (Switch != 0 && Switch != LastInput) {
        // if the switch is pressed, and last time it was released:
            // the number of the ACTUAL press increase by 1
            NumberOfPresses++;
          }
        LastInput = Switch;       // save the current input for next time
        if (NumberOfPresses%2 == 0) {
        // if the number of press is even
        // (those presses at the 0, 2nd, 4th, 6th,... time)
            // then turn off PA2
            GPIO_PORTA_DATA_R &= ~0x04;
        }

        else {
        // if the number of press is odd
        // (those presses at the 1st, 3rd, 5th, 7th,... time)
            // then periodically interrupt the system each 880 Hz
            WaitForInterrupt();
        // the SysTick will automatically
        // trigger itself every (1 s)/(880 Hz) = 1.13636 ms
        }
    }
}
