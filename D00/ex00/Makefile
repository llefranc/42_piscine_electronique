# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/13 15:03:52 by llefranc          #+#    #+#              #
#    Updated: 2023/03/13 16:53:55 by llefranc         ###   ########.fr        #
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

FILE		= main


all: hex flash

# Genere les bin files puis les hex files au format intel
hex:
	$(CC) $(FLAGS) -c $(FILE).c -o $(FILE).bin
	$(OBJCOPY) -O ihex $(FILE).bin $(FILE).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILE).hex

# Flash en usb les fichiers hex
flash:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(USB) -b $(BAUD) -U flash:w:$(FILE).hex:i

clean:
	rm -rf *.bin *.hex