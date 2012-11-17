#ifndef toprsIRectH
#define toprsIRectH

#include <iosfwd>
#include <vector>
#include "base/toprsIpt.h"
#include "base/toprsDpt.h"
#include "common/toprsCommon.h"

class toprsDRect;

class TOPRSDLLEXPORT toprsIRect
{
public:
	toprsIRect(): ul(0,0), ur(0,0), dl(0,0), dr(0,0), theOrientMode(TOPRS_LEFT_HANDED) {}
	toprsIRect(toprsIpt _ul, toprsIpt _dr, toprsCSMode mode = TOPRS_LEFT_HANDED)
		: ul(_ul),ur(_dr.x, _ul.y), dl(_ul.x, _dr.y), dr(_dr.x, _dr.y),theOrientMode(mode) {}
	toprsIRect(toprs_int32 ul_x, toprs_int32 ul_y, toprs_int32 dr_x, toprs_int32 dr_y, toprsCSMode mode = TOPRS_LEFT_HANDED)
		:ul(ul_x,ul_y), ur(dr_x, ul_y), dl(ul_x, dr_y), dr(dr_x, dr_y),theOrientMode(mode){}
	toprsIRect(const toprsIRect& rect)
		: ul(rect.left_up()), ur(rect.right_up()), dl(rect.left_down()), dr(rect.right_down()),theOrientMode(rect.OrientMode()) {}
	toprsIRect(const toprsDRect& rect);
	toprsIRect(const std::vector<toprsIpt>& pts, toprsCSMode mode = TOPRS_LEFT_HANDED);
	toprsIRect(const toprsIpt& p1, const toprsIpt& p2, const toprsIpt& p3, const toprsIpt& p4,
		       toprsCSMode mode = TOPRS_LEFT_HANDED);
	toprsIRect(const toprsIpt& center, int w, int h, toprsCSMode mode = TOPRS_LEFT_HANDED);

	~toprsIRect() {}

	const toprsIRect& operator=  (const toprsIRect& rect);
	const toprsIRect& operator=  (const toprsDRect& rect);
	bool         operator!= (const toprsIRect& rect) const;
	bool         operator== (const toprsIRect& rect) const;

	friend toprsIRect operator*(double scalar, const toprsIRect& rect)
	{
		return toprsIRect((int)floor(rect.left_up().x*scalar),
			(int)floor(rect.left_up().y*scalar),
			(int)ceil(rect.left_up().x*scalar+rect.width()*scalar-1),
			(int)ceil(rect.left_up().y*scalar+rect.height()*scalar-1),
			rect.theOrientMode);
	}

	const toprsIRect& operator*=(toprs_int32 scalar)
	{
		*this = toprsIRect((ul.x*scalar),
			(ul.y*scalar),
			(ul.x*scalar+width()*scalar-1),
			(ul.y*scalar+height()*scalar-1),
			theOrientMode);
		return *this;
	}

	toprsIRect operator*(toprs_int32 scalar)const
	{
		return toprsIRect((ul.x*scalar),
			(ul.y*scalar),
			(ul.x*scalar+width()*scalar-1),
			(ul.y*scalar+height()*scalar-1),
			theOrientMode);
	}

	const toprsIRect& operator *=(double scalar)
	{
		*this = toprsIRect((int)floor(ul.x*scalar),
			(int)floor(ul.y*scalar),
			(int)ceil(ul.x*scalar+width()*scalar-1),
			(int)ceil(ul.y*scalar+height()*scalar-1),
			theOrientMode);
		return *this;
	}

	toprsIRect operator *(double scalar)const
	{
		return toprsIRect((int)floor(ul.x*scalar),
			(int)floor(ul.y*scalar),
			(int)ceil(ul.x*scalar+width()*scalar-1),
			(int)ceil(ul.y*scalar+height()*scalar-1),
			theOrientMode);
	}

