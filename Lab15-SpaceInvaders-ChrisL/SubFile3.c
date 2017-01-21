/* Main program */
int main(void) {
    int i;

    init();
    // Display Space Invaders title screen
    Nokia5110_PrintBMP(0, SCREENH - 1, Marqueecjl, 0);
    Nokia5110_DisplayBuffer();
    Delay100ms(20); // delay 2 seconds
    // Initialise the game board
    reset_board();
    show_level();

    while (lives > 0) {
        player.exists    = TRUE;
        powerbar.exists  = TRUE;
        powerbar.bitmapn = power_level - 1;

        while (player.exists) {
            while (aliens_exist() && player.exists) {
                while (!Semaphore2A) WaitForInterrupt();
                Semaphore2A = FALSE;
                move_sprites();
                draw_board();
            }

            if (!player.exists) {
                Delay100ms(1); // delay 0.1 seconds

                // remove a life and any missiles
                for (i = 0; i < AMISSILES; i++) destroy(&alien_missiles[i]);

                for (i = 0; i < PMISSILES; i++) destroy(&player_missiles[i]);
                lives--;
            } else {
                // Speed up the aliens and re-initialise the game board
                if (--AlienInitialSpeed == 1) AlienInitialSpeed = 2;
                reset_board();
                level += 1;
                show_level();
            }
        }
    }
    Delay100ms(5); // delay 0.5 seconds
    game_over();

    while (1) {}
}

/* Initialise Timer 2 used for main control loop*/
void Timer2_Init(unsigned long period) {
    unsigned long volatile delay;

    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R2; // 0a) activate timer2
    // SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3; // 0b) activate timer3
    delay = SYSCTL_RCGCTIMER_R;
    // configure timer 2A
    TimerCount2A   = 0;
    Semaphore2A    = 0;
    TIMER2_CTL_R   = 0x00000000;             // 1a) disable timer2 during setup
    TIMER2_CFG_R   = TIMER_CFG_32_BIT_TIMER; // 2a) configure timer2 for 32-bit mode
    TIMER2_TAMR_R |= TIMER_TAMR_TAMR_PERIOD; // 3a) periodic count down is default but needed by simulator
    TIMER2_TAILR_R = period - 1;             // 4a) timer2A reload value
    TIMER2_TAPR_R  = 0;                      // 5a) timer2A bus clock resolution
    TIMER2_ICR_R  |= TIMER_ICR_TATOCINT;     // 6a) clear timer2A timeout flag
    TIMER2_IMR_R  |= TIMER_IMR_TATOIM;       // 7a) arm timer2A timeout interrupt
    TIMER2_CTL_R  |= TIMER_CTL_TAEN;         // 10a) enable timer2A
    // interrupts are enabled in the main program after all devices initialized
    // vector number 39, interrupt number 23
    NVIC_PRI5_R                              // 8) priority 4
        = (NVIC_PRI5_R & (~NVIC_PRI5_INT23_M))
          | (4UL << NVIC_PRI5_INT23_S);
    NVIC_EN0_R = 1 << 23;                    // 9) enable IRQ 23 in NVIC
}

/* Update player position ADC data and enable main loop to run */
void Timer2A_Handler(void) {
    TIMER2_ICR_R = TIMER_ICR_TATOCINT;                    // acknowledge timer2A timeout
    TimerCount2A++;
    Semaphore2A = 1;                                      // trigger
    ADCdata     = (((long)ADC0_In() - 515L) * 28L) >> 10; // 3 - collect an ADC sample of player x pos
}

/* General purpose 100ms delay */
void Delay100ms(unsigned long count) {
    unsigned long volatile time;

    while (count > 0) {
        time = 727240; // 0.1sec at 80 MHz

        while (time) {
            time--;
        }
        count--;
    }
}

