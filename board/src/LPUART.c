/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
 * Copyright (c) 2016 - 2018, NXP.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "device_registers.h"	/* include peripheral declarations S32K144 */
#include "LPUART.h"

void PORT_init (void)
{
	/*!
	 * Pins definitions
	 * ===================================================
	 *
	 * Pin number        | Function
	 * ----------------- |------------------
	 * PTC6              | UART1 RX
	 * PTC7              | UART1 TX
	 */
  PCC->PCCn[PCC_PORTC_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */
  PORTC->PCR[6]|=PORT_PCR_MUX(2);	/* Port C6: MUX = ALT2, UART1 RX */
  PORTC->PCR[7]|=PORT_PCR_MUX(2);   /* Port C7: MUX = ALT2, UART1 TX */
}

void LPUART1_RxTx_IRQHandler(void) {
	char buffer_input[MAX_RECEIVE];

	if(LPUART1->STAT & LPUART_STAT_RDRF_MASK){
		LPUART1_receive_string(buffer_input, sizeof(buffer_input));
		LPUART1->CTRL |= LPUART_CTRL_TIE_MASK;
	}

	if(LPUART1->STAT & LPUART_STAT_TDRE_MASK){
		LPUART1_transmit_string("Package sent: ");
		LPUART1_transmit_string(buffer_input);
		LPUART1->CTRL &= ~LPUART_CTRL_TIE_MASK;
	}
}

void LPUART1_init(void)  /* Init. summary: 9600 baud, 1 stop bit, 8 bit format, no parity */
{
	PCC->PCCn[PCC_LPUART1_INDEX] &= ~PCC_PCCn_CGC_MASK;    /* Ensure clk disabled for config */
	PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_PCS(2)    /* Clock Src = 2 (SIRCDIV2_CLK) */
                            	 |  PCC_PCCn_CGC_MASK;     /* Enable clock for LPUART1 regs */

	LPUART1->BAUD = LPUART_BAUD_SBR(0x34)  	/* Initialize for 9600 baud, 1 stop: */
                	|LPUART_BAUD_OSR(15);  	/* SBR=52 (0x34): baud divisor = 8M/9600/16 = ~52 */
											/* OSR=15: Over sampling ratio = 15+1=16 */
											/* SBNS=0: One stop bit */
											/* BOTHEDGE=0: receiver samples only on rising edge */
											/* M10=0: Rx and Tx use 7 to 9 bit data characters */
											/* RESYNCDIS=0: Resync during rec'd data word supported */
											/* LBKDIE, RXEDGIE=0: interrupts disable */
											/* TDMAE, RDMAE, TDMAE=0: DMA requests disabled */
											/* MAEN1, MAEN2,  MATCFG=0: Match disabled */

	LPUART1->CTRL =	LPUART_CTRL_RE_MASK
					|LPUART_CTRL_TE_MASK	   	/* Enable transmitter & receiver, no parity, 8 bit char: */
					|LPUART_CTRL_RIE_MASK;		/* RE=1: Receiver enabled */
												/* TE=1: Transmitter enabled */
												/* PE,PT=0: No hw parity generation or checking */
												/* M7,M,R8T9,R9T8=0: 8-bit data characters*/
												/* DOZEEN=0: LPUART enabled in Doze mode */
												/* ORIE,NEIE,FEIE,PEIE,TIE,TCIE,RIE,ILIE,MA1IE,MA2IE=0: no IRQ*/
												/* TxDIR=0: TxD pin is input if in single-wire mode */
												/* TXINV=0: TRansmit data not inverted */
												/* RWU,WAKE=0: normal operation; rcvr not in statndby */
												/* IDLCFG=0: one idle character */
												/* ILT=0: Idle char bit count starts after start bit */
												/* SBK=0: Normal transmitter operation - no break char */
												/* LOOPS,RSRC=0: no loop back */
	S32_NVIC->ICPR[1] = (1 << (33 % 32));
	S32_NVIC->ISER[1] = (1 << (33 % 32));
	S32_NVIC->IP[33] = 0x02;
}

void LPUART1_transmit_char(char send) {    /* Function to Transmit single Char */
	while((LPUART1->STAT & LPUART_STAT_TDRE_MASK)>>LPUART_STAT_TDRE_SHIFT==0);
	/* Wait for transmit buffer to be empty */
	LPUART1->DATA=send;              /* Send data */
}

void LPUART1_transmit_string(char data_string[])  {  /* Function to Transmit whole string */
	uint32_t i=0;
	while(data_string[i] != '\0')  {           /* Send chars one at a time */
		LPUART1_transmit_char(data_string[i]);
		i++;
	}
}

char LPUART1_receive_char(void) {    /* Function to Receive single Char */
	char receive;
	while((LPUART1->STAT & LPUART_STAT_RDRF_MASK)>>LPUART_STAT_RDRF_SHIFT==0);
	/* Wait for received buffer to be full */
	receive= LPUART1->DATA;            /* Read received data*/
	return receive;
}

void LPUART1_receive_string(char* buffer, uint32_t buffer_size){
    uint32_t i = 0;
    char received_char;

    while (i < buffer_size - 1) {  // Giu lai 1 ky tu de dam bao chuoi co '\0'
        received_char = LPUART1_receive_char();

        if (received_char == '.') {  // Dung khi gap ky tu "."
            break;
        }

        buffer[i++] = received_char;  // Luu ky tu vao buffer
    }

    buffer[i] = '\0';
}

void LPUART1_receive_and_echo_char(void)  {  /* Function to echo received char back */
	char send = LPUART1_receive_char();        /* Receive Char */
	LPUART1_transmit_char(send);               /* Transmit same char back to the sender */
	LPUART1_transmit_char('\n');               /* New line */
	LPUART1_transmit_char('\r');               /* Return */
}
