/****************************************************************************
**	R@P PROJECT
*****************************************************************************
**
*****************************************************************************
**	Author: 			Orso Eric
**	Creation Date:		24/08/11
**	Last Edit Date:
**	Revision:			1
**	Version:			0.1 ALFA
****************************************************************************/

/****************************************************************************
**	HYSTORY VERSION
*****************************************************************************
**	>fi -127 = -pi, +127 = pi
**	>implemented at_arc8 (FULL PRECISION MODE)
**	>implemented at_sin8
**	>!! i have seen that cos8 would require another arc, because if i put 180° to 127
**	i can't have exactly 90°, so now:
**	fi (-127:+127) -128 = -pi
**	>implemented at_cos8
**	>implemented at_aarc8, to save memory i used a search in the arc8 lut, it make
**	the function a lot slower but save 128b of memory and i reuse the arc8 lut :)
**	i fixed the rounder as best as possible,, but some angle are at dead center
**	and it round in the wrong direction, no big deal
**	>implemented asin8, the total integral error is 24, because of the rounding
**	es. asin(95) = 34.4, but get rounded to 35.
**	>I now round down in the aarc8 if the index is equal to the halfway but
**	the input is bigger than a treshold
**	>acos8 implemented, suffer the same problem of asin8
**	>tan8 implemented, it uses it's own lut. Input angle is limited from -45° to +45°
**	>atan8 implemented. Suffer the same problem as aarc8, some value close to 0.5
**	are rounded in the wrong direction. no big deal. total int error: 12
****************************************************************************/

/****************************************************************************
**	DESCRIPTION
*****************************************************************************
**	Angle are described by an int8:t
**	-128	|	ERR
**	-127	|	-pi
**	...		|	...
**	0		|	0
**	...		|	...
**	+127	|	+pi
**
**	The output of sin and cos use the full range -127:+127
**	-128	|	ERR
**	-127	|	-pi
**	...		|	...
**	0		|	0
**	...		|	...
**	+127	|	+pi

****************************************************************************/

/****************************************************************************
**	KNOWN BUG
*****************************************************************************
**
****************************************************************************/

/****************************************************************************
**	INCLUDE
****************************************************************************/

#include <stdint.h>
//Useful macros
#include "at_utils.h"

#include "at_trig8.h"

/****************************************************************************
**	GLOBAL VARIABILE
****************************************************************************/

	///**************************************************************************
	///	LUT_ARC8
	///**************************************************************************
	//	This lut contain the value for a arc of sin, mirroring of this table allow
	//	to compose sin and cos
	//	This LUT can be used in reverse, searching an output, the row will allow to
	//	Calculate the angle for the asin and acos functions
	//	63 values

//If: I store LUTs in the flash
#ifdef USE_PROGSPACE
	PROGMEM const S8 lut_arc8[] =
//If: i store LUTs in the RAM
#else 
	const static S8 lut_arc8[] =
#endif
{
	3,
	6,
	9,
	12,
	16,
	19,
	22,
	25,
	28,
	31,
	34,
	37,
	40,
	43,
	46,
	49,
	51,
	54,
	57,
	60,
	63,
	65,
	68,
	71,
	73,
	76,
	78,
	81,
	83,
	85,
	88,
	90,
	92,
	94,
	96,
	98,
	100,
	102,
	104,
	106,
	107,
	109,
	111,
	112,
	113,
	115,
	116,
	117,
	118,
	120,
	121,
	122,
	122,
	123,
	124,
	125,
	125,
	126,
	126,
	126,
	127,
	127,
	127
};

	///**************************************************************************
	///	LUT_TAN8
	///**************************************************************************
	//	This lut contain the value for the tangent function

//If: I store LUTs in the flash
#ifdef USE_PROGSPACE
	PROGMEM const S8 lut_tan8[] =
//If: i store LUTs in the RAM
#else 
	const static S8 lut_tan8[] =
#endif
{
	3,
	6,
	9,
	13,
	16,
	19,
	22,
	25,
	29,
	32,
	35,
	39,
	42,
	45,
	49,
	53,
	56,
	60,
	64,
	68,
	72,
	76,
	80,
	85,
	89,
	94,
	99,
	104,
	110,
	115,
	121
};

