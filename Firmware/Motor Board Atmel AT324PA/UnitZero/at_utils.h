/****************************************************************************
**	R@P PROJECT
*****************************************************************************
**	AT_UTILS
**	Useful Atmel configuration macros
*****************************************************************************
**	Author: 			Orso Eric
**	Creation Date:		21/05/09
**	Last Edit Date:
**	Revision:			4
**	Version:			1.3
****************************************************************************/

/****************************************************************************
**	HYSTORY VERSION
*****************************************************************************
**	R1V1.0:
**		>implemented generic macro
**		>implemented port configuration macro
**		>implemented UART module macro
**	R2V1.1:
**		>added NOP macro
**		>added MASK macro
**		>added TOGGLE_BIT macro
**		>removed SET_2BIT macro (useless)
**		>removed SET_PORT_VALUE macro (useless)
**		>renamed SET_PORT macro to SET_VAR
**		>renamed CLEAR_PORT macro to CLEAR_VAR
**		>updated PORT_CONFIG macro
**		>updated USART macro handling
**		>added USART enviroment variablile to avoid those annoying warning
**		>comment updated
**		>code reformatted
**	R3V1.2
**		>Removed Shift Macro
**		>Removed Clear and SEt variabile macros
**		>Added conditional creation of PORT MACROS
**		>Added Saturated Math MACROS
**		>Added Fixed point MUL macros
**		>Added 8 bit Fixed point mul macros
**		>Added 8 bit fixed point rounded toward even mul macro
**		>Added 16 bit Fixed point mul macros
**		X>Added 16 bit fixed point rounded toward even mul macro
**	R4V1.3
**		>added circular buffer structure and handling macros
**		>added circular buffer with semaphor, allow for safe buffer push and kick
**		use in ISR and concurrent code
**		>added 8 bit exact logarithm macros
**		>added IS_NUMBR macro
****************************************************************************/

