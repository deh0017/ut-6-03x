//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 7: Heart Block
//
// File Name: main.c
//
// Description:
//     SW1 is released: Green light turns on and waits
//     SW1 is held: Green light goes off
//     SW1 is released after being pressed: Red light goes on for 250ms,
//     then it turns off, and Green light turns on again.
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created March 09, 2016
// Updated December 28, 2016
//
//****************************************************************************

//**********1. Pre-processor Section**********
#include "TExaS.h"  // lab grader functions

// Constant declarations to access port registers
// using symbolic names instead of addresses
#define GPIO_PORTF_DATA_R		(*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R		(*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R		(*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R		(*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R		(*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R		(*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R			(*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R		(*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R		(*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R			(*((volatile unsigned long *)0x400FE108))

//**********2. Declarations Section**********
// Function Prototypes
void PortF_Init(void);              // port F initial function
void Delay1ms(unsigned long msec);  // delay function
void EnableInterrupts(void);        // enable interrupts
void WaitForASLow(void);            // sensor-off waiting function
void WaitForASHigh(void);           // sensor-on waiting function
void SetVT(void);                   // trigger setting function
void ClearVT(void);                 // trigger clearing function
void SetReady(void);                // indicator setting function
void ClearReady(void);              // indicator clearing function

//**********3. Subroutines Section**********
//----------PortF_Init----------
// Initializes port F pins for input and output
// Inputs: None
// Outputs: None
void PortF_Init(void) {
    volatile unsigned long delay;

    SYSCTL_RCGC2_R |= 0x00000020;   // 1) F clock
    delay = SYSCTL_RCGC2_R;         // delay to allow clock to stabilize
    GPIO_PORTF_LOCK_R = 0x4C4F434B; // 2) unlock Port F
    GPIO_PORTF_CR_R |= 0x1A;        // allow changes to PF4, PF3, PF1
    GPIO_PORTF_AMSEL_R = 0x00;      // 3) disable analog function
    GPIO_PORTF_PCTL_R = 0x00;       // 4) GPIO clear bit PCTL
    GPIO_PORTF_DIR_R |= 0x0A;       // 5) PF4 is input. PF3, PF1 are outputs
    GPIO_PORTF_AFSEL_R = 0x00;      // 6) no alternate function
    GPIO_PORTF_PUR_R |= 0x10;       // enable pullup resistor on PF4
    GPIO_PORTF_DEN_R |= 0x1A;       // 7) enable digital pins PF4, PF3, PF1
}
// Color        LED(s)    PortF
// dark         ---       0
// red          R--       0x02
// blue         --B       0x04
// green        -G-       0x08
// yellow       RG-       0x0A
// sky blue     -GB       0x0C
// white        RGB       0x0E

//----------WaitForASLow----------
// Reads AS input and waits for signal to be low
// If AS is already low, it returns right away
// If AS is currently high, it will wait until it to go low
// Inputs: None
// Outputs: None
void WaitForASLow(void) {
    unsigned long AS;   // variable for input from SW1

    do {
        AS = GPIO_PORTF_DATA_R & 0x10;  // read the status from SW1 repeatedly
    } while (!AS);                      // as long as SW1 is pressed
}

//----------WaitForASHigh----------
// Reads AS input and waits for signal to be high
// If AS is already high, it returns right away
// If AS is currently low, it will wait until it to go high
// Inputs: None
// Outputs: None
void WaitForASHigh(void) {
    unsigned long AS;   // variable for input from SW1

    do {
        AS = GPIO_PORTF_DATA_R & 0x10;  // read the status from SW1 repeatedly
    } while (AS);                       // as long as SW1 is released
}

//----------SetVT----------
// Sets VT high
// Inputs: None
// Outputs: None
void SetVT(void) {
    GPIO_PORTF_DATA_R ^= 0x02;  // turn the red LED on
}

//----------ClearVT----------
// Clears VT low
// Inputs: None
// Outputs: None
void ClearVT(void) {
    GPIO_PORTF_DATA_R = 0;      // turn the light off
}

//----------SetReady----------
// Sets Ready high
// Inputs: None
// Outputs: None
void SetReady(void) {
    GPIO_PORTF_DATA_R = 0x08;   // turn the green LED on
}

//----------ClearReady----------
// Clears Ready low
// Inputs: None
// Outputs: None
void ClearReady(void) {
    GPIO_PORTF_DATA_R = 0;      // turn the light off
}

//----------Delay1ms----------
// Delays in units of milliseconds
// Inputs: msec    number of milliseconds to delay
// Outputs: None
// Assumes: 80-MHz clock
void Delay1ms(unsigned long msec) {
    unsigned long count;      // declare count down variable

    while (msec) {
        count = 15913;  // 15913 for simulation and 11934 for the real board
        while (count) {
            count--;    // when counting down is finished, it takes 0.999875ms
        }
        msec--;         // count down the number of milliseconds you want
    }
}

//**********4. Main function**********
int main(void) {
    // setup:
    // activate grader and set system clock to 80 MHz
    TExaS_Init(SW_PIN_PF40, LED_PIN_PF31, ScopeOn);
    PortF_Init();           // port F initalization
    EnableInterrupts();     // enable interrupts for the grader

    // loop:
    while (1) {
        SetReady();         // ready signal goes high
        WaitForASHigh();    // wait for switch to be pressed
        ClearReady();       // ready signal goes low
        Delay1ms(10);       // wait 10ms
        WaitForASLow();     // wait for switch to be released
        Delay1ms(250);      // wait 250ms
        SetVT();            // trigger signal goes high
        Delay1ms(250);      // wait 250ms
        ClearVT();          // trigger signal goes low
    }
}
