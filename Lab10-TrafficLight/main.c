/*****************************************************************************

UTAustinX: UT.6.03x Embedded Systems - Shape the World
Lab 10: Traffic Light

File Name: main.c

Description:
    Simulate a traffic light system with three modes:
    Going South, Going West, and Pedestrians

Compatibility: EK-TM4C123GXL

Phi Luu
Portland, Oregon, United States
Created March 31, 2016
Updated July 17, 2016

*****************************************************************************/

//**********Required Hardware I/O Connections**********
// West's Red-Yellow-Green connected to PB5-PB4-PB3
// South's Red-Yellow-Green connected to  PB2-PB1-PB0
// Pedestrian's Red connected to PF1
// Pedestrian's Green connected to PF3
// West's switch connected to PE0
// South's switch connected to PE1
// Pedestrian's switch connected to PE2

//**********1. Pre-processor Section**********
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// Constant declarations to access port registers
// using symbolic names instead of addresses
// Port F
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
// Port B
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
// Port E
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
// System Clock
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
// SysTick Timer
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))

//**********2. Global Declarations Section**********
// Function Prototypes
void DisableInterrupts(void);         // disable interrupts
void EnableInterrupts(void);          // enable interrupts
void Ports_Init(void);                // ports initialization
void SysTick_Init(void);              // SysTick initialization
void SysTick_Wait(unsigned long delay);   // SysTick wait function
void SysTick_Wait1m(unsigned long howManyTime); // SysTick delay function

// Struct Declaration
struct State {           // represents a state of the FSM
    unsigned short outB; // ouput of Port B for the state (cars output)
    unsigned short outF; // output of Port F for the state (pedestrian output)
    unsigned short wait; // time to wait when in this state
    unsigned char next[5];  // next state array
};

// Shortcuts to refer to the various states in the FSM array
#define GoS        0
#define WaitS      1
#define GoW        2
#define WaitW      3
#define GoP        4
#define HurryP1    5
#define OffP1      6
#define HurryP2    7
#define OffP2      8
#define HurryP3    9
#define OffP3     10

// FSM Declaration
typedef const struct State StateType;
StateType FSM[11] = {
    // 0) Go South:
    {0x21, 0x02, 3000,  {GoS, GoS, WaitS, WaitS, WaitS}},
    // 1) Wait South:
    {0x22, 0x02, 500,   {GoW, GoW, GoW, GoP, GoW}},
    // 2) Go West:
    {0x0C, 0x02, 3000,  {GoW, WaitW, GoW, WaitW, WaitW}},
    // 3) Wait West:
    {0x14, 0x02, 500,   {GoS, GoS, GoS, GoP, GoP}},
    // 4) Go Pedestrian:
    {0x24, 0x08, 3000,  {GoP, HurryP1, HurryP1, GoP, HurryP1}},
    // 5) Hurry Pedestrian 1:
    {0x24, 0x02, 250,   {OffP1, OffP1, OffP1, OffP1, OffP1}},
    // 6) Off Pedestrian 1:
    {0x24, 0x00, 250,   {HurryP2, HurryP2, HurryP2, HurryP2, HurryP2}},
    // 7) Hurry Pedestrian 2:
    {0x24, 0x02, 250,   {OffP2, OffP2, OffP2, OffP2, OffP2}},
     // 8) Off Pedestrian 2:
    {0x24, 0x00, 250,   {HurryP3, HurryP3, HurryP3, HurryP3, HurryP3}},
    // 9) Hurry Pedestrian 3:
    {0x24, 0x02, 250,   {OffP3, OffP3, OffP3, OffP3, OffP3}},
    // 10) Off Pedestrian 3:
    {0x24, 0x00, 250,   {GoW, GoS, GoW, GoS, GoS}}
};

// Global Variables
unsigned char currentState;       // current state
unsigned char input;              // input from switches

