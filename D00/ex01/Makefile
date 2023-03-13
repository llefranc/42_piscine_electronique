# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/13 15:03:52 by llefranc          #+#    #+#              #
#    Updated: 2023/03/13 15:23:00 by llefranc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DEVICE		= atmega328p
F_CPU		= 1000000
BAUD		= 115200

CC		= avr-gcc
OBJCOPY		= objcopy
FLAGS		= -Wall -Os -mmcu=$(DEVICE) -DF_CPU=$(F_CPU)
# arduino bootloader
PROGRAMMER	= arduino
USB 		= /dev/ttyUSB0

FILES		= main

all: hex flash

# Genere les bin files puis les hex files au format intel
hex:
	$(CC) $(FLAGS) -c $(FILES).c -o $(FILES).bin
	$(OBJCOPY) -O ihex $(FILES).bin $(FILES).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILES).hex

# Flash en usb les fichiers hex
flash:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(USB) -b $(BAUD) -U flash:w:$(FILES).hex:i

clean:
	rm -rf *.bin *.hex
