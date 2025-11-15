/****************************************************************************
**	ROBOTICA@POLITO PROJECT
*****************************************************************************
**	AT_STRING
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
**	>Tested the u8_to_str for all values
**	>Tested the s8_to_str, it's basicaly a wrapper of u8_to_str that handles the signs
**	>tester u16_to_str for all values (I used sprintf and compared output strings)
**	>tester s16_to_str for all values (I used sprintf and compared output strings)
****************************************************************************/

/****************************************************************************
**	DESCRIPTION
*****************************************************************************
**	This library provide basic string functionality for atmel project
****************************************************************************/

/****************************************************************************
**	USED PIN
**	TIPS: compile this field before writing any code: it really helps!
*****************************************************************************
**
****************************************************************************/

/****************************************************************************
**	USED PHERIPERALS
**	TIPS: compile this field before writing any code: it really helps!
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
****************************************************************************/

#include <stdint.h>
#include "at_utils.h"
#include "at_string.h"


/****************************************************************************
**	GLOBAL VARIABILES
****************************************************************************/

/****************************************************************************
** FUNCTIONS
****************************************************************************/


/****************************************************************************
**	U8 TO STR
*****************************************************************************
**	Convert a U8 in string form, and loads it into a provided vector, terminating it with '\0'
**	User must care about providing a valid vector with sufficient space
**	ALGORITHM
**	I feel there are vastly faster algorithms possibiles
**	for example division is done by successive subtractions, dividing 90/10 is slower than 20/10
**	For now I use a naive version
**
****************************************************************************/

U8 u8_to_str( U8 num, U8 *str )
{
	//***********************************************************************
	//	STATIC VARIABILE
	//***********************************************************************

	//***********************************************************************
	//	LOCAL VARIABILE
	//***********************************************************************

	//decimal base
	U8 base[] =
	{
		100,
		10,
		1
	};
	//temp var
	U8 t, u8t, index;
	//flag used to blank non meaningful zeros
	U8 flag;

	//***********************************************************************
	//	BODY
	//***********************************************************************

	///Setup
	//index to the string
	index = 0;
	//this flag is used to blank non meaningful most signficant zeros
	flag = 1;
	///Exe
	//For all bases
	for (t = 0;t < MAX_DIGIT8; t++)
	{
		//If the base is bigger or equal than the number (division is meaningful)
		if (base[t] <= num)
		{
			//Divide number by base, get the digit
			u8t = num/base[t];
			//Write the digit
			str[ index ] = '0' +u8t;
			//Update the number
			num = num - base[t] * u8t;
			//I have found a meaningful digit
			flag = 0;
			//Jump to the next digit
			index++;
		}
		//If: The base is smaller then the number, and I have yet to find a non zero digit, and I'm not to the last digit
		else if ( (flag == 1) && (t != (MAX_DIGIT8 -1)) )
		{
			//do nothing
		}
		//If: I have a meaningful zero
		else
		{
			//It's a zero
			str[ index ] = '0';
			//Jump to the next digit
			index++;
		}
	}	//End for: all bases
	//Append the terminator
	str[ index ] = '\0';

	//***********************************************************************
	//	RETURN
	//***********************************************************************

	return index;
}   //End function:

/****************************************************************************
**	S8 TO STRING
*****************************************************************************
**	Convert an S8 to string. It's actualy a wrapper of the u8_to_string, it just handle the sign
**
****************************************************************************/

U8 s8_to_str( S8 num, U8 *str )
{
	//***********************************************************************
	//	STATIC VARIABILE
	//***********************************************************************

	//***********************************************************************
	//	LOCAL VARIABILE
	//***********************************************************************

	//temp var
	U8 u8t;

	//***********************************************************************
	//	BODY
	//***********************************************************************

	//If: negative
	if (num < 0)
	{
		//Write minus '-'
		str[ 0 ] = '-';
		//Correct sign
		u8t = -num;
		//launch the u8_to_str to the corrected num, but feed the vector shifted by 1 to make room for the sign. save the return value
		u8t = u8_to_str( u8t, &str[1] );
	}
	//If: zero or positive
	else
	{
		//Write minus '+'
		str[ 0 ] = '+';
		//save num, for consistency
		u8t = +num;
		//launch the u8_to_str to the corrected num, but feed the vector shifted by 1 to make room for the sign. save the return value
		u8t = u8_to_str( u8t, &str[1] );
	}

	//***********************************************************************
	//	RETURN
	//***********************************************************************

	//I have the sign, sum 1
	return (u8t +1);
}   //End function:

