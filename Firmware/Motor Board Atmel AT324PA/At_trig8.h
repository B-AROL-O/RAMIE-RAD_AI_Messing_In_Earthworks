#ifndef AT_TRIG8
	#define AT_TRIG8

	/**********************************************************************************
	**	ENVIROMENT VARIABILE
	**********************************************************************************/

	//The LUT will be stored in the flash of the microcontroller instead of memory
	//It requires macros to access
	#define USE_PROGSPACE

	/**********************************************************************************
	**	GLOBAL INCLUDE
	**********************************************************************************/

	//If: I store LUTs in the flash
	#ifdef USE_PROGSPACE
		//runtime flash access library
		#include <avr/pgmspace.h>
	#endif

	/**********************************************************************************
	**	DEFINE
	**********************************************************************************/

		///--------------------------------------------------------------------------
		///	LUT DEFINITIONS
		///--------------------------------------------------------------------------

	//size of the arc8 LUT, used in arc8, sin, cos and their inverse
	#define LUT_ARC8_SIZE		63
	//Size of the tan8 LUT
	#define LUT_TAN8_SIZE		31

		///--------------------------------------------------------------------------
		///	PARAMETERS
		///--------------------------------------------------------------------------

		///aarc8 specific parameters
	//If the q of the aarc8 is bigger then this, will round down in the 0.5 case
	#define RD_ON_EQUAL_HALFWAY_QTH	93

		///--------------------------------------------------------------------------
		///	ANGLES
		///--------------------------------------------------------------------------

	//Error angle (special code) it would be -180°
	#define AT_ERR_ANGLE		-128
	//90° (pi/2)
	#define AT_PI2				64
	//45° (pi/4)
	#define AT_PI4				32

	/**********************************************************************************
	**	MACRO
	**********************************************************************************/

	//If: I store LUTs in the flash
	#ifdef USE_PROGSPACE
		//Get a data of the arc LUT from flash
		#define READ_LUT_ARC8( index )	\
			pgm_read_byte( &lut_arc8[ (index) ] )
		//Get a data of the tan LUT from flash
		#define READ_LUT_TAN8( index )	\
			pgm_read_byte( &lut_tan8[ (index) ] )

	//If: i store LUTs in the RAM
	#else
		//Get a data of the arc LUT from RAM
		#define READ_LUT_ARC8( index )	\
			lut_arc8[ (index) ]
		//Get a data of the tan LUT from RAM
		#define READ_LUT_TAN8( index )	\
			lut_tan8[ (index) ]
	#endif

	/**********************************************************************************
	**	TYPEDEF
	**********************************************************************************/

	//Describe a LUT
	typedef struct _At_LUT At_Lut;
	//i define the 2d cartesian vector type
	typedef struct _Vect8_xy Vect8_xy;
	//i define the 2d polar vector type
	typedef struct _Vect8_rt Vect8_rt;

	/**********************************************************************************
	**	PROTOTYPE: STRUCTURE
	**********************************************************************************/

	//Describe a LUT
	struct _At_LUT
	{
		//number of elements in the LUT
		uint8_t lut_size;

		//LUT payload
		const int8_t *data;
	};
	//i define the 2d cartesian vector type
	struct _Vect8_xy
	{
		//[-127:+127] -> [-1.0:+1.0]
		int8_t x;
		//[-127:+127] -> [-1.0:+1.0]
		int8_t y;
	};
	//i define the 2d polar vector type
	struct _Vect8_rt
	{
		//[0:+254] -> [0:+2.0]
		uint8_t r;
		//[-127:+127] -> (-PI:+PI) //128 = PI (180°) 127 = 178.594° 64 ° 90°
		int8_t t;
	};

	/**********************************************************************************
	**	PROTOTYPE: GLOBAL VARIABILE
	**********************************************************************************/

	/**********************************************************************************
	**	PROTOTYPE: FUNCTION
	**********************************************************************************/

		///--------------------------------------------------------------------------
		///	UTILITY FUNCTIONS
		///--------------------------------------------------------------------------

	//Generic LUT interpolation function
	int8_t at_lut8( uint8_t lut_size, const int8_t *lut, uint8_t zoom, uint8_t index );


	//Calculate the square root of a number. 127 = 1.0
	uint8_t at_sqrt8( uint8_t );

		///--------------------------------------------------------------------------
		///	DIRECT FUNCTIONS
		///--------------------------------------------------------------------------

	//Compute an arc of sin, cos and sin are just mirroring of this arc
	extern int8_t at_arc8( int8_t fi );
	//Compute the sin of an angle
	extern int8_t at_sin8( int8_t fi );
	//Compute the cos of an angle
	extern int8_t at_cos8( int8_t fi );
	//Compute the tangent of an angle fi and q obey to the same limitation. i use an alternate 32b lut
	//fi is limited from [-32:+32] [-45°:+45°]. the output is [-127:+127] [-1.0:+1.0]
	extern int8_t at_tan8(int8_t fi );

		///--------------------------------------------------------------------------
		///	INVERSE FUNCTIONS
		///--------------------------------------------------------------------------

	//compute the arc of the inverse sin and cos functions
	extern int8_t at_aarc8( int8_t q );
	//inverse sin function, the input range is -127:+127 that equal -1:+1
	extern int8_t at_asin8( int8_t q );
	//inverse cos function, the input range is -127:+127 that equal -1:+1
	extern int8_t at_acos8( int8_t q );
	//inverse tan function, input range: [-127:+127] [-1.0:+1.0]. Output range: [-32:+32] [-45°:+45°]
	extern int8_t at_atan8( int8_t q );
	//Calculate the angle of this vector, apply angle correction
	extern uint8_t at_atan8_2( int8_t x, int8_t y );

		///--------------------------------------------------------------------------
		///	VECTOR FUNCTIONS
		///--------------------------------------------------------------------------

	//Calculate the norm of a vector
	extern uint8_t at_norm8( Vect8_xy );
	//convert a rt vvector into a xy vector if possible (r<128) && (t!=-128)
	extern Vect8_xy at_vect8_rt2xy( Vect8_rt );
	//convert a xy vect into a rt vector if possible resulting t != -128
	extern Vect8_rt at_vect8_xy2rt( Vect8_xy );

#else
	#warning "multiple inclusion of the header file AT_TRIG8"
#endif


