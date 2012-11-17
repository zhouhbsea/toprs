#ifndef TOPRSIPTH
#define TOPRSIPTH

#include <iosfwd>
#include <math.h>
#include <string>
#include "common/toprsConstants.h"
#include "common/toprsCommon.h"
#include "base/toprsDpt.h"


class TOPRSDLLEXPORT toprsIpt
{
public:
	union {toprs_int32 x; toprs_int32 u; toprs_int32 samp;};
	union {toprs_int32 y; toprs_int32 v; toprs_int32 line;};
public:
	toprsIpt() : x(0), y(0) {}
	toprsIpt(toprs_int32 _x, toprs_int32 _y) : x(_x), y(_y) {}
	toprsIpt(const toprsIpt& _pt) : x(_pt.x), y(_pt.y) {}
	toprsIpt(const toprsDpt& _pt);

	const toprsIpt& operator=(const toprsIpt& pt);
	const toprsIpt& operator=(const toprsDpt& pt);

	bool operator==(const toprsIpt& pt) const
	{ return ( (x == pt.x) && (y == pt.y) ); } 

	bool operator!=(const toprsIpt& pt) const
	{ return ( (x != pt.x) || (y != pt.y) ); }

	void makeNAN() {x = TOPRS_INT_NAN; y = TOPRS_INT_NAN; }  //  NAN stands for not a number
	bool hasNAN() const {return (x == TOPRS_INT_NAN) || (y == TOPRS_INT_NAN);}
	bool isNAN() const {return (x == TOPRS_INT_NAN) && (y == TOPRS_INT_NAN); }


    std::string toString() const;
    void toPoint(const std::string& s);
   
	std::ostream& print(std::ostream& os) const;
	friend TOPRSDLLEXPORT std::ostream& operator<<(std::ostream& os,const toprsIpt& pt);
    /**
    * Method to input the formatted string of the "operator<<".
    *
    * Expected format:  ( 30, -90 )
    *                     -x- -y-
    * 
    * This method starts by doing a "makeNAN" on pt.  So if anything goes
    * wrong with the stream or parsing pt could be all or partially NAN.
    *
    * @param is Input stream istream to formatted text.
    * @param pt osimIpt to be initialized from stream.
    * @return istream pass in.
    */
   friend TOPRSDLLEXPORT std::istream& operator>>(std::istream& is,toprsIpt& pt);

   bool isEqualto(const toprsIpt& rhs) const;

   double length() const { return std::sqrt((double)x*x+(double)y*y); }

    //toString(); toPoint()

   toprsIpt operator+(const toprsIpt& p) const
   { return toprsIpt(x+p.x, y+p.y); }
   toprsIpt operator-(const toprsIpt& p) const
   { return toprsIpt(x-p.x, y-p.y); }
   const toprsIpt& operator+=(const toprsIpt& p)
   { x += p.x; y += p.y; return *this; }
   const toprsIpt& operator-=(const toprsIpt& p)
   { x -= p.x; y -= p.y; return *this; }

   toprsDpt operator*(const double& d) const
   { return toprsDpt(d*x, d*y); }
   toprsDpt operator/(const double& d) const
   { return toprsDpt(x/d, y/d); }
};

inline const toprsIpt& toprsIpt::operator=(const toprsIpt& pt)
{
	if (this != &pt)
	{
		x = pt.x;
		y = pt.y;
	}
	return *this;
}

#endif