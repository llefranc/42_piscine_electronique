# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/13 15:03:52 by llefranc          #+#    #+#              #
#    Updated: 2023/03/28 13:48:29 by lucaslefran      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DEVICE		= atmega328p
F_CPU		= 16000000
UART_BAUDRATE	= 115200

CC		= avr-gcc
OBJCOPY		= avr-objcopy
# arduino bootloader
PROGRAMMER	= arduino
USB 		= /dev/cu.usbserial-141320

NAME		= atmega

SRCS_MA		= main.c
BINS_MA		= $(SRCS_MA:.c=.bin)

PATH_MO		= modes/
SRCS_MO		= mode.c mode_adc.c mode_aht.c mode_date.c mode_rgb.c
BINS_MO		= $(SRCS_MO:.c=.bin)

PATH_UT		= utils/
SRCS_UT		= i2c_pca.c uart.c timer.c
BINS_UT		= $(SRCS_UT:.c=.bin)

all: FLAGS = -Wall -Wextra -Werror -Os -mmcu=$(DEVICE) -DUART_BAUDRATE=$(UART_BAUDRATE) -DF_CPU=$(F_CPU)
all: clean hex flash

debug: FLAGS = -Wall -Wextra -Werror -Os -mmcu=$(DEVICE) -DDEBUG -DUART_BAUDRATE=$(UART_BAUDRATE) -DF_CPU=$(F_CPU)
debug: clean hex flash

# Genere les bin files puis le hex files au format intel
hex: $(SRCS_MA) $(addprefix $(PATH_MO), $(BINS_MO)) $(addprefix $(PATH_UT), $(BINS_UT))
	$(CC) -o $(NAME).bin $(FLAGS) $(SRCS_MA) $(addprefix $(PATH_MO), $(BINS_MO)) \
	$(addprefix $(PATH_UT), $(BINS_UT))
	$(OBJCOPY) -O ihex $(NAME).bin $(NAME).hex

# Flash en usb les fichiers hex
flash:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(USB) -b $(UART_BAUDRATE) -U flash:w:$(NAME).hex:i

clean:
	rm -rf *.bin *.hex $(PATH_MO)*.bin $(PATH_UT)*.bin

phony: all hex clean

%.bin: %.c %.h 
	@$(CC) $(FLAGS) -o $@ -c $<

$(PATH_MO)%.bin: $(PATH_MO)%.c $(PATH_MO)%.h 
	@$(CC) $(FLAGS) -o $@ -c $<

$(PATH_UT)%.bin: $(PATH_UT)%.c $(PATH_UT)%.h
	@$(CC) $(FLAGS) -o $@ -c $<