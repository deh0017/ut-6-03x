//****************************************************************************
//
// File Name: PLL.h
//
// Description: Phase-Locked Loop (PLL)
//     configures the crystal frequency to 16 MHz and
//     sets the clock speed to run at 80 MHz
// for more information, see
// page 256 (for crystal values) and
// page 224 (for divisor values) of the data sheet
//
// Compatibility: EK-TM4C123GXL
//
// Phi Luu
// Portland, Oregon, United States
// Created June 05, 2016
// Updated January 07, 2017
//
//****************************************************************************

#define SYSCTL_RCC2_R             (*((volatile unsigned long *)0x400FE070))
#define SYSCTL_RCC2_USERCC2       0x80000000 // bit 31  Use RCC2
#define SYSCTL_RCC2_BYPASS2       0x00000800 // bit 11  PLL Bypass 2
#define SYSCTL_RCC_R              (*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCC_XTAL_M         0x000007C0 // bit 10-6  Crystal Value
#define SYSCTL_RCC_XTAL_16MHZ     0x00000540 // bit 10-6  16-MHz Crystal
                                             // 10101 = 16 MHz
#define SYSCTL_RCC2_OSCSRC2_M     0x00000070 // bit 6-4  Oscillator Source 2
#define SYSCTL_RCC2_OSCSRC2_MO    0x00000000 // use Main Oscillator MOSC
#define SYSCTL_RCC2_PWRDN2        0x00002000 // bit 13  Power-Down PLL 2
#define SYSCTL_RCC2_DIV400        0x40000000 // bit 30  divide PLL as
                                             // 400 MHz vs. 200 MHz
#define SYSCTL_RCC2_SYSDIV2       0x1FC00000 // bit 28-23  System Clock Divider
#define SYSCTL_RIS_R              (*((volatile unsigned long *)0x400FE050))
#define SYSCTL_RIS_PLLLRIS        0x00000040 // bit 7
                                             // PLL Lock Raw Interrupt Status

// Sets the crystal frequency to 16 MHz and the clock speed to 80 MHz
void PLL_XTAL_16_CLOCK_80(void);
