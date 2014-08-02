
//
// dmxreciver
// AtTiny2313
//

#ifndef dmxreciver_h
#define dmxreciver_h

// dmx states
enum {IDLE = 0, BREAK, READ};

// actual dmx state
volatile uint8_t DMXState;

// channel input read count
volatile uint16_t DMXFrameCount;

// public functions
void dmxreciver_init(void);


#endif
