
#include <string.h>
#include "lights.h"

/** 
 * INIT THE LIGHTS AND DIMM ARRAY
 * ---------------------------------------------------------------------------------------
 */ 
 void lights_init(void){
 
	DIM[0] = 0b00000000;
	DIM[1] = 0b00000001;
	DIM[2] = 0b00000011;
	DIM[3] = 0b00000111;
	DIM[4] = 0b00001111;
	DIM[5] = 0b00011111;
	DIM[6] = 0b00111111;
	DIM[7] = 0b01111111;
	DIM[8] = 0b11111111;

	//memset(LIGHT, 0, 64 * 1);

 }