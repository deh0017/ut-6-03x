/*******************************************************************************

UTAustinX: UT.6.03x Embedded Systems - Shape the World
Lab 8: Switch & LED Interface

Name: main.c

Description:
    The first lab building the circuit on the breadboard
    The switch is pressed: The LED flashes 5 Hz
    The switch is released: The LED is steadily on.

Compatibility: EK-TM4C123GXL

Phi Luu
David Douglas High School
Portland, OR
July 14, 2016

*******************************************************************************/

//**********Required Hardware I/O Connections**********
// Switch connected to PE0
// LED connected to PE1

//**********1. Pre-processor Directives Section**********
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// Constant declarations to access port registers
// using symbolic names instead of addresses
#define GPIO_PORTE_DATA_R           (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R            (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R          (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R            (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R            (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R           (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R             (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R          (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R           (*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R              (*((volatile unsigned long *)0x400FE108))
#define PE0                         (*((volatile unsigned long *)0x40024004))
#define PE1                         (*((volatile unsigned long *)0x40024008))

//**********2. Global Declarations Section**********
// Function Prototypes
void DisableInterrupts(void);         // disable interrupts
void EnableInterrupts(void);          // enable interrupts
void PortE_Init(void);                // port E initialization
void delay(unsigned long ms);         // delay ms milliseconds

//**********3. Subroutines Section**********
//----------PortE_Init----------
// Initializes port E pins for input and output
// Inputs: None
// Outputs: None
void PortE_Init(void) {
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000010;     // 1) E clock
    delay = SYSCTL_RCGC2_R;           // delay
    GPIO_PORTE_LOCK_R = 0x4C4F434B;   // 2) unlock PortE
    GPIO_PORTE_CR_R |= 0x03;          // allow changes to PE1, PE0
    GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
    GPIO_PORTE_PCTL_R = 0x00;         // 4) GPIO clear bit PCTL
    GPIO_PORTE_DIR_R |= 0x02;         // 5) PE0 input, PE1 output
    GPIO_PORTE_AFSEL_R = 0x00;        // 6) no alternate function
    GPIO_PORTE_DEN_R |= 0x03;         // 7) enable digital pins PE1, PE0
}

//----------delay----------
// Delays in units of milliseconds
// Inputs: ms    Number of milliseconds to delay
// Outputs: None
// Assumes: 80-MHz clock
void delay(unsigned long ms) {
    unsigned long count;
    while (ms) {
      count = 15998;  // approximately 16000 to delay 1 ms
      while (count) {
        count--;
      }
     ms--;
    }
}

//**********4. Main function**********
int main(void) {
    // set up:
    // activate grader and set system clock to 80 MHz
    TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);
    PortE_Init();         // initialize port E
    EnableInterrupts();   // enable interrupts for the grader
    PE1 = 0x02;           // start with LED on
    
    // loop:
    while (1) {
        delay(100);         // wait 100 ms
        if (PE0 == 0x00) {  // if the switch is not pressed
            PE1 = 0x02;     // turn on the LED steadily
        }
        else {              // if the switch is pressed:
            PE1 ^= 0x02;    // toggle the LED
        }
    }
}
