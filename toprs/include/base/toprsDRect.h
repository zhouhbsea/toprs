#ifndef TOPRSDRECTH
#define TOPRSDRECTH
#include <vector>
#include "common/toprsConstants.h"
#include "base/toprsDpt.h"
class toprsIRect;
class toprsDRect
{
public:
	toprsDRect(): ul(0.0,0.0), ur(0.0,0.0), dl(0.0,0.0), dr(0.0,0.0), theOrientMode(TOPRS_LEFT_HANDED) {}
	toprsDRect(toprsDpt _ul, toprsDpt _dr, toprsCSMode mode = TOPRS_LEFT_HANDED)
		: ul(_ul),ur(_dr.x, _ul.y), dl(_ul.x, _dr.y), dr(_dr.x, _dr.y),theOrientMode(mode) {}
	toprsDRect(double ul_x, double ul_y, double dr_x, double dr_y, toprsCSMode mode = TOPRS_LEFT_HANDED)
		:ul(ul_x,ul_y), ur(dr_x, ul_y), dl(ul_x, dr_y), dr(dr_x, dr_y),theOrientMode(mode){}
	toprsDRect(const toprsDRect& rect)
		: ul(rect.left_up()), ur(rect.right_up()), dl(rect.left_down()), dr(rect.right_down()),theOrientMode(rect.orientMode()) {}
	toprsDRect(const toprsIRect& rect);
	toprsDRect(const std::vector<toprsDpt>& pts, toprsCSMode mode = TOPRS_LEFT_HANDED);
	toprsDRect(const toprsDpt& p1, const toprsDpt& p2, const toprsDpt& p3, const toprsDpt& p4,toprsCSMode mode = TOPRS_LEFT_HANDED);
	
	toprsDRect(const toprsDpt& center, const double& size_x, const double& size_y, toprsCSMode mode = TOPRS_LEFT_HANDED);



	~toprsDRect();

	const toprsDRect& operator=  (const toprsDRect& rect);
	const toprsDRect& operator=  (const toprsIRect& rect);
	bool         operator!= (const toprsDRect& rect) const;
	bool         operator== (const toprsDRect& rect) const;

	friend toprsDRect operator*(double scalar, const toprsDRect& rect)
	{
		toprsDpt ulp(rect.ul.x*scalar,
			rect.ul.y*scalar);

		if(rect.theOrientMode == TOPRS_LEFT_HANDED)
		{
			return toprsDRect(ulp.x,
				ulp.y,
				ulp.x+rect.width()*scalar-1,
				ulp.y+rect.height()*scalar-1,
				rect.theOrientMode);
		}
		return toprsDRect(ulp.x,
			ulp.y,
			ulp.x+rect.width()*scalar-1,
			ulp.y-(rect.height()*scalar-1),
			rect.theOrientMode);

	}

	const toprsDRect& operator *=(double scalar)
	{
		toprsDpt ulp(ul.x*scalar,
			ul.y*scalar);

		if(theOrientMode == TOPRS_LEFT_HANDED)
		{
			*this = toprsDRect(ulp.x,
				ulp.y,
				(ulp.x+width()*scalar-1),
				(ulp.y+height()*scalar-1),
				theOrientMode);
		}
		else
		{
			*this = toprsDRect(ulp.x,
				ulp.y,
				(ulp.x+width()*scalar-1),
				(ulp.y-(height()*scalar-1)),
				theOrientMode);

		}
		return *this;
	}

	toprsDRect operator *(double scalar)const
	{
		toprsDpt ulp((ul.x*scalar),
			(ul.y*scalar));

		if(theOrientMode == TOPRS_LEFT_HANDED)
		{
			return toprsDRect(ulp.x,
				ulp.y,
				(ulp.x+width()*scalar-1),
				(ulp.y+height()*scalar-1),
				theOrientMode);
		}

		return toprsDRect(ulp.x,
			ulp.y,
			(ulp.x+width()*scalar-1),
			ulp.y-(height()*scalar-1),
			theOrientMode);
	}
	const toprsDRect& operator *=(const toprsDpt& scalar)
	{
		toprsDpt ulp((ul.x*scalar.x),
			(ul.y*scalar.y));

		if(theOrientMode == TOPRS_LEFT_HANDED)
		{
			*this = toprsDRect(ulp.x,
				ulp.y,
				(ulp.x+width()*scalar.x - 1),
				(ulp.y+height()*scalar.y - 1),
				theOrientMode);
		}
		else
		{
			*this = toprsDRect(ulp.x,
				ulp.y,
				(ulp.x+width()*scalar.x - 1),
				(ulp.y-(height()*scalar.y - 1)),
				theOrientMode);            
		}
		return *this;
	}