	const toprsIRect& operator *=(const toprsDpt& scalar)
	{
		*this = toprsIRect((int)floor(ul.x*scalar.x),
			(int)floor(ul.y*scalar.y),
			(int)ceil(ul.x*scalar.x+width()*scalar.x-1),
			(int)ceil(ul.y*scalar.y+height()*scalar.y-1),
			theOrientMode);
		return *this;
	}

	toprsIRect operator *(const toprsDpt& scalar)const
	{
		return  toprsIRect((int)floor(ul.x*scalar.x),
			(int)floor(ul.y*scalar.y),
			(int)ceil(ul.x*scalar.x+width()*scalar.x-1),
			(int)ceil(ul.y*scalar.y+height()*scalar.y-1),
			theOrientMode);
	}
	const toprsIRect& operator +=(const toprsIpt& shift)
	{
		*this = toprsIRect(ul.x+shift.x,
			ul.y+shift.y,
			dr.x+shift.x,
			dr.y+shift.y,
			theOrientMode);
		return *this;
	}

	const toprsIRect& operator -=(const toprsIpt& shift)
	{
		*this = toprsIRect(ul.x-shift.x,
			ul.y-shift.y,
			dr.x-shift.x,
			dr.y-shift.y,
			theOrientMode);
		return *this;
	}

	toprsIRect operator +(const toprsIpt& shift)const
	{
		return toprsIRect(ul.x+shift.x,
			ul.y+shift.y,
			dr.x+shift.x,
			dr.y+shift.y,
			theOrientMode);
	}

	toprsIRect operator -(const toprsIpt& shift)const
	{
		return toprsIRect(ul.x-shift.x,
			ul.y-shift.y,
			dr.x-shift.x,
			dr.y-shift.y,
			theOrientMode);
	}


	const toprsIRect& operator +=(const toprsDpt& shift)
	{
		*this = toprsIRect((int)floor(ul.x+shift.x),
			(int)floor(ul.y+shift.y),
			(int)ceil(ul.x+shift.x+width()-1),
			(int)ceil(ul.y+shift.y+height()-1),
			theOrientMode);
		return *this;
	}

	const toprsIRect& operator -=(const toprsDpt& shift)
	{
		*this = toprsIRect((int)floor(ul.x-shift.x),
			(int)floor(ul.y-shift.y),
			(int)ceil(ul.x-shift.x+width()-1),
			(int)ceil(ul.y-shift.y+height()-1),
			theOrientMode);
		return *this;
	}

	toprsIRect operator +(const toprsDpt& shift)const
	{
		return toprsIRect((int)floor(ul.x+shift.x),
			(int)floor(ul.y+shift.y),
			(int)ceil(ul.x+shift.x+width()-1),
			(int)ceil(ul.y+shift.y+height()-1),
			theOrientMode);
	}

	toprsIRect operator -(const toprsDpt& shift)const
	{
		return toprsIRect((int)floor(ul.x-shift.x),
			(int)floor(ul.y-shift.y),
			(int)ceil(ul.x-shift.x+width()-1),
			(int)ceil(ul.y-shift.y+height()-1),
			theOrientMode);
		return *this;
	}


	const toprsIpt& left_up() const { return ul; }
	const toprsIpt& right_up() const { return ur; }
	const toprsIpt& right_down() const { return dr; }
	const toprsIpt& left_down() const { return dl; }  

	const toprsIRect& changeOrientationMode(toprsCSMode mode)
	{
		// if we are already in the orientation then return
		//
		if(mode == theOrientMode) return *this;
		if(mode == TOPRS_LEFT_HANDED)
		{
			// we must be right handed so change to left handed
			*this = toprsIRect(ul.x,
				dl.y,
				dr.x,
				ul.y,
				TOPRS_LEFT_HANDED);
		}
		else
		{
			// we must be left handed so change to RIGHT handed
			*this = toprsIRect(ul.x,
				dl.y,
				dr.x,
				ul.y,
				TOPRS_RIGHT_HANDED);
		}
		theOrientMode = mode;
		return *this;
	}


