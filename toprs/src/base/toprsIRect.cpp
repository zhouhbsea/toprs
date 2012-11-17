#include <ostream>
#include <sstream>

#include "base/toprsIRect.h"
#include "base/toprsDRect.h"



toprsIRect::toprsIRect(const toprsDRect& rect)
	:
    ul(rect.left_up()),
	ur(rect.right_up()),
	dl(rect.left_down()),
	dr(rect.right_down()),
	theOrientMode(rect.orientMode())
{}

toprsIRect::toprsIRect(const std::vector<toprsIpt>& points,
                       toprsCSMode mode)
   : theOrientMode (mode)
{
   if(points.size())
   {
      toprs_uint32 index;
      toprs_int32  minx = points[0].x;
      toprs_int32  miny = points[0].y;
      toprs_int32  maxx = points[0].x;
      toprs_int32  maxy = points[0].y;
            
      // find the bounds
      for(index = 1; index < points.size();index++)
      {
         minx = toprs::min(minx, points[index].x);
         miny = toprs::min(miny, points[index].y);
         maxx = toprs::max(maxx, points[index].x);
         maxy = toprs::max(maxy, points[index].y);
         
      }
      if(theOrientMode == TOPRS_LEFT_HANDED)
      {
         *this = toprsIRect(minx, miny, maxx, maxy, mode);
      }
      else
      {
         *this = toprsIRect(minx,maxy, maxx, miny, mode);
      }
   }
   else
   {
      makeNAN();
   }
}

toprsIRect::toprsIRect(const toprsIpt& p1,
                       const toprsIpt& p2,
                       const toprsIpt& p3,
                       const toprsIpt& p4,
                       toprsCSMode mode)
   :theOrientMode(mode)
{
   if(p1.hasNAN()||p2.hasNAN()||p3.hasNAN()||p4.hasNAN())
   {
      makeNAN();
   }
   else
   {
      toprs_int32 minx, miny;
      toprs_int32 maxx, maxy;
      
      minx = toprs::min( p1.x, toprs::min(p2.x, toprs::min(p3.x, p4.x)));
      miny = toprs::min( p1.y, toprs::min(p2.y, toprs::min(p3.y, p4.y)));
      maxx = toprs::max( p1.x, toprs::max(p2.x, toprs::max(p3.x, p4.x)));
      maxy = toprs::max( p1.y, toprs::max(p2.y, toprs::max(p3.y, p4.y)));
      
      if(theOrientMode == TOPRS_LEFT_HANDED)
      {
         *this = toprsIRect(minx, miny, maxx, maxy, mode);
      }
      else
      {
         *this = toprsIRect(minx,maxy, maxx, miny, mode);
      }
   }
}


toprsIRect::toprsIRect(const toprsIpt& center, 
                       int    w, 
                       int    h,
                       toprsCSMode mode)
: theOrientMode (mode)
{
   toprs_int32 minx = center.x - w/2;
   toprs_int32 maxx = minx + w - 1;

   toprs_int32 miny = center.y - h/2;
   toprs_int32 maxy = miny + h - 1;

   if(mode == TOPRS_LEFT_HANDED)
      *this = toprsIRect(minx, miny, maxx, maxy, mode);
   else
      *this = toprsIRect(minx,maxy, maxx, miny, mode);
}



bool toprsIRect::insureMinimumSize(const toprsIpt& width_height)
{
   bool resized = false;

   int dx = width_height.x - width();
   int dy = width_height.y - height();

   if (dx > 0)
   {
      dx = (int) ceil((double)dx/2.0);
      ul.x -= dx;
      dr.x += dx;
      resized = true;
   }

   if (dy > 0)
   {
      resized = true;
      if(theOrientMode == TOPRS_LEFT_HANDED)
      {
         dy = (int) ceil((double)dy/2.0);
         ul.y -= dy;
         dr.y += dy;
      }
      else
      {
         dy = (int) ceil((double)dy/2.0);
         ul.y += dy;
         dr.y -= dy;
      }
   }

   if (resized)
      *this = toprsIRect(ul, dr, theOrientMode);

   return resized;
}


