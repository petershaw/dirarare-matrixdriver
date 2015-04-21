//
//  main.c
//  AtTiny2313 matrix dev board
//
//

#include <stdlib.h>
#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "config.h"
#include "lights.h"
#include "dmx/dmxreciver.h"
#include "status/status_indicator.h"

// DUMMY FUNCTION
// ---------------------------------------------
void *dummy(void){return (void *) NULL;}

// Location memmory
// ---------------------------------------------
volatile uint8_t dimCounter	= 0;
volatile uint8_t rowCounter	= 1;
volatile uint8_t doNextRow 	= 0;
uint8_t doLastRow 	= 0;


// INTERRUPT SERVICE
// ---------------------------------------------
// Row multiplexer
ISR (TIMER0_OVF_vect){ 
 	// set value and move to next row

	// Calculate next row
	uint8_t value = 0x00;
	/**
	 * Get dimming of light by ((a >> 3) & 0x01)
	 * and write to value position of row position
	 */ 	
	for(int curLight = 0; curLight < 8; ++curLight){
		uint8_t lightdimvalue = LIGHT[(rowCounter *8) +curLight];
		value |= 
			((DIM[lightdimvalue] >> dimCounter) & 0x01)
			<< curLight
			;
	}

	MATRIX_PORT = ~(value);
	CLOCK_PORT &= ~_BV(CLOCK_NEXT_PIN);
	CLOCK_PORT |= _BV(CLOCK_NEXT_PIN);
	
    ++rowCounter;
    if(rowCounter == 8){
     	rowCounter = 0;
 		++dimCounter;
		if(dimCounter == 8){
			dimCounter = 0;
            STATUSLED_PORT ^= _BV(STATUSLED_PIN);
		}
     }
}

int main(void) {
	// SET ALL LIGHTS OFF
	DDRB |= 0b11111111;
	MATRIX_PORT = 0b11111111;

	init_status_indicator();
	mark_as_error();
	lights_init();
	_delay_ms(300);
		
 	// INIT CLOCK
	CLOCK_REGISTER |= _BV(CLOCK_NEXT_PIN) | _BV(CLOCK_RESET_PIN);
   	CLOCK_PORT |= _BV(CLOCK_NEXT_PIN);

 	//RESET COUNTER - be sure to start at Q1
   	CLOCK_PORT |=  _BV(CLOCK_RESET_PIN);
   	CLOCK_PORT &=  ~_BV(CLOCK_RESET_PIN);
   	CLOCK_REGISTER &= ~_BV(CLOCK_RESET_PIN);
	
	// INIT MULTIPLEX TIMER
    // -----------------------------------------
    // Timer 0, Prescaller /8; OVF-Interrupt
    TCCR0B = (1<<CS01);
    // Overflow Interrupt erlauben
 	TIMSK |= (1<<TOIE0);
 	// Set MAX 
 	OCR0A = 156-1;
    	
   	release_error();		
	
 
    dmxreciver_init();
	sei();

	while(1){

	}
	
}
