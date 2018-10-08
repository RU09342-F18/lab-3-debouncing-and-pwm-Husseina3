# Software PWM
In this part of the lab a software pulse width modulation was implemented by generating a 1khz PWM signal. The duty cycle ranges from 0% to 100%. Initially the duty cycle is set to be at a 50% duty cycle. On each button press the duty cycle increments by 10% until it reaches 100%. 

## Implementation on MSP430G2553 and MSP430FR2311
The first thing that was done on the implementation process was to initialize the duty cycle of PWW as 50%. The buttons that send the interrupt were set for the respective microcontroller. 
Both TimerA_0 and TimerA_1 were configured for Debouncing and PWM purposes.Then in the Interrupt service routine the PWM duty cycle was set to increase 10% every button press. 

### Demo Video
https://youtu.be/ZfYTflezPIs