bool toprsIRect::intersects(const toprsIRect& rect) const
{
   if(rect.hasNAN() || hasNAN())
   {
      return false;
   }
   if (theOrientMode != rect.theOrientMode)
      return false;
   
   toprs_int32  ulx = toprs::max(rect.left_up().x,left_up().x);
   toprs_int32  drx = toprs::min(rect.right_down().x,right_down().x);
   toprs_int32  uly, dry;
   bool rtn=false;
   if (theOrientMode == TOPRS_LEFT_HANDED)
   {
      uly  = toprs::max(rect.left_up().y,left_up().y);
      dry  = toprs::min(rect.right_down().y,right_down().y);
      rtn = ((ulx <= drx) && (uly <= dry));
   }
   else
   {
      uly  = toprs::max(rect.left_down().y,left_down().y);
      dry  = toprs::min(rect.right_up().y,right_up().y);
      rtn = ((ulx <= drx) && (uly <= dry));
   }
   
   return (rtn);
}

void toprsIRect::stretchToTileBoundary(const toprsIpt& tileWidthHeight)
{
   toprsIpt ulp;
   toprsIpt drp;

   if(theOrientMode == TOPRS_LEFT_HANDED)
   {
      ulp.x = ul.x;
      if( (ul.x % tileWidthHeight.x) != 0)
      {
         ulp.x = ((ulp.x / tileWidthHeight.x))*tileWidthHeight.x;
         if(ulp.x > ul.x)
         {
            ulp.x -= tileWidthHeight.x;
         }
      }
      ulp.y = ul.y;
      if( (ul.y % tileWidthHeight.y) != 0)
      {
         ulp.y = ((ulp.y / tileWidthHeight.y))*tileWidthHeight.y;
         if(ulp.y > ul.y)
         {
            ul.y -= tileWidthHeight.y;
         }
      }
      toprs_int32 w = (dr.x - ulp.x) + 1;
      toprs_int32 h = (dr.y - ulp.y) + 1;
      
      toprs_int32 nw = (w / tileWidthHeight.x)*tileWidthHeight.x;
      toprs_int32 nh = (h / tileWidthHeight.y)*tileWidthHeight.y;
      
      if(w%tileWidthHeight.x)
      {
         nw += tileWidthHeight.x;
      }
      if(h%tileWidthHeight.y)
      {
         nh += tileWidthHeight.y;
      }
           
      drp.x = ulp.x + (nw-1);
      drp.y = ulp.y + (nh-1);
   }
   else
   {
      ulp.x = ul.x;
      ulp.y = ul.y;
      if( (ul.x%tileWidthHeight.x)!= 0)
      {
         ulp.x = ((ulp.x/ tileWidthHeight.x))*tileWidthHeight.x;
         if(ulp.x > ul.x)
         {
            ulp.x -= tileWidthHeight.x;
         }
      }
      if( (ul.y%tileWidthHeight.y)!=0 )
      {
         ulp.y = ((ulp.y / tileWidthHeight.y))*tileWidthHeight.y;
         if(ulp.y < ul.y)
         {
            ulp.y += tileWidthHeight.y;
         }
      }
      toprs_int32 w = dr.x - ulp.x;
      if (w < 0)
      {
         w = -w;
      }
      w += 1;
      toprs_int32 h = dr.y - ulp.y;
      if (h < 0)
      {
         h = -h;
      }
      h += 1;

      toprs_int32 nw = (w / tileWidthHeight.x)*tileWidthHeight.x;
      toprs_int32 nh = (h / tileWidthHeight.y)*tileWidthHeight.y;
      
      if(w%tileWidthHeight.x)
      {
         nw += tileWidthHeight.x;
      }
      if(h%tileWidthHeight.y)
      {
         nh += tileWidthHeight.y;
      }
           
      drp.x = ulp.x + (nw-1);
      drp.y = ulp.y - (nh-1);
  }

   *this = toprsIRect(ulp, drp, theOrientMode);
}


