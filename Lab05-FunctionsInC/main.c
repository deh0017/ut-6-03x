//****************************************************************************
//
// UTAustinX: UT.6.03x Embedded Systems - Shape the World
// Lab 5: Functions in C
//
// File Name: main.c
//
// Description: Calculate the area of a rectangle
//     If Length or Width is in the close interval [3,20]
//     Then Area = Length * Width
//     Otherwise, Area = 0
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created March 06, 2016
// Updated December 28, 2016
//
//****************************************************************************

//**********1. Pre-processor Section**********
#include <stdio.h>          // standard C library
#include "UART.h"           // functions to implement I/O
#include "TExaS.h"          // lab grader functions

//**********2. Declarations Section**********
// Function Prototypes
// area calculator
unsigned long calc_area(unsigned long l, unsigned long w);
// enable interrupts
void EnableInterrupts(void);

//**********3. Subroutines Section**********
//----------calc_area----------
// Computes the Area of a rectangular room whose sides are given
// Inputs: l    the length of a rectangle
//         w    the width of a rectangle
// Outputs: The area of the rectangular room
// Notes: Return a result of zero if
//  the length is less than 3,
//  the width is less than 3,
//  the length is greater than 20 or
//  the width is greater than 20.
unsigned long calc_area(unsigned long l, unsigned long w) {
    unsigned long result;

    if (l < 3 || w < 3 || l > 20 || w > 20) {   // out of range cases
        return 0;                               // area = 0
    }
    result = l * w;                             // area = length * width
    return result;
}

//**********4. Main function**********
int main(void) {
    // set up:
    unsigned long length, width, area;

    // initialize the TExaS grader lab 5
    TExaS_Init(UART_PIN_PA0, UART_PIN_PA1);
    UART_Init();            // initialize UART for printing
    printf("\nThis program calculates areas of rectangular rooms.\n");
    EnableInterrupts();     // the grader needs interrupts

    // loop:
    while (1) {
        // Get length input
        printf("\nGive length: ");
        scanf("%ld", &length);
        // Get width input
        printf("\nGive width: ");scanf("%ld", &width);
        area = calc_area(length, width);  // calulate the area
        // write the value on the screen
        printf("\nArea of the room = %ld\n", area);
    }
}