	toprsDRect operator *(const toprsDpt& scalar)const
	{
		toprsDpt ulp((ul.x*scalar.x),
			(ul.y*scalar.y));

		if(theOrientMode == TOPRS_LEFT_HANDED)
		{
			return toprsDRect(ulp.x,
				ulp.y,
				(ulp.x+width()*scalar.x-1),
				(ulp.y+height()*scalar.y-1),
				theOrientMode);
		}
		return toprsDRect(ulp.x,
			ulp.y,
			(ulp.x+width()*scalar.x-1),
			(ulp.y-(height()*scalar.y-1)),
			theOrientMode);
	}
	const toprsDRect& operator +=(const toprsDpt& shift)
	{
		toprsDpt ulp((ul.x+shift.x),
			(ul.y+shift.y));

		if(theOrientMode == TOPRS_LEFT_HANDED)
		{
			*this = toprsDRect(ulp.x,
				ulp.y,
				ulp.x+width()-1,
				ulp.y+height()-1,
				theOrientMode);
		}
		else
		{
			*this = toprsDRect(ulp.x,
				ulp.y,
				ulp.x+width()-1,
				ulp.y-(height()-1),
				theOrientMode);            
		}
		return *this;
	}

	const toprsDRect& operator -=(const toprsDpt& shift)
	{
		toprsDpt ulp((ul.x-shift.x),
			(ul.y-shift.y));

		if(theOrientMode == TOPRS_LEFT_HANDED)
		{
			*this = toprsDRect(ulp.x,
				ulp.y,
				ulp.x+width()-1,
				ulp.y+height()-1,
				theOrientMode);
		}
		else
		{
			*this = toprsDRect(ulp.x,
				ulp.y,
				ulp.x+width()-1,
				ulp.y-(height()-1),
				theOrientMode);
		}
		return *this;
	}

	toprsDRect operator +(const toprsDpt& shift)const
	{
		toprsDpt ulp((ul.x+shift.x),
			(ul.y+shift.y));

		if(theOrientMode == TOPRS_LEFT_HANDED)
		{
			return toprsDRect(ulp.x,
				ulp.y,
				ulp.x+width()-1,
				ulp.y+height()-1,
				theOrientMode);
		}
		else
		{
			return toprsDRect(ulp.x,
				ulp.y,
				ulp.x+width()-1,
				ulp.y-(height()-1),
				theOrientMode);
		}
	}  

	toprsDRect operator -(const toprsDpt& shift)const
	{
		toprsDpt ulp(toprs::round<int>(ul.x-shift.x),
			toprs::round<int>(ul.y-shift.y));

		if(theOrientMode == TOPRS_LEFT_HANDED)
		{
			return toprsDRect(ulp.x,
				ulp.y,
				ulp.x+width()-1,
				ulp.y+height()-1,
				theOrientMode);
		}
		else
		{
			return toprsDRect(ulp.x,
				ulp.y,
				ulp.x+width()-1,
				ulp.y-(height()-1),
				theOrientMode);            
		}
	}  

	const toprsDpt& left_up() const { return ul; }
	const toprsDpt& right_up() const { return ur; }
	const toprsDpt& right_down() const { return dr; }
	const toprsDpt& left_down() const { return dl; }  

	const toprsDRect& changeOrientationMode(toprsCSMode mode)
	{
		// if we are already in the orientation then return
		//
		if(mode == theOrientMode) return *this;
		if(mode == TOPRS_LEFT_HANDED)
		{
			// we must be right handed so change to left handed
			*this = toprsDRect(ul.x,
				dl.y,
				dr.x,
				ul.y,
				TOPRS_LEFT_HANDED);
		}
		else
		{
			// we must be left handed so change to RIGHT handed
			*this = toprsDRect(ul.x,
				dl.y,
				dr.x,
				ul.y,
				TOPRS_RIGHT_HANDED);
		}
		theOrientMode = mode;

		return *this;
	}

	void getBounds(double& minx, double& miny,
		double& maxx, double& maxy)const
	{
		minx = ul.x;
		maxx = ur.x;
		if(theOrientMode == TOPRS_LEFT_HANDED)
		{
			miny = ul.y;
			maxy = dr.y;
		}
		else
		{
			maxy = ul.y;
			miny = dr.y;
		}
	}
	void makeNAN()
	{
		ul.makeNAN();
		dl.makeNAN();
		dr.makeNAN();
		ur.makeNAN();
	}

	bool hasNAN()const{ return (ul.hasNAN() ||
		dl.hasNAN() ||
		dr.hasNAN() ||
		ur.hasNAN());}

	bool isNAN()const{ return (ul.hasNAN() &&
		dl.hasNAN() &&
		dr.hasNAN() &&
		ur.hasNAN());}

	double height() const { return fabs(dl.y - ul.y) + 1.0; }
    double width()  const { return fabs(dr.x - dl.x) + 1.0; }

    toprsDpt size() const { return toprsDpt(width(), height()); }
	double area()const
	{
		return width()*height();
	}

	const toprsCSMode orientMode() const { return theOrientMode; }
	 toprsCSMode orientMode()  { return theOrientMode; }

