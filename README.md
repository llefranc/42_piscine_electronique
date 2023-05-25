# Piscine électronique (@42Paris)

https://www.youtube.com/watch?v=Q54Xc41ZGNc&ab_channel=lucaslefrancq

## About

This was an optionnal project that I did which introduced me to the world of microcontroller programming in C. 

42Chips, the electronic association from 42 school designed a dev kit with several components:
- An ATMega328P microcontroller with RAM and EEPROM
- Several push buttons
- Several red LEDs
- A temperature and humidity sensor reachable through I2C bus
- A real time clock reachable through I2C bus
- A 16 bits I/O expander reachable through I2C bus
- A segments display
- RGB LEDs reachable through SPI bus
- An UART to USB port
- 3 analog sensors: a potentiometer, a light detector and a NTC

> Full schema of the devkit available [here][2]

## The exercises

The goal of the project was to get familiar and interact will all these different components.  

We were allowed to only use C language and no external library. Each exercise has to be completed by interacting with the ATMega328P registers, using the chip's [documentation][1] as source.  

A new pool of exercises for a new thematic was unlocked each day of the week during two weeks.

- ➡️ DAY0: I/O registers.
- ➡️ DAY1: Timers and PWM cycles.
- ➡️ DAY2: Interruptions.
- ➡️ DAY3: UART protocol.
- ➡️ DAY4: I2C protocol.
- ➡️ DAY5: EEPROM.
- ➡️ DAY6: RGB LED.
- ➡️ DAY7: Analog-to-Digital Converter (ADC).
- ➡️ DAY8: SPI protocol.
- ➡️ DAY9: Expander and segments display.

There was also each weekend a rush project.

- ➡️ DAY0: Game using I2C protocol (unfortunately I didn't have enough time to do this one).
- ➡️ DAY1: A program interacting with all the devkit's components.

[1]: file:///mnt/nfs/homes/llefranc/Downloads/ATMEGA48A.PDF
[2]: https://github.com/llefranc/42_piscine_electronique/blob/main/devkit_schema.pdf
