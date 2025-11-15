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
**
****************************************************************/

/****************************************************************
**	HISTORY VERSION
****************************************************************
**
****************************************************************/

/****************************************************************
**	KNOWN BUGS
****************************************************************
**
****************************************************************/

/****************************************************************
**	TODO
****************************************************************
**
****************************************************************/

/****************************************************************
**	INCLUDES
****************************************************************/

//Standard C Libraries
//#include <cstdio>
//#include <cstdlib>
#include <stdint.h>

//Standard C++ libraries
#include <iostream>
//#include <array>
//#include <vector>
//#include <queue>
//#include <string>
//#include <fstream>
//#include <chrono>
//#include <thread>

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

//#define TEST_SYNTAX

//#define TEST_EMPTY_DICTIONARY

//#define TEST_ONECHAR

//#define TEST_ONECHAR_ARG

//#define TEST_MULTICHAR

//#define TEST_TWO_S32

//#define TEST_ALL_FOUR

//#define TEST_SEEKER_OF_WAYS_B

//#define TEST_MIXED_TWO_ARGS

//#define TEST_MIXED_THREE_ARGS

#define TEST_MIXED_FOUR_ARGS

/****************************************************************
**	MACROS
****************************************************************/

/****************************************************************
**	PROTOTYPES
****************************************************************/

extern bool test_bench( void );

//Zero argument function
extern void my_noarg_function( void );
//One argument function. Template based parameters
template <typename Ta>
extern void my_function( Ta arg_a );

template <typename Ta, typename Tb>
extern void my_function( Ta arg_a, Tb arg_b );

template <typename Ta, typename Tb, typename Tc>
extern void my_function( Ta arg_a, Tb arg_b, Tc arg_c );

template <typename Ta, typename Tb, typename Tc, typename Td>
extern void my_function( Ta arg_a, Tb arg_b, Tc arg_c, Td arg_d );