/****************************************************************************
**	FUNCTION
****************************************************************************/

/****************************************************************************
**	at_sqrt8
*****************************************************************************
**	PARAMETER:
**	RETURN:
**	DESCRIPTION:
**	Compute the square root of the input
**	127 = 1.0
****************************************************************************/
/*
uint8_t at_sqrt8( uint8_t )
{
	///--------------------------------------------------------------------------
	///	STATIC VARIABILE
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	LOCAL VARIABILE
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	CHECK AND INITIALIZATIONS
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	BODY
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	RETURN
	///--------------------------------------------------------------------------

	return;
}	//end function: at_sqrt8
*/

/****************************************************************************
**	at_arc8
*****************************************************************************
**	PARAMETER:
**	RETURN:
**	DESCRIPTION:
**	Compute the arc of sin from fi = 1 to fi = 63
**	It's the main component of the sin and cos function
****************************************************************************/

int8_t at_arc8( int8_t fi )
{
	///--------------------------------------------------------------------------
	///	STATIC VARIABILE
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	LOCAL VARIABILE
	///--------------------------------------------------------------------------

	//temp return value
	int8_t ret;

	///--------------------------------------------------------------------------
	///	CHECK AND INITIALIZATIONS
	///--------------------------------------------------------------------------

	if ( (fi < +0) || (fi > +63) )
	{
		ret = -128;
		return ret;
	}

	///--------------------------------------------------------------------------
	///	BODY
	///--------------------------------------------------------------------------
	//	here i compute the arc approximations

	ret = READ_LUT_ARC8(fi);

	///--------------------------------------------------------------------------
	///	RETURN
	///--------------------------------------------------------------------------

	return ret;
}

/****************************************************************************
**	at_sin8
*****************************************************************************
**	PARAMETER:
**	>fi
**		The input angle range from -127 to +127 (-128 is an error code)
**		fi		rad
**		-128 	| -pi
**		-64		| -pi/2
**		-32		| -pi/4
**		0 		| 0
**		+32		| +pi/4
**		+64		| +pi/2
**		+128	| +pi
**	>Out
**		The output range from -127 to +127 (-128 is an error code
**		out			sin(fi)
**		-127 	| -1
**		0 		| 0
**		+127	| +1
**	RETURN:
**	DESCRIPTION:
**	Calculate the sin of the angle fi
**		SYMMETRY:
**	I just need to compute the ramp 1:63, all other values are mirroring of this
**	IN			OUT
**	----------------
**	-127	|	-arc(0)
**	-65		|	-arc(62)
**	-64		|	-127
**	-63		|	-arc(62)
**	-1		|	-arc(0)
**	0		|	0
**	+1		|	+arc(0)
**	+63		|	+arc(62)
**	+64		|	+127
**	+65		| 	+arc(62)
**	+127	|	+arc(0)
****************************************************************************/

int8_t at_sin8( int8_t fi )
{
	///--------------------------------------------------------------------------
	///	STATIC VARIABILE
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	LOCAL VARIABILE
	///--------------------------------------------------------------------------

	//store the sign of the output (default 0 = POSITIVE)
	uint8_t sign_flag = 0;
	//output value
	int8_t out;

	///--------------------------------------------------------------------------
	///	CHECK AND INITIALIZATIONS
	///--------------------------------------------------------------------------

	//If the input angle is error, the output is also error
	if (fi == -128)
	{
		return -128;
	}

	///--------------------------------------------------------------------------
	///	BODY
	///--------------------------------------------------------------------------

	//if i have the origin
	if (fi == 0)
	{
		//the sin will also return 0
		return 0;
	}

		//Compute the angle accounting for the various simmetry of sin
	//the function is odd
	if (fi < 0)
	{
		sign_flag = 1;
		fi = -fi;
	}
	//90° angle
	if (fi == 64)
	{
		//angle that will return 127
		fi = 62;
	}
	else if (fi > 64)
	{

		fi = 127-fi;
	}
	//from +1:+63
	else
	{
		//become +0:+62 in the lut
		fi = fi -1;
	}

	//I have the modified angle, i compute the sin value
	out = at_arc8( fi );
	//I apply sign correction
	if (sign_flag == 1)
	{
		out = -out;
	}

	///--------------------------------------------------------------------------
	///	RETURN
	///--------------------------------------------------------------------------

	return out;
}	//end function: at_sin8

