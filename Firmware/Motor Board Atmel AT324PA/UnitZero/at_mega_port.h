/****************************************************************************
**	ORANGEBOT PROJECT
*****************************************************************************
**	AT MEGA PORT
**	Port configuration macros specific to AtMega uC
*****************************************************************************
**	Author: 			Orso Eric
**	Creation Date:		
**	Last Edit Date:
**	Revision:			1
**	Version:			
****************************************************************************/

#ifndef AT_MEGA_PORT_H
	//header envroiment variabile, is used to detect multiple inclusion
	//of the same header, and can be used in the c file to detect the
	//included library
	#define AT_MEGA_PORT_H

	/****************************************************************************
	**	ENVROIMENT VARIABILE
	****************************************************************************/

	/****************************************************************************
	**	GLOBAL INCLUDE
	**	TIPS: you can put here the library common to all source file
	****************************************************************************/

	/****************************************************************************
	**	DEFINE
	****************************************************************************/

	/****************************************************************************
	**	MACRO
	*****************************************************************************
	**	Those macros setup pin in a port in one of four states
	**	'Z' |	Hi Impedence Input
	**	'R'	|	Input with pull-up resistence
	**	'L'	|	Output Low
	**	'H'	|	Output Hi
	****************************************************************************/
	
	//this macro return TRUE if the port should be configurated as output
	#define COND_L_H(An) \
			( ( (An) == 'L') || ( (An) == 'H' ) )

	//this macro return TRUE if the port should be configurated as output high or pull-up input
	#define COND_R_H(An) \
			( ( (An) == 'R' ) || ( (An) == 'H' ) )

	//this macro can configure a generic port, by setting the value of the DataDirectionRegister
	//and the PORT register, first instruction configure the Hi-Low information or the Pull-up information
	//the second instruction configure the output/input information
	#define PORT_CONFIG( DDRx, PORTx, A0, A1, A2, A3, A4, A5, A6, A7) \
			PORTx =	SHIFTL( COND_R_H( A0 ), 0 ) | \
					SHIFTL( COND_R_H( A1 ), 1 ) | \
					SHIFTL( COND_R_H( A2 ), 2 ) | \
					SHIFTL( COND_R_H( A3 ), 3 ) | \
					SHIFTL( COND_R_H( A4 ), 4 ) | \
					SHIFTL( COND_R_H( A5 ), 5 ) | \
					SHIFTL( COND_R_H( A6 ), 6 ) | \
					SHIFTL( COND_R_H( A7 ), 7 ); \
			DDRx =	SHIFTL( COND_L_H( A0 ), 0 ) | \
					SHIFTL( COND_L_H( A1 ), 1 ) | \
					SHIFTL( COND_L_H( A2 ), 2 ) | \
					SHIFTL( COND_L_H( A3 ), 3 ) | \
					SHIFTL( COND_L_H( A4 ), 4 ) | \
					SHIFTL( COND_L_H( A5 ), 5 ) | \
					SHIFTL( COND_L_H( A6 ), 6 ) | \
					SHIFTL( COND_L_H( A7 ), 7 )

	//those macros configure the single port
	#ifdef PORTA
		#define PORT_A_CONFIG( A0, A1, A2, A3, A4, A5, A6, A7 ) \
			PORT_CONFIG( DDRA, PORTA, A0, A1, A2, A3, A4, A5, A6, A7)
	#endif

	//PORTB configuration
	#ifdef PORTB
		#define PORT_B_CONFIG( A0, A1, A2, A3, A4, A5, A6, A7 ) \
			PORT_CONFIG( DDRB, PORTB, A0, A1, A2, A3, A4, A5, A6, A7)
	#endif

	//PORTC configuration
	#ifdef PORTC
		#define PORT_C_CONFIG( A0, A1, A2, A3, A4, A5, A6, A7 ) \
			PORT_CONFIG( DDRC, PORTC, A0, A1, A2, A3, A4, A5, A6, A7)
	#endif

	//PORTD configuration
	#ifdef PORTD
		#define PORT_D_CONFIG( A0, A1, A2, A3, A4, A5, A6, A7 ) \
			PORT_CONFIG( DDRD, PORTD, A0, A1, A2, A3, A4, A5, A6, A7)
	#endif

	//PORTE configuration
	#ifdef PORTE
		#define PORT_E_CONFIG( A0, A1, A2, A3, A4, A5, A6, A7 ) \
			PORT_CONFIG( DDRE, PORTE, A0, A1, A2, A3, A4, A5, A6, A7)
	#endif

		///********************************************************************************
		///	UART
		///********************************************************************************
		//	UART specific macros

	//return true if the uart 0 is ready to transmit data
	#define UART0_TX_READY()	\
		((UCSR0A & MASK(UDRE0)) == MASK(UDRE0))

#else
	#warning "multiple inclusion of the header file at_mega_port.h"
#endif