//Feed a string to the parser
extern void parser_executor( Orangebot::Uniparser &my_parser, uint8_t *ptr );

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
//! @brief dummy method to copy the code
//! @details verbose description
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
//! @brief dummy method to copy the code
//! @details verbose description
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

	/*

	my_parser.add_cmd( "F", (void *)&my_handler_noarg );

	my_parser.add_cmd( "VELX%uREI", (void *)&my_handler_u8 );
	my_parser.add_cmd( "UUA%UB%UC%UD%U\0", (void *)&my_handler_four_u16 );

	//Test mixed argument commands
	*/

	//----------------------------------------------------------------
	//	TEST SYNTAX
	//----------------------------------------------------------------

    #ifdef TEST_SYNTAX
	int test_index = 0;
    //Bad handler functio
    if (my_parser.add_cmd( "A", (void *)nullptr ) == true)
    {
		printf("Test %d | ERR: %d\n", test_index, my_parser.get_error());
    }
    test_index++;
    //Begins with a number
    if (my_parser.add_cmd( "1", (void *)&my_handler_noarg ) == true)
    {
		printf("Test %d | ERR: %d\n", test_index, my_parser.get_error());
    }
    test_index++;
    //bad descriptor type
    if (my_parser.add_cmd( "V%x", (void *)&my_handler_noarg ) == true)
    {
		printf("Test %d | ERR: %d\n", test_index, my_parser.get_error());
    }
    test_index++;
    //two arguments back to back
	if (my_parser.add_cmd( "P%u%u", (void *)&my_handler_noarg ) == true)
    {
		printf("Test %d | ERR: %d\n", test_index, my_parser.get_error());
    }
    test_index++;
	//arguments of different kind are allowed
	if (my_parser.add_cmd( "P%uR%U", (void *)&my_handler_noarg ) == true)
    {
		printf("Test %d | ERR: %d\n", test_index, my_parser.get_error());
    }
    test_index++;
	//too many arguments
	if (my_parser.add_cmd( "uA%uB%uC%uD%uE%u", (void *)&my_handler_noarg ) == true)
    {
		printf("Test %d | ERR: %d\n", test_index, my_parser.get_error());
    }
    test_index++;
	//Arguments take up too many bytes
	if (my_parser.add_cmd( "dA%dB%dC%d", (void *)&my_handler_noarg ) == true)
    {
		printf("Test %d | ERR: %d\n", test_index, my_parser.get_error());
    }
    test_index++;
    printf("number of tests: %d\n", test_index +1);


    #endif // TEST_SYNTAX

    //----------------------------------------------------------------
	//	TEST EMPTY DICTIONARY
	//----------------------------------------------------------------

    #ifdef TEST_EMPTY_DICTIONARY

	int cmd_index = 0;

		//!Add commands to the dictionary

	//Call on empty dictionary
	parser_executor( my_parser, (uint8_t *)"P\0" );

	printf("---------------------------------------------------\n");

	//Add a command to the parser. F\0 will automatically execute this handler when received
	if (my_parser.add_cmd( "P", (void *)&my_noarg_function ) == true)
    {
		printf("Test %d | ERR: %d\n", cmd_index, my_parser.get_error());
    }
    cmd_index++;

    printf("Commands added: %d\n", cmd_index);

	printf("---------------------------------------------------\n");
	//Call on single entry dictionary
	parser_executor( my_parser, (uint8_t *)"P\0" );

    #endif // TEST_EMPTY_DICTIONARY



	//----------------------------------------------------------------
	//	TEST ONE CHAR COMMANDS
	//----------------------------------------------------------------

	#ifdef TEST_ONECHAR

	int cmd_index = 0;

		//!Add commands to the dictionary
	//Add a command to the parser. F\0 will automatically execute this handler when received
	if (my_parser.add_cmd( "P", (void *)&my_noarg_function ) == true)
    {
		printf("Test %d | ERR: %d\n", cmd_index, my_parser.get_error());
    }
    cmd_index++;
    //Add a command to the parser. F\0 will automatically execute this handler when received
	if (my_parser.add_cmd( "F", (void *)&my_noarg_function ) == true)
    {
		printf("Test %d | ERR: %d\n", cmd_index, my_parser.get_error());
    }
    cmd_index++;
    printf("Commands added: %d\n", cmd_index);

	printf("---------------------------------------------------\n");
	//Good inputs
	parser_executor( my_parser, (uint8_t *)"P\0" );
	parser_executor( my_parser, (uint8_t *)"F\0" );
	//bad inputs
	parser_executor( my_parser, (uint8_t *)"H\0" );

	#endif

	//----------------------------------------------------------------
	//	TEST ONE CHAR COMMANDS WITH U8 ARGUMENTS
	//----------------------------------------------------------------

	#ifdef TEST_ONECHAR_ARG

	int cmd_index = 0;

		//!Add commands to the dictionary
	//Add a command to the parser. F\0 will automatically execute this handler when received
	if (my_parser.add_cmd( "X%u", (void *)&my_function<uint8_t> ) == true)
    {
		printf("Test %d | ERR: %d\n", cmd_index, my_parser.get_error());
    }
    cmd_index++;
    //Add a command to the parser. F\0 will automatically execute this handler when received
	if (my_parser.add_cmd( "VEL%u", (void *)&my_function<uint8_t> ) == true)
    {
		printf("Test %d | ERR: %d\n", cmd_index, my_parser.get_error());
    }
    cmd_index++;
    printf("Commands added: %d\n", cmd_index);


	printf("---------------------------------------------------\n");
	//Bad inputs
	parser_executor( my_parser, (uint8_t *)"X7T\0" );
	parser_executor( my_parser, (uint8_t *)"VEL++255\0" );

	printf("---------------------------------------------------\n");
	//Good inputs
	parser_executor( my_parser, (uint8_t *)"X7\0" );
	parser_executor( my_parser, (uint8_t *)"VEL+255\0" );

	#endif	//TEST_ONECHAR_ARG

	//----------------------------------------------------------------
	//	TEST MULTICHAR COMMANDS
	//----------------------------------------------------------------

	#ifdef TEST_MULTICHAR

	int cmd_index = 0;

		//!Add commands to the dictionary
	//Add a command to the parser. F\0 will automatically execute this handler when received
	if (my_parser.add_cmd( "XENOSCUM", (void *)&my_noarg_function ) == true)
    {
		printf("Test %d | ERR: %d\n", cmd_index, my_parser.get_error());
    }
    cmd_index++;

    printf("Commands added: %d\n", cmd_index);

	//Bad inputs
	printf("---------------------------------------------------\n");
	parser_executor( my_parser, (uint8_t *)"XENORUM\0" );
	parser_executor( my_parser, (uint8_t *)"XENOXENOSCUM\0" );
	parser_executor( my_parser, (uint8_t *)"HXENOSCUM\0" );

	//Good inputs
	printf("---------------------------------------------------\n");
	parser_executor( my_parser, (uint8_t *)"XENOSCUM\0" );

	#endif



	//----------------------------------------------------------------
	//	TEST_TWO_S32
	//----------------------------------------------------------------

	#ifdef TEST_TWO_S32

	int cmd_index = 0;

		//!Add commands to the dictionary
	//Add a command to the parser. F\0 will automatically execute this handler when received
	if (my_parser.add_cmd( "POS%dNEG%dSHAKA\0", (void *)&my_function<int32_t, int32_t> ) == true)
    {
		printf("Test %d | ERR: %d\n", cmd_index, my_parser.get_error());
    }
    cmd_index++;
    printf("Commands added: %d\n", cmd_index);

	//bad input
	printf("---------------------------------------------------\n");
	parser_executor( my_parser, (uint8_t *)"POS-2147483648NEG++2147483647SHAKA\0" );
	//Good input
	printf("---------------------------------------------------\n");
	parser_executor( my_parser, (uint8_t *)"POS-2147483648NEG+2147483647SHAKA\0" );

	#endif

	//----------------------------------------------------------------
	//	TEST_ALL_FOUR
	//----------------------------------------------------------------

	#ifdef TEST_ALL_FOUR

	int cmd_index = 0;

		//!Add commands to the dictionary
	//Add a command to the parser.
	if (my_parser.add_cmd( "POS%dNEG%dSHAKA\0", (void *)&my_function<int32_t, int32_t> ) == true)
    {
		printf("Test %d | ERR: %d\n", cmd_index, my_parser.get_error());
    }
    cmd_index++;
	//Add a command to the parser.
	if (my_parser.add_cmd( "SA%sB%sC%sD%s\0", (void *)&my_function<int8_t, int8_t, int8_t, int8_t> ) == true)
    {
		printf("Test %d | ERR: %d\n", cmd_index, my_parser.get_error());
    }
    cmd_index++;
	//Add a command to the parser.
	if (my_parser.add_cmd( "SSA%SB%SC%SD%S\0", (void *)&my_function<int16_t, int16_t, int16_t, int16_t> ) == true)
    {
		printf("Test %d | ERR: %d\n", cmd_index, my_parser.get_error());
    }
    cmd_index++;
	//Add a command to the parser.
	if (my_parser.add_cmd( "A%uB%uC%uD%u\0", (void *)&my_function<uint8_t, uint8_t, uint8_t, uint8_t> ) == true)
    {
		printf("Test %d | ERR: %d\n", cmd_index, my_parser.get_error());
    }
    cmd_index++;
    printf("Commands added: %d\n", cmd_index);

	printf("---------------------------------------------------\n");
	parser_executor( my_parser, (uint8_t *)"SSA-32768B+32767C-0D-1\0" );
	parser_executor( my_parser, (uint8_t *)"SA-5B+127C0D-128\0" );
	parser_executor( my_parser, (uint8_t *)"A+5B+200C0D+88\0" );
	parser_executor( my_parser, (uint8_t *)"POS+1NEG-1SHAKA\0" );

	#endif

	#ifdef TEST_SEEKER_OF_WAYS_B
	//There was abug where sign would not be initialized and signless argument after - would be decoded as negative. Test bugfix
	//Seeker Of Ways B PWM
	my_parser.add_cmd( "PWMR%SL%S\0", (void *)&my_function<int16_t, int16_t> );

	printf("---------------------------------------------------\n");
	parser_executor( my_parser, (uint8_t *)"PWMR127L127\0" );
	parser_executor( my_parser, (uint8_t *)"PWMR-127L-127\0" );
	parser_executor( my_parser, (uint8_t *)"PWMR127L-127\0" );
	parser_executor( my_parser, (uint8_t *)"PWMR-127L127\0" );

	#endif // TEST_SEEKER_OF_WAYS_B

	#ifdef TEST_MIXED_TWO_ARGS

	bool f_ret = false;
	f_ret |= my_parser.add_cmd( "MIXA%u:%U\0", (void *)&my_function<uint8_t,uint16_t> );
	f_ret |= my_parser.add_cmd( "MIXB%u:%D\0", (void *)&my_function<uint8_t,uint32_t> );
	f_ret |= my_parser.add_cmd( "MIXC%d:%U\0", (void *)&my_function<int32_t,uint16_t> );

	if (f_ret == true)
	{
		printf("Failed to register command!!!\n");
	}

	printf("---------------------------------------------------\n");

	parser_executor( my_parser, (uint8_t *)"MIXA255:65535\0" );
	parser_executor( my_parser, (uint8_t *)"MIXB255:99999999\0" );
	parser_executor( my_parser, (uint8_t *)"MIXC-99999999:+9999\0" );

	#endif // TEST_MIXED_TWO_ARGS

	#ifdef TEST_MIXED_THREE_ARGS

	bool f_ret = false;
	f_ret |= my_parser.add_cmd( "MIXA%u:%U:%D\0", (void *)&my_function<uint8_t,uint16_t,uint32_t> );
	f_ret |= my_parser.add_cmd( "MIXB%u:%D:%s\0", (void *)&my_function<uint8_t,uint32_t,int8_t> );
	f_ret |= my_parser.add_cmd( "MIXC%d:%U:%U\0", (void *)&my_function<int32_t,uint16_t,uint16_t> );

	if (f_ret == true)
	{
		printf("Failed to register command!!!\n");
	}

	printf("---------------------------------------------------\n");

	parser_executor( my_parser, (uint8_t *)"MIXA255:65535:99999999\0" );
	parser_executor( my_parser, (uint8_t *)"MIXB255:99999999:-99\0" );
	parser_executor( my_parser, (uint8_t *)"MIXC-99999999:+9999:9999\0" );

	#endif // TEST_MIXED_THREE_ARGS

	#ifdef TEST_MIXED_FOUR_ARGS

	bool f_ret = false;
	f_ret |= my_parser.add_cmd( "MIXA%u:%s:%u:%s\0", (void *)&my_function<uint8_t,int8_t,uint8_t,int8_t> );
	f_ret |= my_parser.add_cmd( "MIXB%u:%D:%s:%S\0", (void *)&my_function<uint8_t,uint32_t,int8_t,int16_t> );
	f_ret |= my_parser.add_cmd( "MIXC%S:%U:%U:%S\0", (void *)&my_function<int16_t,uint16_t,uint16_t,int16_t> );

	if (f_ret == true)
	{
		printf("Failed to register command!!!\n");
	}

	printf("---------------------------------------------------\n");

	parser_executor( my_parser, (uint8_t *)"MIXA255:-128:99:127\0" );
	parser_executor( my_parser, (uint8_t *)"MIXB255:99999999:-99:-9999\0" );
	parser_executor( my_parser, (uint8_t *)"MIXC-9999:+9999:9999:9999\0" );

	#endif // TEST_MIXED_THREE_ARGS

	//----------------------------------------------------------------
	//	RETURN
	//----------------------------------------------------------------

	//Trace Return vith return value
	DRETURN_ARG("out: %d\n", 0);

	return true; //OK
}	//end main

