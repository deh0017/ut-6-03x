/*******************************************************************************

UTAustinX: UT.6.03x Embedded Systems - Shape the World
Lab 4: I/O

Name: IO.c

Description:
    SW1 is pressed: Turn on red LED
    SW2 is pressed: Turn on green LED
    Both SW1 and SW2 are pressed: Turn on blue LED
    No switch is pressed: Turn off all LED

Compatibility: EK-TM4C123GXL

Phi Luu
David Douglas High School
Portland, OR
July 03, 2016

*******************************************************************************/

//**********1. Pre-processor Directives Section**********
#include "TExaS.h"    // lab grader functions

// Constant declarations to access port registers
// using symbolic names instead of addresses
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

//**********2. Declarations Section**********
// Global Variables
unsigned long SW1, SW2;   // inputs from PF4, PF0
unsigned long Out;        // outputs to PF3, PF2, PF1 (multicolor LED)

// Function Prototypes
void PortF_Init(void);          // port F initial fuction
void Delay(void);               // delay function
void EnableInterrupts(void);    // enable interrupts

//**********3. Subroutines Section**********
//----------PortF_Init----------
// Initializes port F pins for input and output
// Inputs: None
// Outputs: None
void PortF_Init(void) {
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
    delay = SYSCTL_RCGC2_R;           // delay
    GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock Port F
    GPIO_PORTF_CR_R |= 0x1F;          // allow changes to PF4-PF0
    GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
    GPIO_PORTF_PCTL_R = 0x00;   // 4) GPIO clear bit PCTL
    GPIO_PORTF_DIR_R |= 0x0E;   // 5) PF4, PF0 inputs. PF3, PF2, PF1 outputs
    GPIO_PORTF_AFSEL_R = 0x00;  // 6) no alternate function
    GPIO_PORTF_PUR_R |= 0x11;   // enable pullup resistors on PF4, PF0
    GPIO_PORTF_DEN_R |= 0x1F;   // 7) enable digital pins PF4-PF0
}
// Color        LED(s)    PortF
// dark         ---       0x00
// red          R--       0x02
// blue         --B       0x04
// green        -G-       0x08
// yellow       RG-       0x0A
// sky blue     -GB       0x0C
// white        RGB       0x0E
// pink         R-B       0x06

//**********4. Main function**********
int main(void) {
    // setup:
    // initializes the real board grader for lab 4
    TExaS_Init(SW_PIN_PF40, LED_PIN_PF321);
    PortF_Init();           // initialize PF4, PF3, PF2, PF1, PF0
    EnableInterrupts();     // the grader uses interrupts
    
    // loop:
    while (1) {
        SW1 = GPIO_PORTF_DATA_R & 0x10;   // read PF4 into SW1
        SW2 = GPIO_PORTF_DATA_R & 0x01;   // read PF0 into SW2
        // SW1 and SW2 are negative logic
        if (SW1 && SW2) {                 // neither switch
            GPIO_PORTF_DATA_R = 0x00;     // no light
        }
        else {                           
            if (SW1 && (!SW2)) {            // only SW2 is pressed
                GPIO_PORTF_DATA_R = 0x08;   // LED is green
            }
            else {                        
                if ((!SW1) && SW2) {        // only SW1 is pressed
                    GPIO_PORTF_DATA_R = 0x02;   // LED is red
                }
                else {                        // both SW1 and SW2 are pressed
                    GPIO_PORTF_DATA_R = 0x04; // LED is blue
                }
            }
        }
    }
}