/****************************************************************************
**	at_cos8
*****************************************************************************
**	PARAMETER:
**	>fi
**		The input angle range from -127 to +127 (-128 is an error code)
**		fi		rad
**		-128 	| -pi
**		-64		| -pi/2
**		-32		| -pi/4
**		0 		| 0
**		+32		| +pi/4
**		+64		| +pi/2
**		+128	| +pi
**	RETURN:
**		The output range from -127 to +127 (-128 is an error code
**		out			cos(fi)
**		-127 	| -1
**		0 		| 0
**		+127	| +1
**	DESCRIPTION:
**	Calculate the cos of the angle fi
**		SYMMETRY:
**	I just need to compute the ramp 1:63, all other values are mirroring of this
**	IN			OUT
**	----------------
**	-127	|	-arc(62)
**	-65		|	-arc(0)
**	-64		|	0
**	-63		|	+arc(0)
**	-1		|	+arc(62)
**	0		|	+127
**	+1		|	+arc(62)
**	+63		|	+arc(0)
**	+64		|	0
**	+65		| 	-arc(0)
**	+127	|	-arc(62)
****************************************************************************/

int8_t at_cos8( int8_t fi )
{
	///--------------------------------------------------------------------------
	///	STATIC VARIABILE
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	LOCAL VARIABILE
	///--------------------------------------------------------------------------

	//store the sign of the output (default 0 = POSITIVE)
	uint8_t sign_flag = 0;
	//output value
	int8_t out;

	///--------------------------------------------------------------------------
	///	CHECK AND INITIALIZATIONS
	///--------------------------------------------------------------------------

	//If the input angle is error, the output is also error
	if (fi == -128)
	{
		return -128;
	}

	///--------------------------------------------------------------------------
	///	BODY
	///--------------------------------------------------------------------------

	//if i have the origin
	if (fi == 0)
	{
		//the sin will return 1
		return +127;
	}

		//Compute the angle accounting for the various simmetry of sin
	//the function is even
	if (fi < 0)
	{
		fi = -fi;
	}
	//90° angle
	if (fi == 64)
	{
		//at 90°, the cos is 0
		return 0;
	}
	//from +65:+127
	else if (fi > 64)
	{
		//angle > 90° return negative
		sign_flag = 1;
		//become +0:+62
		fi = fi-65;
	}
	//from +1:+63
	else
	{
		//become +62:+0 in the lut
		fi = 63 -fi;
	}

	//I have the modified angle, i compute the sin value
	out = at_arc8( fi );
	//I apply sign correction
	if (sign_flag == 1)
	{
		out = -out;
	}

	///--------------------------------------------------------------------------
	///	RETURN
	///--------------------------------------------------------------------------

	return out;
}	//end function: at_sin8

/****************************************************************************
**	AT_TAN8
*****************************************************************************
**	PARAMETER:
**	RETURN:
**	DESCRIPTION:
**	Compute the tangent of an angle fi and q obey to the same limitation. i use an alternate 32b lut
**	fi is limited from [-32:+32] [-45°:+45°]. the output is [-127:+127] [-1.0:+1.0]
**	If i want to compute the angle between 45° and 90°, the equation is simple
**	>tan(fi) = 1/tan( 90° -fi )
**	I think this is the best way since it keeps fi and q the same as the others functions
****************************************************************************/

int8_t at_tan8(int8_t fi )
{
	///--------------------------------------------------------------------------
	///	STATIC VARIABILE
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	LOCAL VARIABILE
	///--------------------------------------------------------------------------

	//save the sign of the argument
	uint8_t sign_flag = 0;
	//save the output
	int8_t q;

	///--------------------------------------------------------------------------
	///	CHECK AND INITIALIZATIONS
	///--------------------------------------------------------------------------

	//If: the argument is out-of-range
	if ((fi < -AT_PI4) || (fi > +AT_PI4))
	{
		return AT_ERR_ANGLE;
	}

	//If: input is 0°
	if (fi == 0)
	{
		//return 0.0
		return 0;
	}

	///--------------------------------------------------------------------------
	///	BODY
	///--------------------------------------------------------------------------

	//tan has odd parity
	if (fi < 0)
	{
		//make argument positive
		fi = -fi;
		//Save the sign
		sign_flag = 1;
	}
	//If: Trival angle: +45°
	if (fi == AT_PI4)
	{
		//q = +1.0
		q = 127;
	}
	//If: all others angles
	else
	{
		//Fetch q from the lut
		q = READ_LUT_TAN8(fi -1);
	}
	//Sign correction
	if (sign_flag == 1)
	{
		q = -q;
	}


	///--------------------------------------------------------------------------
	///	RETURN
	///--------------------------------------------------------------------------

	return q;
}	//end function: at_tan8