/* Initialise the player position ADC hardware */
void ADC0_Init(void) {
    // This initialization function sets up the ADC
    // Max sample rate: <=125,000 samples/second
    // SS3 triggering event: software trigger
    // SS3 1st sample source:  channel 1
    // SS3 interrupts: enabled but not promoted to controller
    volatile unsigned long delay;

    SYSCTL_RCGC2_R     |= BIT(4);                    // Enable GPIO Port E clock
    delay               = SYSCTL_RCGC2_R;
    GPIO_PORTE_DIR_R   &= ~(BIT(2));                 // PE2 is input
    GPIO_PORTE_DEN_R   &= ~(BIT(2));                 // Disable digital input on PE2
    GPIO_PORTE_AFSEL_R |= BIT(2);                    // Enable PE2 alternate function
    GPIO_PORTE_AMSEL_R |= BIT(2);                    // Enable PE2 analog functions
    GPIO_PORTE_PCTL_R  &= ~(PCTL(2));                // Clear PCTL content for PE2
    SYSCTL_RCGC0_R     |= BIT(16);                   // Enable ADC clock
    delay               = SYSCTL_RCGC0_R;
    SYSCTL_RCGC0_R     &= ~(SYSCTL_RCGC0_ADC0SPD_M); // Max Sample speed MAXADCSPD = 125 KHz
    SYSCTL_RCGC0_R     |= SYSCTL_RCGC0_ADC0SPD_125K;
    ADC0_SSPRI_R        = 0x3210L;                   // SS3 is lowest priority
    ADC0_EMUX_R        &= ~(0xF000L);                // EM3 is software triggered
    ADC0_SSCTL3_R       = (BIT(1) | BIT(2));         // Interrupt enabled IE0=1, END0=1, TS0=0, D0=0
    ADC0_PSSI_R        &= ~(BIT(3));                 // Clear S/W request for SS3 sample
    ADC0_ACTSS_R       |= BIT(3);                    // Enable sequencer 3
    ADC0_SSMUX3_R       = (ADC0_SSMUX3_R & ~(0xF)) | 1L;
    ADC0_ISC_R         |= BIT(3);                    // Clear SS3 ADC reading ready flag
}

/* Initialise the game buttons */
void Button_Init(void) {
    /* Fire button is positive logic switch on PE0 */
    volatile unsigned long delay;

    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE; // Enable GPIO Port E clock
    delay           = SYSCTL_RCGC2_R;
    // PE0 is fire button
    GPIO_PORTE_DIR_R   &= ~(BIT(0));      // PE0 is input
    GPIO_PORTE_DEN_R   |= BIT(0);         // Enable digital input on PE0
    GPIO_PORTE_PDR_R   |= BIT(0);         // Enable pull-down resistor on PE0
    GPIO_PORTE_AFSEL_R &= BIT(0);         // Disable PE0 alternate function
    GPIO_PORTE_AMSEL_R &= BIT(0);         // Disable PE0 analog functions
    GPIO_PORTE_PCTL_R  &= ~(PCTL(0));     // Clear PCTL content for PE0
    // PE1 is special weapon
    GPIO_PORTE_DIR_R   &= ~(BIT(1));      // PE1 is input
    GPIO_PORTE_DEN_R   |= BIT(1);         // Enable digital input on PE1
    GPIO_PORTE_PDR_R   |= BIT(1);         // Enable pull-down resistor on PE1
    GPIO_PORTE_AFSEL_R &= BIT(1);         // Disable PE1 alternate function
    GPIO_PORTE_AMSEL_R &= BIT(1);         // Disable PE1 analog functions
    GPIO_PORTE_PCTL_R  &= ~(PCTL(1));     // Clear PCTL content for PE1
}

/* Request and wait for an ADC sample */
unsigned long ADC0_In(void) {
    // ------------ADC0_In------------
    // Busy-wait Analog to digital conversion
    // Input: none
    // Output: 12-bit result of ADC conversion
    unsigned long ADC_value;

    ADC0_PSSI_R |= BIT(3);                // Request SS3 reading

    while (!(ADC0_RIS_R & BIT(3))) {}
    ;                                     // wait for ADC sample capture
    ADC_value   = ADC0_SSFIFO3_R & 0xFFF; // get sample
    ADC0_ISC_R |= BIT(3);                 // Clear SS3 ADC reading ready flag
    return ADC_value;
}

/* Initialise sound DAC */
void DAC_Init(void) {
    // **************DAC_Init*********************
    // Initialize 4-bit DAC
    // Input: none
    // Output: none
    volatile unsigned long delay;

    SYSCTL_RCGC2_R     |= GPIO_PORTB;
    delay               = SYSCTL_RCGC2_R;
    GPIO_PORTB_AMSEL_R &= ~(DAC_PINS);
    GPIO_PORTB_AFSEL_R &= ~(DAC_PINS);
    GPIO_PORTB_DIR_R   |= DAC_PINS;
    GPIO_PORTB_PCTL_R  &= ~(DAC_PCTL_PINS);
    GPIO_PORTB_DR8R_R  |= DAC_PINS;
    GPIO_PORTB_DEN_R   |= DAC_PINS;
}