/****************************************************************************
**	U16 TO STR
*****************************************************************************
**	Convert a U16s in string form, and loads it into a provided vector, terminating it with '\0'
**	User must care about providing a valid vector with sufficient space
**	ALGORITHM
**	I feel there are vastly faster algorithms possibiles
**	for example division is done by successive subtractions, dividing 90/10 is slower than 20/10
**	For now I use a naive version
**
****************************************************************************/

U8 u16_to_str( U16 num, U8 *str )
{
	//***********************************************************************
	//	STATIC VARIABILE
	//***********************************************************************

	//***********************************************************************
	//	LOCAL VARIABILE
	//***********************************************************************

	//decimal base
	U16 base[] =
	{
		10000,
		1000,
		100,
		10,
		1
	};
	//temp var
	U8 t, u8t, index;
	//flag used to blank non meaningful zeros
	U8 flag;

	//***********************************************************************
	//	BODY
	//***********************************************************************

	///Setup
	//index to the string
	index = 0;
	//this flag is used to blank non meaningful most signficant zeros
	flag = 1;
	///Exe
	//For all bases
	for (t = 0;t < MAX_DIGIT16; t++)
	{
		//If the base is bigger or equal than the number (division is meaningful)
		if (base[t] <= num)
		{
			//Divide number by base, get the digit
			u8t = num/base[t];
			//Write the digit
			str[ index ] = '0' +u8t;
			//Update the number
			num = num - base[t] * u8t;
			//I have found a meaningful digit
			flag = 0;
			//Jump to the next digit
			index++;
		}
		//If: The base is smaller then the number, and I have yet to find a non zero digit, and I'm not to the last digit
		else if ( (flag == 1) && (t != (MAX_DIGIT16 -1)) )
		{
			//do nothing
		}
		//If: I have a meaningful zero
		else
		{
			//It's a zero
			str[ index ] = '0';
			//Jump to the next digit
			index++;
		}
	}	//End for: all bases
	//Append the terminator
	str[ index ] = '\0';

	//***********************************************************************
	//	RETURN
	//***********************************************************************

	return index;
}   //End function:

/****************************************************************************
**	S16 TO STRING
*****************************************************************************
**	Convert an S16 to string. It's actualy a wrapper of the s16_to_string, it just handle the sign
**
****************************************************************************/

U8 s16_to_str( S16 num, U8 *str )
{
	//***********************************************************************
	//	STATIC VARIABILE
	//***********************************************************************

	//***********************************************************************
	//	LOCAL VARIABILE
	//***********************************************************************

	//temp var
	U8 u8t;

	U16 u16t;

	//***********************************************************************
	//	BODY
	//***********************************************************************

	//If: negative
	if (num < 0)
	{
		//Write minus '-'
		str[ 0 ] = '-';
		//Correct sign
		u16t = -num;
		//launch the u8_to_str to the corrected num, but feed the vector shifted by 1 to make room for the sign. save the return value
		u8t = u16_to_str( u16t, &str[1] );
	}
	//If: zero or positive
	else
	{
		//Write minus '+'
		str[ 0 ] = '+';
		//save num, for consistency
		u16t = +num;
		//launch the u8_to_str to the corrected num, but feed the vector shifted by 1 to make room for the sign. save the return value
		u8t = u16_to_str( u16t, &str[1] );
	}

	//***********************************************************************
	//	RETURN
	//***********************************************************************

	//I have the sign, sum 1
	return (u8t +1);
}   //End function:
