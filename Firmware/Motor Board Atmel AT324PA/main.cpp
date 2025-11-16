/****************************************************************************
**	ROBOTICA@POLITO PROJECT
*****************************************************************************
**	Unit Zero Zero
*****************************************************************************
**	Author: 			Orso Eric
**	Creation Date:
**	Last Edit Date:
**	Revision:			1
**	Version:			0.1 ALPHA
****************************************************************************/

/****************************************************************************
**	HYSTORY VERSION
*****************************************************************************
**	R1 V0.1ALPHA
**		>I wrote a cool function that allow to update servo in one function,
**		i think it's best to update them in sequence to avoid artifact
**		>i rewrote the servo scan function, now it scan the servo in order
**		i eliminated the artifact and greatly reduced var usage and time
**		>added the interpolator function, now servo position is extrapolated
**		from time and points in space and time, offset is accounted for during calculations
**		>global time is now same advancement as servo period
**		~20mS tick, 5.12S longest single movement length
**		>work like a charm
**		>I added the LCD and the trigonometric function library
**		>i moved timer 0 to 156.0uS to update the display and mot is now 156.8*128= 20.07mS
**		>I added progmem capability to the LUT of the trig library to save data memory
**		>tested at_sin8 and at_cos8 by showing them to the display, they seems to work fine
**		>added safe and servo_idle status flags
**		>now the interpolator copy Xe,Te in Xs, Ts when motion end, it setup for next motion and stop the servo
**	V0.2 ALPHA
**		>I vastly improved upon the servo driver. The version I did before
**		is not that user friendly, the user have to meddle with thing to do things.
**		What the user want is to set a position and a speed, and have a flag when 
**		pos is reached. My new driver does just that.
**		The user see three sets of var
**			CONTROL
**			>servo_pos
**			>servo_speed
**			FEEDBACK
**			>servo_lock
**			CONFIG
**			>servo_off
**			>SERVO_MAX_SPEED
**		>Porting to LCD library V3. Here the driver sync the display with a
**		memory. The user can make 1M printchar a second and the driver still
**		won't bug out. Muchbetter abstraction and lower overhead compared
**		to command fifo approach.
**		>Added DPRINTEF macros to at_utils. used in codeblock.
**
**		2025-11-09
**	I'm reporpousing UnitZero firmware 
**	Adding UART and uniparser
**		2025-11-15
**	Added uniparser
**	Redone the project because C++ wasn't working
**	Linked the parser and the serial commands and the callbacks
**	Tested operation from PC terminal and it works!
****************************************************************************/

/****************************************************************************
**	DESCRIPTION
*****************************************************************************
**	This is the control progam for the 5 servos lego 4 leg robots
*****************************************************************************
**	CALCULATION OF TIMER 1 OCR FOR PULSE WIDTH
**	Constant for the conversion from angle (S8) to OCR (U16)
**	x	= [I8] input angle, i use from -127 to +127, -128 is the disable special code
**	Tn 	= [s] (typ 1.5mS) neutral time of the servo, pulse width required for 0°
**	Td	= [s] (typ 0.9mS) maximum skew of the pulse width, from minimum angle (Tn-Td) to maximum angle (Tn+Td)
**	Tt	= [s] (typ 50nS) N/Fclk width of a single stepof the timer, required to calculate the OCR
**	N	= [1] timer prescaler, 1 maximum precision OCR bigger (might require 16bit), 2^n OCR smaller lower precision require less bits
**	K0	= [1] Tn / Tt neutral value of OCR
**	K1	= [1] Td / Tt / 127 angle constant, multiply angle to obtain OCR, 127 is the maximum value of the input
**	OCR	= K0 + K1 * x, calculation of OCR
**	Example
**	Fclk = 20e6, N = 1, Tn = 1.5mS, Td = 0.9mS, x = [-127,+127]
**	K0 = 30e3, K1 = 142 (141.7), >>>OCR = 30e3 + x*142<<<
**	SERVO OFFSET:
**	This is the correction for the 0 position, it is added during the caluclation
**	and does not eat into the servo position range
**	JOINTS:
**	Index		|	Joint		|	Increase position move toward what?
**	-------------------------------------------------------------------
**	0			|	front DX	|	front
**	1			|	front SX	|	back
**	2			|	rear SX		|	back
**	3			|	rear DX		|	front
**	4			|	front hip	|	rise 0, lower 1
**	5			|	rear hip	|	rise 3, lower 2
*****************************************************************************
**	INTERPOLATOR
**	Above the timing ISR for the servo there is the interpolator that generate the positions
**	The user write the wished positions and times in the following global vars
**		Xs	Ts	Xe	Te
**	0
**	...
**	5
**	The time is in tick unit (about 80mS), example:
**	servo_global_time = 45 		//i'm in the 45° tick of the cycle
**	SERVO_POS( 0 , 3 ) = -50	//i start the new motion from current time and in position -50 (50/127*90°=-35.4°)
**	SERVO_TIME( 0 , 3 ) = 45
**	SERVO_POS( 1 , 3 ) = -30	//i end the new motion in 480mS and in position -30 (50/127*90°=-21.3°)
**	SERVO_TIME( 1 , 3 ) = 45+6
**	The interpolator is a powerful function, allow for much easier high level motion generation
**	since it handle all the complexity of sub angle calculation and the timing routine
**	handle all the complexity of the time generations
****************************************************************************/

