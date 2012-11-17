#ifndef TOPRSDPTH
#define TOPRSDPTH
#include <iosfwd>
#include <math.h>
#include <string>
#include "common/toprsConstants.h"
#include "common/toprsCommon.h"

class toprsIpt;

class TOPRSDLLEXPORT toprsDpt
{
public:
	union {double x; double samp; double u; double lon;};
	union {double y; double line; double v; double lat;};
public:
	toprsDpt() : x(0.0), y(0.0) {}
	toprsDpt(double _x, double _y) : x(_x), y(_y) {}
	toprsDpt(const toprsDpt& _pt) : x(_pt.x), y(_pt.y) {}
	toprsDpt(const toprsIpt& pt);


	const toprsDpt& operator=(const toprsDpt& pt);
	const toprsDpt& operator=(const toprsIpt& pt);

	bool operator==(const toprsDpt& pt) const
	{ return (toprs::almostEqual(x,pt.x) && toprs::almostEqual(y,pt.y)); } 

	bool operator!=(const toprsDpt& pt) const
	{ return ( !(*this == pt) ); }

	void makeNAN() {x = toprs::nan(); y = toprs::nan(); }  //  NAN stands for not a number
	bool hasNAN() const {return (toprs::isnan(x)) || (toprs::isnan(y));}
	bool isNAN() const  {return (toprs::isnan(x)) && (toprs::isnan(y));}


   std::string toString(int precision = 15) const;
   void toPoint(const std::string& s);
   
	std::ostream& print(std::ostream& os, int precision = 15) const;
	friend TOPRSDLLEXPORT std::ostream& operator<<(std::ostream& os,const toprsDpt& pt);
    /**
    * Method to input the formatted string of the "operator<<".
    *
    * Expected format:  ( 30.00000000000000, -90.0000000000 )
    *                     -x- -y-
    * 
    * This method starts by doing a "makeNAN" on pt.  So if anything goes
    * wrong with the stream or parsing pt could be all or partially NAN.
    *
    * @param is Input stream istream to formatted text.
    * @param pt osimIpt to be initialized from stream.
    * @return istream pass in.
    */
   friend TOPRSDLLEXPORT std::istream& operator>>(std::istream& is,toprsDpt& pt);

   bool isEqualto(const toprsDpt& rhs) const;
   double length() const { return std::sqrt((double)x*x+(double)y*y); }

    //toString(); toPoint()

   toprsDpt operator+(const toprsDpt& p) const
   { return toprsDpt(x+p.x, y+p.y); }
   toprsDpt operator-(const toprsDpt& p) const
   { return toprsDpt(x-p.x, y-p.y); }
   const toprsDpt& operator+=(const toprsDpt& p)
   { x += p.x; y += p.y; return *this; }
   const toprsDpt& operator-=(const toprsDpt& p)
   { x -= p.x; y -= p.y; return *this; }

   toprsDpt operator*(const double& d) const
   { return toprsDpt(d*x, d*y); }
   toprsDpt operator/(const double& d) const
   { return toprsDpt(x/d, y/d); }

};

inline const toprsDpt& toprsDpt::operator=(const toprsDpt& pt)
{
	if (this != &pt)
	{
		x = pt.x;
		y = pt.y;
	}

	return *this;
}

#endif