//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 14: Analog-Digital Conversion
//
// File Name: main.c
//
// Description: Capture analog signals of the slide potentiometer
// and convert to digital signal, which is displayed by the Nokia5110
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created May 07, 2016
// Updated August 13, 2016
//
//****************************************************************************

//**********Required Hardware I/O Connections**********
// Slide pot pin 1 connected to GND
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V
// Switch connected to PF4
// Red LED built in PF2

//**********1. Pre-processor Section**********
#include "ADC.h"
#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "TExaS.h"

//**********2. Global Declarations Section**********
// Function Prototypes
void EnableInterrupts(void);

// Global Variables
unsigned char String[10]; // null-terminated ASCII string
unsigned long Distance;   // unit of 0.001 cm
unsigned long ADCdata;    // 12-bit 0 to 4095 sample
unsigned long Flag;       // 1 = valid Distance, 0 = empty Distance

//**********3. Subroutines Section**********
//----------Convert----------
// Converts a 12-bit binary ADC sample into a 32-bit unsigned
// fixed-point distance (resolution 0.001 cm).  Calibration
// data is gathered using known distances and reading the
// ADC value measured on PE2.
// Inputs: sample    12-bit ADC sample
// Outputs: 32-bit distance (resolution 0.001cm)
unsigned long Convert(unsigned long sample) {
    // Truth        ADCdata
    // 500              471
    // 1000            1220
    // 1500            1976

    // slope_1 = (1000 - 500)/(1220 - 471) = 0.668
    // slope_2 = (1500 - 1000)/(1976 - 1220) = 0.661
    // slope_average = (0.668 + 0.661)/2 = 0.6645

    // offset_1 = 500 - 0.6645*471 = 187.0205
    // offset_2 = 1000 - 0.6645*1220 = 189.31
    // offset_3 = 1500 - 0.6645*1976 = 186.948

    // offset_average = (187.0205 + 189.31 + 186.948)/3 = 188

    // B = 188
    // A = 0.6645*1024 = 680

    unsigned long result;
    // Distance = ((A*ADCdata) >> 10) + B
    result = ((680*sample) >> 10) + 188;
    return result;
}

//----------SysTick_Init----------
// Initializes SysTick interrupts to trigger at 40 Hz, 25 ms
// Inputs: None
// Outputs: None
// Assumes: 80-MHz clock
void SysTick_Init() {
    NVIC_ST_CTRL_R = 0;         // disable SysTick during startup
    NVIC_ST_RELOAD_R = 1999999; // trigger every 25ms
    // period = 1/(40Hz) * (80,000,000Hz) = 2,000,000
    // period - 1 = 1,999,999
    NVIC_ST_CURRENT_R = 0 ;     // any write to CURRENT clear it
    NVIC_ST_CTRL_R |= 0x05;     // set bits CLK_SRC and ENABLE of SysTick
    // priority 1
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000;
    NVIC_ST_CTRL_R |= 0x07;     // set bits CLK_SRC, INTEN, and ENABLE
}

//----------SysTick_Handler----------
// Executes every 25 ms, collects a sample, converts and stores in mailbox
// Inputs: None
// Ouputs: None
// Assumes: 80-MHz clock
void SysTick_Handler(void) {
    GPIO_PORTF_DATA_R ^= 0x04;      // toggle PF2
    GPIO_PORTF_DATA_R ^= 0x04;      // toggle PF2 again
    ADCdata = ADC0_In();            // sample the ADC
    Distance = Convert(ADCdata);    // convert to Distance
    Flag = 1;                       // set the flag - new data is ready
    GPIO_PORTF_DATA_R ^= 0x04;      // toggle PF2 the third time
}