/****************************************************************************
**	USED PIN
**	TIPS: compile this field before writing any code: it really helps!
*****************************************************************************
**	PB0			|	SERVO 0 - Wheel RX
**	PB1			|	SERVO 1 - Wheel LX
**	PB2			|	
**	PB3			|	
**	PB4			|	
**	PB5			|	
**	PB6			| 	
**	PB7			|	SERVO 7
**	
**	PD0			|	UART0 RXI
**	PD1			|	UART0 TX0
**	PD2			|	L
*****************************************************************************
**
****************************************************************************/

/****************************************************************************
**	USED PHERIPERALS
**	TIPS: compile this field before writing any code: it really helps!
*****************************************************************************
**	HS-311 Servo
**	Voltage		|	5.0
**	Current		|	7m (idle)		160m (free)		700m (stall)
**	Torque		|	3 [Kg*cm]		294 [mNm]
**	Angle		|
**	Control		|	1.5mS (center)	+/-900uS (90°)	~20mS (period)
**				|	HZ -> no force
**				|	positive DT -> clockwise direction
**	UART		| 
*****************************************************************************

**
****************************************************************************/

/****************************************************************************
**	KNOWN BUG
*****************************************************************************
**	>
****************************************************************************/

/****************************************************************************
**	TODO
*****************************************************************************
**
****************************************************************************/

/****************************************************************************
**	ENVROIMENT VARIABILE
****************************************************************************/

/****************************************************************************
**	INCLUDE
**	TIPS: ".h" should not include other ".h", it lower the legibility of the code
**	TIPS: ".h" must not contain anything that generate code, write only declaration or prototype
**	this help the legibility and the debug phase
**	TIPS: type from the stdint.h have a well defined width and signedness, use them
**	( uint8_t = unsigned 8 bit, int8_t = signed 8 bit, uint32_t = unsigned 32 bit, etc... )
****************************************************************************/

#include "global.h"

//Parse commands and calls callback functions automagically
#include "uniparser.h"

//Movement
#include "st_wheel_speed_duration.h"

//Sequence movements with duration
#include "cl_motion_queue.hpp"

/****************************************************************************
**	DEFINE
****************************************************************************/

//define for the mail loop
#define EVER (;;)

/****************************************************************************
**	MACRO
****************************************************************************/

/****************************************************************************
**	STRUCTURE
****************************************************************************/

/****************************************************************************
**	PROTOTYPE: FUNCTION
**	TIPS: use "extern" in function prototype, it's not necessary, but any other
**	prototype need it, it help the legibility of the code
****************************************************************************/

extern U8 seesaw();

extern uint8_t demo_velocity();

extern uint8_t demo_timed_velocity();

