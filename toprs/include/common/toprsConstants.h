#ifndef TOPRSCONSTANTS_H
#define TOPRSCONSTANTS_H



#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus   
#include <cfloat>
#else
#include <float.h>
#endif

#ifdef NULL
#undef NULL
#define NULL 0
#endif
/**
 * DLL IMPORT/EXORT SECTION
 */
#if defined(TOPRS_STATIC)
#  define TOPRSDLLEXPORT
#  define TOPRSDLLIMPORT
#  define TOPRSDLLDATA(T) T
#elif defined(__MINGW32__) || defined(__CYGWIN__) || defined(_MSC_VER) || defined(__VISUALC__) || defined(__BORLANDC__) || defined(__WATCOMC__)
#  define TOPRSDLLEXPORT __declspec(dllexport)
#  define TOPRSDLLIMPORT __declspec(dllimport)
#  define TOPRSDLLDATA(T) __declspec(dllexport) T
#else /* not #if defined(_MSC_VER) */
#  define TOPRSDLLEXPORT
#  define TOPRSDLLIMPORT
#  define TOPRSDLLDATA(T) T
#  endif /* #if defined(_MSC_VER) */



typedef char                   toprs_int8;
typedef unsigned char          toprs_uint8;
typedef signed char            toprs_sint8;

typedef short                  toprs_int16;
typedef unsigned short         toprs_uint16;
typedef signed short           toprs_sint16;

typedef int                    toprs_int32;
typedef unsigned int           toprs_uint32;
typedef signed int             toprs_sint32;

typedef float                  toprs_float32;
typedef double                 toprs_float64;

typedef long long              toprs_int64;
typedef unsigned long long     toprs_uint64;
typedef signed long long       toprs_sint64;

enum toprsCSMode
{
	TOPRS_LEFT_HANDED  = 0,
	TOPRS_RIGHT_HANDED = 1
};
enum toprsVertexOrdering
{
	TOPRS_VERTEX_ORDER_UNKNOWN    = 0,
	TOPRS_CLOCKWISE_ORDER         = 1,
	TOPRS_COUNTERCLOCKWISE_ORDER  = 2
};

enum toprsInterleaveType
{
	TOPRS_INTERLEAVE_UNKNOWN = 0,
	TOPRS_BIL                = 1,  /* band interleaved by line */
	TOPRS_BIP                = 2,  /* band interleaved by pixel */
	TOPRS_BSQ                = 3,  /* band sequential */
	TOPRS_BSQ_MULTI_FILE     = 4   /* band sequential in separate files */
};

enum toprsDataObjectStatus
{
	TOPRS_STATUS_UNKNOWN = 0,
	TOPRS_NULL           = 1, // not initialized
	TOPRS_EMPTY          = 2, // initialized but blank or empty
	TOPRS_PARTIAL        = 3, // contains some null/invalid values
	TOPRS_FULL           = 4  // all valid data
};
#ifndef M_PI
#  define M_PI             ((toprs_float64)3.141592653589793238462643)
#endif
#ifndef TWO_PI
#  define TWO_PI           ((toprs_float64)(2.0 * M_PI))
#endif
#ifndef DEG_PER_RAD
#  define DEG_PER_RAD      ((toprs_float64)(180.0/M_PI))
#endif
#ifndef SEC_PER_RAD
#  define SEC_PER_RAD      ((toprs_float64)206264.8062471)
#endif
#ifndef RAD_PER_DEG
#  define RAD_PER_DEG      ((toprs_float64)(M_PI/180.0))
#endif
#ifndef MTRS_PER_FT
#  define MTRS_PER_FT      ((toprs_float64)0.3048)
#endif
#ifndef FT_PER_MTRS
#  define FT_PER_MTRS      ((toprs_float64)3.280839895013122)
#endif
#ifndef FT_PER_MILE
#  define FT_PER_MILE      ((toprs_float64)5280.0)
#endif
#ifndef SQRT_TWO_PI
#  define SQRT_TWO_PI      ((toprs_float64)2.50662827463100050242)
#endif
#ifndef SQRT_TWO_PI_DIV2
#  define SQRT_TWO_PI_DIV2 ((toprs_float64)7.07106781186547524401E-1)
#endif


#define TOPRS_DEFAULT_TILE_HEIGHT  ((toprs_int32)64)
#define TOPRS_DEFAULT_TILE_WIDTH   ((toprs_int32)64)
enum toprsDataType
{
	TOPRS_DATA_UNKNOWN    =  0, 
	TOPRS_UINT8             =  1, /**< 8 bit unsigned integer        */
	TOPRS_SINT8             =  2, /**< 8 bit signed integer          */
	TOPRS_UINT16            =  3, /**< 16 bit unsigned integer       */
	TOPRS_SINT16            =  4, /**< 16 bit signed integer         */
	TOPRS_UINT32            =  5, /**< 32 bit unsigned integer       */
	TOPRS_SINT32            =  6, /**< 32 bit signed integer         */
	TOPRS_FLOAT32           =  7, /**< 32 bit floating point         */
	TOPRS_FLOAT64           =  8, /**< 64 bit floating point         */
	TOPRS_CINT16            =  9, /**< 16 bit complex integer        */
	TOPRS_CINT32            = 10, /**< 32 bit complex integer        */
	TOPRS_CFLOAT32          = 11, /**< 32 bit complex floating point */
	TOPRS_CFLOAT64          = 12, /**< 64 bit complex floating point */

