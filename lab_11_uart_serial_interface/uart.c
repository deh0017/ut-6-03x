/**
 * UTAustinX: UT.6.03x Embedded Systems - Shape the World
 * Lab 11: UART - Serial Interface
 *
 * File Name: uart.c
 *
 * Description:
 *   Convert numbers into ASCII strings and
 *   display on the UART0 (TExaSdisplay)
 *
 * Author: Phi Luu
 * Location: Portland, Oregon, United States
 * Created: April 07, 2016
 * Updated: June 22, 2017
 */

#include "tm4c123gh6pm.h"
#include "uart.h"

/**
 * Initializes the UART for 115,200 baud rates.
 * 8-bit word length, no parity bits, one stop bit, FIFOs enabled.
 *
 * @assumption    80-MHz clock
 */
void UartInit(void) {
    volatile unsigned long delay;

    SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; // activate UART0
    SYSCTL_RCGC2_R |= 0x03;               // activate port A and Port B
    delay           = SYSCTL_RCGC2_R;     // allow time for clock to start
    UART0_CTL_R    &= ~UART_CTL_UARTEN;   // disable UART
    // IBRD = int(80,000,000 / (16 * 115,200)) = int(43.402778):
    UART0_IBRD_R = 43;
    // FBRD = round(0.402778 * 64) = 26:
    UART0_FBRD_R = 26;
    // 8 bit word length (no parity bits, one stop bit, FIFOs)
    UART0_LCRH_R        = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
    UART0_CTL_R        |= UART_CTL_UARTEN; // enable UART
    GPIO_PORTA_AFSEL_R |= 0x03;            // enable alt funct on PA1, PA0
    GPIO_PORTA_DEN_R   |= 0x03;            // enable digital I/O on PA1, PA0
    // configure PA1, PA0 as UART0
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) + 0x00000011;
    // disable analog functionality on PA1, PA0:
    GPIO_PORTA_AMSEL_R &= ~0x03;
}

/**
 * Waits for new serial port input.
 *
 * @return    ASCII code for key typed
 */
unsigned char UartInChar(void) {
    // wait until the Receiver FIFO Empty Flag is empty:
    while ((UART0_FR_R & UART_FR_RXFE) != 0) {
        ;
    }
    // then read from UART0 and mask with 8 bits:
    return (unsigned char)(UART0_DR_R & 0xFF);
}

/**
 * Gets oldest serial port input and returns immediately if there is no data.
 *
 * @return    ASCII code for key typed
 * @return    0 if there is no data
 */
unsigned char UartInCharNonBlock(void) {
    // if the Receiver FIFO Empty Flag is full:
    if ((UART0_FR_R & UART_FR_RXFE) == 0) {
        // get the data immediately and then exit:
        return (unsigned char)(UART0_DR_R & 0xFF);
    }
    // if the Receiver FIFO Empty Flag is empty:
    else {
        return 0; // get nothing and then exit
    }
}

/**
 * Outputs 8-bit data to serial port.
 *
 * @param  data  an 8-bit ASCII character to be transferred
 */
void UartOutChar(unsigned char data) {
    // if the Transmitter FIFO Full Flag is not busy:
    while ((UART0_FR_R & UART_FR_TXFF) != 0) {
        ;
    }
    UART0_DR_R = data; // write the data
}

/**
 * Accepts ASCII input in unsigned decimal format
 * and converts to a 32-bit unsigned number
 * valid range is 0 to 4,294,967,295 (2^32-1).
 *
 * @return    32-bit unsigned number
 *
 * @notes     if you enter a number above 4294967295,
 *            it will return an incorrect value.
 *            Backspace will remove last digit typed
 */
unsigned long UartInNum(void) {
    unsigned long number = 0, length = 0;
    char character;

    character = UartInChar(); // get input from the UART

    while (character != CR) { // accept until <enter> is typed
        // The next line checks that the input is a digit, 0-9.
        // If the character is not 0-9, it is ignored and not echoed
        if ((character >= '0') && (character <= '9')) {
            // overflows if above 4,294,967,295
            number = 10 * number + (character - '0');
            length++;
            UartOutChar(character);
        }
        // If the input is a backspace, then the return number is
        // changed and a backspace is outputted to the screen
        else if ((character == BS) && length) {
            number /= 10;
            length--;
            UartOutChar(character);
        }
        character = UartInChar();
    }
    return number;
}

/**
 * Outputs out_str (NULL termination).
 *
 * @param  buffer[]  pointer to a NULL-terminated string to be transferred
 */
void UartOutString(unsigned char buffer[]) {
    unsigned long i = 0;

    // go from the beginning to the ending of the character string:
    while (buffer[i] != '\0') {
        UartOutChar(buffer[i]); // write every single character
        i++;                    // move to next character
    }
}

unsigned char  out_str[15];
unsigned short out_str_len;

/**
 * Puts number into global variable out_str[15]
 *
 * @param  n  32-bit unsigned number to be put in
 */
