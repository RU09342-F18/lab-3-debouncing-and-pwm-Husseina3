/*
 * File: main.c for MSP430G2553 for Hardware PWm
 * Author: Anwar Hussein
 * Created on: 10/4/2018
 * Last Edited: 10/6/2018
 */

#include <msp430.h> 

int Duty = 500; // Initialize with 50% duty cycle

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   //Disable Watchdog Timer to prevent the system from unnecessary reset

    P1DIR |= BIT6;          // P1.6 as output
    P1SEL |= BIT6;          // This line of code selects Pin1.6 as a GPIO pin

    P1DIR &= ~BIT3;         // Port1's Direction register bit is set to 0 at pin 1.3(makes the bit an input bit)
    P1REN |= BIT3;          // Pin1.3 register is enabled
    P1OUT |= BIT3;          // The enabled register is set to be a pullup register
    P1IE |= BIT3;           // Pin1.3 interrupt is enabled
    P1IFG &= ~BIT3;         // Clear interrupt flag

    TA1CCTL0 = CCIE;        // The TimerA_1 Capture and compare register Interrupt is enabled.
    TA1CCR0 = 10000;        // The CCR0 up limit is 10ms and it fires interrupt

    TA0CCTL1 = OUTMOD_7;    // This sets the output mode to  Set/Reset
    TA0CCR0 = 1000-1;       // Timera_0's upper bound for counting is 1ms
    TA0CCR1 = 500;          // This will initialize the duty cycle as 50%
    TA0CTL = TASSEL_2 | MC_1; // SMCLK, up mode

    __bis_SR_register(LPM0_bits + GIE);       // Low Power Mode (LPM)

    return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{

    TA1CTL = TASSEL_2 + MC_1;  // TimerA_1 is set to up mode

    P1IFG &= ~BIT3;     // Clear Pin 1.3 Interrupt
    P1IES &= ~BIT3;     // Interrupt is set to rising edge
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{
    if(Duty < 1000) Duty += 100; // Duty cycle increments by 10%
    else Duty = 0;
    P1IE |= BIT3;      // Pin1.3 interrupt is enabled
    TA0CCR1 = Duty;    // The duty cycle is initialized at 50% and keeps incrementing
    TA1CTL = 0x00;     // Timer is stopped.
}