/****************************************************************************
**	AT_AARC8
*****************************************************************************
**	PARAMETER:
**	RETURN:
**	DESCRIPTION:
**	Compute the arc for the inverse sin and cos functions
**	>idea0: i have a lut with all the inverse-arc value (128b)
**	>idea1: i search in the direct arc lut the value and obtain the original angle
****************************************************************************/

int8_t at_aarc8( int8_t q )
{
	///--------------------------------------------------------------------------
	///	STATIC VARIABILE
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	LOCAL VARIABILE
	///--------------------------------------------------------------------------

	//temp index to the arc lut
	int8_t t = 0;
	//
	int8_t temp;
	//Output angle
	uint8_t fi;
	//lut value at index
	int8_t val;
	//lut value prec to index
	int8_t prec;

	//used to detect rest of the division
	uint8_t odd_flag;

	///--------------------------------------------------------------------------
	///	CHECK AND INITIALIZATIONS
	///--------------------------------------------------------------------------

	//The arc function accept only positive values
	if (q < 0)
	{
		return -128;
	}

	///--------------------------------------------------------------------------
	///	BODY
	///--------------------------------------------------------------------------

	///STEP0:
	//If i have a trival value i return the angle
	//If: out = 0
	if (q == 0)
	{
		//fi = 0°
		return 0;
	}
	//If: out = +1
	if (q == 127)
	{
		//fi = 90°
		return 64;
	}
	///STEP1:
	//Search in the arc lut for the first value higher or equal than what i am searching
	//While:
	while
	(
		(t < LUT_ARC8_SIZE) && 			//I have not reached the end of the lut
		(READ_LUT_ARC8(t) < q)		//I have not found a number bigger than what i am searching
	)
	{
		//check next entry
		t++;
	}
	//printf("lut index: %d\n",t);
	///STEP2:
	//I have the row of the arc lut theat contain the first output equal or bigger then q
	//i have to filter the result

	//store current value
	val = READ_LUT_ARC8(t);
	//printf("index val: %d\n",val);
	//If: the Index value BIGGER then what i am searching?
	if (val > q)
	{
		//Check the value before, if q is less than half-way, round down result
		//Extract prec
		if (t > 0)
		{
			prec = READ_LUT_ARC8(t -1);
		}
		else
		{
			prec = 0;
		}
		//printf("prec val: %d\n",prec);
		//Calculate Halfway value
		temp = val-prec;
		//If odd the division will leave a rest (+0.5)
		odd_flag = 0;
		if ((temp & 0x01) == 0x01)
		{
			odd_flag = 1;
		}
		//halfway value
		temp = prec + temp/2;
		//printf("halfway: %d, parity: %d\n", temp, odd_flag);
		//Is q less then halfway?
		if
		(
			(q < temp) ||						//q less then halfway
			((q == temp) && (odd_flag == 1)) 	//q is equal to halfway but halfway was rounded down
		)
		{
			//round down then ncrease by one to obtain angle
			fi = t;
		}
		//Is q exactly in the halfway?
		else
		{
			//!!! it should not make much difference, but rounding this way make
			//the resulting table rounded closer to the original function
			//For low q i round up
			if (q < RD_ON_EQUAL_HALFWAY_QTH)
			{
				fi = t +1;
			}
			//For high q iround down
			else
			{
				fi =t;
			}
		}
	}	//End If: the Index value BIGGER then what i am searching?
	//If: the index value is equal to what i am searching?
	else
	{
		//are there multiple enrty in the lut?
		//fetch succ
		temp = READ_LUT_ARC8(t +1);
		//If there is at least 1 more entry
		if (temp == q)
		{
			//round up and account for the missing 0 in the lut
			fi = t +1 +1;
		}
		//
		else
		{
			//calculate angle
			fi = t +1;
		}
	}

	///--------------------------------------------------------------------------
	///	RETURN
	///--------------------------------------------------------------------------

	//
	return fi;
}	//end function: at_aarc8