const toprsIRect& toprsIRect::expand(const toprsIpt& padding)
{
   ul.x -= padding.x;
   ur.x += padding.x;
   dr.x += padding.x;
   dl.x -= padding.x;
   if(theOrientMode == TOPRS_LEFT_HANDED)
   {
      ul.y -= padding.y;
      ur.y -= padding.y;
      dr.y += padding.y;
      dl.y += padding.y;
   }
   else
   {
      ul.y += padding.y;
      ur.y += padding.y;
      dr.y -= padding.y;
      dl.y -= padding.y;
   }
   
   return *this;
}

//toprsString toprsIRect::toString()const
//{
//   toprsString result="(";
//   
//   if(theOrientMode == TOPRS_LEFT_HANDED)
//   {
//      toprsIpt origin = ul();
//      result += (toprsString::toString(origin.x) + ",");
//      result += (toprsString::toString(origin.y) + ",");
//      result += (toprsString::toString(width()) + ",");
//      result += (toprsString::toString(height()) + ",");
//      result += "LH";
//   }
//   else 
//   {
//      toprsIpt origin = ll();
//      result += (toprsString::toString(origin.x) + ",");
//      result += (toprsString::toString(origin.y) + ",");
//      result += (toprsString::toString(width()) + ",");
//      result += (toprsString::toString(height()) + ",");
//      result += "RH";
//   }
//
//   result += ")";
//   return result;
//}
//
//bool toprsIRect::toRect(const toprsString& rectString)
//{
//   bool result = false;
//   makeNAN();
//   
//   std::istringstream in(rectString);
//   toprs::skipws(in);
//   char charString[2];
//   charString[1] = '\0';
//   toprsString interior;
//   if(in.peek() == '(')
//   {
//      in.ignore();
//      while((in.peek() != ')')&&
//            (in.peek() != '\n') &&
//            in.good())
//      {
//         charString[0] = in.get();
//         interior += charString;
//      }
//      if(in.peek() == ')')
//      {
//         result = true;
//      }
//   }
//   if(result)
//   {
//      std::vector<toprsString> splitArray;
//      interior.split(splitArray, ",");
//      
//      // assume left handed
//      if(splitArray.size() >= 4)
//      {
//         toprs_int64 x = splitArray[0].toInt64();
//         toprs_int64 y = splitArray[1].toInt64();
//         toprs_int64 w = splitArray[2].toInt64();
//         toprs_int64 h = splitArray[3].toInt64();
//         toprsString orientation = "lh";
//         if(splitArray.size() == 5)
//         {
//            orientation = splitArray[4].downcase();
//         }
//         if(orientation == "lh")
//         {
//            // origin upper left
//            *this = toprsIRect(x,y,x + (w-1), y+h-1, TOPRS_LEFT_HANDED);
//         }
//         else 
//         {
//            // origin lower left
//            *this = toprsIRect(x,y+(h-1),x + (w-1), y, toprs_RIGHT_HANDED);
//         }
//         
//      }
//      else
//      {
//         result = false;
//      }
//
//   }
//   return result;
//}

//*******************************************************************
// Public Method:
//*******************************************************************
bool toprsIRect::completely_within(const toprsIRect& rect) const
{
   if(rect.hasNAN() || hasNAN())
   {
      return false;
   }
   
   if (theOrientMode != rect.theOrientMode)
      return false;
   
   /*  --------------
       |     1      |
       | ---------- |
       | |        | |
       | |        | |
       | |   2    | |
       | |        | |
       | |        | |
       | ---------- |
       |            |
       --------------  */

   bool rtn = true;
   
   if (ul.x < rect.left_up().x)
      rtn = false;
   
   else if (dr.x > rect.right_down().x)
      rtn = false;
   
   else if (theOrientMode == TOPRS_LEFT_HANDED)
   {
      if (ul.y < rect.left_up().y)
         rtn = false;
   
      else if (dr.y > rect.right_down().y)
         rtn = false;
   }
   
   else
   {
      if (ul.y > rect.left_up().y)
         rtn = false;
   
      else if (dr.y < rect.right_down().y)
         rtn = false;
   }

   return rtn;
}


