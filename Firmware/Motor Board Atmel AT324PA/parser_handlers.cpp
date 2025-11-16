
/****************************************************************
**	INCLUDES
****************************************************************/

//#include <cstdint>

//#include <cstdio>

#include "global.h"

//Only used for implementing DPRINT
#include "debug.h"

//Universal Parser V6
#include "uniparser.h"

/****************************************************************
** DEFINITIONS
****************************************************************/

//Maximum size of a signature string
#define MAX_SIGNATURE_LENGTH	32

/****************************************************************
** FUNCTION PROTOTYPES
****************************************************************/

	///----------------------------------------------------------------------
	///	PARSER
	///----------------------------------------------------------------------
	//	Handlers are meant to be called automatically when a command is decoded
	//	User should not call them directly

//UART Command Handlers
extern void handle_ping(void);

extern void handle_sign(void);

extern void handle_revision(void);

extern void handle_stop(void);

extern void handle_set_velocity(int8_t right_speed, int8_t left_speed);

extern void handle_set_velocity_timed(uint8_t time, int8_t right_speed, int8_t left_speed);

/****************************************************************
** GLOBAL VARS PROTOTYPES
****************************************************************/

//Count successful commands
uint8_t g_u8_command_counter = 0;

/****************************************************************
** GLOBAL VARS
****************************************************************/

	///--------------------------------------------------------------------------
	///	PARSER
	///--------------------------------------------------------------------------


//Board Signature
//BUG: I couldn't be bothered to do a proper FSM to pick the character as needed, so it's all dumped into the buffer that might not be long enough
const char *g_ps8_board_sign = "RAMIE";
//Firmware Revision
const char *g_ps8_board_revision = "2025-11-16";

//communication timeout counter
//uint8_t g_uart_timeout_cnt = 0;
//Communication timeout has been detected
//bool g_f_timeout_detected = false;

/***************************************************************************/
//!	function
//!	init_parser_commands | Orangebot::Uniparser &
/***************************************************************************/
//! @param parser_tmp | Orangebot::Uniparser | Parser to which commands are to be atached
//! @return bool | false = OK | true = command was invalid and was not registered
//! @brief
//! @details
/***************************************************************************/

bool init_parser_commands( Orangebot::Uniparser &i_rcl_parser )
{
	//----------------------------------------------------------------
	//	VARS
	//----------------------------------------------------------------

	//Return flag
	bool f_ret = false;

	//----------------------------------------------------------------
	//	INIT
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	//	BODY
	//----------------------------------------------------------------

	//! Register commands and handler for the universal parser class. A masterpiece :')

	//Register all UART commands with their handlers

    f_ret = false;

	f_ret |= i_rcl_parser.add_cmd( "P", (void *)&handle_ping );

	f_ret |= i_rcl_parser.add_cmd( "SIGN", (void *)&handle_sign );

	f_ret |= i_rcl_parser.add_cmd( "REV", (void *)&handle_revision );

	f_ret |= i_rcl_parser.add_cmd( "STOP", (void *)&handle_stop );

	f_ret |= i_rcl_parser.add_cmd( "VR%sL%s", (void *)&handle_set_velocity );

	f_ret |= i_rcl_parser.add_cmd( "T%uVR%sL%s", (void *)&handle_set_velocity_timed );

	//If: Uniparser V4 failed to register a command
	if (f_ret == true)
	{
		//TODO: signal error
	}

	//----------------------------------------------------------------
	//	RETURN
	//----------------------------------------------------------------

	return f_ret;
}	//End function: init_parser_commands | Orangebot::Uniparser &

/****************************************************************************
**	Function
**	handle_ping | void
****************************************************************************/
//! @brief Handles ping command - resets connection timeout
//! @details No action required, just resets the connection timeout
/***************************************************************************/
void handle_ping(void)
{
	DENTER();
	
	g_u8_command_counter++;
	
	DPRINT("EXE | %s  -> Ping received\n", __FUNCTION__);
	
	DRETURN();
	return;
}