//----------UART_ConvertDistance----------
// Converts a 32-bit distance into an ASCII string
// and store the conversion in global variable String[10]
// Inputs: n    32-bit number to be converted (resolution 0.001cm)
// Outputs: None
// Notes:
// Fixed format 1 digit, point, 3 digits, space, units, null termination
// Examples
//     4 to "0.004 cm"
//    31 to "0.031 cm"
//   102 to "0.102 cm"
//  2210 to "2.210 cm"
// 10000 to "*.*** cm"  any value larger than 9999 converted to "*.*** cm"
void UART_ConvertDistance(unsigned long n) {
    // first, put m into String[10]
    short i = 0;
    unsigned short StringLength;
    unsigned char ReverseString[15];
    short j = 0;
    unsigned long m = n;    // keep n the same value after converting
    // special case
    if (m == 0) {
        StringLength = 1;   // the length of String[10] is only 1
        String[0] = 0x30;   // String[10] only contains only one '0'
        return;             // then exit immediately
    }
    // otherwise
    while (m != 0) {
    // using 10-division to get every single digit of m until m = 0
        ReverseString[j] = m%10 + 0x30; // get the last digit of n
        m /= 10;    // after get a digit, decrease n for the next time
        // eventually, n will equal 0 and the loop will be terminated
        j++;        // increase the index
    }
    // the result string is reversed because of this method
    // change to the right order
    StringLength = j;
    for (j = StringLength - 1; j >= 0; j--) {
        String[i] = ReverseString[j];
        // the last digit in ReverseString[10] is the first one in String[10]
        i++;        // increase the index of String[10]
    }

    // second, go over each case
    // n has 1 digits
    if (n <= 9) {
        String[4] = String[0];
        String[0] = 0x30;
        String[1] = '.';
        String[2] = 0x30;
        String[3] = 0x30;
        String[5] = 0x20;
        String[6] = 'c';
        String[7] = 'm';
    }
    // n has 2 digits
    else if (n >= 10 && n <= 99) {
        String[4] = String[1];
        String[3] = String[0];
        String[0] = 0x30;
        String[1] = '.';
        String[2] = 0x30;
        String[5] = 0x20;
        String[6] = 'c';
        String[7] = 'm';
    }
    // n has 3 digits
    else if (n >= 100 && n <= 999) {
        String[4] = String[2];
        String[3] = String[1];
        String[2] = String[0];
        String[0] = 0x30;
        String[1] = '.';
        String[5] = 0x20;
        String[6] = 'c';
        String[7] = 'm';
    }
    // n has 4 digits
    else if (n >= 1000 && n <= 9999) {
        String[4] = String[3];
        String[3] = String[2];
        String[2] = String[1];
        String[1] = '.';
        String[5] = 0x20;
        String[6] = 'c';
        String[7] = 'm';
    }
    // n has more than 4 digits (Overflow)
    else {
        String[0] = '*';
        String[1] = '.';
        String[2] = '*';
        String[3] = '*';
        String[4] = '*';
        String[5] = 0x20;
        String[6] = 'c';
        String[7] = 'm';
    }
}

//----------PortF_Init----------
// Initializes port F for the blue LED and SW1
// Inputs: None
// Outputs: None
void PortF_Init(void) {
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x20;           // enable port F clock
    delay = SYSCTL_RCGC2_R;           // allow time for clock to start
    GPIO_PORTF_LOCK_R |= 0x4C4F434B;  // unlock GPIO port F
    GPIO_PORTF_CR_R |= 0x14;          // allow change to PF4, PF2
    GPIO_PORTF_DIR_R |= 0x04;         // PF4 input, PF2 output
    GPIO_PORTF_AFSEL_R &= ~0x14;  // disable alternate function on PF4, PF2
    GPIO_PORTF_PCTL_R = 0;        // disable PCTL
    GPIO_PORTF_DEN_R |= 0x14;     // enable digital I/O on PF4, PF2
    GPIO_PORTF_PUR_R |= 0x10;     // enable pull-up resistor on PF4
    GPIO_PORTF_AMSEL_R &= ~0x14;  // disable analog function on PF4, PF2
}

//**********4. Main Function:**********
int main(void) {
    TExaS_Init(ADC0_AIN1_PIN_PE2, SSI0_Real_Nokia5110_Scope);
    // initialize ADC0, channel 1, sequencer 3
    ADC0_Init();
    Nokia5110_Init();
    // initialize SysTick for 40 Hz interrupts
    SysTick_Init();
    PortF_Init();
    EnableInterrupts();
    while(1) {
        Flag = 0;                       // read mailbox
        while (Flag != 1) {}            // wait for the flag to be set
        Nokia5110_OutString(String);    // output to Nokia5110 LCD
        UART_ConvertDistance(Distance); // convert Distance to a proper format
        Nokia5110_SetCursor(0, 0);      // set the initial cursor position
    }
}
