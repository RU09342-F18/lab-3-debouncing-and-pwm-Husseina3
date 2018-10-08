/*
 * File: main.c for MSP430G2553 Software PWM
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

    P1DIR |=  BIT6;       // Port1's direction register is set to 1 at pin6 (makes it an output pin)
    P1OUT &= ~BIT6;       // This initializes the LED as off


//TimerA_0 is configured for Pulse with Modulation

    TA0CCR0 = 1000;    // TimerA_0's frequency
    TA0CTL = TASSEL_2 + MC_1 + TACLR; // Configure timer to SMCLK, Set it to up mode, Interrupt enable

//TimerA_1 is configured for Debouncing

    TA1CCTL0 = CCIE;         // Interrupt is enabled
    TA1CCR0 = 60000;         // Capture/compare register counts upto 10ms


    P1DIR &= ~BIT3;          // Port1's Direction register is set to 0 at pin 1.3(to make it as an input pin)
    P1REN |= BIT3;           // The resister at pin1.3 is enabled
    P1OUT |= BIT3;           // The enabled register at pin1.3 is set to be a pullup resister
    P1IE |= BIT3;            // Pin1.2 Interrupt enabled
    P1IES |= BIT3;           // The interrupt is set to be triggered at a falling edge
    P1IFG &= ~BIT3;          // Interrupt flag cleared

    __bis_SR_register(GIE);     // General purpose interrupt was


    for(;;) //Infinite for loop for the PWM
    {
        if(TA0R <= duty)

        P1OUT ^= BIT6; // 50% duty cycle

        else if (TA0R > duty)

        P1OUT &= ~BIT6;  // toggle LED
    }
}


#pragma vector=PORT1_VECTOR  // Interrupt Service Routine(ISR
__interrupt void PORT_1(void)
{
        switch(state)
        {
        case 0: // While state is 0 ( when LED goes from OFF to ON) do the following
            TA1CTL = TASSEL_2 + MC_1 + TACLR;       // Configure timer to SMCLK, Set it to up mode, Interrupt enable
            P1IFG &= ~BIT3;                         // Interrupt interrupt flag
            P1IE &= ~BIT3;                          // Pin1.3 Interrupt is disabled
            break;
        case 1: // When case is 1 do the following
            TA1CTL = TASSEL_2 + MC_1 + TACLR;       // Configure timer to SMCLK, Set it to up mode, Interrupt enable
            P1IFG &= ~BIT3;                         // Interrupt interrupt flag
            P1IE &= ~BIT3;                          // Pin1.3 Interrupt is disabled
            break;
        }
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A0 (void) // TimerA_0 Interrupt Service Routine (ISR)
{
        switch(state)
        {
        case 0:// While LED is on and staying on, do the following
            if(duty < 1000) duty += 100;  // Duty cycle increases by 10%
            else duty = 0;                // Duty cycle is set to 0
            P1IE |= BIT3;                 // The pin1.3 interrupt is enabled
            P1IES &= ~BIT3;               // The interrupt is set to be triggered at a rising edge
            TA1CTL &= ~TASSEL_2;          // Disable SMCLK
            TA1CTL |= TACLR;              // Clear Timer
            state = 1;                    // Set the state to state 1
            break;
        case 1://While the LED is off and stays off, Perform the following
            P1IE |= BIT3;               // The pin1.3 interrupt is enabled
            P1IFG &= ~BIT3;             // Pin1.3 interrupt flag is disabled
            P1IES |= BIT3;              // Pin1.3 interrupt is set to be triggered on a falling edge
            TA1CTL &= ~TASSEL_2;        // Disable SMCLK
            TA1CTL |= TACLR;            // Clear timer
            state = 0;                  // Set the state to state 0
            break;
        }


}