/****************************************************************************
**	Function
**	handle_sign | void
****************************************************************************/
//! @brief Handles signature request command
//! @details Responds with board signature string
/***************************************************************************/
void handle_sign(void)
{
	DENTER();
	
	g_u8_command_counter++;
	
	DPRINT("EXE | %s  -> Board signature requested\n", __FUNCTION__);
	DPRINT("Board Signature: %s\n", g_ps8_board_sign);
	
	uart_send_string( g_ps8_board_sign );
	
	DRETURN();
	return;
}

/****************************************************************************
**	Function
**	handle_revision | void
****************************************************************************/
//! @brief Handles firmware revision request command
//! @details Responds with firmware revision string
/***************************************************************************/

void handle_revision(void)
{
	DENTER();
	
	g_u8_command_counter++;
	
	DPRINT("EXE | %s  -> Firmware revision requested\n", __FUNCTION__);
	DPRINT("Firmware Revision: %s\n", g_ps8_board_revision);
	
	uart_send_string( g_ps8_board_revision );
	
	DRETURN();
	return;
}

/****************************************************************************
**	Function
**	handle_stop | void
****************************************************************************/
//! @brief Handles stop command - sets motor speeds to zero
//! @details Stops all motors by setting speed to 0
/***************************************************************************/

void handle_stop(void)
{
	DENTER();
	
	g_u8_command_counter++;
	
	DPRINT("EXE | %s  -> Stop command received\n", __FUNCTION__);
	DPRINT("Setting motor speeds to 0\n");
	
	servo_target_pos[SERVO_WHEEL_RIGHT] = 0;
	servo_target_pos[SERVO_WHEEL_LEFT] = 0;
	
	uart_send_string( "STOP\0" );
	
	DRETURN();
	return;
}

/****************************************************************************
**	Function
**	handle_set_velocity | int16_t, int16_t
****************************************************************************/
//! @brief Handles velocity command - sets motor speeds
//! @param right_speed Speed for right motor
//! @param left_speed Speed for left motor
//! @details Sets the speed of both motors to specified values
/***************************************************************************/

void handle_set_velocity(int8_t i_s8_right_speed, int8_t i_s8_left_speed)
{
	DENTER_ARG("in: Right=%d, Left=%d\n", i_s8_right_speed, i_s8_left_speed);
	
	g_u8_command_counter++;
	
	DPRINT("EXE | %s  -> Set velocity command\n", __FUNCTION__);
	DPRINT("Right motor speed: %d\n", i_s8_right_speed);
	DPRINT("Left motor speed: %d\n", i_s8_left_speed);
	
	//Ask the servo driver to set the speed
	//Since it's a continuous rotation servo, position is speed, and speed is acceleration.
	//Don't worry about it
	servo_target_pos[SERVO_WHEEL_RIGHT] = i_s8_right_speed;
	servo_target_pos[SERVO_WHEEL_LEFT] = i_s8_left_speed;
	
	DRETURN();
	return;
}

/****************************************************************************
**	Function
**	handle_set_velocity_timed | int16_t, int16_t, uint8_t
****************************************************************************/
//! @brief Handles timed velocity command - sets motor speeds for specific time
//! @param right_speed Speed for right motor
//! @param left_speed Speed for left motor
//! @param time Duration in seconds
//! @details Sets the speed of both motors for specified duration
/***************************************************************************/

void handle_set_velocity_timed(uint8_t time, int8_t right_speed, int8_t left_speed)
{
	DENTER_ARG("in: Right=%d, Left=%d, Time=%d\n", right_speed, left_speed, time);
	
	DPRINT("EXE | %s  -> Set velocity timed command\n", __FUNCTION__);
	DPRINT("Right motor speed: %d\n", right_speed);
	DPRINT("Left motor speed: %d\n", left_speed);
	DPRINT("Duration: %d seconds\n", time);
	
	
	DRETURN();
	return;
}