extern bool init_parser_commands( Orangebot::Uniparser &i_rcl_parser );



/****************************************************************************
**	PROTOTYPE: GLOBAL VARIABILE
****************************************************************************/

/****************************************************************************
**	GLOBAL VARIABILE:
**	TIPS: "const" variable will be loaded in the flash memory, saving the ram,
**	use it for string that will not be modified
**	TIPS: if you want a ISR to manipulate a global variable, than that variable
**	**must** be declared "volatile" so that the c compiler will not wipe out the
**	variable by optimizing the code, use that variable as less as possible
**	because it will not be optimized
**	TIPS: "volatile int" variable may give problem, don't use it (uP is 8 bits, while
**	int is 16 bits, it's implemented by concatenating 2 byte, the volatile statement
**	disable the optimization on that variable, and mess up that code)
****************************************************************************/

	//-----------------------------------------------------------------------
	//	STATUS VARS
	//-----------------------------------------------------------------------

//Those are the flags updated by ISRs
volatile Isr_flags f;
//Status variable for the servos, keep track of which servo to do next
volatile U8 servo_cnt	= N_SERVOS;

	///----------------------------------------------------------------------
	///	BUFFERS
	///----------------------------------------------------------------------
	//	Buffers structure and data vectors

//Safe circular buffer for UART input data
volatile At_buf8_safe uart_rx_buf;
//Safe circular buffer for uart tx data
At_buf8 uart_tx_buf;
//allocate the working vector for the buffer
U8 v0[ UART_RX_BUF_SIZE ];
//allocate the working vector for the buffer
U8 v1[ UART_TX_BUF_SIZE ];

	//-----------------------------------------------------------------------
	//	UART PARSER
	//-----------------------------------------------------------------------

//Create a new parser
Orangebot::Uniparser g_cl_parser;
	
Cl_motion_queue g_cl_motion_queue;
	
	//-----------------------------------------------------------------------
	//	SERVOS VARS
	//-----------------------------------------------------------------------

//One flag per servo. '1' if the servo is keeping up with target_pos
U8 servo_lock			= 0x00;
//Current position of the servos. Used by the driver, user should not write here
U16 servo_delay[ N_SERVOS ];
//Target position. The user write here, the driver will do it's best to reach it
S8 servo_target_pos[ N_SERVOS ];
//The servo will rotate at this speed [unit/second]
U8 servo_target_speed[ N_SERVOS ];
//servo position offset for true 0 position
//offsets are accounted for in separately from the position, it does not eat into the dynamic
S8 servo_off[ N_SERVOS ];

	//-----------------------------------------------------------------------
	//	TRAJECTORY VARS
	//-----------------------------------------------------------------------

//Timebase for trajectory generation. 1 time unit = 1/50Hz.
U16 servo_global_time 	= 0;
//Current motion plan
//Trajectories trajectory = MOVE_IDLE;



//Operation mode of the servomotors
E_servo_mode g_e_servo_mode = SERVO_SPEED_MODE;

/****************************************************************************
**	MAIN
****************************************************************************/

