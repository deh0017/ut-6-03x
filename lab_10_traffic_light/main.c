/**
 * UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Lab 10: Traffic Light
 *
 * File Name: main.c
 *
 * Description:
 *     Simulates a traffic light system with three modes:
 *     Going South, Going West, and Pedestrians.
 *
 * Compatibility: EK-TM4C123GXL
 *
 * Author: Phi Luu
 * Location: Portland, Oregon, United States
 * Created: March 31, 2016
 * Updated: June 23, 2017
 */

/**
 * Required hardware I/O connections
 *
 * West's Red-Yellow-Green connected to PB5-PB4-PB3
 * South's Red-Yellow-Green connected to  PB2-PB1-PB0
 * Pedestrian's Red connected to PF1
 * Pedestrian's Green connected to PF3
 * West's switch connected to PE0
 * South's switch connected to PE1
 * Pedestrian's switch connected to PE2
 */

#include "TExaS.h"
#include "tm4c123gh6pm.h"

// Constant declarations to access port registers
// using symbolic names instead of addresses
// Port F
#define GPIO_PORTF_DATA_R     (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R      (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R    (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R      (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R      (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R     (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R       (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R    (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R     (*((volatile unsigned long *)0x4002552C))
// Port B
#define GPIO_PORTB_DATA_R     (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R      (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R    (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_PUR_R      (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_DEN_R      (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_LOCK_R     (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R       (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R    (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R     (*((volatile unsigned long *)0x4000552C))
// Port E
#define GPIO_PORTE_DATA_R     (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R      (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R    (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R      (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R      (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R     (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R       (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R    (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R     (*((volatile unsigned long *)0x4002452C))
// System Clock
#define SYSCTL_RCGC2_R        (*((volatile unsigned long *)0x400FE108))
// SysTick Timer
#define NVIC_ST_CTRL_R        (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R      (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R     (*((volatile unsigned long *)0xE000E018))

// Function prototypes
void EnableInterrupts(void);                    // enable interrupts
void PortsInit(void);                           // ports initialization
void SystickInit(void);                         // SysTick initialization
void SystickWait(unsigned long delay);          // SysTick wait function
void SystickWait1ms(unsigned long wait_time_ms); // SysTick delay function

// Struct declaration
struct FiniteStateMachine {                     // represents a state of the FSM
    unsigned short port_b_out;                  // ouput of Port B for the state (cars output)
    unsigned short port_f_out;                  // output of Port F for the state (pedestrian output)
    unsigned short wait;                        // time to wait when in this state
    unsigned char  next[5];                     // next state array
};

// Shortcuts to refer to the various states in the FSM array
#define GO_SOUTH        0
#define WAIT_SOUTH      1
#define GO_WEST         2
#define WAIT_WEST       3
#define GO_WALK         4
#define HURRY_WALK_1    5
#define OFF_WALK_1      6
#define HURRY_WALK_2    7
#define OFF_WALK_2      8
#define HURRY_WALK_3    9
#define OFF_WALK_3      10
#define NUM_STATES      11

// FSM declaration
typedef const struct FiniteStateMachine STATE[NUM_STATES] = {
    // 0) Go South
    {
        0x21, 0x02, 3000,
        { GO_SOUTH, GO_SOUTH, WAIT_SOUTH, WAIT_SOUTH, WAIT_SOUTH }
    },
    // 1) Wait South
    {
        0x22, 0x02,  500,
        { GO_WEST, GO_WEST, GO_WEST, GO_WALK, GO_WEST }
    },
    // 2) Go West
    {
        0x0C, 0x02, 3000,
        { GO_WEST, WAIT_WEST, GO_WEST, WAIT_WEST, WAIT_WEST }
    },
    // 3) Wait West
    {
        0x14, 0x02,  500,
        { GO_SOUTH, GO_SOUTH, GO_SOUTH, GO_WALK, GO_WALK }
    },
    // 4) Go Pedestrian
    {
        0x24, 0x08, 3000,
        { GO_WALK, HURRY_WALK_1, HURRY_WALK_1, GO_WALK, HURRY_WALK_1 }
    },
    // 5) Hurry Pedestrian 1
    {
        0x24, 0x02,  250,
        { OFF_WALK_1, OFF_WALK_1, OFF_WALK_1, OFF_WALK_1, OFF_WALK_1 }
    },
    // 6) Off Pedestrian 1
    {
        0x24, 0x00,  250,
        { HURRY_WALK_2, HURRY_WALK_2, HURRY_WALK_2, HURRY_WALK_2, HURRY_WALK_2 }
    },
    // 7) Hurry Pedestrian 2
    {
        0x24, 0x02,  250,
        { OFF_WALK_2, OFF_WALK_2, OFF_WALK_2, OFF_WALK_2, OFF_WALK_2 }
    },
    // 8) Off Pedestrian 2
    {
        0x24, 0x00,  250,
        { HURRY_WALK_3, HURRY_WALK_3, HURRY_WALK_3, HURRY_WALK_3, HURRY_WALK_3 }
    },
    // 9) Hurry Pedestrian 3:
    {
        0x24, 0x02,  250,
        { OFF_WALK_3, OFF_WALK_3, OFF_WALK_3, OFF_WALK_3, OFF_WALK_3 }
    },
    // 10) Off Pedestrian 3:
    {
        0x24, 0x00,  250,
        { GO_WEST, GO_SOUTH, GO_WEST, GO_SOUTH,  GO_SOUTH }
    }
};

// Global variables
unsigned char this_state;   // current state
unsigned char switch_input; // input from switches

