/****************************************************************
**	OrangeBot Project
*****************************************************************
**        /
**       /
**      /
** ______ \
**         \
**          \
*****************************************************************
**	Project
*****************************************************************
**  Brief
****************************************************************/

/****************************************************************
**	DESCRIPTION
****************************************************************
**	This file implements the UART command protocols for the OrangeBot
**  system, including ping, signature, revision, stop, and velocity commands.
****************************************************************/

/****************************************************************
**	HISTORY VERSION
****************************************************************
**	2025-11-09 - Initial implementation of UART command protocols
****************************************************************/

/****************************************************************
**	KNOWN BUGS
****************************************************************
**
****************************************************************/

/****************************************************************
**	TODO
****************************************************************
**	- Add error handling for invalid command parameters
**	- Implement proper timeout management for connection reset
**	- Add unit tests for all command handlers
****************************************************************/

/****************************************************************
**	INCLUDES
****************************************************************/

//Standard C Libraries
#include <stdint.h>

//Standard C++ libraries
#include <iostream>
#include <string>

//OS Libraries
//#define _WIN32_WINNT 0x0500	//Enable GetConsoleWindow
//#include <windows.h>

//User Libraries
//Include user log trace
#define ENABLE_DEBUG
#include "debug.h"
//Universal parser class
#include "uniparser.h"

/****************************************************************
**	NAMESPACES
****************************************************************/

//Never use a whole namespace. Use only what you need from it.
using std::cout;
using std::endl;

/****************************************************************
**	DEFINES
****************************************************************/


/****************************************************************
**	MACROS
****************************************************************/

/****************************************************************
**	PROTOTYPES
****************************************************************/

extern bool test_bench( void );

//Feed a string to the parser
extern void parser_executor( Orangebot::Uniparser &my_parser, uint8_t *ptr );

//Initialize and register the commands
extern bool init_parser_commands( Orangebot::Uniparser &parser_tmp );

//UART Command Handlers
extern void handle_ping(void);
extern void handle_sign(void);
extern void handle_revision(void);
extern void handle_stop(void);
extern void handle_set_velocity(int16_t right_speed, int16_t left_speed);
extern void handle_set_velocity_timed(int8_t right_speed, int8_t left_speed, uint8_t time);

/****************************************************************
**	GLOBAL VARIABILES
****************************************************************/

//User::Dummy my_class;

/****************************************************************
**	FUNCTIONS
****************************************************************/

/****************************************************************************
**	Function
**	main |
****************************************************************************/
//! @return bool |
//! @brief Main entry point for OrangeBot project
//! @details Initializes the system and runs test bench
/***************************************************************************/

int main()
{
	//----------------------------------------------------------------
	//	STATIC VARIABILE
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	//	LOCAL VARIABILE
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	//	CHECK AND INITIALIZATIONS
	//----------------------------------------------------------------

	//Start Debugging. Show function nesting level 0 and above
	DSTART( 0 );
	//Trace Enter main
	DENTER();

	//----------------------------------------------------------------
	//	BODY
	//----------------------------------------------------------------

	cout << "OrangeBot Projects\n";
	//print in the 'debug.log' file. works just like a fully featured printf
	DPRINT("OrangeBot Projects\n");

	test_bench();

	//----------------------------------------------------------------
	//	FINALIZATIONS
	//----------------------------------------------------------------

	//Trace Return from main
	DRETURN();
	//Stop Debugging
	DSTOP();

    return 0;
}	//end function: main

/****************************************************************************
**	Function
**	test_bench | void
****************************************************************************/
//! @param f bool
//! @return bool |
//! @brief Runs comprehensive tests for the parser with UART commands
//! @details Tests all implemented UART command protocols
/***************************************************************************/

bool test_bench( void )
{
	//Trace Enter with arguments
	DENTER();

	//----------------------------------------------------------------
	//	VARS
	//----------------------------------------------------------------

	//Create a new parser
	Orangebot::Uniparser my_parser;

	//----------------------------------------------------------------
	//	INIT
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	//	BODY
	//----------------------------------------------------------------
	//! @details algorithm:



    init_parser_commands( my_parser );


	//----------------------------------------------------------------
	//	TEST EXECUTION
	//----------------------------------------------------------------

	printf("---------------------------------------------------\n");
	printf("Testing UART Command Protocols\n");
	printf("---------------------------------------------------\n");

	// Test ping command
	parser_executor( my_parser, (uint8_t *)"P\0" );

	// Test signature command
	parser_executor( my_parser, (uint8_t *)"F\0" );

	// Test revision command
	parser_executor( my_parser, (uint8_t *)"REV\0" );

	// Test stop command
	parser_executor( my_parser, (uint8_t *)"STOP\0" );

	// Test velocity command
	parser_executor( my_parser, (uint8_t *)"VR+100L-50\0" );

	// Test timed velocity command
	parser_executor( my_parser, (uint8_t *)"VR+123L-123T234\0" );

	//----------------------------------------------------------------
	//	RETURN
	//----------------------------------------------------------------

	//Trace Return vith return value
	DRETURN_ARG("out: %d\n", 0);

	return true; //OK
}	//end main

/****************************************************************************
**	Function
**	parser_executor | Orangebot::Uniparser &, uint8_t *
****************************************************************************/
//! @param parser |
//! @param str |
//! @brief Feeds a string to the parser for processing
//! @details Processes each character in the input string and feeds it to the parser
/***************************************************************************/

void parser_executor( Orangebot::Uniparser &my_parser, uint8_t *ptr )
{
	//Trace Enter with arguments
	DENTER_ARG("in: %d\n", 0);

	//----------------------------------------------------------------
	//	VARS
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	//	INIT
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	//	BODY
	//----------------------------------------------------------------

	uint8_t t = 0;
	//While: String not over
	while ((t == 0) || ((t >= 1) && !((ptr[t] == '\0') && (ptr[t-1] == '\0'))))
	{
		//Print char
		if ((ptr[t] >= '0') || (ptr[t] >= 'z') || (ptr[t] == '-') || (ptr[t] == '+'))
		{
			printf("feed: >0x%x< >%c<\n", ptr[t], ptr[t]);
		}
		else
		{
			printf("feed: >0x%x<\n", ptr[t]);
		}
		//Feed char to parser
		my_parser.parse(ptr[t]);
		//Next char
		t++;
		if (t> 128)
		{
			printf("infinite loop detected. Maybe you forgot \\0 at end of string?\n");
			exit(0);
		}
	}
	//Feed an additional terminator
    my_parser.parse('\0');

	//----------------------------------------------------------------
	//	RETURN
	//----------------------------------------------------------------

	//Trace Return vith return value
	DRETURN();

	return; //OK
}	//end function: parser_executor | Orangebot::Uniparser & | uint8_t *
