# Software Debouncing
In this section of the lab a software deboucer was implmented using a timer module. The two boards that were used were MSP430G3553 and MSP430F5529. 
## Implementation
MSP430G2553 has two timers, Timer0_A and Timer1_A. Timer0_A being the default, it has a register named TA0R.

|Term| Description|
|---| ------------|
|TA0R| is the TimerA_0 Register|
|TA0CCR0|is the capture/compare register which is used as the terminal count when counting UP.|
|TA0CTL| is the control register used to configure the timer.|
|TASSEL_2|selects the internal 1MHZ Clock (SMCLK).|
|MC_1| sets the count mode to count up.|

Ther TA0CCR0 was set to count up to 0.6 sec and fires the interrupt. After the Toggle the inturrupt is clearned and the processor sleeps until the next intturpt or button press. Debouncing is very important when using switches to send signal to a digital circuit. That is because sometimes when a button is pressed once it might appear in the digital circuit that the button was pressed multiple times. we avaid this we use hardware or software debounicng. In our case a software debouncing was implemented. 

### Demo Video
https://youtu.be/JH4PBkjGvb0
