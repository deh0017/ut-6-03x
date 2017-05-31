/**
 * UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Lab 5: Functions in C
 *
 * File Name: main.c
 *
 * Description: Calculate the area of a rectangle
 *   If Length or Width is in the close interval [3,20]
 *   Then Area = Length * Width
 *   Otherwise, Area = 0
 *
 * Compatibility: EK-TM4C123GXL
 *
 * Phi Luu
 * Portland, Oregon, United States
 * Created March 06, 2016
 * Updated May 30, 2017
 */

#include <stdio.h> // standard C library
#include "uart.h"  // functions to implement I/O
#include "TExaS.h" // lab grader functions

// Function prototypes
unsigned long calc_area(unsigned long l, unsigned long w);
void          EnableInterrupts(void);

int main(void) {
    // Setup
    unsigned long length, width, area;

    // initialize the TExaS grader lab 5
    TExaS_Init(UART_PIN_PA0, UART_PIN_PA1);
    UART_Init();        // initialize UART for printing
    printf("\nThis program calculates areas of rectangular rooms.\n");
    EnableInterrupts(); // the grader needs interrupts

    // Loop
    while (1) {
        // get length input
        printf("\nGive length: ");
        scanf("%ld", &length);
        // get width input
        printf("\nGive width: ");
        scanf("%ld", &width);
        area = calc_area(length, width); // calulate the area
        // write the value on the screen
        printf("\nArea of the room = %ld\n", area);
    }
}

/**
 * Computes the area of a rectangular room whose sides are given
 *
 * @param   l  the length of a rectangle
 * @param   w  the width of a rectangle
 *
 * @return     the area of the rectangular room
 * @return     0 if
 *               the length is less than 3, or
 *               the width is less than 3, or
 *               the length is greater than 20, or
 *               the width is greater than 20
 */
unsigned long calc_area(unsigned long l, unsigned long w) {
    unsigned long result;

    if ((l < 3) || (w < 3) || (l > 20) || (w > 20)) { // out of range cases
        return 0;                                     // area = 0
    }
    result = l * w;                                   // area = length * width
    return result;
}