//**********3. Subroutines Section**********
//----------Ports_Init----------
// Initializes port F-B-E pins for input and output
// Inputs: None
// Outputs: None
void Ports_Init(void) {
    // clock
    volatile unsigned long delay;
    // 1) activate clock for Port F, Port B, and Port E
    SYSCTL_RCGC2_R |= 0x00000032;
    delay = SYSCTL_RCGC2_R;   // allow time for clock to start
    // Port F
    GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
    GPIO_PORTF_CR_R |= 0x0A;          // allow changes to PF3, PF1
    GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
    GPIO_PORTF_PCTL_R = 0x00;         // 4) PCTL GPIO on PF3, PF1
    GPIO_PORTF_DIR_R |= 0x0A;         // 5) PF3, PF1 are outputs
    GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alternate function
    GPIO_PORTF_PUR_R = 0x00;          // disable pull-up resistor
    GPIO_PORTF_DEN_R |= 0x0A;         // 7) enable digital I/O on PF3, PF1
    // Port B
    GPIO_PORTB_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port B
    GPIO_PORTB_CR_R |= 0x3F;          // allow changes to PB5-PB0
    GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog function
    GPIO_PORTB_PCTL_R = 0x00;         // 4) PCTL GPIO on PB5-PB0
    GPIO_PORTB_DIR_R |= 0x3F;         // 5) PB5-PB0 are outputs
    GPIO_PORTB_AFSEL_R = 0x00;        // 6) disable alternate function
    GPIO_PORTB_PUR_R = 0x00;          // disable pull-up resistor
    GPIO_PORTB_DEN_R |= 0x3F;         // 7) enable digital I/O on PB5-PB0
    // Port E
    GPIO_PORTE_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port E
    GPIO_PORTE_CR_R |= 0x07;          // allow changes to PE2-PE0
    GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
    GPIO_PORTE_PCTL_R = 0x00;         // 4) PCTL GPIO on PE2-PE0
    GPIO_PORTE_DIR_R = 0x00;          // 5) PE2-PE0 are inputs
    GPIO_PORTE_AFSEL_R = 0x00;        // 6) disable alternate function
    GPIO_PORTE_PUR_R = 0x00;          // disable pull-up resistor
    GPIO_PORTE_DEN_R |= 0x07;         // 7) enable digital I/O on PE2-PE0
}

//----------SysTick_Init----------
// Initializes SysTick timer
// Inputs: None
// Outputs: None
void SysTick_Init(void) {
    NVIC_ST_CTRL_R = 0;           // disable SysTick during set up
    NVIC_ST_RELOAD_R = 0xFFFFFF;  // maximum value to RELOAD register
    NVIC_ST_CURRENT_R = 0;        // overwrite to CURRENT to clear it
    NVIC_ST_CTRL_R = 0x05;        // enable CLK_SRC bit and ENABLE bit
}

//----------SysTick_Wait----------
// Delays the program
// Inputs: delay    count value
// Outputs: None
// Assumes: 80-MHz clock
// Notes: delay = Time_To_Delay_In_Seconds/12.5/0.000000001
void SysTick_Wait(unsigned long delay) {
    NVIC_ST_RELOAD_R = delay - 1;     // number of counts to wait
    NVIC_ST_CURRENT_R = 0;            // overwrite the CURRENT register
    // wait until COUNT is flagged:
    while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {
    }
}

//----------SysTick_Wait1ms----------
// Delays the program some ms
// Inputs: howManyTime      milliseconds to delay
// Outputs: None
// Assumes: 80-MHz clock
void SysTick_Wait1ms(unsigned long howManyTime) {
    unsigned long i;
    for (i = 0; i < howManyTime; i++) {
        // count = 0.001/12.5/0.000000001 = 80000 <-> 1 milliseconds
        SysTick_Wait(80000);
    }
}

//**********4. Main function**********
int main(void) {
    //set up:
    // activate grader and set system clock to 80 MHz
    TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff);
    Ports_Init();         // Port B, Port F, and Port E initialization
    SysTick_Init();       // SysTick timer initialization
    currentState = GoS;   // GoS is initial state
    EnableInterrupts();   // enable interrupts for grader

    // loop:
    while(1){
      // output based on current state
      // output to cars (port B):
      GPIO_PORTB_DATA_R = FSM[currentState].outB;
      // output to pedestrians (port F)
      GPIO_PORTF_DATA_R = FSM[currentState].outF;
      // wait for time relevant to state
      SysTick_Wait1ms(FSM[currentState].wait);
      // get input
      // if no switch is pressed
      if (GPIO_PORTE_DATA_R == 0x00) {
        input = 0;  // then it is case 0 of the next[] array...
      }   // ... all LEDs stay the way they are since the last pressing
      // if south switch is pressed
      else if (GPIO_PORTE_DATA_R == 0x02) {
        input = 1;  // then it is case 1 of the next[] array...
      }   // ... all LEDs correspond to Go South mode
      // if west switch is pressed
      else if (GPIO_PORTE_DATA_R == 0x01) {
        input = 2;  // then it is case 2 of the next[] array...
      }   // ... all LEDs correspond to Go West mode
      // if pedestrian switch is pressed
      else if (GPIO_PORTE_DATA_R == 0x04) {
        input = 3;  // then it is case 3 of the next[] array...
      }   // ... all LEDs correspond to Go Pedestrian mode
      // if all switches are pressed
      else if (GPIO_PORTE_DATA_R == 0x07) {
        input = 4;  // then it is case 4 of the next[] array...
      }   // ... all LEDs correspond periodically: South, West, Pedestrian
      // change state based on input and current state
      currentState = FSM[currentState].next[input];
    }
}
