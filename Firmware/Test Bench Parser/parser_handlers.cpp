
/****************************************************************
**	INCLUDES
****************************************************************/

#include <stdint.h>

#include <cstdio>

#include "debug.h"

//Universal Parser V6
#include "uniparser.h"

/*
//type definition using the bit width and sign

//RNG
#include <stdlib.h>
//define the ISR routune, ISR vector, and the sei() cli() function
#include <avr/interrupt.h>
//name all the register and bit
#include <avr/io.h>
//General purpose macros
#include "at_utils.h"
//AT4809 PORT macros definitions
#include "at4809_port.h"
//
#include "global.h"
//hard coded delay
#include <util/delay.h>

//Driver for the UART communication. Provides init, ISR and buffers
#include "uart.h"
//Embedded string (TODO embedded_string.cpp is a better implementation, and I have an even better implementation in the longan nano screen driver)
#include "string_uc.h"

//Driver for the 16b multi channel timeout ISR servo driver
#include "servo.h"
*/
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

extern void handle_set_velocity_timed(int8_t right_speed, int8_t left_speed, uint8_t time);

/****************************************************************
** GLOBAL VARS PROTOTYPES
****************************************************************/

/****************************************************************
** GLOBAL VARS
****************************************************************/

	///--------------------------------------------------------------------------
	///	PARSER
	///--------------------------------------------------------------------------

//UART Command IDs (0 and 255 are forbidden)
#define UART_CMD_PING           1
#define UART_CMD_SIGN           2
#define UART_CMD_REVISION       3
#define UART_CMD_STOP           4
#define UART_CMD_SET_VELOCITY   5
#define UART_CMD_SET_VELOCITY_TIMED 6

//Command dictionary. Command IDs 0 and 255 are forbidden
/*
uint8_t uart_cmd[] =
{
	//Ping: No action. Effect is to reset the connection timeout
	UART_CMD_PING		, 'P', '\0',
	//Sign: Ask for board signature
	UART_CMD_SIGN		, 'F', '\0',
    //Sign: Ask for firmware revision
	UART_CMD_REVISION	, 'R', 'E', 'V', '\0',
    //STOP: set speed 0 to the motors
	UART_CMD_STOP		, 'S', 'T', 'O', 'P', '\0',
	//VELOCITY: R right engine L left engine (last forever until overwritten)
	UART_CMD_SET_VELOCITY     , 'V', 'R', '%', 's', 'L', '%', 's', '\0',
    //VELOCITY TIMED: R right engine L left engine T time of the motion
    UART_CMD_SET_VELOCITY_TIMED , 'V', 'R', '%', 's', 'L', '%', 's', 'T', '%', 'u', '\0',
	//Dictionary terminator
	'\0'
};
*/

//Board Signature
const char *g_ps8_board_sign = "Industrious_Resonance";
//Firmware Revision
const char *g_ps8_board_revision = "2025-11-09";

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

	f_ret |= i_rcl_parser.add_cmd( "F", (void *)&handle_sign );

	f_ret |= i_rcl_parser.add_cmd( "REV", (void *)&handle_revision );

	f_ret |= i_rcl_parser.add_cmd( "STOP", (void *)&handle_stop );

	f_ret |= i_rcl_parser.add_cmd( "VR%sL%s", (void *)&handle_set_velocity );

	f_ret |= i_rcl_parser.add_cmd( "VR%sL%sT%u", (void *)&handle_set_velocity_timed );

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
	printf("EXE | %s  -> Ping received\n", __FUNCTION__);
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
	printf("EXE | %s  -> Board signature requested\n", __FUNCTION__);
	printf("Board Signature: %s\n", g_ps8_board_sign);
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
	printf("EXE | %s  -> Firmware revision requested\n", __FUNCTION__);
	printf("Firmware Revision: %s\n", g_ps8_board_revision);
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
	printf("EXE | %s  -> Stop command received\n", __FUNCTION__);
	printf("Setting motor speeds to 0\n");
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
	printf("EXE | %s  -> Set velocity command\n", __FUNCTION__);
	printf("Right motor speed: %d\n", i_s8_right_speed);
	printf("Left motor speed: %d\n", i_s8_left_speed);
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

void handle_set_velocity_timed(int8_t right_speed, int8_t left_speed, uint8_t time)
{
	DENTER_ARG("in: Right=%d, Left=%d, Time=%d\n", right_speed, left_speed, time);
	printf("EXE | %s  -> Set velocity timed command\n", __FUNCTION__);
	printf("Right motor speed: %d\n", right_speed);
	printf("Left motor speed: %d\n", left_speed);
	printf("Duration: %d seconds\n", time);
	DRETURN();
	return;
}

