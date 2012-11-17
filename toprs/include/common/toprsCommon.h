#ifndef TOPRSCOMMONH
#define TOPRSCOMMONH

#include <cmath>
#include "common/toprsConstants.h"
class toprsIpt;

namespace toprs
{
	template <class T>
	inline bool almostEqual(T x, T y, T tolerence = FLT_EPSILON)
		// are x and y within tolerance distance of each other?
	{ return std::fabs(x - y) <= tolerence; }

	template <class T>
	inline bool inInterval(T x, T a, T b)
		// is x in the closed interval [a,b]?
	{ return x >= a && x <= b; }

	template <class T>
	inline bool inOpenInterval(T x, T a, T b)
		// is x in the open interval (a,b)?
	{ return x > a && x < b; }

	   /**
     * isnan Test for floating point Not A Number (NAN) value.
     * This should be used test for nan.
     * DO NOT USE operator==.  Like "if (height == toprs::nan())"
     *
     * @return true if nan, false if not.
     *
     * @see nan()
     */
#if defined(WIN32) || defined(_MSC_VER) && !defined(__CYGWIN__) && !defined(__MWERKS__)
    inline bool isnan(const float& v) { return _isnan(v); }
    inline bool isnan(const double& v) { return _isnan(v); }
#elif defined(sun) || defined(__sun)
#    if defined(__SVR4) || defined(__svr4__)
/* Solaris */
    inline bool isnan(const float& v)  { return ( ::isnan(v) ); }
    inline bool isnan(const double& v) { return ( ::isnan(v) ); }
#    else
/* SunOS */
    inline bool isnan(const float& v)  { return ( ::isnan(v) ); }
    inline bool isnan(const double& v) { return ( ::isnan(v) ); }
#   endif
#else
    inline bool isnan(const float& v)  { return ( std::isnan(v) ); }
    inline bool isnan(const double& v) { return ( std::isnan(v) ); }
#endif

	class TOPRSDLLEXPORT IntFloatBitCoercion
	{
	public:
		union
		{ 
			toprs_int64 intname;
			toprs_float64 floatname;
		} bits;
		IntFloatBitCoercion(toprs_int64 x)   { bits.intname = x;   }
		IntFloatBitCoercion(toprs_float64 x) { bits.floatname = x; }
	};
   extern TOPRSDLLDATA(const IntFloatBitCoercion) nanValue;
   inline double nan() { return nanValue.bits.floatname; }

	template <class T>
	inline T abs(const T& value)
	{
		if(value < 0)
		{
			return -value;
		}
		return value;
	}
			
	template <class T> 
	inline T clamp(T x, T a, T b) // clamp x to [a, b]
	{
		if (toprs::isnan(x)) return x;
		if (x < a) return a;
		if (b < x) return b;
		return x;
	}

	template <class T>
	T wrap(T x, T a, T b)	// wrap x modularly into [a,b)
	{

		if (a == b && !toprs::isnan(x))
			return a;
		else {
			T z = x < a ? b : a;
			return std::fmod(x - z, b - a) + z;
		}
	}

	template <class T>
	inline T min(T a, T b)
		// min of a,b; nan if either a or b are nan
	{
		return (a < b) ? a : b;
	}

	template <>
	inline toprs_float32 min<toprs_float32>(toprs_float32 a, toprs_float32 b)
	{
		if (toprs::isnan(a)||toprs::isnan(b))
		{
			return toprs::nan();
		}
		else
		{
			return (a < b) ? a : b;
		}
	}

	template <>
	inline toprs_float64  min<toprs_float64>(toprs_float64  a, toprs_float64 b)
	{
		if (toprs::isnan(a)||toprs::isnan(b))
		{
			return toprs::nan();
		}
		else
		{
			return (a < b) ? a : b;
		}
	}

	template <class T>
	inline T max(T a, T b)
		// max of a,b; nan if either a or b are nan
	{
		return (a < b) ? b : a;
	}
	template <>
	inline toprs_float32 max<toprs_float32>(toprs_float32 a, toprs_float32 b)
	{
		if (toprs::isnan(b))
			return b;
		else
			return (a < b) ? b : a;       
	}
	template <>
	inline toprs_float64 max<toprs_float64>(toprs_float64 a, toprs_float64 b)
	{
		if (toprs::isnan(b))
			return b;
		else
			return (a < b) ? b : a;       
	}

	inline double radiansToDegrees(double x) { return x*DEG_PER_RAD;}
	inline double degreesToRadians(double x) { return x*RAD_PER_DEG;}
	inline double cosd(double x)             { return std::cos(x*RAD_PER_DEG); }
	inline double sind(double x)             { return std::sin(x*RAD_PER_DEG); }
	inline double tand(double x)             { return std::tan(x*RAD_PER_DEG); }
	// trig fncs with parameter in degrees

	inline double acosd(double x)            { return DEG_PER_RAD*std::acos(x); }
	inline double asind(double x)            { return DEG_PER_RAD*std::asin(x); }
	inline double atand(double x)            { return DEG_PER_RAD*std::atan(x); }
	inline double atan2d(double y, double x) { return DEG_PER_RAD*std::atan2(y,x); }
	// trig fncs with result in degrees

	template<class T> 
	inline T square(T x) { return x*x; }

	// identical to copysign() but usable in templates
	template <class T>
	inline T sgn(T x) 
		// signum function, returns 0, 1, -1, or nan
	{
		const T table[] = {T(0), T(1), T(-1)};
		return table[((x < T(0)) << 1) | (x > T(0))];
	}
	template <>
	inline toprs_float32 sgn<toprs_float32>(toprs_float32 x) 
		// signum function, returns 0, 1, -1, or nan
	{
		const toprs_float32 table[] = {toprs_float32(0), toprs_float32(1), toprs_float32(-1)};
		return toprs::isnan(x) ? x : table[((x < toprs_float32(0)) << 1) | (x > toprs_float32(0))];
	}
	template <>
	inline toprs_float64 sgn(toprs_float64 x) // signum function, returns 0, 1, -1, or nan
	{
		const toprs_float64 table[] = {toprs_float64(0), toprs_float64(1), toprs_float64(-1)};
		return toprs::isnan(x) ? x : table[((x < toprs_float64(0)) << 1) | (x > toprs_float64(0))];
	}

	template <class R, class F>    
	inline R round(F x) // correctly round a float, and cast to desired type R
	{
		R result = static_cast<R>((x < F(0)) ? std::ceil(x - F(0.5)) : std::floor(x + F(0.5)));
		return result;
	}


	TOPRSDLLEXPORT void defaultTileSize(toprsIpt& tileSize);

	TOPRSDLLEXPORT double defaultMin(toprsDataType scalarType);
	TOPRSDLLEXPORT double defaultMax(toprsDataType scalarType);
	TOPRSDLLEXPORT double defaultNull(toprsDataType scalarType);
    TOPRSDLLEXPORT toprs_uint32 dataSizeInBytes(toprsDataType scalarType);
	template<class T>
	TOPRSDLLEXPORT auto defaultNull(toprsDataType scalarType, T dummy) -> decltype(dummy);

}



#endif