int main( void )
{
	///**********************************************************************
	///	LOCAL VARIABILE
	///	TIPS: main local variable last the entire program, but unlike the global variable,
	///	they are not visible from other function, the c compiler can better optimize them
	///**********************************************************************

	//temp vars
	U8 u8t;
	U16 u16t;
	//S8 s8t;

	U8 pre_traj = 0;

	///**********************************************************************
	///	VARIABILES INITIALISATION
	///**********************************************************************
	
		///UART RX BUFFER INIT
	//I init the rx and tx buffers
	//attach vector to buffer
	AT_BUF_ATTACH( uart_rx_buf, v0, UART_RX_BUF_SIZE);
	//attach vector to buffer
	AT_BUF_ATTACH( uart_tx_buf, v1, UART_TX_BUF_SIZE);
	
	//Clear global servo time
	//Initialize servo position to zero (offset is accounted for during calculations, i must not add it here)
	for (u8t = 0;u8t < N_SERVOS;u8t++)
	{
		servo_off[u8t]			= +0;
		servo_delay[u8t] 		= K0;	//Servo true position
		servo_target_pos[u8t] 	= +0;	//Servo target position (user)
		servo_target_speed[u8t]	= MOVE_SPEED;	//Servo target speed (default)
	}

	///**********************************************************************
	///	PHERIPERALS INITIALISATION
	///**********************************************************************

	//Initialize devices
	global_init();
	//Register the commands for the parser
	init_parser_commands( g_cl_parser );
	
	//The proud name of this unit
	lcd_print_str( LCD_POS(0,0), (U8 *)"RAMIE");
	lcd_print_str( LCD_POS(0,8), (U8 *)"CMD:");
	lcd_print_str( LCD_POS(1,0), (U8 *)"Time:");

	///**********************************************************************
	///	MAIN LOOP
	///**********************************************************************

	for EVER
	{
		//-----------------------------------------------------------------------
		//	LCD DIPSPLAY UPDATE
		//-----------------------------------------------------------------------
		//	call the lcd display driver

		//If: update the LCD display (10KHz)
		if (f.lcd_update == 1)
		{
			f.lcd_update = 0;
			//Update display
			lcd_update();
		}

		//-----------------------------------------------------------------------
		//	START MOTOR SCAN (5.8uS max all servo functions)
		//-----------------------------------------------------------------------
		//	Flag raised by [Timer 0]
		//	>activity pin (led signal uC use, oscilloscope allow to measure function times)
		//	>clear servo status var
		//	>calculate first delay, pull down first line
		//	>setup first delay, enable [Timer 1]
		//	>Timer 1 ISR will handle the update of the servos and disable it self when done

		//If: Start Servo Scan (50Hz)
		if (f.servo_scan == 1)
		{
			//clear flag
			f.servo_scan = 0;
				//Startup the servo scan
			//clear status var
			servo_cnt 	= 0;
			//calculate delay
			u16t		= servo_calc_delay( 0 );
			//Store delay on T1
			OCR1A 		= u16t;
			//start T1
			START_TIMER1();
			//pull up first line
			SET_BIT( SERVO_PORT, 0 +SERVO_PIN_OFFSET );
				//Advance global time by one tick
			servo_global_time++;
			lcd_print_u16( LCD_POS(1,6), servo_global_time );

			pre_traj = AT_TOP_INC( pre_traj, MOVE_STEP_TIME );
			if (pre_traj == 0)
			{
				f.servo_traj = 1;
			}
			
			//In timed servo mode
			if (g_e_servo_mode == E_servo_mode::SERVO_TIMED_SPEED_MODE)
			{
				St_wheel_speed_duration st_timed_speed;
				//Ask the orchestrator for a motion
				bool x_fail = g_cl_motion_queue.execute_time_step( st_timed_speed );
				//If no motion in the queue
				if (x_fail == true)
				{
					//Stop the motors
					servo_target_pos[SERVO_WHEEL_RIGHT] = 0;
					servo_target_pos[SERVO_WHEEL_LEFT] = 0;
					//Go back to direct speed mode
					g_e_servo_mode = E_servo_mode::SERVO_SPEED_MODE;
				}
				else
				{
					
					servo_target_pos[SERVO_WHEEL_RIGHT] = st_timed_speed.s8_speed_right;
					servo_target_pos[SERVO_WHEEL_LEFT] = st_timed_speed.s8_speed_left;
				}
				
			}
			
		}	//End If: motor scan flag

		//-----------------------------------------------------------------------
		//	TRAJECTORY GENERATION
		//-----------------------------------------------------------------------

		if (f.servo_traj == 1)
		{
			f.servo_traj = 0;
			
			//lcd_print_u16( LCD_POS(0,11), g_u8_command_counter );
			
			uint8_t u8_queue = g_cl_motion_queue.get_num_element();
			lcd_print_u16( LCD_POS(0,11), u8_queue );
			
			//Signal to user the speed mode
			if (g_e_servo_mode == E_servo_mode::SERVO_SPEED_MODE)
			{
				lcd_print_str( LCD_POS(1,14), (U8 *)"DS" );
			}
			else if (g_e_servo_mode == E_servo_mode::SERVO_TIMED_SPEED_MODE)
			{
				lcd_print_str( LCD_POS(1,14), (U8 *)"TS" );
			}
			
			//Debug send via UART
			//AT_BUF_PUSH( uart_tx_buf, 'A'+g_u8_command_counter );
			//TOGGLE_BIT( PORTD, PD1 );
			
			//Toggle LED
			TOGGLE_BIT( PORTC, PC0 );
			
			//seesaw();
			
			//demo_timed_velocity();
			
		} //END: Trajectory generation
		
		///----------------------------------------------------------------------
		/// UART RX
		///----------------------------------------------------------------------
		//	Handle RX from RS232
		//	Loopback

		//if: uart rx buffer is not empty
		if ( AT_BUF_NUMELEM( uart_rx_buf ) > 0)
		{
			///Get data
			//Get the byte from the RX buffer (ISR put it there)
			u8t = AT_BUF_PEEK( uart_rx_buf );
			AT_BUF_KICK_SAFER( uart_rx_buf );
			///Loopback
			//Push into tx buffer
			//AT_BUF_PUSH( uart_tx_buf, u8t );
			
			//Feed the byte to the parser for decoding
			//It will take care to call the callback functions
			g_cl_parser.parse( u8t );
		}
			
		///----------------------------------------------------------------------
		/// UART TX
		///----------------------------------------------------------------------

		//if: the Uart0 HW buffer is empty and the UART tx buffer is not empty
		if ( (UART0_TX_READY()) && (AT_BUF_NUMELEM( uart_tx_buf ) > 0) )
		{
			//Get the byte to be filtered out
			u8t = AT_BUF_PEEK( uart_tx_buf );
			AT_BUF_KICK( uart_tx_buf );
			//Write on UART tx buffer.
			UDR0 = u8t;

			//lcd_print_char( LCD_POS( 1, 14), u8t );
		}	//End If: uart tx
	}	//end for: for EVER

	return 0;
}	//end main

