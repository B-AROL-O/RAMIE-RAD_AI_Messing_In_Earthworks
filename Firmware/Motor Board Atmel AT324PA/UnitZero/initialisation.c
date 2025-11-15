/****************************************************************************
**	INCLUDE
****************************************************************************/

#include "global.h"

/****************************************************************************
** GLOBAL INITIALISATION
****************************************************************************/

void global_init( void )
{
	///**********************************************************************
	///	PORT I/O SETTING:
	///**********************************************************************
	///	'I' = Input (HI-Z)
	///	'R' = Input (Rpu)
	///	'L' = Output (low)
	///	'H' = Output (Hi)
	///	the bit are in the order: LSB, ... , MSB
	///	TIPS: unused pin should be configured as 'R' to avoid power losses
	///	by spurius transition on the disconnected pin
	///	TIPS: I/O is the primary function of any pin, if you engage a pheriperal
	///	that use a pin, that configuration will override the pin's I/O configuration
	///	TIPS: if you want to use oc0,1a,1b,2 waveform pins, you must
	///	configure the related I/O pin as output
	///**********************************************************************

	//PA0			: Servo 0
	//PA1			: Servo 1
	//PA2			: Servo 2
	//PA3			: Servo 3
	//PA4			: Servo 4
	//PA5			: Servo 5
	//PA6			: Servo 6
	//PA7			:
	PORT_A_CONFIG('L','L','L','L','L','L','L','L');
	

	//PB0			:
	//PB1			:
	//PB2			:
	//PB3			:
	//PB4			:
	//PB5			:
	//PB6			:
	//PB7			:
	PORT_B_CONFIG('R','R','R','R','R','R','R','R');

	//PC0			: LED#
	//PC1			: LCD_PWR#
	//PC2			: LCD_EN
	//PC3			: LCD_RS
	//PC4			: LCD_D4
	//PC5			: LCD_D5
	//PC6			: LCD_D6
	//PC7			: LCD_D7
	PORT_C_CONFIG('H','H','L','L','L','L','L','L');

	//PD0			: RPI_TXO AT_RXI
	//PD1			: RPI_RXI AT_TXO
	//PD2			: dummy zero volt
	//PD3			:
	//PD4			:
	//PD5			:
	//PD6			:
	//PD7			:
	PORT_D_CONFIG('R','H','L','R','R','R','R','R');

	///**********************************************************************
	///	DEVICE INITIALISATION:
	///**********************************************************************

	//Init the serial port 1. Rx INT active
	usart0_init();

	//100Hz time base
	timer0_init();
	//Precise delay for the servos
	timer1_init();

			///LCD Init
	//Turn OFF Delay
	//This is meant to allow LCD display to safely power down and reset
	//If it's too short, the LCD will bug out when quickly doing ON -> OFF -> ON 
	_delay_ms( LCD_INIT_DELAY );
	//Power Up the LCD Display
	CLEAR_BIT( PORTC, 1 );
	//Turn ON Delay
	//This is meant to give the LCD Display time to safely power Up
	//Ifit's too short, The LCD will bug out when doing OFF -> ON -> COMMANDS
	_delay_ms( LCD_INIT_DELAY );
	//Initialise the display and the driver: Send the sequences that configure the display
	lcd_init();

	///**********************************************************************
	///	ENABLE ALL INTERRUPT:
	///	TIPS: sei() should be called after all the pheriperals have been configured,
	///	so that ISR will not mess up other initialisation.
	/// TIPS: the "all interrupt enable flag" is automatically shutted down at the
	///	start of any ISR and engaged again at it's bottom to avoid slow nested ISR call,
	///	if you need nested ISR call then call sei() at the beginning of the ISR
	///	TIPS: the function to disable all the interrupt is "cli()"
	///**********************************************************************

	sei();

	return;
}	//end function: global_initialisation

/****************************************************************************
** PHERIPERALS INITIALISATION
****************************************************************************/

