/*
 * File: main.c for MSP430FR2311 for Hardware PWm
 * Author: Anwar Hussein
 * Created on: 10/4/2018
 * Last Edited: 10/6/2018
 */

#include <msp430.h> 

int Duty = 500; // Initialize with 50% duty cycle

PM5CTL0 &= ~LOCKLPM5;            

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   //Disable Watchdog Timer to prevent the system from unnecessary reset

    P2DIR |= BIT0;          // P2.0 as output
    P2SEL |= BIT0;          // This line of code selects Pin2.0 as a GPIO pin

    P1DIR &= ~BIT1;         // Port1's Direction register bit is set to 0 at pin 1.1(makes the bit an input bit)
    P1REN |= BIT1;          // Pin1.1 register is enabled
    P1OUT |= BIT1;          // The enabled register is set to be a pullup register
    P1IE |= BIT1;           // Pin1.1 interrupt is enabled
    P1IFG &= ~BIT1;         // Clear interrupt flag

    TB1CCTL0 = CCIE;        // The TimerB_1 Capture and compare register Interrupt is enabled.
    TB1CCR0 = 10000;        // The CCR0 up limit is 10ms and it fires interrupt

    TB0CCTL1 = OUTMOD_7;    // This sets the output mode to  Set/Reset
    TB0CCR0 = 1000-1;       // Timera_0's upper bound for counting is 1ms
    TB0CCR1 = 500;          // This will initialize the duty cycle as 50%
    TB0CTL = TASSEL_SMCKL + MC--UP + TBCLR + ID_2; // SMCLK, up mode

    __bis_SR_register(LPM0_bits + GIE);       // Low Power Mode (LPM)

    return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{

    TB1CTL = TASSEL_SMCKL + MC--UP + TBCLR + ID_2;  // TimerA_1 is set to up mode

    P1IFG &= ~BIT1;     // Clear Pin 1.1 Interrupt
    P1IES &= ~BIT1;     // Interrupt is set to rising edge
}

#pragma vector=TIMER1_B0_VECTOR
__interrupt void Timer1_B0 (void)
{
    if(Duty < 1000) Duty += 100; // Duty cycle increments by 10%
    else Duty = 0;
    P1IE |= BIT1;      // Pin1.1 interrupt is enabled
    TB0CCR1 = Duty;    // The duty cycle is initialized at 50% and keeps incrementing
    TB0CTL = 0x00;     // Timer is stopped.
}
