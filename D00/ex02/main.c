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
 * sbiw: substract immediate from word (2 clocks cycle)
 * sbc: substract with carry two registers (1 clock cycle)
 * brne: branch if not equal (2 clock cycle if not equal)
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
		 * We need to switch LED every 0,5 sec.
		 * while loop takes 4 instruction, but 6 clocks cycle. 
		 * 
		 * The frequency seems to be 16MHz.
		 * - Switching LED every 0,5 sec => 16 0000 0000 / 2 = 8 000 000
		 * - One loop tour takes 6 clock cycles => 8 000 000 / 6 = 1 333 333 
		 */
		while (i < 1333333)
			i = i + 1;
		PORTB ^= (1 << PB0); /* switching between HIGH and LOW for PB0 */
	}
}