/* Output a single sound DAC step */
void DAC_Out(unsigned long data) {
    // **************DAC_Out*********************
    // output to DAC
    // Input: 4-bit data, 0 to 15
    // Output: none
    data = (data & (DAC_PINS))
           | (GPIO_PORTB_DATA_R & ~(DAC_PINS));
    GPIO_PORTB_DATA_R = data;
}

/* Initialise the DAC hardware for 5.5 KHz wav sound playback */
void Sound_Init(void) {
    // **************Sound_Init*********************
    // Initialize Systick periodic interrupts
    // Also calls DAC_Init() to initialize DAC
    // Input: none
    // Output: none
    DAC_Init();
    NVIC_ST_CTRL_R = 0;
    // NVIC_ST_RELOAD_R = 7526; // 11 KHz sample rate
    NVIC_ST_RELOAD_R  = 15052; // 5.5 KHz sample rate
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_CTRL_R    = 7;
}

/* Turn LED1 on or off */
void LED1(int on) {
    if (on) GPIO_PORTB_DATA_R |= BIT(4);
    else GPIO_PORTB_DATA_R &= ~(BIT(4));
}

/* Turn LED2 on or off */
void LED2(int on) {
    if (on) GPIO_PORTB_DATA_R |= BIT(5);
    else GPIO_PORTB_DATA_R &= ~(BIT(5));
}

/* Start playing a sound */
void Sound_Tone(enum Sound sound) {
    // **************Sound_Tone*********************
    // Change Systick periodic interrupts to start sound output
    // Input: interrupt period
    // Units of period are 12.5ns
    // Maximum is 2^24-1
    // Minimum is determined by length of ISR
    // Output: none
    // this routine sets the RELOAD and starts SysTick
    sound_enabled[sound] = TRUE;

    if (sound == INVADER_SOUND) LED1(TRUE);
    else if (sound == SHOOT_SOUND) LED2(TRUE);
    DAC_step[sound] = 0;
}

/* Stop playing a sound */
void Sound_Off(int sound) {
    // **************Sound_Off*********************
    // stop outputing to DAC
    // Output: none
    // this routine stops the sound output
    sound_enabled[sound] = FALSE;

    if (sound == INVADER_SOUND) LED1(FALSE);
    else if (sound == SHOOT_SOUND) LED2(FALSE);
}

/* Initialise LED hardware */
void LED_Init(void) {
    /* LEDs are on PB4 and PB5 */
    volatile unsigned long delay;

    SYSCTL_RCGC2_R     |= SYSCTL_RCGC2_GPIOB;   // Enable GPIO Port B clock
    delay               = SYSCTL_RCGC2_R;
    GPIO_PORTB_DIR_R   |= (BIT(4) | BIT(5));    // PB4,5 are output
    GPIO_PORTB_AFSEL_R &= ~(BIT(4) | BIT(5));   // Disable alternate function
    GPIO_PORTB_AMSEL_R &= ~(BIT(4) | BIT(5));   // Disable analog functions
    GPIO_PORTB_PCTL_R  &= ~(PCTL(4) | PCTL(5)); // Clear PCTL content
    GPIO_PORTB_DEN_R   |= (BIT(4) | BIT(5));    // Enable digital
}

/* Combine active sounds and pass to DAC hardware */
void SysTick_Handler(void) {
    // Interrupt service routine
    // Executed at 11KHz
    int i;
    unsigned long DAC_acc = 0;
    int num_sounds_active = 0;

    for (i = 0; i < MAX_SOUND; i++) {
        if (sound_enabled[i]) {
            unsigned char DAC_val;
            num_sounds_active++;
            DAC_val = *(DAC_wav[i] + (DAC_step[i] / 2));

            if (DAC_step[i] % 2) DAC_acc += DAC_val & 0xF;
            else DAC_acc += DAC_val >> 4;

            if (DAC_step[i] / 2 >= DAC_max[i]) Sound_Off(i);
            DAC_step[i]++;
        }

        if (num_sounds_active > 0) DAC_Out(DAC_acc / num_sounds_active);
    }
}