/****************************************************************************
** FUNCTION:
****************************************************************************/

U8 seesaw()
{
	//static U8 u8_cnt = 0;
	static U8 u8_dir = 0;
	
	servo_target_speed[0] = 5;
	
	if (u8_dir == FALSE)
	{
		servo_target_pos[0] += 1;
		if (servo_target_pos[0]>=10)
		{
			u8_dir = TRUE;
		}
	}
	else
	{
		servo_target_pos[0] -= 1;
		if (servo_target_pos[0]<=-10)
		{
			u8_dir = FALSE;
		}
	}
	
	servo_target_pos[1] = servo_target_pos[0];
	
	return 0;
}


//Feed timed velocity instructions directly into the RX queue to test the decoding
uint8_t demo_velocity()
{
	static uint8_t u8_cnt = 0;
	uint8_t u8_index = 0;
	
	if (u8_cnt == 0)
	{
		
		//const char *ps8_cmd = "VT5R+10L-10\0";
		const char *ps8_cmd = "VR+10L-10\0";
		
		while (ps8_cmd[u8_index] != '\0')
		{
			AT_BUF_PUSH_SAFER( uart_rx_buf, ps8_cmd[u8_index++] );
		}
		AT_BUF_PUSH_SAFER( uart_rx_buf, '\0' );
		u8_cnt++;
	}
	else if (u8_cnt == 1)
	{
		const char *ps8_cmd = "VR-10L+10\0";
		
		while (ps8_cmd[u8_index] != '\0')
		{
			AT_BUF_PUSH_SAFER( uart_rx_buf, ps8_cmd[u8_index++] );
		}
		AT_BUF_PUSH_SAFER( uart_rx_buf, '\0' );
		u8_cnt=0;
	}
	else
	{
		u8_cnt=0;
	}
	
	return 0; //OK
}