	void setOrientMode(toprsCSMode mode) { theOrientMode = mode; }

	void set_ul(const toprsDpt& pt);
	void set_ur(const toprsDpt& pt);
	void set_dr(const toprsDpt& pt);
	void set_dl(const toprsDpt& pt);
	void set_ulx(double x);
	void set_uly(double y);
	void set_urx(double x);
	void set_ury(double y);
	void set_drx(double x);
	void set_dry(double y);
	void set_dlx(double x);
	void set_dly(double y);
	 void initBoundingRect(const std::vector<toprsDpt>& points);


   bool pointWithin(const toprsDpt& pt, double epsilon=0.0) const;

   bool intersects(const toprsDRect& rect) const;

   bool completely_within(const toprsDRect& rect) const;

   void stretchOut();//Stretches this rectangle out to integer boundaries.

   void stretchToTileBoundary(const toprsDpt& widthHeight);

   const toprsDRect& expand(const toprsDpt& padding);

   void splitToQuad(toprsDRect& ulRect,
	   toprsDRect& urRect,
	   toprsDRect& lrRect,
	   toprsDRect& llRect);

   toprsDpt findClosestEdgePointTo(const toprsDpt& arg_point) const;

   toprsDRect clipToRect(const toprsDRect& rect)const;

   inline toprsDpt midPoint()const;



   toprsDRect combine(const toprsDRect& rect)const;


   bool clip(toprsDpt &p1,  toprsDpt  &p2)const;

   static long getCode(const toprsDpt& aPoint, const toprsDRect& clipRect);


private:
	enum CohenSutherlandClipCodes
	{
		NONE   = 0,
		LEFT   = 1,
		RIGHT  = 2,
		BOTTOM = 4,
		TOP    = 8
	};
	toprsDpt ul, ur, dl, dr;
	toprsCSMode theOrientMode;
};

inline const toprsDRect& toprsDRect::operator=(const toprsDRect& rect)
{
   if (this != &rect)
   {
      ul   = rect.ul;
      ur   = rect.ur;
      dr   = rect.dr;
      dl   = rect.dl;
      theOrientMode = rect.theOrientMode;
   }

   return *this;
}

inline bool toprsDRect::operator!=(const toprsDRect& rect) const
{
   return ( (ul   != rect.ul) ||
            (ur   != rect.ur) ||
            (dr   != rect.dr) ||
            (dl   != rect.dl) ||
            (theOrientMode != rect.theOrientMode));
}

inline bool toprsDRect::operator==(const toprsDRect& rect) const
{
   return ( (ul   == rect.ul) &&
            (ur   == rect.ur) &&
            (dr   == rect.dr) &&
            (dl   == rect.dl) &&
            (theOrientMode == rect.theOrientMode));
}

inline void toprsDRect::set_ul(const toprsDpt& pt)
{
	if(pt.hasNAN())
	{
		makeNAN();
	}
	else
	{
		ul   = pt;
		ur.y = pt.y;
		dl.x = pt.x;
	}
}

inline void toprsDRect::set_ur(const toprsDpt& pt)
{
	if(pt.hasNAN())
	{
		makeNAN();
	}
	else
	{
		ur   = pt;
		ul.y = pt.y;
		dr.x = pt.x;
	}
}

inline void toprsDRect::set_dr(const toprsDpt& pt)
{
	if(pt.hasNAN())
	{
		makeNAN();
	}
	else
	{
		dr   = pt;
		ur.x = pt.x;
		dl.y = pt.y;
	}
}

inline void toprsDRect::set_dl(const toprsDpt& pt)
{
	if(pt.hasNAN())
	{
		makeNAN();
	}
	else
	{
		dl   = pt;
		ul.x = pt.x;
		dr.y = pt.y;
	}
}


inline void toprsDRect::set_ulx(double x)
{
	ul.x = x;
	dl.x = x;
}

inline void toprsDRect::set_uly(double y)
{
	ul.y = y;
	ur.y = y;
}

inline void toprsDRect::set_urx(double x)
{
	ur.x = x;
	dr.x = x;
}

inline void toprsDRect::set_ury(double y)
{
	ur.y = y;
	ul.y = y;
}

inline void toprsDRect::set_drx(double x)
{
	dr.x = x;
	ur.x = x;
}

inline void toprsDRect::set_dry(double y)
{
	dr.y = y;
	dl.y = y;
}

inline void toprsDRect::set_dlx(double x)
{
	dl.x = x;
	ul.x = x;
}

inline void toprsDRect::set_dly(double y)
{
	dl.y = y;
	dr.y = y;
}

inline toprsDpt toprsDRect::midPoint()const
{
	double x = (left_up().x + right_up().x + left_down().x + right_down().x) * 0.25;
	double y = (left_up().y + right_up().y + left_down().y + right_down().y) * 0.25;
	return toprsDpt(x, y);
}
#endif