/****************************************************************************
**	@brief Function
**	my_noarg_function
****************************************************************************/
//! @return
//! @details
//! one to four arguments with template based mixed types
//! overload the templated functions to get different number of arguments
/***************************************************************************/

void my_noarg_function( void )
{
	DENTER();
	printf( "EXE | %s  -> void\n", __FUNCTION__ );
	DRETURN();
	return;
}

template <typename Ta>
void my_function( Ta arg_a )
{
	DENTER_ARG("in: %d\n", arg_a );
	printf("EXE | %s  -> void | arg: %d\n", __FUNCTION__, arg_a );
	DRETURN();
	return;
}

template <typename Ta, typename Tb>
void my_function( Ta arg_a, Tb arg_b )
{
	DENTER_ARG("in: %d %d\n", arg_a, arg_b );
	printf("EXE | %s  -> void | arg: %d %d\n", __FUNCTION__, arg_a, arg_b );
	DRETURN();
	return;
}

template <typename Ta, typename Tb, typename Tc>
void my_function( Ta arg_a, Tb arg_b, Tc arg_c )
{
	DENTER_ARG("in: %d %d %d\n", arg_a, arg_b, arg_c );
	printf("EXE | %s  -> void | arg: %d %d %d\n", __FUNCTION__, arg_a, arg_b, arg_c );
	DRETURN();
	return;
}

template <typename Ta, typename Tb, typename Tc, typename Td>
void my_function( Ta arg_a, Tb arg_b, Tc arg_c, Td arg_d )
{
	DENTER_ARG("in: %d %d %d %d\n", arg_a, arg_b, arg_c, arg_d );
	printf("EXE | %s  -> void | arg: %d %d %d %d\n", __FUNCTION__, arg_a, arg_b, arg_c, arg_d );
	DRETURN();
	return;
}

/****************************************************************************
**	Function
**	parser_executor | Orangebot::Uniparser &, uint8_t *
****************************************************************************/
//! @param parser |
//! @param str |
//! @brief dummy method to copy the code
//! @details Feed a string to the parser
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

	//----------------------------------------------------------------
	//	RETURN
	//----------------------------------------------------------------

	//Trace Return vith return value
	DRETURN();

	return; //OK
}	//end function: parser_executor | Orangebot::Uniparser & | uint8_t *