/****************************************************************************
**	SERVO CALC POS
*****************************************************************************
**	This function will calculate the delay that move servo[index] closer to the target position
**		SLEW RATE LIMITER: The function will only move at the speed set by the user
**		OFFSET CORRECTION:	In this function i account for mechanical misalignement
**	Formula:
**	xpos = xtarget if ABS(xtarget -xpos) < xspeed/50
**	delay[OCR] = K0 + K1*xoff + K1*xpos
****************************************************************************/

U16 servo_calc_delay( U8 index )
{
	///--------------------------------------------------------------------------
	///	STATIC VARIABILE
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	LOCAL VARIABILE
	///--------------------------------------------------------------------------

	U16 delay;

	S16 slew_rate;

	S16 s16t;

	U16 ret;

	///--------------------------------------------------------------------------
	///	CHECK AND INITIALIZATIONS
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	BODY
	///--------------------------------------------------------------------------
	//	>Calculate target delay
	//	>Calculate maximum delay change target_delay-old delay (slew rate)
	//		>based on user defined target_speed 
	//		>limited by servo max slew rate (max rotation speed normalized to my units)
	//	>calculate new delay (apply delay change)
	//	>save delay on servo_delay
	//	>return new delay
		///Calculate target OCR of servo [index]
	//Calculate position dependent coefficient
	s16t 	= K1 *servo_off[ index ] +K1 *servo_target_pos[ index ];
	//apply sign correction
	if (IS_BIT_ONE( SERVO_DIR, index ))
	{
		s16t = -s16t;
	}
	delay	= K0 + s16t;
		///Speed Limiter
	//calculate maximum allowed motion. I need to convert from [unit/second] -> [unit/20mS]
	slew_rate = K1*servo_target_speed[ index ] /50;
	//If: The user wants to exceed the servo limits
	if (slew_rate > SERVO_MAX_SLEW_RATE)
	{
		//Clip the Slew rate
		slew_rate = SERVO_MAX_SLEW_RATE;
	}
	//calculate slew rate required to meet user input
	s16t = delay - servo_delay[ index ];
	if (s16t > +slew_rate)
	{
		//I'm NOT locked: the servo is moving at max speed
		CLEAR_BIT( servo_lock, index );
		ret = servo_delay[ index ] +slew_rate;
	}
	else if (s16t < -slew_rate)
	{
		//I'm NOT locked: the servo is moving at max speed
		CLEAR_BIT( servo_lock, index );
		ret = servo_delay[ index ] -slew_rate;
	}
	else
	{
		//I'm locked: The motor is not moving at max speed
		SET_BIT( servo_lock, index );
		ret = delay;
	}
	//Write back result and return the delay
	servo_delay[ index ] = ret;

	///--------------------------------------------------------------------------
	///	RETURN
	///--------------------------------------------------------------------------	

	return ret;
}	//end function: servo_calc_delay


/****************************************************************************
**	UART PUSH STRING
*****************************************************************************
**
****************************************************************************/


bool uart_send_string( const char * i_pu8_string )
{
	
	uint8_t u8_index = 0;
	while ((AT_BUF_NUMELEM(uart_tx_buf) < UART_TX_BUF_SIZE) && (i_pu8_string[u8_index] != '\0'))
	{
		AT_BUF_PUSH( uart_tx_buf, i_pu8_string[u8_index++] );
	}
	
	if (AT_BUF_NUMELEM(uart_tx_buf) >= UART_TX_BUF_SIZE)
	{
		return true; //ERR
	}
	
	AT_BUF_PUSH( uart_tx_buf, '\0' );
	
	return false; //OK
}

/****************************************************************************
**	add_timed_speed
*****************************************************************************
**
****************************************************************************/

//visible to the parser handler, used to connect parser and motion orchestration
bool add_timed_speed( uint8_t i_u8_time, int8_t i_s8_speed_right, int8_t i_s8_speed_left )
{
	St_wheel_speed_duration st_timed_speed;
	st_timed_speed.u8_duration = i_u8_time;
	st_timed_speed.s8_speed_right = i_s8_speed_right;
	st_timed_speed.s8_speed_left = i_s8_speed_left;
	
	bool x_fail = g_cl_motion_queue.push(st_timed_speed);
	
	return x_fail; //Propagate FAIL
}