void toprsIRect::print(std::ostream& os) const
{
  // os << toString();
}

std::ostream& operator<<(std::ostream& os, const toprsIRect& rect)
{
   rect.print(os);

   return os;
}


#ifndef MAX
#  define MAX(x,y) ((x)>(y)?(x):(y))
#  define MIN(x,y) ((x)>(y)?(y):(x))
#endif

toprsIRect toprsIRect::clipToRect(const toprsIRect& rect)const
{   
   if (theOrientMode != rect.theOrientMode)
      return (*this);

   int x0 = MAX(rect.left_up().x, left_up().x);
   int x1 = MIN(rect.right_down().x, right_down().x);
   int y0, y1;

   if(!this->intersects(rect))
   {
      return toprsIRect(TOPRS_INT_NAN,
         TOPRS_INT_NAN,
         TOPRS_INT_NAN,
         TOPRS_INT_NAN);

   }
   if (theOrientMode == TOPRS_LEFT_HANDED)
   {
      y0 = MAX(rect.left_up().y, left_up().y);
      y1 = MIN(rect.right_down().y, right_down().y);

      if( (x1 < x0) || (y1 < y0) )
         return toprsIRect(toprsIpt(0,0), toprsIpt(0,0), theOrientMode);
      else
         return toprsIRect(x0, y0, x1, y1, theOrientMode);
   }
   else
   {
      y1 = MIN(rect.left_up().y,left_up().y);
      y0 = MAX(rect.right_down().y,right_down().y);

      if((x1 < x0) || (y1 < y0))
         return toprsIRect(toprsIpt(0,0), toprsIpt(0,0), theOrientMode);
      else
         return toprsIRect(x0, y1, x1, y0, theOrientMode);
   }
}


toprsIRect toprsIRect::combine(const toprsIRect& rect) const
{   
   // If any rect has NANs, it is assumed uninitialized, so assign the result to just the other
   if (hasNAN()) 
      return rect;
   if(rect.hasNAN())
      return *this;

   if (theOrientMode != rect.theOrientMode)
      return(*this);

   toprsIpt ulCombine;
   toprsIpt lrCombine;

   if(theOrientMode == TOPRS_LEFT_HANDED)
   {
      ulCombine.x = ((left_up().x <= rect.left_up().x)?left_up().x:rect.left_up().x);
      ulCombine.y = ((left_up().y <= rect.left_up().y)?left_up().y:rect.left_up().y);
      lrCombine.x = ((right_down().x >= rect.right_down().x)?right_down().x:rect.right_down().x);
      lrCombine.y = ((right_down().y >= rect.right_down().y)?right_down().y:rect.right_down().y);
   }
   else
   {
      ulCombine.x = ((left_up().x <= rect.left_up().x)?left_up().x:rect.left_up().x);
      ulCombine.y = ((left_up().y >= rect.left_up().y)?left_up().y:rect.left_up().y);
      lrCombine.x = ((right_down().x >= rect.right_down().x)?right_down().x:rect.right_down().x);
      lrCombine.y = ((right_down().y <= rect.right_down().y)?right_down().y:rect.right_down().y);
   }

   return toprsIRect(ulCombine, lrCombine, theOrientMode);
}


const toprsIRect& toprsIRect::operator=(const toprsDRect& rect)
{
   ul   = rect.left_up();
   ur   = rect.right_up();
   dr   = rect.right_down();
   dl   = rect.left_down();
   theOrientMode = rect.orientMode();
   
   return *this;
}


void toprsIRect::getCenter(toprsDpt& center_point) const
{
   if (hasNAN())
   {
      center_point.makeNAN();
      return;
   }

   double d = (ul.x + ur.x + dr.x + dl.x);
   center_point.x = d / 4.0;

   d = (ul.y + ur.y + dr.y + dl.y);
   center_point.y = d / 4.0;
}

