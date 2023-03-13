/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:54:03 by llefranc          #+#    #+#             */
/*   Updated: 2023/03/13 17:48:18 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

/*
 * sbi: set bit in register
 * ldi: load immedate
 * sbiw: substract immediate from word
 * sbc: subsctract with carry two registers
 * brne: branch if not equal
 * eor: exclusive OR register
 * in: in port
 * out: out port
 * rjmp: relative jump
 */

/*
Dump of assembler code for function main:
   0x00000080 <+0>:	sbi	0x04, 0		; set bit 0 in register DDRB (addr: 0x04) to 1
   0x00000082 <+2>:	ldi	r18, 0x01
   0x00000084 <+4>:	ldi	r24, 0x20       ; 500000 = 0x0007A120 > stored in register r24-27 in little endian
   0x00000086 <+6>:	ldi	r25, 0xA1
   0x00000088 <+8>:	ldi	r26, 0x07
   0x0000008a <+10>:	ldi	r27, 0x00
   0x0000008c <+12>:	sbiw	r24, 0x01       ; 500000--
   0x0000008e <+14>:	sbc	r26, r1
   0x00000090 <+16>:	sbc	r27, r1
   0x00000092 <+18>:	brne	.-8             ;  0x8c <main+12>  >>> while loop is 4 instructions long (from main+12 to main+18 included)
   0x00000094 <+20>:	in	r24, 0x05
   0x00000096 <+22>:	eor	r24, r18
   0x00000098 <+24>:	out	0x05, r24
   0x0000009a <+26>:	rjmp	.-24     	;  0x84 <main+4>
End of assembler dump.
*/

int main(void)
{
	long i;

	DDRB |= (1 << PIN0);
	while (1) {
		i = 0;

		/*
		 * Frequency is 1MHz. CPU executes 1 million instructions / s / MHz.
		 * We need to switch LED every 0,5 sec >> every 500 000 instructions.
		 * while loop takes 4 instruction. We should inc i until 500 000 / 4 = 125 000.
		 * 
		 * But i < 125 000 doesn't work !! But i < 2 000 000 (500 000 * 4) works,
		 * doesn't make any sense. Maybe the frequency is 4 MHz (the variable F_CPU in
		 * Makefile doesn't seem to work).
		 */
		while (i < 500000 * 4)
			i = i + 1;
		PORTB ^= (1 << PB0); /* switching between HIGH and LOW for PB0 */
	}
}