/****************************************************************************
** USART0 INITIALISATION
*****************************************************************************
**	BAUD = Fclk / Kp / (UBRR +1)
**
**	Fclk = 		20 MHz		clock speed [Hz]
**	BAUD = 					usart baud rate [b/s]
**	Kp = 					prescaler (16 normal mode, 8 fast mode)
**	UBRR = 					second prescaler
**
**	UBRR = Fclk / BAUD / Kp - 1
**
**	20MHz
**	BAUD		|	UBRR
**	-----------------
**	50 Kb/s		|	24
**	62,5 Kb/s	|	19
**	125 Kb/s	|	9
**	250 Kb/s	|	4
**
**	8MHz
**	BAUD		|	UBRR
**	-----------------
**	62,5 Kb/s	|	7
****************************************************************************/

void usart0_init( void )
{
	///----------------------------------------------------------------------
	//	STATIC VARIABILE
	///----------------------------------------------------------------------

	///----------------------------------------------------------------------
	//	LOCAL VARIABILE
	///----------------------------------------------------------------------

	uint8_t ucsr0a_temp = 0x00;
	uint8_t ucsr0b_temp = 0x00;
	uint8_t ucsr0c_temp = 0x00;

	///----------------------------------------------------------------------
	//	REGISTER CONFIGURATION
	///----------------------------------------------------------------------

	//Double the USART0 speed
	//SET_BIT( ucsr0a_temp, U2X0 );

	//Multi-processor mode
	//SET_BIT( ucsr0a_temp, MPCM0 );

	//Rx interrupt enable
	SET_BIT( ucsr0b_temp, RXCIE0 );

	//Tx interrupt enable
	//SET_BIT( ucsr0b_temp, TXCIE0 );

	//Tx buffer empty interrupt enable
	//SET_BIT( ucsr0b_temp, UDRIE0 );

	//Enable receiver
	SET_BIT( ucsr0b_temp, RXEN0 );

	//Enable Transmitter
	SET_BIT( ucsr0b_temp, TXEN0 );

	//Word size
	//	2	1	0	|	Mode
	//-------------------------------
	//	0	0	0	|	5 bits
	//	0	0	1	|	6 bits
	//	0	1	0	|	7 bits
	//	0	1	1	|	8 bits
	//	1	0	0	|	Reserved
	//	1	0	1	|	Reserved
	//	1	1	0	|	Reserved
	//	1	1	1	|	9 bits
	//SET_BIT( ucsr0b_temp, UCSZ02 );
	SET_BIT( ucsr0c_temp, UCSZ01 );
	SET_BIT( ucsr0c_temp, UCSZ00 );

	//Operation mode
	//	1	0	|	Mode
	//-------------------------------
	//	0	0	|	Asynchronous USART
	//	0	1	|	Synchronous USART
	//	1	0	|	Reserved
	//	1	1	|	Master SPI
	//SET_BIT( ucsr0c_temp, UMSEL01 );
	//SET_BIT( ucsr0c_temp, UMSEL00 );

	//Parity Check
	//	1	0	|	Mode
	//-------------------------------
	//	0	0	|	Disabled
	//	0	1	|	Reserved
	//	1	0	|	Even Parity
	//	1	1	|	Odd Parity
	//SET_BIT( ucsr0c_temp, UPM01 );
	//SET_BIT( ucsr0c_temp, UPM00 );

	//Two stop bits enabled
	//SET_BIT( ucsr0c_temp, USBS0 );

	//XCK0 clock polarity (synchronous mode only)
	//SET_BIT( ucsr0c_temp, UCPOL0 );

	///----------------------------------------------------------------------
	//	REGISTER WRITE BACK
	///----------------------------------------------------------------------

	UCSR0A = ucsr0a_temp;
	UCSR0B = ucsr0b_temp;
	UCSR0C = ucsr0c_temp;

	UBRR0H = 0;
	UBRR0L = UART_UBRR;

	return;
}	//end function: usart0_initialisation




