
# Settings for the basic options of the target board

# Name of target controller
MCU = attiny2313a

# Frequency of the controller
F_CPU = 16000000UL

# Maximal Memory size of eeprom in byte
EE_MAX_SIZE = 0

# Define the board configuration
HAS_LCD				= 0
HAS_UART 			= 0

# Boards-LED
LED_OK				= 0
LED_WARNING			= 1
STATUSLED_REGISTER	= DDRD
STATUSLED_PORT		= PORTD
STATUSLED_PIN		= PD2

CLOCK_REGISTER		= DDRD
CLOCK_PORT			= PORTD
CLOCK_NEXT_PIN		= PD4
CLOCK_RESET_PIN		= PD3

MATRIX_PORT			= PORTB

# Output debug messages on UART
DEBUG 				= 	0

BOARD_OPTS = 		-DHAS_LCD=$(HAS_LCD) \
					-DHAS_UART=$(HAS_UART) \
					-DLED_OK=$(LED_OK) \
					-DLED_WARNING=$(LED_WARNING) \
					-DSTATUSLED_REGISTER=$(STATUSLED_REGISTER) \
					-DSTATUSLED_PORT=$(STATUSLED_PORT) \
					-DSTATUSLED_PIN=$(STATUSLED_PIN) \
					-DCLOCK_REGISTER=$(CLOCK_REGISTER) \
					-DCLOCK_PORT=$(CLOCK_PORT) \
					-DCLOCK_NEXT_PIN=$(CLOCK_NEXT_PIN) \
					-DCLOCK_RESET_PIN=$(CLOCK_RESET_PIN) \
					-DMATRIX_PORT=$(MATRIX_PORT) \
					-DDEBUG=$(DEBUG)