void PutIntoString(unsigned long n) {
    unsigned short i = 0;
    unsigned char  reverse_str[15];
    short j = 0;

    // check the special case n = 0:
    if (!n) {
        out_str_len = 1;
        out_str[0]  = 0x30;
        return;
    }

    // for other cases:
    while (n) {
        // store the number from right to left into reverse_str:
        reverse_str[j] = n % 10 + 0x30;
        n             /= 10; // cut off the most right digit
        j++;                 // prepare for next digit
    }
    out_str_len = j;         // set out_str_len

    // move back to out_str in right order:
    for (j = out_str_len - 1; j >= 0; j--) {
        out_str[i] = reverse_str[j];
        i++;
    }
}

/**
 * Converts a 32-bit unsigned number in unsigned decimal format.
 *
 * @param  n  32-bit unsigned number to be transferred
 *
 * @notes     Fixed format 4 digits, one space after, null termination
 *
 * @examples
 *   4 to "   4 "
 *   31 to "  31 "
 *   102 to " 102 "
 *   2210 to "2210 "
 *   10000 to "**** "  any value larger than 9999 converted to "**** "
 */
void UartNumToString(unsigned long n) {
    PutIntoString(n); // put n into out_str

    if (n <= 9) {     // in case n has only 1 digit:
        out_str[3] = out_str[0];
        out_str[0] = 0x20;
        out_str[1] = 0x20;
        out_str[2] = 0x20;
        out_str[4] = 0x20;
    } else if ((n >= 10) && (n <= 99)) { // in case n has 2 digits:
        out_str[3] = out_str[1];
        out_str[2] = out_str[0];
        out_str[0] = 0x20;
        out_str[1] = 0x20;
        out_str[4] = 0x20;
    } else if ((n >= 100) && (n <= 999)) { // in case n has 3 digits:
        out_str[3] = out_str[2];
        out_str[2] = out_str[1];
        out_str[1] = out_str[0];
        out_str[0] = 0x20;
        out_str[4] = 0x20;
    } else if ((n >= 1000) && (n <= 9999)) { // in case n has 4 digits:
        out_str[4] = 0x20;
    } else {                                 // overflowing case:
        out_str[0] = '*';
        out_str[1] = '*';
        out_str[2] = '*';
        out_str[3] = '*';
        out_str[4] = 0x20;
        out_str[5] = '\0';
    }
}

/**
 * Outputs a 32-bit unsigned number in unsigned decimal format.
 *
 * @param  n  32-bit unsigned number to be transferred
 *
 * @notes     Fixed format 4 digits, one space after, null termination
 */
void UartOutNum(unsigned long n) {
    UartNumToString(n);     // convert to decimal
    UartOutString(out_str); // output
}

/**
 * Converts a 32-bit distance into an ASCII string.
 *
 * @param  n  32-bit unsigned number to be converted (resolution 0.001cm)
 *
 * @notes     Fixed format
 *            1 digit, point, 3 digits, space, units, null termination
 *
 * @examples
 *   4 to "0.004 cm"
 *   31 to "0.031 cm"
 *   102 to "0.102 cm"
 *   2210 to "2.210 cm"
 *   10000 to "*.*** cm"  any value larger than 9999 converted to "*.*** cm"
 */
void UartDistanceToString(unsigned long n) {
    PutIntoString(n); // put n into out_str

    if (n <= 9) {     // in case n has only 1 digit:
        out_str[4] = out_str[0];
        out_str[0] = 0x30;
        out_str[1] = '.';
        out_str[2] = 0x30;
        out_str[3] = 0x30;
        out_str[5] = 0x20;
        out_str[6] = 'c';
        out_str[7] = 'm';
    } else if ((n >= 10) && (n <= 99)) { // in case n has 2 digits:
        out_str[4] = out_str[1];
        out_str[3] = out_str[0];
        out_str[0] = 0x30;
        out_str[1] = '.';
        out_str[2] = 0x30;
        out_str[5] = 0x20;
        out_str[6] = 'c';
        out_str[7] = 'm';
    } else if ((n >= 100) && (n <= 999)) { // in case n has 3 digits:
        out_str[4] = out_str[2];
        out_str[3] = out_str[1];
        out_str[2] = out_str[0];
        out_str[0] = 0x30;
        out_str[1] = '.';
        out_str[5] = 0x20;
        out_str[6] = 'c';
        out_str[7] = 'm';
    } else if ((n >= 1000) && (n <= 9999)) { // in case n has 4 digits:
        out_str[4] = out_str[3];
        out_str[3] = out_str[2];
        out_str[2] = out_str[1];
        out_str[1] = '.';
        out_str[5] = 0x20;
        out_str[6] = 'c';
        out_str[7] = 'm';
    } else { // overflowing case:
        out_str[0] = '*';
        out_str[1] = '.';
        out_str[2] = '*';
        out_str[3] = '*';
        out_str[4] = '*';
        out_str[5] = 0x20;
        out_str[6] = 'c';
        out_str[7] = 'm';
    }
}

/**
 * Outputs a 32-bit number in unsigned decimal fixed-point format.
 *
 * @param  n  32-bit unsigned number to be transferred (resolution 0.001cm)
 *
 * @notes     Fixed format
 *            1 digit, point, 3 digits, space, units, null termination
 */
void UartOutDistance(unsigned long n) {
    UartDistanceToString(n); // convert to distance
    UartOutString(out_str);  // output
}