/****************************************************************************
**	TIMER0 INITIALISATION
*****************************************************************************
**	50Hz interrupt time base
**	CTC mode
**	Fclk = 20e6
**	N = 1024
**	F = Fclk / N / (OCR+1)
**	OCR = Fclk * T / N - 1
**	T = N * (OCR+1) / Fclk
**
**	Fclk	N	OCR		T
**	20e6	64	48		156.8uS
**	20e6	8	249		100uS
**
****************************************************************************/

void timer0_init( void )
{
	///**********************************************************************
	///	STATIC VARIABILE
	///**********************************************************************

	///**********************************************************************
	///	LOCAL VARIABILE
	///**********************************************************************

	//temp control register variabile
	uint8_t tccr0a_temp = 0x00;
	uint8_t tccr0b_temp = 0x00;
	uint8_t timsk0_temp = 0x00;

	///**********************************************************************
	///	CONTROL REGISTER SETTINGS
	///**********************************************************************

	//	1	0	|	OC0A
	//-------------------------------
	//	0	0	|	Disconnected
	//	0	1	|	Toggle on CM
	//	1	0	|	Clear on CM
	//	1	1	|	Set on CM

	//SET_BIT( tccr0a_temp, COM0A0 );
	//SET_BIT( tccr0a_temp, COM0A1 );

	//	1	0	|	OC0B
	//-------------------------------
	//	0	0	|	Disconnected
	//	0	1	|	Toggle on CM
	//	1	0	|	Clear on CM
	//	1	1	|	Set on CM

	//SET_BIT( tccr0a_temp, COM0B0 );
	//SET_BIT( tccr0a_temp, COM0B1 );

	//Compare Out Mode
	//	2	1	0	|	Mode
	//-------------------------------
	//	0	0	0	|	Normal (TOP = 0xff)
	//	0	0	1	|	PWM Phase Correct (TOP = 0xff)
	//	0	1	0	|	CTC (TOP = OCRA)
	//	0	1	1	|	Fast PWM (TOP = 0xff)
	//	1	0	0	|	Reserved
	//	1	0	1	|	PWM Phase Correct (TOP = OCRA)
	//	1	1	0	|	Reserved
	//	1	1	1	|	Fast PWM (TOP = OCRA)

	//SET_BIT( tccr0a_temp, WGM00 );
	SET_BIT( tccr0a_temp, WGM01 );
	//SET_BIT( tccr0b_temp, WGM02 );

	///Clock Source
	///	CS02	|	CS01	|	CS00	||
	///-------------------------------------------
	///	0		|	0		|	0		|| no clock
	///	0		|	0		|	1		|| clk / 1
	///	0		|	1		|	0		|| clk / 8
	///	0		|	1		|	1		|| clk / 64
	///	1		|	0		|	0		|| clk / 256
	///	1		|	0		|	1		|| clk / 1024
	///	1		|	1		|	0		|| T0 pin, falling edge
	///	1		|	1		|	1		|| T0 pin, rising edge

	//SET_BIT( tccr0b_temp, CS00 );
	SET_BIT( tccr0b_temp, CS01 );
	//SET_BIT( tccr0b_temp, CS02 );

	//Output compare 0a interrupt
	//This interrupt is launched when TCNT0 is equal to OCR0A
	SET_BIT( timsk0_temp, OCIE0A );

	//Output compare 0a interrupt
	//This interrupt is launched when TCNT0 is equal to OCR0B
	//SET_BIT( timsk0_temp, OCIE0B );

	//overflow interrupt enable
	//is launched when TCNT0 goes in overflow
	//SET_BIT( timsk0_temp, TOIE0 );

	///**********************************************************************
	///	CONTROL REGISTER WRITEBACK
	///**********************************************************************

	TCCR0A = tccr0a_temp;
	TCCR0B = tccr0b_temp;
	TIMSK0 = timsk0_temp;

	OCR0A = 249;
	OCR0B = 0;

	return;
}	//end function: timer0_initialisation