/****************************************************************************
**	AT_ASIN8
*****************************************************************************
**	PARAMETER:
**	RETURN:
**	DESCRIPTION:
**	inverse sin function, the input range is -127:+127 that equal -1:+1
**	I use the inverse arc function and mirror it in the right way
**	q
**	-127	|	-aarc(127)
**	-1		|	-aarc(1)
**	0		|	0
**	+1		|	+aarc(1)
**	+127	|	+aarc(127)
**
****************************************************************************/

extern int8_t at_asin8( int8_t q )
{
	///--------------------------------------------------------------------------
	///	STATIC VARIABILE
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	LOCAL VARIABILE
	///--------------------------------------------------------------------------

	//
	uint8_t sign_flag=0;
	//return angle
	int8_t fi;

	///--------------------------------------------------------------------------
	///	CHECK AND INITIALIZATIONS
	///--------------------------------------------------------------------------

	if (q == -128)
	{
		return -128;
	}

	///--------------------------------------------------------------------------
	///	BODY
	///--------------------------------------------------------------------------

	//asin is odd, if argument is <0
	if (q < 0)
	{
		//make argument >0
		q = -q;
		//invert sign of result
		sign_flag = 1;
	}
	//calculate angle
	fi = at_aarc8( q );
	//correct the sign
	if (sign_flag == 1)
	{
		fi = -fi;
	}

	///--------------------------------------------------------------------------
	///	RETURN
	///--------------------------------------------------------------------------

	return fi;
}	//End function: asin8

/****************************************************************************
**	AT_ACOS8
*****************************************************************************
**	PARAMETER:
**	RETURN:
**	DESCRIPTION:
**	inverse cos function, the input range is -127:+127 that equal -1:+1
**	I use the inverse arc function and mirror it in the right way
**	q
**	-127	|	-128			//+pi is non existent, would be 64 + aarc(127)
**	-126	|	64 +aarc(126)
**	-1		|	64 +aarc(1)
**	0		|	64 -aarc(0)		//64
**	+1		|	64 -aarc(1)
**	+126	|	64 -aarc(126)
**	+127	|	64-aarc(127)	//0
**
****************************************************************************/

extern int8_t at_acos8( int8_t q )
{
	///--------------------------------------------------------------------------
	///	STATIC VARIABILE
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	LOCAL VARIABILE
	///--------------------------------------------------------------------------

	uint8_t sign_flag = 0;
	//return angle
	int8_t fi;

	///--------------------------------------------------------------------------
	///	CHECK AND INITIALIZATIONS
	///--------------------------------------------------------------------------

	//return err in case of err or if the result would be 128 (+pi)
	if ((q == -128) || (q == -127))
	{
		return -128;
	}

	///--------------------------------------------------------------------------
	///	BODY
	///--------------------------------------------------------------------------

	//acos is even, if argument is <0
	if (q < 0)
	{
		//make argument >0
		q = -q;
		//save the sign
		sign_flag = 1;
	}
	//fetch the arc value
	fi = at_aarc8( q );
	//Calculate the angle
	if (sign_flag == 0)
	{
		fi = 64 -fi;
	}
	else
	{
		fi = 64 +fi;
	}

	///--------------------------------------------------------------------------
	///	RETURN
	///--------------------------------------------------------------------------

	return fi;
}	//End function: acos8

/****************************************************************************
**	AT_ATAN8
*****************************************************************************
**	PARAMETER:
**	input range: [-127:+127] [-1.0:+1.0]
**	RETURN:
**	Angle
**	Output range: [-32:+32] [-45°:+45°]
**	-128 is an error code, all other value are forbidden
**	DESCRIPTION:
**	inverse tan function
****************************************************************************/

