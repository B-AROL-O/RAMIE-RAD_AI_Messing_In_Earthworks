#ifndef GLOBAL_H
	//header envroiment variabile, is used to detect multiple inclusion
	//of the same header, and can be used in the c file to detect the
	//included library
	#define GLOBAL_H

	/****************************************************************************
	**	ENVROIMENT VARIABILE
	****************************************************************************/

	#define F_CPU	20000000

	/****************************************************************************
	**	GLOBAL INCLUDE
	**	TIPS: you can put here the library common to all source file
	****************************************************************************/

	//type definition using the bit width and signedness
	#include <stdint.h>
	//define the ISR routune, ISR vector, and the sei() cli() function
	#include <avr/interrupt.h>
	//name all the register and bit
	#include <avr/io.h>
	//hard delay
	#include <util/delay.h>
	//
	//#include <avr/pgmspace.h>
	//General purpose macros
	#include "at_utils.h"
	//Port configuration macros specific to AtMega Microcontrollers
	#include "at_mega_port.h"
	//LCD device functions and init
	#include "at_lcd.h"
	//trigonometric functions
	//#include "at_trig8.h"

	/****************************************************************************
	**	DEFINE
	****************************************************************************/

		///----------------------------------------------------------------------
		///	UART
		///----------------------------------------------------------------------

	//56818
	//#define UART_UBRR			(23-1)
	//156 250
	//#define UART_UBRR			(9-1)
	//250 000 
	#define UART_UBRR			(5-1)

		///----------------------------------------------------------------------
		///	BUFFERS
		///----------------------------------------------------------------------

	#define UART_RX_BUF_SIZE	16
	#define UART_TX_BUF_SIZE	16

		///----------------------------------------------------------------------
		///	LCD
		///----------------------------------------------------------------------

	//millisecond hard delay for turn of and turn on of the LCD display
	#define LCD_INIT_DELAY		250.0

		///----------------------------------------------------------------------
		///	SERVOS
		///----------------------------------------------------------------------
 
 	//Prescaler that generate servo_scan (50Hz) from lcd_update (10080.6Hz)
	//	PRE_SERVO_TOP = ROUND(10080.6/50)
	#define PRE_SERVO_TOP 		(202-1)
	//
	#define SERVO_PLANNER_PRE	2
	//Total number of servos
	#define N_SERVOS			7
	//Special code to disable a servo
	#define SERVO_OFF			-128
	//Neutral OCR
	#define	K0					30000
	//Linear OCR (40°-400uS-63) (90°-900uS-141.7)
	#define K1					142
	//Port where the servos are placed
	#define SERVO_PORT			PORTA
	//offset at which servo pins starts
	#define SERVO_PIN_OFFSET	0
	//Servo maximum angular velocity [Angular Units/Ticks], at 5V i have 190mS per 60°, 180° are 256 Units, 1 tick is 20mS. Every Tick i can move ~ Units
	#define SERVO_MAX_SPEED		4
	//Convert to OCR K1*
	#define SERVO_MAX_SLEW_RATE	631

		//-----------------------------------------------------------------------
		//	MOTOR ALIASES
		//-----------------------------------------------------------------------
	
	//enum servos names (positions and functions in the robot)
	enum
	{
		SERVO_WHEEL_RIGHT		= 0,	//Leg Front DX 			(+ => Leg front -> Rear)
		SERVO_WHEEL_LEFT		= 1,	//Leg Front SX 			(+ => Leg rear -> front)
	};

		//-----------------------------------------------------------------------
		//	MOTOR CORRECTIONS
		//-----------------------------------------------------------------------

	//servo direction correction mask, allow for the logical direction of a joint to be reversed in sign
	//each bit is associated with a servo, if the bit is '1', the direction is reversed
	#define SERVO_DIR	( MASK(SERVO_WHEEL_LEFT) )

		//-----------------------------------------------------------------------
		//	PHYSICAL CONSTANTS OF THE ROBOT
		//-----------------------------------------------------------------------
		//	Those physical dimension in mm are used to calculate the trajectories
		//	DEFINITIONS:
		//	X:		Rear to Front dimension, positive toward head
		//	Y:		Side to Side dimension, positive toward Dx
		//	Z: 		Bottom to Top dimension, positive toward Top 
		//	Hip: 	Leg Joing

	//Length of the Spine in X [mm] from Rear Hip to Front Hip, length of the robot
	//#define SIZE_SX				170
	//Distance from Spine to Hip in Y, half of the shoulder to shoulder length
	//#define SIZE_HY				50
	//Distance from Hip to base of the Boot in Z, length of a Leg
	//#define SIZE_LZ				130

		//-----------------------------------------------------------------------
		//	TRAJECTORIES
		//-----------------------------------------------------------------------

	//Step time in 1/50 of seconds
	#define MOVE_STEP_TIME		20
	//Maximum movement speed
	#define MOVE_SPEED			80
	
	//Enumerates modes of operation of the servo motors
	enum E_servo_mode
	{
		//Direct speed mode will sustain the speed given until different instruction
		SERVO_SPEED_MODE,
		//Timed servo mode will fetch from the orchestrator
		SERVO_TIMED_SPEED_MODE
	};

	/****************************************************************************
	**	MACRO
	****************************************************************************/

		//-----------------------------------------------------------------------
		//	LCD MACROS
		//-----------------------------------------------------------------------

	#define LCD_ON()	\
		CLEAR_BIT( PORTC, PC1 )

	#define LCD_OFF()	\
		SET_BIT( PORTC, PC1 )

		//-----------------------------------------------------------------------
		//	TIMER 1 MACROS
		//-----------------------------------------------------------------------
		//	Timer 1 is used to wait precises delay to pull down the servo lines
		//	I have to start it up, stop it, clear the content and setup the delay

	//connect the clock at N=1
	#define START_TIMER1()	\
		SET_BIT( TCCR1B, CS10 )

	//disconnect the clock (if N==1)
	#define STOP_TIMER1()	\
		CLEAR_BIT( TCCR1B, CS10 )

		//-----------------------------------------------------------------------
		//	SERVOS MACROS
		//-----------------------------------------------------------------------
		//	Those macros are wrappers to access servos variables

		//access servo pos as matrix
	#define SERVO_POS( col, index )	\
		servo_pos[ N_SERVOS*(col) + (index) ]

	//access servo pos as matrix
	#define SERVO_TIME( col, index )	\
		servo_time[ N_SERVOS*(col) + (index) ]

	//offset
	#define SERVO_OFFSET( index )	\
		servo_off[ (index) ]

	/****************************************************************************
	**	TYPEDEF
	****************************************************************************/
	
	//Global flags raised by ISR functions
	typedef struct _Isr_flags Isr_flags;

	//typedef enum _Trajectories Trajectories;

	/****************************************************************************
	**	STRUCTURE
	****************************************************************************/

	//Global flags raised by ISR functions
	struct _Isr_flags
	{
		//First byte
		U8 lcd_update	: 1;	//lcd update flag
		U8 servo_scan	: 1;	//start the motor scan
		U8 servo_traj	: 1;	//
		U8 				: 5;	//unused bits
	};

	/****************************************************************************
	**	PROTOTYPE: INITIALISATION
	****************************************************************************/

	//port configuration and call the peripherals initialization
	extern void global_init( void );
	
	//UART communication
	extern void usart0_init( void );
	
	//System tick
	extern void timer0_init( void );
	//servo lines handler
	extern void timer1_init( void );

	/****************************************************************************
	**	PROTOTYPE: FUNCTION
	****************************************************************************/

		//-----------------------------------------------------------------------
		//	SERIAL COMMUNICATION
		//-----------------------------------------------------------------------

	extern bool uart_send_string( const char * i_pu8_string );

		//-----------------------------------------------------------------------
		//
		//-----------------------------------------------------------------------

	//very complex function, it is called by the ISR to calculate the delay
	//delay is calculated from global time, initial and ending time and position of the servo
	extern U16 servo_calc_delay( U8 index );

	//The meat of the trajectory generation, it's called when the buffer of a servo is emptied
	extern U8 servo_motion_planner( U8 index );

	//Execute a precalculated point. Allow for parameters
	extern U8 servo_trajectory_point( U8 index, S8 arg, S8 arg_aux );

	/****************************************************************************
	**	PROTOTYPE: GLOBAL VARIABILE
	****************************************************************************/

		//-----------------------------------------------------------------------
		//	STATUS VARS
		//-----------------------------------------------------------------------

	//Those are the flags updated by ISRs
	extern volatile Isr_flags f;	
	//Status variable for the servos, keep track of which servo to do next
	extern volatile  U8 servo_cnt;
	//each servo has 1 bit, it's raised when the interpolator is done
	extern volatile U8 servo_idle;

		///----------------------------------------------------------------------
		///	BUFFERS
		///----------------------------------------------------------------------

	//Safe circular buffer for RS485 input data
	extern volatile At_buf8_safe uart_rx_buf;
	//Safe circular buffer for RS485 output data
	extern At_buf8 uart_tx_buf;

		///----------------------------------------------------------------------
		///	PARSER
		///----------------------------------------------------------------------


	//Count successful commands
	extern uint8_t g_u8_command_counter;


		///----------------------------------------------------------------------
		///	SERVO
		///----------------------------------------------------------------------

	//servo direction correction mask, allow for the logical direction of a joint to be reversed in sign
	//each bit is associated with a servo, if the bit is '1', the direction is reversed
	extern U8 servo_dir;
	//One flag per servo '1' if the servo is keeping up with target_pos
	extern U8 servo_lock;
	//servo position offset for true 0 position
	//offsets are accounted for in separately from the position, it does not eat into the dynamic
	extern S8 servo_off[ N_SERVOS ]; 
	//Current position of the servos. Used by the driver, user should not write here
	extern U16 servo_delay[ N_SERVOS ];
	//Target position. The user write here, the driver will do it's best to reach it
	extern S8 servo_target_pos[ N_SERVOS ];
	//The servo will rotate at this speed [unit/second]
	extern U8 servo_target_speed[ N_SERVOS ];

		///----------------------------------------------------------------------
		///	TRAJECTORY
		///----------------------------------------------------------------------

	//Operation mode of the servomotors
	extern E_servo_mode g_e_servo_mode;

#else
	#warning "multiple inclusion of the header file global.h"
#endif
