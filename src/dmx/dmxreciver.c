
#include <stdio.h>
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../config.h"
#include "../lights.h"
#include "../status/status_indicator.h"
#include "dmxreciver.h"

/** 
 * DMX RECIVE
 * ---------------------------------------------------------------------------------------
 */ 
ISR (USART_RX_vect){
	mark_as_error();
	// store ucsra register for frame error flag
	uint8_t statusreg = UCSRA;

	// get data
	uint8_t DMXDataByte = UDR;

	// check for break (error flag is set)
	if (bit_is_set(statusreg, FE)){
		// reset frame counter
		DMXFrameCount = 0;

		// set break status
		DMXState = BREAK;
	
		// abort on frame error
		return;
	}

	// state machine	
	switch (DMXState){
		case BREAK:
			// check for valid startbyte
			DMXState = (DMXDataByte == 0) ? READ : IDLE;
			break;

		case IDLE:
			// do nothing -> wait for break
			break;

		case READ:
			// increment frame count
			DMXFrameCount++;	
			
			// wait for dmx address
			if (DMXFrameCount>=DMXAddress){
				// calculate dimm value
				uint8_t value = (DMXDataByte +1) /32;
				// store value in ligth array
				LIGHT[DMXFrameCount-DMXAddress] = value;
			}

			// wait for end of reading channels
			if (DMXFrameCount >= DMXAddress+DMX_CHANNELS-1){
				// set status idle => no action
				DMXState = IDLE;
			}

			break;
	}
	release_error();
}

/** 
 * DMX INIT
 * ---------------------------------------------------------------------------------------
 */ 
void dmxreciver_init(void){
	DMXFrameCount = 0;
	
	// 250k baud - 16MHz
	// unsigned long baud = 250000;
	UBRRH = 0;
	UBRRL = 3;

	
	// UART CONTROL
	// 7:0 - RXC - USART Receive Complete
	// 6:0 - TXC - USART Transmit Complete
	// 5:0 - UDRE - USART Data Register Empty
	// 4:0 - FE - Frame Error
	// 3:0 - DOR - Data OverRun
	// 2:0 - UPE - USART Parity Error
	// 1:0 - U2X - Double the USART Transmission Speed
	// 0:0 - MPCM - Multi-processor Communication Mode
	UCSRA = 0b00000000;
	
	// 7:0 - reserved
	// 6:0 - UMSEL - async operation
	// 5:0 - UPM1 - no paritiy
	// 4:0 - UPM0 - no paritiy
	// 3:1 - USBS - 2 stop bits
	// 2:1 - UCSZ1 - 8bit mode
	// 1:1 - UCSZ0 - 8bit mode
	// 0:0 - UCPOL - clock polarity, rising edge
	UCSRC = 0b00001110;

	// UART CONTROL
	// 7:1 - RXCIE - RX Complete Interrupt Enable
	// 6:0 - TXCIE - TX Complete Interrupt Disable
	// 5:0 - UDRIE - USART Data Register Empty Interrupt Disable
	// 4:1 - RXEN - Receiver Enable
	// 3:0 - TXEN - Transmitter Disable
	// 2:0 - UCSZ2 - 8bit mode
	// 1:0 - RXB8 - Receive Data Bit 8
	// 0:0 - TXB8 - Transmit Data Bit 8
	UCSRB = 0b10010010;

	
	// initialize DMX State
	DMXState = IDLE;
	
}