	//---
	// Below for backward compatibility only.  Please use above enums in
	// conjunction with null, min, max settings to determine bit depth.
	//---
	TOPRS_UCHAR             = 1,  /**< 8 bit unsigned iteger */
	TOPRS_USHORT16          = 3,  /**< 16 bit unsigned iteger */
	TOPRS_SSHORT16          = 4,  /**< 16 bit signed integer */
	TOPRS_USHORT11          = 13, /**< 16 bit unsigned integer (11 bits used) */
	TOPRS_FLOAT             = 7,  /**< 32 bit floating point */
	TOPRS_NORMALIZED_FLOAT  = 18, /**< normalized floating point  32 bit */
	TOPRS_DOUBLE            = 8,  /**< double 64 bit    */
	TOPRS_NORMALIZED_DOUBLE = 20  /**< Normalized double 64 bit */
};
	
//---
// Integer nan kept for toprsIpt.
// This should be the most negative int: -2147483648
//---
#define TOPRS_INT_NAN ((toprs_sint32)0x80000000)

#define TOPRS_DEFAULT_NULL_PIX_UCHAR ((toprs_uint8)0)
#define TOPRS_DEFAULT_MIN_PIX_UCHAR ((toprs_uint8)1)
#define TOPRS_DEFAULT_MAX_PIX_UCHAR ((toprs_uint8)255)

#define TOPRS_DEFAULT_NULL_PIX_UINT8 ((toprs_uint8)0)
#define TOPRS_DEFAULT_MIN_PIX_UINT8 ((toprs_uint8)1)
#define TOPRS_DEFAULT_MAX_PIX_UINT8 ((toprs_uint8)255)

#define TOPRS_DEFAULT_NULL_PIX_SINT8 ((toprs_sint8)0x80)
#define TOPRS_DEFAULT_MIN_PIX_SINT8 ((toprs_sint8)0x81)
#define TOPRS_DEFAULT_MAX_PIX_SINT8 ((toprs_sint8)0x7F)

#define TOPRS_DEFAULT_NULL_PIX_SINT16 ((toprs_sint16)0x8000)
#define TOPRS_DEFAULT_MIN_PIX_SINT16 ((toprs_sint16)0x8001)
#define TOPRS_DEFAULT_MAX_PIX_SINT16 ((toprs_sint16)0x7FFF)

#define TOPRS_DEFAULT_NULL_PIX_UINT16 ((toprs_uint16)0)
#define TOPRS_DEFAULT_MIN_PIX_UINT16 ((toprs_uint16)1)
#define TOPRS_DEFAULT_MAX_PIX_UINT16 ((toprs_uint16)0xFFFF)

#define TOPRS_DEFAULT_NULL_PIX_SINT32 ((toprs_sint32)0x80000000)
#define TOPRS_DEFAULT_MIN_PIX_SINT32 ((toprs_sint32)0x80000001)
#define TOPRS_DEFAULT_MAX_PIX_SINT32 ((toprs_sint32)0x7FFFFFFF)

#define TOPRS_DEFAULT_NULL_PIX_UINT32 ((toprs_uint32)0)
#define TOPRS_DEFAULT_MIN_PIX_UINT32 ((toprs_uint32)1)
#define TOPRS_DEFAULT_MAX_PIX_UINT32 ((toprs_uint32)0xFFFFFFFF)

#define TOPRS_DEFAULT_NULL_PIX_UINT11 ((toprs_uint16)0)
#define TOPRS_DEFAULT_MIN_PIX_UINT11 ((toprs_uint16)1)
#define TOPRS_DEFAULT_MAX_PIX_UINT11 ((toprs_uint16)0x07FF)

#define TOPRS_DEFAULT_NULL_PIX_FLOAT    ((toprs_float32)-1.0/FLT_EPSILON)
#define TOPRS_DEFAULT_MIN_PIX_FLOAT ((toprs_float32)((-1.0/FLT_EPSILON) + 1))
#define TOPRS_DEFAULT_MAX_PIX_FLOAT  ((toprs_float32)((1.0/FLT_EPSILON)))

#define TOPRS_DEFAULT_NULL_PIX_NORM_FLOAT ((toprs_float32)0)
#define TOPRS_DEFAULT_MIN_PIX_NORM_FLOAT ((toprs_float32)((2*FLT_EPSILON)))
#define TOPRS_DEFAULT_MAX_PIX_NORM_FLOAT ((toprs_float32)1.0)

#define TOPRS_DEFAULT_NULL_PIX_DOUBLE ((toprs_float64)-1.0/DBL_EPSILON)
#define TOPRS_DEFAULT_MIN_PIX_DOUBLE ((toprs_float64)((-1.0/DBL_EPSILON) + 1))
#define TOPRS_DEFAULT_MAX_PIX_DOUBLE ((toprs_float64)((1.0/DBL_EPSILON)))

#define TOPRS_DEFAULT_NULL_PIX_NORM_DOUBLE ((toprs_float64)0)
#define TOPRS_DEFAULT_MIN_PIX_NORM_DOUBLE ((toprs_float64)((2*DBL_EPSILON)))
#define TOPRS_DEFAULT_MAX_PIX_NORM_DOUBLE ((toprs_float64)(1.0))

#define TOPRS_DEFAULT_MEAN_SEA_LEVEL ((toprs_float64)0.0)




#ifdef __cplusplus
}
#endif
#endif