int main(void) {
    // Setup
    // activate grader and set system clock to 80 MHz
    TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210, ScopeOff);
    PortsInit();           // Port B, Port F, and Port E initialization
    SystickInit();         // SysTick timer initialization
    this_state = GO_SOUTH; // GO_SOUTH is initial state
    EnableInterrupts();    // enable interrupts for grader

    // Loop
    while (1) {
        // make outputs
        GPIO_PORTB_DATA_R = FSM[this_state].port_b_out; // to cars (port B)
        GPIO_PORTF_DATA_R = FSM[this_state].port_f_out; // to pedestrians (port F)
        SystickWait1mss(FSM[this_state].wait);

        // get inputs
        // if no switch is pressed
        if (GPIO_PORTE_DATA_R == 0x00) {
            switch_input = 0; // then it is case 0 of the next[] array...
        }  // ... all LEDs stay the way they are since the last pressing
           // if south switch is pressed
        else if (GPIO_PORTE_DATA_R == 0x02) {
            switch_input = 1; // then it is case 1 of the next[] array...
        }  // ... all LEDs correspond to Go South mode
           // if west switch is pressed
        else if (GPIO_PORTE_DATA_R == 0x01) {
            switch_input = 2; // then it is case 2 of the next[] array...
        }  // ... all LEDs correspond to Go West mode
           // if pedestrian switch is pressed
        else if (GPIO_PORTE_DATA_R == 0x04) {
            switch_input = 3; // then it is case 3 of the next[] array...
        }  // ... all LEDs correspond to Go Pedestrian mode
           // if all switches are pressed
        else if (GPIO_PORTE_DATA_R == 0x07) {
            switch_input = 4; // then it is case 4 of the next[] array...
        }  // ... all LEDs correspond periodically: South, West, Pedestrian
           // change state based on input and current state
        this_state = FSM[this_state].next[switch_input];
    }
}

/**
 * Initializes port F-B-E pins for input and output
 */
void PortsInit(void) {
    // clock
    volatile unsigned long delay;

    // 1) activate clock for Port F, Port B, and Port E
    SYSCTL_RCGC2_R |= 0x00000032;
    delay           = SYSCTL_RCGC2_R; // allow time for clock to start
    // Port F
    GPIO_PORTF_LOCK_R  = 0x4C4F434B;  // 2) unlock GPIO Port F
    GPIO_PORTF_CR_R   |= 0x0A;        // allow changes to PF3, PF1
    GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
    GPIO_PORTF_PCTL_R  = 0x00;        // 4) PCTL GPIO on PF3, PF1
    GPIO_PORTF_DIR_R  |= 0x0A;        // 5) PF3, PF1 are outputs
    GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alternate function
    GPIO_PORTF_PUR_R   = 0x00;        // disable pull-up resistor
    GPIO_PORTF_DEN_R  |= 0x0A;        // 7) enable digital I/O on PF3, PF1
    // Port B
    GPIO_PORTB_LOCK_R  = 0x4C4F434B;  // 2) unlock GPIO Port B
    GPIO_PORTB_CR_R   |= 0x3F;        // allow changes to PB5-PB0
    GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog function
    GPIO_PORTB_PCTL_R  = 0x00;        // 4) PCTL GPIO on PB5-PB0
    GPIO_PORTB_DIR_R  |= 0x3F;        // 5) PB5-PB0 are outputs
    GPIO_PORTB_AFSEL_R = 0x00;        // 6) disable alternate function
    GPIO_PORTB_PUR_R   = 0x00;        // disable pull-up resistor
    GPIO_PORTB_DEN_R  |= 0x3F;        // 7) enable digital I/O on PB5-PB0
    // Port E
    GPIO_PORTE_LOCK_R  = 0x4C4F434B;  // 2) unlock GPIO Port E
    GPIO_PORTE_CR_R   |= 0x07;        // allow changes to PE2-PE0
    GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
    GPIO_PORTE_PCTL_R  = 0x00;        // 4) PCTL GPIO on PE2-PE0
    GPIO_PORTE_DIR_R   = 0x00;        // 5) PE2-PE0 are inputs
    GPIO_PORTE_AFSEL_R = 0x00;        // 6) disable alternate function
    GPIO_PORTE_PUR_R   = 0x00;        // disable pull-up resistor
    GPIO_PORTE_DEN_R  |= 0x07;        // 7) enable digital I/O on PE2-PE0
}

/**
 * Initializes SysTick timer
 */
void SystickInit(void) {
    NVIC_ST_CTRL_R    = 0;        // disable SysTick during set up
    NVIC_ST_RELOAD_R  = 0xFFFFFF; // maximum value to RELOAD register
    NVIC_ST_CURRENT_R = 0;        // overwrite to CURRENT to clear it
    NVIC_ST_CTRL_R    = 0x05;     // enable CLK_SRC bit and ENABLE bit
}

/**
 * Delays the program
 *
 * @param  delay  count value
 *
 * @assumption    80-MHz clock
 *
 * @notes         delay = Time_To_Delay_In_Seconds / 12.5 / 0.000000001
 */
void SystickWait(unsigned long delay) {
    NVIC_ST_RELOAD_R  = delay - 1; // number of counts to wait
    NVIC_ST_CURRENT_R = 0;         // overwrite the CURRENT register

    // wait until COUNT is flagged:
    while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {}
}

/**
 * Delays the program some milliseconds
 *
 * @param  wait_time_ms  milliseconds to delay
 *
 * @assumption          80-MHz clock
 */
void SystickWait1mss(unsigned long wait_time_ms) {
    unsigned long i;

    for (i = 0; i < wait_time_ms; i++) {
        // count = 0.001 / 12.5 / 0.000000001 = 80000
        // equivalent to 1 millisecond
        SystickWait(80000);
    }
}