int8_t at_atan8( int8_t q )
{
	///--------------------------------------------------------------------------
	///	STATIC VARIABILE
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	LOCAL VARIABILE
	///--------------------------------------------------------------------------

	//store the reminder of the halfway
	uint8_t odd_flag = 0;
	//store the sign of the argument for sign correction
	uint8_t sign_flag = 0;
	//index to the lut, after rounding will be the angle which: tan8(angle) = q
	int8_t index;
	//current lut value and prec lut value
	int8_t curr, prec;
	//temp var
	int8_t temp;
	//output angle
	int8_t fi;

	///--------------------------------------------------------------------------
	///	CHECK AND INITIALIZATIONS
	///--------------------------------------------------------------------------

	//If the input is error
	if (q == AT_ERR_ANGLE)
	{
		//return error angle
		return AT_ERR_ANGLE;
	}

	///--------------------------------------------------------------------------
	///	BODY
	///--------------------------------------------------------------------------
	//	I will do the same as the aarc8, i will read the tangent lut in reverse
	//	then apply rounding and correction

	///STEP0:
	//Sign correction, the function is odd
	//If: negative argument
	if (q < 0)
	{
		//correct sign
		q = -q;
		//save sign
		sign_flag = 1;
	}
	///STEP1:
	//Search the first enfry in the lut bigger or equal then q
	//Reset while variabiles
	index = 0;
	//While:
	while
	(
		(index < LUT_TAN8_SIZE) &&	//I have not reached the end of the LUT
		(READ_LUT_TAN8(index) < q)	//The current lut entry is smaller then q
	)
	{
		//
		index++;
	}
	//Store current lut entry
	curr = READ_LUT_TAN8(index);
	//If: my search found the last entry, but the entry value is lower then q
	if ( ( index == LUT_TAN8_SIZE ) && (curr < q) )
	{
		//in reality i should have chosen the last entry (which was trival)
		curr = +127;
	}
	///STEP2:
	//Rounding. I need to round up or down the index based on the previous and prec entry value
	//If: lut entry is bigger than q (i may need to round down)
	if (curr > q)
	{
		//store prec entry
		//if: i am in the first entry already
		if (index == 0)
		{
			//I have not stored the first value, i stored the first non trival value
			prec = 0;
		}
		//
		else
		{
			prec = READ_LUT_TAN8(index -1);
		}
		//Calculate halfway and reminder
		temp = curr-prec;
		//If odd the division will leave a rest (+0.5)
		odd_flag = 0;
		if ((temp & 0x01) == 0x01)
		{
			odd_flag = 1;
		}
		//halfway value
		temp = prec + temp/2;
		//printf("halfway: %d, parity: %d\n", temp, odd_flag);
		//Is q less then halfway?
		if
		(
			(q < temp) ||						//q less then halfway
			((q == temp) && (odd_flag == 1)) 	//q is equal to halfway but halfway was rounded down
		)
		{
			//round down (-1), lut start from 1° entry (+1)
			fi = index; //-1 +1
		}
		//Is q exactly in the halfway?
		else
		{
			//no rounding (+0), lut start from 1° entry (+1)
			fi = index +0 +1;
		}
	}
	//If: lut entry is equal to q (i may need to round up)
	else
	{
		//If: the next value is equal to the entry?
		//fetch succ
		temp = READ_LUT_TAN8(index +1);
		//If there is at least 1 more entry
		if (temp == q)
		{
			//round up (+1),lut start from 1° entry (+1)
			fi = index +1 +1;
		}
		//
		else
		{
			//no rounding (+0), lut start from 1° entry (+1)
			fi = index +0 +1;
		}
	}
	///STEP3:
	//Sign correction
	//If
	if (sign_flag == 1)
	{
		fi = -fi;
	}

	//printf("index: %d\n",index);

	///--------------------------------------------------------------------------
	///	RETURN
	///--------------------------------------------------------------------------

	return fi;
}	//end function: atan8

/****************************************************************************
**	AT_ATAN8_2
*****************************************************************************
**	PARAMETER:
**	RETURN:
**	DESCRIPTION:
**	Calculate the angle of this vector, apply angle correction
**	It's the most useful trigonometric function, i input the coordinate and
**	get it's angle, work over full range
**
**	Trick:
**	I can use my incomplete atan function to get angle over the full range
**	if y<x -> fi = atan(y/x)
**	if y>x -> fi = 90° - atan(x/y)
**
**	Sign correction:
**	y 	| 	x	|
**	+	|	+	| fi
**	+	|	-	| 180° -fi
**	-	|	+	| -fi
**	-	|	-	| -180°+fi
****************************************************************************/