/****************************************************************************
** TIMER1 INITIALISATION
*****************************************************************************
**	I need to handle 6 special PWM channels, i use this this timer as an interrupt
**	countdown to pull down the channels at the right moment
**
****************************************************************************/

void timer1_init( void )
{
	//***********************************************************************
	//	STATIC VARIABILE
	//***********************************************************************

	//***********************************************************************
	//	LOCAL VARIABILE
	//***********************************************************************

	uint8_t tccr1a_temp = 0x00;
	uint8_t tccr1b_temp = 0x00;
	uint8_t tccr1c_temp = 0x00;
	uint8_t timsk1_temp = 0x00;

	//***********************************************************************
	//	REGISTER CONFIGURATION
	//***********************************************************************

	//OC1A pin bheaviour
	//	1	0	|	OC1A
	//-------------------------------
	//	0	0	|	Disconnected
	//	0	1	|	Toggle on CM if WGM1(3:0) = 15, Disconnected otherwise
	//	1	0	|	Clear on CM, Set at Bottom (non inverting mode)
	//	1	1	|	Set on CM, Clear at Bottom (inverting mode)
	//SET_BIT( tccr1a_temp, COM1A1 );
	//SET_BIT( tccr1a_temp, COM1A0 );

	//OC1B pin bheaviour
	//	1	0	|	OC1B
	//-------------------------------
	//	0	0	|	Disconnected
	//	0	1	|	Disconnected
	//	1	0	|	Clear on CM, Set at Bottom (non inverting mode)
	//	1	1	|	Set on CM, Clear at Bottom (inverting mode)
	//SET_BIT( tccr1a_temp, COM1B1 );
	//SET_BIT( tccr1a_temp, COM1B0 );

	//CTC mode with TOP in OCR1A
	//SET_BIT( tccr1b_temp, WGM13 );
	SET_BIT( tccr1b_temp, WGM12 );
	//SET_BIT( tccr1a_temp, WGM11 );
	//SET_BIT( tccr1a_temp, WGM10 );

	//Input capture noise canceller enabled
	//SET_BIT( tccr1b_temp, ICNC1 );

	//Input capture edge select
	//SET_BIT( tccr1b_temp, ICES1 );

	//Clock and Prescaler selection
	//	2	1	0	|	Mode
	//-------------------------------
	//	0	0	0	|	Disconnected
	//	0	0	1	|	Clk/1
	//	0	1	0	|	Clk/8
	//	0	1	1	|	Clk/64
	//	1	0	0	|	Clk/256
	//	1	0	1	|	Clk/1024
	//	1	1	0	|	T1 pin falling edge
	//	1	1	1	|	T1 pin rising edge
	//SET_BIT( tccr1b_temp, CS12 );
	//SET_BIT( tccr1b_temp, CS11 );
	//SET_BIT( tccr1b_temp, CS10 );

	//Input Time Capture Interrupt Enable
	//SET_BIT( timsk1_temp, ICIE1 );

	//Compare Match 1A Interrupt Enable
	SET_BIT( timsk1_temp, OCIE1A );

	//Compare Match 1B Interrupt Enable
	//SET_BIT( timsk1_temp, OCIE1B );

	//Timer Overflow Interrupt Enable 1
	//SET_BIT( timsk1_temp, TOIE1 );

	//***********************************************************************
	//	REGISTER WRITE-BACK
	//***********************************************************************

	OCR1A 	= 0xffff;

	OCR1B	= 0;

	ICR1 	= 0;

	TCCR1A = tccr1a_temp;
	TCCR1B = tccr1b_temp;
	TCCR1C = tccr1c_temp;
	TIMSK1 = timsk1_temp;

	return;
}	//end function: timer1_init
