
//
// lights
// 
//

#ifndef lights_h
#define lights_h

#include <inttypes.h>

// LIGHT SETUP
// ---------------------------------------------
	
// lookup table of dim values
 uint8_t	DIM[9]; /*  = {
		 0b00000000
		,0b00000001
		,0b00000011
		,0b00000111
		,0b00001111
		,0b00011111
		,0b00111111
		,0b01111111
		,0b11111111
	};
*/

// Stores dim value of all off the 64 lights
volatile uint8_t LIGHT[64];

void lights_init(void);

#endif
