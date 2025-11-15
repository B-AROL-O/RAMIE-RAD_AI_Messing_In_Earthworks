#ifndef AT_STRING_H
	//header envroiment variabile, is used to detect multiple inclusion
	//of the same header, and can be used in the c file to detect the
	//included library
	#define AT_STRING_H

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

	//max digit of a 8 bit number
	#define MAX_DIGIT8				3
	//max digit of a 16 bit number
	#define MAX_DIGIT16				5

	/****************************************************************************
	**	MACRO
	****************************************************************************/

	/****************************************************************************
	**	TYPEDEF
	****************************************************************************/

	/****************************************************************************
	**	STRUCTURE
	****************************************************************************/

	/****************************************************************************
	**	PROTOTYPE: GLOBAL VARIABILE
	****************************************************************************/

	/****************************************************************************
	**	PROTOTYPE: FUNCTIONS
	****************************************************************************/

	//Convert a U8 in string form, and loads it into a provided vector, terminating it with '\0'
	//User must care about providing a valid vector with sufficient space. Return the index of the terminator inside the vector
	extern U8 u8_to_str( U8 num, U8 *str );
	//Convert an S8 to string. It's actualy a wrapper of the u8_to_string, it just handle the sign
	extern U8 s8_to_str( S8 num, U8 *str );
	//Convert a U16 in string form, and loads it into a provided vector, terminating it with '\0'
	//User must care about providing a valid vector with sufficient space. Return the index of the terminator inside the vector
	extern U8 u16_to_str( U16 num, U8 *str );
	//Convert an S8 to string. It's actualy a wrapper of the u8_to_string, it just handle the sign
	extern U8 s16_to_str( S16 num, U8 *str );


#else
	#warning "multiple inclusion of the header file at_string.h"
#endif

