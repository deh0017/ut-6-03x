/**
 * File Name: systick.h
 *
 * Description: Initialize System Timer (SysTick) and create a delay function
 * This program assumes the clock is running at 80 MHz
 * The RELOAD value must be changed if the clock is running at
 * a different speed
 *
 * Compatibility: EK-TM4C123GXL
 *
 * Phi Luu
 * Portland, Oregon, United States
 * Created June 05, 2016
 * Updated June 03, 2017
 */

#define NVIC_ST_CTRL_R       (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R     (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R    (*((volatile unsigned long *)0xE000E018))
#define NVIC_ST_CTRL_R       (*((volatile unsigned long *)0xE000E010))

/**
 * Activates the system timer of the microcontroller
 */
void SysTick_Init(void);

/**
 * Delays the program for a number of milliseconds
 *
 * @param   ms   the number of milliseconds to delay
 *
 * @assumption   80-MHz clock
 *
 * @notes        1 period = (real_time_delay seconds) * (80,000,000 Hz)
 */
void SysTick_Delay(unsigned long ms);
