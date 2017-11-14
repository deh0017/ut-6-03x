/**
 * @file     main.c
 * @author   Phi Luu
 * @date     May 20, 2016
 *
 * @brief    UTAustinX: UT.6.03x Embedded Systems - Shape the World
 *           Lab 15: SpaceInvaders
 *
 * @section  DESCRIPTION
 *
 * Mandatory file containing the main routine and interrupt service routines.
 */

/**
 * Required hardware I/O connections
 *
 * Slide pot pin 1 connected to ground
 * Slide pot pin 2 connected to PE2/AIN1
 * Slide pot pin 3 connected to +3.3V
 * Fire switch connected to PE1
 * 8*R resistor DAC bit 0 on PB0 (least significant bit)
 * 4*R resistor DAC bit 1 on PB1
 * 2*R resistor DAC bit 2 on PB2
 * 1*R resistor DAC bit 3 on PB3 (most significant bit)
 */

#include "data.h"
#include "led.h"
#include "move.h"
#include "nokia5110.h"
#include "random.h"
#include "sound.h"
#include "sprites.h"
#include "timer.h"
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// Function prototypes
void EnableInterrupts(void);
void DisableInterrupts(void);
void WaitForInterrupt(void);

void SysTick_Handler(void);
void Timer2A_Handler(void);
void InitializeGame(void);
void FinalizeGame(void);

int main(void) {
    int i;

    InitializeGame();

    // display splash screen for 2 seconds
    Nokia5110_PrintBMP(0, SCREEN_HEIGHT - 1, BMP_SPLASH_SCREEN, 0);
    Nokia5110_DisplayBuffer();
    Delay100ms(20);

    // initialize game board
    ResetBoard();
    ShowLevel();

    // play until the player dies
    while (lives > 0) {
        player.exists = 1;
        powerbar.exists = 1;
        powerbar.bitmapn = power_level - 1;
        while (player.exists) {
            while (AliensExist() && player.exists) {
                while (!semaphore2a) {
                    WaitForInterrupt();
                }

                semaphore2a = 0;
                MoveSprites();
                DrawBoard();
            }

            if (!player.exists) {
                Delay100ms(1); // delay 0.1 seconds

                // remove a life and any missiles
                for (i = 0; i < AMISSILES; i++) {
                    DestroySprite(&alien_missiles[i]);
                }

                for (i = 0; i < PMISSILES; i++) {
                    DestroySprite(&player_missiles[i]);
                }
                lives--;
            } else {
                // Speed up the aliens and re-initialise the game board
                if (--alien_init_speed == 1) {
                    alien_init_speed = 2;
                }

                ResetBoard();
                level += 1;
                ShowLevel();
            }
        }
    }
    Delay100ms(5);
    FinalizeGame();
    return 0;
}

/**
 * Updates player position and ADC data and enables the main loop to run.
 */
void Timer2A_Handler(void) {
    TIMER2_ICR_R = TIMER_ICR_TATOCINT; // acknowledge timer2A timeout
    timer2a_count++;
    semaphore2a = 1;                   // trigger
    // adc_data = (((long)ReadAdc0()-515L)*28L)>>10;    // 3 - collect an ADC sample of player x pos
    adc_data = 66 - ReadAdc0() * (SCREEN_WIDTH - BMP_WIDTH_INDEX) / 4095;
}

/**
 * Combines active sounds and passes to DAC hardware. 11-kHz ISR.
 */
void SysTick_Handler(void) {
    int i;
    unsigned long dac_acc = 0;
    int num_active_sounds = 0;

    for (i = 0; i < MAX_SOUND; i++) {
        if (sound_enabled[i]) {
            unsigned char dac_val;
            num_active_sounds++;
            dac_val = *(WAV_DAC[i] + (dac_step[i] / 2));

            if (dac_step[i] % 2) {
                dac_acc += dac_val & 0xF;
            } else {
                dac_acc += dac_val >> 4;
            }

            if (dac_step[i] / 2 >= dac_step_max[i]) {
                StopSound(i);
            }

            dac_step[i]++;
        }
        if (num_active_sounds > 0) {
            WriteDac(dac_acc / num_active_sounds);
        }
    }
}

/**
 * Initializes hardware, game, and sprites.
 */
void InitializeGame(void) {
    TExaS_Init(SSI0_Real_Nokia5110_Scope);
    Random_Init(1);
    Nokia5110_Init();
    InitDac();
    InitAdc0();
    InitSound();
    InitButton();
    InitLed();
    score = 0;
    lives = 3;
    InitSprites();
    InitTimer2(CLOCK_SPEED / 60); // 60-Hz interrupt
}

/**
 * Shows the game-over screen.
 */
void FinalizeGame(void) {
    Nokia5110_Clear();
    Nokia5110_SetCursor(1, 1);
    Nokia5110_OutString("GAME OVER");
    Nokia5110_SetCursor(1, 2);
    Nokia5110_OutString("Nice try,");
    Nokia5110_SetCursor(1, 3);
    Nokia5110_OutString("Earthling!");
    Nokia5110_SetCursor(2, 4);
    Nokia5110_OutUDec(score);
}
