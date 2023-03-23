# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/13 15:03:52 by llefranc          #+#    #+#              #
#    Updated: 2023/03/23 15:01:38 by llefranc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DEVICE		= atmega328p
F_CPU		= 16000000
UART_BAUDRATE	= 115200

CC		= avr-gcc
OBJCOPY		= avr-objcopy
FLAGS		= -Wall -Wextra -Werror -Os -mmcu=$(DEVICE) -DUART_BAUDRATE=$(UART_BAUDRATE) -DF_CPU=$(F_CPU)
# arduino bootloader
PROGRAMMER	= arduino
USB 		= /dev/ttyUSB0

NAME		= atmega
HEAD		= eeprom.h uart.h utils.h
SRCS		= main.c eeprom.c uart.c utils.c
BINS		= $(SRCS:.c=.bin)

all: hex flash

# Genere les bin files puis les hex files au format intel
hex: $(BINS)
	$(CC) -o $(NAME).bin $(FLAGS) $(BINS)
	$(OBJCOPY) -O ihex $(NAME).bin $(NAME).hex

# Flash en usb les fichiers hex
flash:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(USB) -b $(UART_BAUDRATE) -U flash:w:$(NAME).hex:i

$(BINS) : $(HEAD)

clean:
	rm -rf *.bin *.hex

phony: all hex clean

%.bin:	%.c
	@$(CC) $(FLAGS) -o $@ -c $<