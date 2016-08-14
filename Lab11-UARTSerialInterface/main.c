//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 11: UART - Serial Interface
//
// File Name: main.c
//
// Description:
//     Convert numbers into ASCII strings and
//     display on the UART0 (TExaSdisplay)
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created April 07, 2016
// Updated August 13, 2016
//
//****************************************************************************

//**********1. Pre-processor Section**********
#include "UART.h"   // include all UART related functions
#include "TExaS.h"

//**********2. Global Declarations Section**********
// Function Prototypes
void EnableInterrupts(void);  // enable interrupts

//**********3. Main function**********
int main(void) {
    // set up:
    unsigned long n;
    TExaS_Init();   // initialize grader, set system clock to 80 MHz
    UART_Init();          // initialize UART
    EnableInterrupts();   // needed for TExaS
    UART_OutString("Running Lab 11");

    // loop:
    while (1) {
        UART_OutString("\n\rInput:");
        n = UART_InUDec();      // read n
        UART_OutString(" UART_OutUDec = ");
        UART_OutUDec(n);        // write n in number format
        UART_OutString(",  UART_OutDistance ~ ");
        UART_OutDistance(n);    // write n in distance format
      }
}