	 void getBounds(toprs_int32& minx, toprs_int32& miny,
                  toprs_int32& maxx, toprs_int32& maxy)const
      {
         minx = ul.x;
         maxx = dr.x;
         miny = ul.y;
         maxy = dr.y;

		 /*      ^
          *   ---|---> right hand
		  *      |
		 */
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

   /*!
    * Initializes center_point with center of the rectangle.
    * Makes center_point nan if this rectangle has nans.
    */
   void getCenter(toprsDpt& center_point) const;
   
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
   
   bool isNAN()const{ return (ul.isNAN() &&
                              dl.isNAN() &&
                              dr.isNAN() &&
                              ur.isNAN());}



   void stretchToTileBoundary(const toprsIpt& tileWidthHeight);

   /**
    * @return toprsString representing toprsIRect.
    *
    * Format:  ( 30, -90, 512, 512, [LH|RH] )
    *            -x- -y-  -w-  -h-   -Right or left handed-
    *
    * where:
    *     x and y are origins either upper left if LEFT HANDED (LH) or
    *                                lower left if RIGHT HANDED (RH)
    *     w and h are width and height respectively
    *     The last value is LH or RH to indicate LeftHanded or RightHanded
    *    
    */
   //std::string toString()const;
   
   
   /**
    * expected Format:  form 1: ( 30, -90, 512, 512, [LH|RH] )
    *                            -x- -y-  -w-  -h-   -Right or left handed-
    * 
    *                   form 2: ( 30, -90, 512, 512)
    *                            -x- -y-  -w-  -h-
    *
    * NOTE: Form 2 assumes Left handed were x,y is origin upper left and y positive down.
    *
    * This method starts by doing a "makeNAN" on rect. 
    *
    * @param rectString String to initialize from.
    * @return true or false to indicate successful parsing.
    */
   //bool toRect(const std::string& rectString);
   

   const toprsIRect& expand(const toprsIpt& padding);

   //! Guarantees that this rect will be at least w X h big. If smaller than specified, the 
   //! corresponding side will be stretched equally in + and - direction to meet required size.
   //! Returns TRUE if resizing occurred.
   bool insureMinimumSize(const toprsIpt& width_height);

   toprs_uint32 area()const
      {
         return width()*height();
      }

   /*!
    * Sets the upper left corner to "pt".  Adjusts the remaining corners
    * accordingly.
    */
   void set_ul(const toprsIpt& pt);
   void set_ur(const toprsIpt& pt);
   void set_dr(const toprsIpt& pt);
   void set_dl(const toprsIpt& pt);
   void set_ulx(toprs_int32 x);
   void set_uly(toprs_int32 y);
   void set_urx(toprs_int32 x);
   void set_ury(toprs_int32 y);
   void set_drx(toprs_int32 x);
   void set_dry(toprs_int32 y);
   void set_dlx(toprs_int32 x);
   void set_dly(toprs_int32 y);

   /*!
    * Returns true if "pt" falls within rectangle.  Fall on an edge is also
    * considered to be within.
    */
   bool pointWithin(const toprsIpt& pt) const;

   /*!
    * Returns true if any portion of an input rectangle "rect" intersects
    * "this" rectangle.  
    */
   bool intersects(const toprsIRect& rect) const;

   /*!
    * Returns true if "this" rectangle is contained completely within
    * the input rectangular "rect".
    */
   bool completely_within(const toprsIRect& rect) const;


   /*!
    * Returns the height of the rectangle.
    */
   toprs_uint32 height() const
   {
      toprs_int32 h = dl.y - ul.y;
      if (h < 0)
      {
         h = -h;
      }
      return static_cast<toprs_uint32>( h + 1 );
   }

   /*!
    * Returns the width of a rectangle.
    */
   toprs_uint32 width()  const
   {
      toprs_int32 w = dr.x - dl.x;
      if (w < 0)
      {
         w = -w;
      }
      return static_cast<toprs_uint32>( w + 1 );
   }

   toprsIpt size() const { return toprsIpt(width(), height()); }
 
   toprsIRect clipToRect(const toprsIRect& rect)const;

   inline toprsIpt midPoint()const;

   void print(std::ostream& os) const;

   toprsIRect combine(const toprsIRect& rect)const;
   
   friend  TOPRSDLLEXPORT std::ostream& operator<<(std::ostream& os,
                                                  const toprsIRect& rect);
   const toprsCSMode OrientMode()const{return theOrientMode;}
 private:
	 toprsIpt ul, ur, dl, dr;
	 toprsCSMode theOrientMode;
};


inline const toprsIRect& toprsIRect::operator=(const toprsIRect& rect)
{
   if (this != &rect)
   {
      ul   = rect.left_up();
      ur   = rect.right_up();
      dr   = rect.right_down();
      dl   = rect.left_down();
      if(rect.hasNAN())
      {
         makeNAN();
      }
   }

   return *this;
}

inline bool toprsIRect::operator!=(const toprsIRect& rect) const
{
   return ( (ul   != rect.left_up()) ||
            (ur   != rect.right_up()) ||
            (dr   != rect.right_down()) ||
            (dl   != rect.left_down()) ||
            (theOrientMode != rect.theOrientMode));
}

inline bool toprsIRect::operator==(const toprsIRect& rect) const
{
   return ( (ul   == rect.left_up()) &&
            (ur   == rect.right_up()) &&
            (dr   == rect.right_down()) &&
            (dl   == rect.left_down()) &&
            (theOrientMode == rect.theOrientMode) );
}


inline void toprsIRect::set_ul(const toprsIpt& pt)
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

inline void toprsIRect::set_ur(const toprsIpt& pt)
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

inline void toprsIRect::set_dr(const toprsIpt& pt)
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

inline void toprsIRect::set_dl(const toprsIpt& pt)
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


inline void toprsIRect::set_ulx(toprs_int32 x)
{
   ul.x = x;
   dl.x = x;
}

inline void toprsIRect::set_uly(toprs_int32 y)
{
   ul.y = y;
   ur.y = y;
}

inline void toprsIRect::set_urx(toprs_int32 x)
{
   ur.x = x;
   dr.x = x;
}

inline void toprsIRect::set_ury(toprs_int32 y)
{
   ur.y = y;
   ul.y = y;
}

inline void toprsIRect::set_drx(toprs_int32 x)
{
   dr.x = x;
   ur.x = x;
}

inline void toprsIRect::set_dry(toprs_int32 y)
{
   dr.y = y;
   dl.y = y;
}

inline void toprsIRect::set_dlx(toprs_int32 x)
{
   dl.x = x;
   ul.x = x;
}

inline void toprsIRect::set_dly(toprs_int32 y)
{
   dl.y = y;
   dr.y = y;
}

inline bool toprsIRect::pointWithin(const toprsIpt& pt) const
{
   if(hasNAN())
   {
      return false;
   }
   if (theOrientMode == TOPRS_LEFT_HANDED)
      return ((pt.x >= left_up().x) &&
              (pt.x <= right_up().x) &&
              (pt.y >= left_up().y) &&
              (pt.y <= left_down().y));
   else
      return ((pt.x >= left_up().x) &&
              (pt.x <= right_up().x) &&
              (pt.y <= left_up().y) &&
              (pt.y >= left_down().y));
}

//*******************************************************************
// Inline Method: toprsIRect::midPoint
//*******************************************************************
inline toprsIpt toprsIRect::midPoint()const
{
   if(hasNAN())
   {
      return toprsIpt(TOPRS_INT_NAN, TOPRS_INT_NAN);
   }
   double x = (left_up().x + right_up().x + left_down().x + right_down().x) * 0.25;
   double y = (left_up().y + right_up().y + left_down().y + right_down().y) * 0.25;
   
   return toprsIpt(toprs::round<int>(x),
                   toprs::round<int>(y));
}

#endif
