/**
 * UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Lab 11: UART - Serial Interface
 *
 * File Name: main.c
 *
 * Description:
 *   Convert numbers into ASCII strings and
 *   display on the UART0 (TExaSdisplay)
 *
 * Compatibility: EK-TM4C123GXL
 *
 * Phi Luu
 * Portland, Oregon, United States
 * Created April 07, 2016
 * Updated January 15, 2017
 */

#include "uart.h" // include all UART related functions
#include "TExaS.h"

// Function prototypes
void EnableInterrupts(void); // enable interrupts

int  main(void) {
    // Setup
    unsigned long n;

    TExaS_Init();       // initialize grader, set system clock to 80 MHz
    UART_Init();        // initialize UART
    EnableInterrupts(); // needed for TExaS
    UART_OutString("Running Lab 11");

    // Loop
    while (1) {
        UART_OutString("\n\rInput:");
        n = UART_InUDec();   // read n
        UART_OutString(" UART_OutUDec = ");
        UART_OutUDec(n);     // write n in number format
        UART_OutString(",  UART_OutDistance ~ ");
        UART_OutDistance(n); // write n in distance format
    }
}
