# Final boss

The goal of this last project was to develop a firmware for the devboard to test that all the components were correctly working.

⚠️ **When I did this project, I wasn't aware that it was a [bad practice to perform longish tasks within a interrupt handler][1]** 😮‍💨 ⚠️  

Nowadays I would have implement this program by splitting the interrupt handlers into two halves, like it's done in Linux, moving the routines to the main loop.

This project still show my coding style if you focus only on the routines code called from ISRs. :thumbsup:

## Demo

:arrow_down: Below is the video of the result and the different modes :arrow_down:

[![Watch the video](https://github.com/llefranc/42_piscine_electronique/blob/main/youtube_screen.png)](https://youtu.be/Q54Xc41ZGNc)

## About

The goal was to switch between 11 modes using SW1 (next mode) and SW2 (previous mode) buttons. The mode number is displayed in binary on the 4 LEDs at the right top corner of the boad.  

Each mode display the value of a component connected to the ATMega328P. When you start the program, there is an init sequence of 5 seconds that turn ON then OFF the LEDs and segments-display. You can then use the buttons to switch bewteen the different modes.

## Code architecture

Each mode has 4 different routines:
- An **init routine**, called to init the mode (init components' ressources and timer0/timer1 interrupts).
- A **clear routine**, called to clear components' ressources and timer0/timer1 interrupts.
- A **timer0 routine**, used to draw the mode value on the segments-display each x ms.
- A **timer1 routine**, used to refresh the mode value each x ms.

> Some modes have the same routines when it was possible to minimize the code size.

When a button is pressed:

- The **clear routine** of the actual mode is called
- The **init routine** of the prev/next mode is then called.

## Explanation of the different modes

- ➡️ **Mode 1**: potentiometer value through ADC.  
- ➡️ **Mode 2**: LDR value through ADC.  
- ➡️ **Mode 3**: NTC value through ADC.  
- ➡️ **Mode 4**: internal temp sensor through ADC.  
- ➡️ **Mode 5**: setting RGB LEDs through SPI bus.  
- ➡️ **Mode 6**: temp sensor in Cel through I2C bus.  
- ➡️ **Mode 7**: temp sensor in Far through I2C bus.   
- ➡️ **Mode 8**: humidity sensor in percentage through I2C bus. 
- ➡️ **Mode 9**: time from RTC through I2C bus.  
- ➡️ **Mode 10**: day and month from RTC through I2C.  
- ➡️ **Mode 11**: year from RTC through I2C.  


[1]: https://www.oreilly.com/library/view/linux-device-drivers/0596000081/ch09s05.html