#ifndef AT_UTILS		//gestisco inclusioni multiple
	#define AT_UTILS  //costante di libreria

	/****************************************************************************
	** ENVROIMENT VARIABILE
	****************************************************************************/

	//If uncommented, it will define debug functions
	//#define DEBUG_PRINTF
	//debug to file
	//#define DEBUG_FILE

	/****************************************************************************
	** GLOBAL VARIABILE
	****************************************************************************/

	//If txt logger is active
	#ifdef DEBUG_FILE
		#include <stdio.h>
		//FILE structure
		extern FILE *fdebug;
		//es, at min_dlevel = 2. only level 2 and above will be executed
		extern uint8_t dlevel, min_dlevel;
	#endif

	/****************************************************************************
	** USEFUL DEFINE
	****************************************************************************/

	#ifndef _At_bool
		enum _At_bool
		{
			FALSE	= 0,
			TRUE	= 1
		};

		typedef enum _At_bool At_bool;
	#endif


		///********************************************************************************
		///	NULL POINTER
		///********************************************************************************

	#ifndef NULL
		#define NULL			(void *)0
	#endif

		///********************************************************************************
		///	DATA TYPE RANGE
		///********************************************************************************

	#define U8 uint8_t
	#define S8 int8_t
	#define U16 uint16_t
	#define S16 int16_t
	#define U32 uint32_t
	#define S32 int32_t


	#define MAX_U8			+255
	#define MAX_U16			+65535
	#define MIN_S8			-128
	#define MAX_S8			+127
	#define MIN_S16			-32768
	#define MAX_S16			+32767

	/****************************************************************************
	** GENERAL PURPOSE MACROS
	****************************************************************************/

	//do nothing for one clock cycle
	#define NOP()	__asm__ __volatile__("nop")

	//generate a mask whith '1' in position shift value
	#define MASK( shift_value ) \
		( 0x01 << (shift_value) )

	//generate invrted mask '0' in position shiftvalue
	#define INV_MASK( shift_value )	\
		(~MASK(shift_value))

	//set a single bit inside a variable leaving the other bit untouched
	#define SET_BIT( var, shift_value ) \
		( (var) |= MASK( shift_value ) )

	//clear a single bit inside a variable leaving the other bit untouched
	#define CLEAR_BIT( var, shift_value ) \
		( (var) &= (INV_MASK( shift_value ) ) )

	//toggle a single bit inside a variable leaving the other bit untouched
	#define TOGGLE_BIT( var, shift_value ) \
		( (var) ^= MASK( shift_value ) )

	//TRUE if a bit in position shift value is '1'
	#define IS_BIT_ONE( var, shift_value )	\
		( ( (var) & MASK( shift_value ) ) == MASK( shift_value ) )

	//Extract H part of U16
	#define U16H( data )	\
		(U8)((U16)data >> 8)

	//Extract L part of U16
	#define U16L( data )	\
		(U8)((U16)data & (U16)0x00ff)
	//Extract the h part of a s16 data
	#define S16H( x )	\
		(S8)((U16)((x)>>8))
	//Extract L part of S16. The result is an unsigned number
	#define S16L( x )	\
		(U8)((U16)(x) & (U16)0x00ff)
	//calculate the log base 2 of X (U8), if fail return 255
	#define LOG2U8( x )	\
		((x == 128)?(7):( ((x == 64)?(6):( ((x == 32)?(5):( ((x == 16)?(4):( ((x == 8)?(3):( ((x == 4)?(2):( ((x == 2)?(1):( ((x == 1)?(0):(255)) )) )) )) )) )) )) ))

	//x is a number?
	#define IS_NUMBER( x )	\
		(((x) >= '0') && ((x) <= '9'))

	//divide by 2^n
	#define SHIFT( a, s )	\
		((a) >> (s))

	//residual of a shift by s position right (divide by 2^n)
	#define SHIFT_RES( a, s )	\
		((a) & ((1<<(s))-1))

	//signed shift
	#define SSHIFT( a, s )	\
		(((a) < 0) ? -SHIFT( (-(a)), (s)) : SHIFT( (a), (s)) )

	//residual of a signed shift
	#define SSHIFT_RES( a, s )	\
		( ((a) < 0) ? -SHIFT_RES( (-(a)), (s) ) : SHIFT_RES( (a), (s) ) )

	//multiply by 2^n
	#define SHIFTL( a, s )	\
		((a) << (s))

	/****************************************************************************
	** DEBUG MACROS
	****************************************************************************/

		///********************************************************************************
		///	DEBUG MACROS
		///********************************************************************************
		//powerful macros that add debug functionality (require printf)

	#ifdef DEBUG_PRINTF
		#include <stdio.h>
		#define DPRINTF_VARS()

		#define DPRINTF_START()

		#define DPRINTF_STOP()

		#define DPRINTF_DEPTH( level )

		#define DPRINTF_DISPLAY_DEPTH( level )

		#define DPRINTF( ... )	\
			printf( __VA_ARGS__ )

		#warning "debug printf active"
	#else
		#ifdef DEBUG_FILE
			#define DPRINTF_VARS()	\
				FILE  *fdebug = NULL;	\
				U8 dlevel = 0, min_dlevel = 0;

			#define DPRINTF_START()	\
				fdebug = fopen( "debug.log", "w+")

			#define DPRINTF_STOP()	\
				((fdebug != NULL)?fclose(fdebug),fdebug = NULL:(0))

			//Specify the minimum level to display
			#define DPRINTF_DISPLAY_DEPTH( level )	\
				min_dlevel = level;

			#define DPRINTF( level, ... )	\
				((fdebug != NULL) && (level >= min_dlevel)?fprintf(fdebug, __VA_ARGS__ ):(0))

			#warning "debug file active"
		#else
			#define DPRINTF_VARS()

			#define DPRINTF_START()

			#define DPRINTF_STOP()

			#define DPRINTF_DEPTH( level )

			#define DPRINTF_DISPLAY_DEPTH( level )

			#define DPRINTF( ... )
		#endif
	#endif

	/****************************************************************************
	** CIRCULAR BUFFER MACROS
	*****************************************************************************
	**	Very useful set of macros to handle circular buffers
	**	i use them all the time, making it in macro form is very efficient
	**	Actually, the macro are very generic, i can easily make many types of buffer
	**	probably void * ones too
	****************************************************************************/

		///********************************************************************************
		///	BUFFER STRUCTURE PROTOTYPE
		///********************************************************************************
		// the data type can be anything as long as the field name stay the same

	//Status byte of an At buf structure
	typedef struct _At_buf_status At_buf_status;

	//Circular buffer structure (maximum size 255 element)
	typedef struct _At_buf8 At_buf8;

	//Circular buffer structure with semaphore (maximum size 255 element)
	typedef struct _At_buf8_safe At_buf8_safe;

		///********************************************************************************
		///	BUFFER STRUCTURE DEFINITIONS
		///********************************************************************************
		// the data type can be anything as long as the field name stay the same

	//Status byte of an At buf structure
	struct _At_buf_status
	{
		uint8_t push_flag	: 1;	//Is someone pushing into this buffer?
		uint8_t kick_flag	: 1;	//Is someone kicking from this buffer?
		uint8_t				: 6;	//Future use
	};

	//Circular buffer structure (maximum size 255 element)
	struct _At_buf8
	{
		//size of the buffer
		uint8_t size;
		//pointer to the top element
		uint8_t bot;
		//pointer to the bot element
		uint8_t top;
		//Pointer to the content (can be anything)
		uint8_t *ptr;
	};

	//Circular buffer structure with semaphore, handle exception caused by ISR
	struct _At_buf8_safe
	{
		//size of the buffer
		uint8_t size;
		//pointer to the top element
		uint8_t bot;
		//pointer to the bot element
		uint8_t top;
		//Pointer to the content (can be anything)
		uint8_t *ptr;
		//Status of the
		At_buf_status status;
	};

		///********************************************************************************
		///	AT_BUF_ATTACH
		///********************************************************************************
		///	Attach an existing vector to an existing At_buf structure

	#define AT_BUF_ATTACH( buf, ptr_in, size_in )	\
		( ((buf).ptr = ptr_in), ((buf).size = size_in) )

		///********************************************************************************
		///	AT_BUF_FLUSH
		///********************************************************************************
		/// Flush a buffer, return 0

	#define AT_BUF_FLUSH( buf )	\
		( ((buf).top = 0), ((buf).bot = 0), (0) )

		///********************************************************************************
		///	AT_BUF_FLUSH_SAFER
		///********************************************************************************
		/// Flush a buffer, clear status flags,return 0
		///	Operate only on safe structure

	#define AT_BUF_FLUSH_SAFE( buf ) \
		( ((buf).top = 0), ((buf).bot = 0), (buf).status.push_flag = 0, (buf).status.kick_flag = 0, (0) )

		///********************************************************************************
		///	AT_BUF_PUSH
		///********************************************************************************
		///	Return the number of elements in a buffer

	#define AT_BUF_NUMELEM( buf )	\
		( ((buf).top >= (buf).bot) ? ((buf).top - (buf).bot) : ( (buf).size +(buf).top -(buf).bot  ) )

		///********************************************************************************
		///	AT_BUF_SIZE
		///********************************************************************************
		///	Return the size of the buffer

	#define AT_BUF_SIZE( buf )	\
		( (buf).size )

		///********************************************************************************
		///	AT_BUF_PUSH
		///********************************************************************************
		/// Push a data into a buffer
		///	Load data
		///	If: top < size
		///		top++
		///	Else:
		///		top = 0
		///	RETURN 0

	#define AT_BUF_PUSH( buf, data )	\
		( ( (buf).ptr[ (buf).top ] = data ), ( ( (buf).top < ((buf).size -1) ) ? ((buf).top++) : ((buf).top = 0) ), (0) )

		///********************************************************************************
		///	AT_BUF_PUSH_SAFE
		///********************************************************************************
		/// Push a data into a buffer, checking that the buffer won't overflow,
		/// return 1 if th data could not be pushed
		///	If: current element < size
		///		AT_BUF_PUSH
		///		RETURN 0
		///	Else:
		///		RETURN 1

	#define AT_BUF_PUSH_SAFE( buf, data )	\
		( (AT_BUF_NUMELEM(buf) < ((buf).size-1)) ? (AT_BUF_PUSH(buf,data)) : (1) )

		///********************************************************************************
		///	AT_BUF_PUSH_SAFEER
		///********************************************************************************
		///	Operate on safe structure, return 1 if buf is full, return 2 if buf is busy
		///	because of other operations
		///	If: busy pushing?
		///		return 2
		///	Else:
		///		busy = 1
		///		ret = AT_BUF_PUSH_SAFE
		///		busy = 0
		///		return ret

	#define AT_BUF_PUSH_SAFER( buf, data ) \
		( ((buf).status.push_flag == 1) ? (2) : ( (((buf).status.push_flag = 1), (AT_BUF_NUMELEM(buf) < ((buf).size-1))) ? ((AT_BUF_PUSH(buf,data)), ((buf).status.push_flag = 0), (0)) : (((buf).status.push_flag = 0), (1)) ) )

		///********************************************************************************
		///	AT_BUF_PEEK
		///********************************************************************************
		///	Return the oldest element of the buffer, the user should check if there are
		///	element in the buffer

	#define AT_BUF_PEEK( buf )	\
		( (buf).ptr[ (buf).bot ]  )

		///********************************************************************************
		///	AT_BUF_KICK
		///********************************************************************************
		///	Kick out the oldest element of the buffer, the user should check that there is
		/// at least 1 element in the buffer, return bot
		///	If: bot < size
		///		bot++
		///	Else:
		///		bot = 0
		///	RETURN 0

	#define AT_BUF_KICK( buf )	\
		( (((buf).bot < ((buf).size -1)) ? ((buf).bot++) : ((buf).bot = 0)), (0))

		///********************************************************************************
		///	AT_BUF_KICK_SAFE
		///********************************************************************************
		///	kick the oldest element if there is at least one element in the buffer
		/// return 1 if fail
		///	If: num_elem > 0
		///		AT_BUF_KICK
		///		RETURN 0
		///	Else
		///		RETURN 1

	#define AT_BUF_KICK_SAFE( buf )	\
		(( AT_BUF_NUMELEM( buf ) > 0 ) ? (AT_BUF_KICK( buf )) : (1))

		///********************************************************************************
		///	AT_BUF_KICK_SAFER
		///********************************************************************************
		///	Kick the oldest element, if another kick operation is rnning, return 2 instead
		///	Work only on safe structure
		///	If: busy  == 1
		///		RETURN 2
		///	Else:
		///		busy = 1
		///		ret = AT_BUF_KICK_SAFE
		///		busy = 0
		///		RETURN ret

	#define AT_BUF_KICK_SAFER( buf )	\
		( ((buf).status.kick_flag == 1) ? (2) : ( (((buf).status.kick_flag = 1), ( AT_BUF_NUMELEM( buf ) > 0 )) ? ((AT_BUF_KICK( buf )), ((buf).status.kick_flag = 0), (0)) : (((buf).status.kick_flag = 0), (1))        ) )

	/****************************************************************************
	** PRESCALER CALCULATION MACROS
	****************************************************************************/

	/*******************************************
	**	AT_ABS
	*******************************************/

	//Absolute value
	#define AT_ABS( x )	\
		((x>=0)?(x):(-(x)))

	/*******************************************
	**	AT_OCR
	********************************************
	**	Setup a counter to obtain a given fout
	**
	**	fout [Hz]	= output frequency
	**	fin [Hz]	= input frequency
	**	N [1]		= prescaler
	**	K [1]		= 0 or 1, depend if use UP or UPDOWN mode
	**
	**	fout = fin / (2^N * (M+1) * 2^K)
	**
	**	M = fin / (fout * 2^(N+K)) -1
	*******************************************/

	#define AT_OCR( fin, fout, n, k)	\
		(( ((fin) / (fout)) >> ((n)+(k)) ) -1)

	/*******************************************
	**	AT_OCR_ROUND
	********************************************
	**	choose the correct M between M and M+1
	**	>M' = f(2*fout)
	**	>M' even?
	**		>M = f(fout)
	**	>else
	**		>M = f(fout) +1
	*******************************************/

	#define AT_OCR_ROUND( fin, fout, n, k)	\
		(( AT_OCR((fin),(fout)*2,(n),(k)) & 1 )?( 1 +AT_OCR((fin),(fout),(n),(k)) ):(AT_OCR((fin),(fout),(n),(k))))

	/****************************************************************************
	**	FIXED POINT ARITH MACROS
	*****************************************************************************
	**	Those macros handle operation with fixed point variabile
	**	The SUM macros are variabile type indipendent and handle saturation limit
	**	ex.
	**	0111.0001 +		// +7.0625
	**	0001.1000 =		// +1.5
	**	-----------
	**	0111.1111		// +7.9375
	**
	**	The MUL macros handle the renormalisation and the saturation
	**	The rounded toward even version of the macro has PROS and CONS:
	**	PROS:
	**	-1 Order of Magnitude reduction for the mean error
	**	CONS:
	**	-2 Sligth increase of the rms error
	**	-3 Increased computational cost (1 additionale heavy brench)
	**
	**	ex.
	**	0000.1000 *		// +0.5
	**	0000.1000 *		// +0.5
	**	-----------
	**	0000.0100		// +0.25
	**
	**	ex.	same number with different point position yeld to different result
	**	00001.000 *		// +1.0
	**	00001.000 *		// +1.0
	**	-----------
	**	00001.000		// +1.0
	****************************************************************************/

		///********************************************************************************
		///	SATURATION HANDLING
		///********************************************************************************
		///	Handle up and low bound of variabile

	#define AT_SAT( x, up_bound, low_bound )	\
		( ((x)>(up_bound))?(up_bound):(((x)<(low_bound))?(low_bound):(x)) )


		///********************************************************************************
		///	TWO WAY SATURATED SUM
		///********************************************************************************
		///	AT_FP_SAT_SUM( x, y, up_bound, low_bound )
		///	x, y				: Fixed Point Operand
		///	up_bound, low_bound	: Bound for the operation (can be lower than the variabile range)

	//This macro handle the sum between two number handling the saturation
	//Algorithm:
	//	(x > 0) && (y >= up_bound - x) ?			//X > 0 and Y > positive overflow treshold
	//	Yes
	//		up_bound								//Positive Overflow
	//	No
	//		(x < 0) && (y <= low_bound - x) ?		//X < 0 and Y < negative overflow treshold
	//		Yes
	//			low_bound							//Negative Overflow
	//		No
	//			x + y								//No overflow
	#define AT_SAT_SUM( x, y, up_bound, low_bound )	\
		( ( (x) > 0 ) && ( (y) >= ((up_bound) - (x)) ) )? (up_bound) : ( ( ( (x) < 0 ) && ( (y) <= ((low_bound) - (x)) ) )? (low_bound) : ((x) + (y)) )

		///********************************************************************************
		///	THREE WAY SATURATED SUM
		///********************************************************************************
		///	AT_FP_SAT_SUM( x, y, z, up_bound, low_bound )
		///	x, y, z				: Fixed Point Operand
		///	up_bound, low_bound	: Bound for the operation (can be lower than the variabile range)

	//This macro solve an issue when i have to sum three element in a saturated way
	//The problem happen when the first partial result saturate (discarding something) and
	//the next operation would subtract from the first one
	//es
	//	a = 127, b = 60, c = -128: 		(expected result = 59)
	//	127 + 60 = 127, 127 - 128 = -1 	(wrong result = -1)
	//Algorithm:
	//	(a + b == up_bound AND c < 0 ) OR (a + b == low_bound AND c > 0 )
	//		YES
	//			(a + c) + b
	//		NO
	//			(a + b) + c
	#define AT_SAT_3SUM( x, y, z, up_bound, low_bound )	\
		( ( (AT_SAT_SUM( (x), (y), (up_bound), (low_bound) ) == (up_bound)) && ((z) < 0) ) || ( (AT_SAT_SUM( (x), (y), (up_bound), (low_bound) ) == (low_bound)) && ((z) > 0) ) )?AT_SAT_SUM( AT_SAT_SUM( (x), (z), (up_bound), (low_bound) ), (y), (up_bound), (low_bound) ):AT_SAT_SUM( AT_SAT_SUM( (x), (y), (up_bound), (low_bound) ), (z), (up_bound), (low_bound) )

		///********************************************************************************
		///	AT_TOP_INC
		///********************************************************************************
		//	Increment. If increment would exceed top -> reset
		//	Ex. top = 7
		//	cnt = 0, 1, 2, 3, 4, 5, 6, 7, 0, ...

	#define AT_TOP_INC( cnt, top )	\
		(((cnt) == (top))?(0):((cnt)+1))

		///********************************************************************************
		///	CIRCULAR SUM
		///********************************************************************************
		//	This function allow sum to work like an 8bit var would start again from 0 once a sum overflow
		//	sometime this is useful
		//	TODO: I can handle the case when i go around max multiple times

	#define AT_CIRCULAR_SUM( x, y, max )	\
		( ((x +y) >= (max))?(x +y -max):(x +y) )

		///********************************************************************************
		///	DIVISION HANDLING
		///********************************************************************************
		///	This macro handle division by power of 2, no rounding at all
		///	Algorithm:
		///		out = x / (1<<fp)

	#define AT_DIVIDE( x, fp )	\
		( (x) / (1 << (fp) ) )

	//sign correction for negative values
	#define AT_DIVIDE_NEG( x, fp )	\
		( (x >= 0)?AT_DIVIDE(x,fp):(-(AT_DIVIDE(-(x),fp))) )

		///********************************************************************************
		///	DIVISION HANDLING (ROUND TOWARD NEGATIVE)
		///********************************************************************************
		///	This macro handle division by power of 2, no rounding at all
		///	The 0.5 case is always rounded down
		///	Algorithm:
		///	>The first bit masked away is '1'?
		///	Y:
		///		out+1
		///	N:
		///		out


	#define AT_DIVIDE_RTN( x, fp )	\
		( ((x)&(MASK(fp-1)))?(((x)/MASK(fp))+1):((x)/MASK(fp)) )

		///********************************************************************************
		///	DIVISION HANDLING (ROUND TOWARD ODD) (POSITIVE ONLY)
		///********************************************************************************
		///	This macro handle division by power of 2, the result is renormalised and rounded
		///	toward the nearest even number, allow for minimal error if the data have non 0 mean value
		///	I round toward odd because is simpler to handle the rounding, i just have to OR the
		///	result with '1'
		///
		///	Algorithm:
		///		The first bit masked away is '1'?
		///		Y:
		///			The other masked bit are not exactly 0?
		///			Y:
		///				out+1
		///			N:
		///				out | 1
		///		N:
		///			out

	#define AT_DIVIDE_RTO( x, fp )	\
		( ((x)&(MASK(fp-1)))?(((x)&(MASK(fp-1)-1))?(((x)/MASK(fp))+1):(((x)/MASK(fp))|1)):((x)/MASK(fp)) )

		///********************************************************************************
		///	NEGATIVE DIVISION HANDLING (ROUND TOWARD ODD)
		///********************************************************************************
		///	Previous macro round in the wrong direction for negatives, i add a correction
		///	i simply do the rounding in positive and then get the negative

	#define AT_DIVIDE_NEG_RTO( x, fp )	\
		((x>=0)?(AT_DIVIDE_RTO(x,fp)):(-AT_DIVIDE_RTO(-x,fp)))

		///********************************************************************************
		///	GENERAL PORPOUSE SATURATED FIXED POINT MUL
		///********************************************************************************
		///	This macro handle the multiplication and renormalization between two number
		///	x, y				: Fixed Point Operand
		///	fp					: Fixed point position
		///	up_bound, low_bound	: Bound for the operation (can be lower than the variabile range)
		///	op_type				: Type of the operand
		///	mul_type			: Type of the mul result (es. if the operand is int16, this is int32
		///	mul mask			: Mask the useless bit of the result it es for int16 operand is 32 bit 0x0000ffff

	//	x, y	= Number
	//	fp = position of the point
	//	Algorithm:
	//	x * y  >= up bound * (1 << fp)?
	//	Yes
	//		up bound
	//	No
	//		x * y  <= low bound * (1 << fp)?
	//			Yes
	//				low_bound
	//			No
	//				x * y / (1 << fp)
	#define AT_FP_SAT_MUL( x, y, fp, up_bound, low_bound, op_type, mul_type, mul_mask )	\
		( ((mul_type)((x)*(y))>=(mul_type)((up_bound)*(1<<(fp))))?((op_type)(up_bound)):( ((mul_type)((x)*(y))<=(mul_type)((low_bound)*(1<<(fp))))?((op_type)(low_bound)):((op_type)((mul_type)(mul_mask)&(mul_type)((x)*(y)/(1<<(fp)))))) )

		///********************************************************************************
		///	SATURATED RENORMALISED FIXED POINT 8BIT MUL
		///********************************************************************************
		/// Wrapper for the 8 bit fixed point saturated multiplication

	//Setting the parameter for the 8 bit mul macro
	#define AT_FP_SAT_MUL8( x, y, fp )	\
		AT_FP_SAT_MUL( x, y, fp, MAX_INT8, MIN_INT8, int8_t, int16_t, 0x00ff )	\

		///********************************************************************************
		///	SATURATED, RENORMALISED FIXED POINT 16BIT MUL
		///********************************************************************************
		/// Wrapper for the 16 bit fixed point saturated multiplication

	//Setting the parameter for the 16 bit mul macro
	#define AT_FP_SAT_MUL16( x, y, fp )	\
		AT_FP_SAT_MUL( x, y, fp, MAX_INT16, MIN_INT16, int16_t, int32_t, 0x0000ffff )	\
		//( ((int32_t)((x)*(y))>=(int32_t)(MAX_INT16*(1<<(fp))))?((int16_t)MAX_INT16):( ((int32_t)((x)*(y))<=(int32_t)(MIN_INT16*(1<<(fp))))?((int16_t)MIN_INT16):((int16_t)((int32_t)0x0000ffff&(int32_t)((x)*(y)/(1<<(fp)))))) )

		///********************************************************************************
		///	SATURATED, ROUNDED TOWARD EVEN, RENORMALISED FIXED POINT 8BIT MUL
		///********************************************************************************
		///	This macros handle the multiplication and renormalization and the rounding toward even
		///	es.
		///	3.0 * 0.6 = 1.8 -> = 2.0	//resto > 0.5 => arrotondato per eccesso
		///	3.0 * 0.5 = 1.5 -> = 2.0	//resto = 0.5 => arrotondato al pari piu' vicino
		///	5.0 * 0.5 = 2.5 -> = 2.0	//resto = 0.5 => arrotondato al pari piu' vicino
		///	3.0 * 0.4 = 1.2 -> = 1.0	//resto < 0.5 => arrotondato per difetto
		///	x * y  >= up bound * (1 << fp)?			//Result Higher than the upper bound?
		///	Yes
		///		up bound							//Return Up bound
		///	No
		///		x * y <= low bound  * (1 << fp)?	//Result lower than Low bound?
		///		Yes
		///			low_bound						//Return Low Bound
		///		No
		///											//Result would be exactly x.5 and x is even OR result is lower than x.5
		///			YES
		///											//X
		///			NO
		///											//X +1

	#define SAT_ROUND_MUL8( x, y, fp )	\
		((int16_t)((x) * (y) / (1 << (fp))) >= (int16_t)MAX_INT8)?(int8_t)MAX_INT8:( ((int16_t)((x) * (y) / (1 << (fp))) <= (int16_t)MIN_INT8)?(int8_t)MIN_INT8:(((fp>0) && ((x*y)&(1<<(fp-1))))?(((x*y)>0)?((int8_t)( (int16_t)0x00ff & (int16_t)((x) * (y) / (1 << (fp))+1))):((int8_t)( (int16_t)0x00ff & (int16_t)((x) * (y) / (1 << (fp))-1)))):(int8_t)( (int16_t)0x00ff & (int16_t)((x) * (y) / (1 << (fp))))) )

		///********************************************************************************
		///	AT_MUL_ONE Multiply a number by a frection m/2^n Rounded Toward Odd
		///********************************************************************************
		///	This function execute a*m/2^n where m/2^n must be <1. It's a very common operation
		///	I start by subtracting (a) with its remainder (a/2^n)*2^n
		///	Then I multiply that by m, and divide by 2^n with the RTO function
		///	Then I sum the unrounded division. If I do this part first, i would need 2a maximum (bad)

	#define AT_MUL_ONE( a, m, n ) \
		( AT_DIVIDE_RTO( ((a-((a/MASK(n))*MASK(n)))*m) , n ) +((a/MASK(n))*m) )

    #define AT_MUL_NEG_ONE( a, m, n) \
		((a<0)?(-AT_MUL_ONE( (-(a)), m, n )):AT_MUL_ONE( a, m, n ))
#else
	#warning "multiple inclusion of at_utils.h"
#endif
