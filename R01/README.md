# Final boss

The goal of this last project was to develop a firmware for the devboard to test that all the components were correctly working.

⚠️ **When I did this project, I wasn't aware that it was a [bad practice to perform longish tasks within a interrupt handler][1].** ⚠️ 

Nowadays I would have implement this program by splitting the interrupt handlers into two halves, like it's done in Linux, moving the routines to the main loop.

This project still show my coding style if you focus only on the routines code called from ISRs.

## About


[1]: https://www.oreilly.com/library/view/linux-device-drivers/0596000081/ch09s05.html
