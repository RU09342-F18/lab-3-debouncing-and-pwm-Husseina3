/*
 * File: main.c for MSP430FR2311
 * Author: Anwar Hussein
 * Created on: 10/02/2018
 * Last Edited: 10/06/2018
 */

#include <msp430.h>

int state = 0;
int duty = 500;     // set the initial duty cycle to 50%

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // Disable the WatchDog Timer

    P1DIR |=  BIT0;       // Port1's direction register is set to 1 at pin0 (makes it an output pin)
    P1OUT &= ~BIT0;       // This initializes the LED as off


//TimerA_0 is configured for Pulse with Modulation

    TB0CCR0 = 1000;    // TimerB_0's frequency
    TB0CTL = TBSSEL_2 + MC_1 + TBCLR; // Configure timer to SMCLK, Set it to up mode, Interrupt enable

//TimerA_1 is configured for Debouncing

    TB1CCTL0 = CCIE;         // Interrupt is enabled
    TB1CCR0 = 50000;         // Capture/compare register counts upto 10ms


    P1DIR &= ~BIT1;          // Port1's Direction register is set to 0 at pin 1.1(to make it as an input pin)
    P1REN |= BIT1;           // The resister at pin1.3 is enabled
    P1OUT |= BIT1;           // The enabled register at pin1.1 is set to be a pullup resister
    P1IE |= BIT1;            // Pin1.1 Interrupt enabled
    P1IES |= BIT1;           // The interrupt is set to be triggered at a falling edge
    P1IFG &= ~BIT1;          // Interrupt flag cleared

    __bis_SR_register(GIE);     // General purpose interrupt was


    for(;;) //Infinite for loop for the PWM
    {
        if(TB0R <= duty)

        P1OUT ^= BIT0; // 50% duty cycle

        else if (TB0R > duty)

        P1OUT &= ~BIT0;  // toggle LED
    }
}


#pragma vector=PORT1_VECTOR  // Interrupt Service Routine(ISR)
__interrupt void PORT_1(void)
{
        switch(state)
        {
        case 0: // While state is 0 ( when LED goes from OFF to ON) do the following
            TB1CTL = TBSSEL_2 + MC_1 + TBCLR;       // Configure timer to SMCLK, Set it to up mode, Interrupt enable
            P1IFG &= ~BIT1;                         // Interrupt interrupt flag
            P1IE &= ~BIT1;                          // Pin1.1 Interrupt is disabled
            break;
        case 1: // When case is 1 do the following
            TB1CTL = TBSSEL_2 + MC_1 + TBCLR;       // Configure timer to SMCLK, Set it to up mode, Interrupt enable
            P1IFG &= ~BIT1;                         // Interrupt interrupt flag
            P1IE &= ~BIT1;                          // Pin1.1 Interrupt is disabled
            break;
        }
}

#pragma vector=TIMER1_B0_VECTOR
__interrupt void Timer_B0 (void) // TimerA_0 Interrupt Service Routine (ISR)
{
        switch(state)
        {
        case 0:// While LED is on and staying on, do the following
            if(duty < 1000) duty += 100;  // Duty cycle increases by 10%
            else duty = 0;                // Duty cycle is set to 0
            P1IE |= BIT1;                 // The pin1.1 interrupt is enabled
            P1IES &= ~BIT1;               // The interrupt is set to be triggered at a rising edge
            TB1CTL &= ~TBSSEL_2;          // Disable SMCLK
            TB1CTL |= TBCLR;              // Clear Timer
            state = 1;                    // Set the state to state 1
            break;
        case 1://While the LED is off and stays off, Perform the following
            P1IE |= BIT1;               // The pin1.1 interrupt is enabled
            P1IFG &= ~BIT1;             // Pin1.1 interrupt flag is disabled
            P1IES |= BIT1;              // Pin1.1 interrupt is set to be triggered on a falling edge
            TB1CTL &= ~TBSSEL_2;        // Disable SMCLK
            TB1CTL |= TBCLR;            // Clear timer
            state = 0;                  // Set the state to state 0
            break;
        }


}