uint8_t at_atan8_2( int8_t x, int8_t y )
{
	///--------------------------------------------------------------------------
	///	STATIC VARIABILE
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	LOCAL VARIABILE
	///--------------------------------------------------------------------------

	//store function flags
	//	0x01: 	x sign
	//	0x02:	y sign
	//	0x04: 	division flag (0 -> x is num, 1 -> y is num)
	uint8_t flags = 0x00;
	//argument and result of the division
	uint8_t num, den, div;
	uint16_t tmp, tmp1;
	//result
	int8_t fi;

	///--------------------------------------------------------------------------
	///	CHECK AND INITIALIZATIONS
	///--------------------------------------------------------------------------

	///--------------------------------------------------------------------------
	///	BODY
	///--------------------------------------------------------------------------

	///STEP 0:
	//make x and y positive and save their sign
	//If: negative
	if (x < 0)
	{
		//make it positive
		x = -x;
		//save sign
		flags = flags | 0x01;
	}
	//If: negative
	if (y < 0)
	{
		//make it positive
		y = -y;
		//save sign
		flags = flags | 0x02;
	}
	//printf("x:%3d,y:%3d,flags:%3x\n",x,y,flags);
	///STEP1:
	//Setup the division variabiles so thet num/den <= 1
	//x and y are postive now, i can store in uint variabiles
	if (y > x)
	{
		//store num and den
		num = x;
		den = y;
		//division flag
		flags = flags | 0x04;
	}
	else
	{
		//store num and den
		num = y;
		den = x;
		//division flag
		//flags = flags | 0x04;
	}
	//printf("num:%3d, den:%3d\n",num, den);
	///STEP2:
	//Calculate the division
	//Special cases
	if (num == 0)
	{
		div = 0;
	}
	else if (num == den)
	{
		div = 127;
	}
	else
	{
		//I have to perform ((127*num)/den) (1.0 = 127)
		//calculate 127*num
		tmp = (uint16_t)0 + ((uint8_t)127 * (uint8_t)num);
		//divide and save the result
		div = (uint8_t)0 + (uint16_t)tmp / (uint8_t)den;
		//now i have to round the result, i calculate the th of tmp that would trigger a round up
		tmp1 = (uint16_t)0 + den*div + den/2;
		//if tmp is bigger then the threshold i round up
		if (tmp > tmp1)
		{
			//round up the result
			div++;
		}
	}
	//printf("div:%3d\n",div);
	///STEP3:
	//calculate the arctan of the division
	fi = at_atan8( div );
	//printf("fi:%3d\n",fi);
	///STEP4:
	//correct the division flag (if rised fi = 90°-fi
	if ((flags & 0x04) == 0x04)
	{
		fi = AT_PI2 -fi;
	}
	//printf("fi:%3d\n");
	///STEP5:
	//correct the x,y signs
	//If: +x, +y
	if ((flags & 0x03) == 0x00)
	{
		//printf("+x +y\n");
		//do nothing
	}
	//If: -x, +y
	else if ((flags & 0x03) == 0x01)
	{
		//printf("-x +y\n");
		//if the result would not be 180°
		if (fi != 0)
		{
			//unpack 128 as 127 +1
			fi = 127 - fi +1;
		}
		else
		{
			//180° is an error angle
			return AT_ERR_ANGLE;
		}
	}
	//If: +x, -y
	else if ((flags & 0x03) == 0x02)
	{
		//printf("+x -y\n");
		fi = -fi;
	}
	//If: -x, -y
	else if ((flags & 0x03) == 0x03)
	{
		//printf("-x -y\n");
		//if the result would not be -180°
		if (fi != 0)
		{
			//unpack -128 as -127 -1
			fi = -127 + fi -1;
		}
		else
		{
			//180° is an error angle
			return AT_ERR_ANGLE;
		}
	}

	//printf("flags:%x\n",flags);
	//printf("x: %3d, y:%3d, div:%3d\n",x,y,div);

	///--------------------------------------------------------------------------
	///	RETURN
	///--------------------------------------------------------------------------

	return fi;
}	//end function:	at_atan8_2
