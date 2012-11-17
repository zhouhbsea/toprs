#include <iostream>
#include <sstream>

#include "base/toprsDRect.h"
#include "base/toprsIRect.h"


#define d_MAX(a,b)      (((a)>(b)) ? a : b)

static int
clip_1d (double *x0, 
	 double *y0, 
	 double *x1, 
	 double *y1, 
	 double maxdim)
{
   double m;			/* gradient of line */
   if (*x0 < 0)
   {				/* start of line is left of window */
      if (*x1 < 0)		/* as is the end, so the line never cuts the window */
         return 0;
      m = (*y1 - *y0) / (double) (*x1 - *x0);	/* calculate the slope of the line */
      /* adjust x0 to be on the left boundary (ie to be zero), and y0 to match */
      *y0 -= m * *x0;
      *x0 = 0;
      /* now, perhaps, adjust the far end of the line as well */
      if (*x1 > maxdim)
      {
         *y1 += m * (maxdim - *x1);
         *x1 = maxdim;
      }
      return 1;
   }
   if (*x0 > maxdim)
   {				/* start of line is right of window -
				   complement of above */
      if (*x1 > maxdim)		/* as is the end, so the line misses the window */
         return 0;
      m = (*y1 - *y0) / (double) (*x1 - *x0);	/* calculate the slope of the line */
      *y0 += m * (maxdim - *x0);	/* adjust so point is on the right
					   boundary */
      *x0 = maxdim;
      /* now, perhaps, adjust the end of the line */
      if (*x1 < 0)
      {
         *y1 -= m * *x1;
         *x1 = 0;
      }
      return 1;
   }
   /* the final case - the start of the line is inside the window */
   if (*x1 > maxdim)
   {				/* other end is outside to the right */
      m = (*y1 - *y0) / (double) (*x1 - *x0);	/* calculate the slope of the line */
      *y1 += m * (maxdim - *x1);
      *x1 = maxdim;
      return 1;
   }
   if (*x1 < 0)
   {				/* other end is outside to the left */
      m = (*y1 - *y0) / (double) (*x1 - *x0);	/* calculate the slope of the line */
      *y1 -= m * *x1;
      *x1 = 0;
      return 1;
   }
   /* only get here if both points are inside the window */
   return 1;
}

toprsDRect::toprsDRect(const toprsIRect& rect)
   :
      ul(rect.left_up()),
      ur(rect.right_up()),
      dr(rect.right_down()),
      dl(rect.left_down()),
      theOrientMode(rect.OrientMode())
{
   if(rect.isNAN())
   {
      makeNAN();
   }
}


toprsDRect::toprsDRect(const std::vector<toprsDpt>& points,
                       toprsCSMode mode)
   :
      theOrientMode (mode)
{
  if(points.size())
   {
      unsigned long index;
      double minx, miny;
      double maxx, maxy;
      
      minx = points[0].x;
      miny = points[0].y;
      maxx = points[0].x;
      maxy = points[0].y;
            
      // find the bounds
      for(index = 1; index < points.size();index++)
      {
         
         minx = std::min(minx, points[index].x);
         miny = std::min(miny, points[index].y);
         maxx = std::max(maxx, points[index].x);
         maxy = std::max(maxy, points[index].y);
         
      }
      if(theOrientMode == TOPRS_LEFT_HANDED)
      {
         *this = toprsDRect(minx, miny, maxx, maxy, mode);
      }
      else
      {
         *this = toprsDRect(minx,maxy, maxx, miny, mode);
      }
   }
   else
   {
      makeNAN();
   }
}
toprsDRect::toprsDRect(const toprsDpt& p1,
                       const toprsDpt& p2,
                       const toprsDpt& p3,
                       const toprsDpt& p4,
                       toprsCSMode mode)
: theOrientMode(mode)
{
   if(p1.hasNAN()||p2.hasNAN()||p3.hasNAN()||p4.hasNAN())
   {
      makeNAN();
   }
   else
   {
      double minx, miny;
      double maxx, maxy;
      
      minx = std::min( p1.x, std::min(p2.x, std::min(p3.x, p4.x)));
      miny = std::min( p1.y, std::min(p2.y, std::min(p3.y, p4.y)));
      maxx = std::max( p1.x, std::max(p2.x, std::max(p3.x, p4.x)));
      maxy = std::max( p1.y, std::max(p2.y, std::max(p3.y, p4.y)));
      
      if(mode == TOPRS_LEFT_HANDED)
      {
         *this = toprsDRect(minx, miny, maxx, maxy, mode);
      }
      else
      {            
         *this = toprsDRect(minx,maxy, maxx, miny, mode);
      }
   }
}



toprsDRect::toprsDRect(const toprsDpt& center, 
                       const double&   size_x, 
                       const double&   size_y,
                       toprsCSMode mode)
: theOrientMode(mode)
{
   double dx = fabs(size_x);
   double dy = fabs(size_y);

   double minx = center.x - dx/2.0;
   double maxx = minx + dx;

   double miny = center.y - dy/2.0;
   double maxy = miny + dy;

   if(mode == TOPRS_LEFT_HANDED)
      *this = toprsDRect(minx, miny, maxx, maxy, mode);
   else
      *this = toprsDRect(minx,maxy, maxx, miny, mode);
}



toprsDRect::~toprsDRect()
{
}

void toprsDRect::initBoundingRect(const std::vector<toprsDpt>& points)
{
   unsigned long index;

   // initialize everyone to the first point
   if(points.size() > 0)
   {
      ul.x = points[0].x;
      ul.y = points[0].y;
      dr.x = ul.x;
      dr.y = ul.y;
   }
   
   // find the bounds
   for(index = 1; index < points.size();index++)
   {
      // find left most
      if(points[index].x < ul.x)
         ul.x = points[index].x;

      // find right most
      else if(points[index].x > dr.x)
         dr.x = points[index].x;

      if (theOrientMode == TOPRS_LEFT_HANDED)
      {
         //find top most
         if(points[index].y < ul.y)
            ul.y = points[index].y;

         // find bottom most
         else if(points[index].y > dr.y)
            dr.y = points[index].y;
      }

      else // right handed coord system
      {
         if(points[index].y > ul.y)
            ul.y = points[index].y;

         // find bottom most
         else if(points[index].y < dr.y)
            dr.y = points[index].y;
      }
   }

   // now set the other points for the rect.
   ur.x = dr.x;
   ur.y = ul.y;
   dl.x = ul.x;
   dl.y = dr.y;
}


bool toprsDRect::intersects(const toprsDRect& rect) const
{
   if(rect.hasNAN() || hasNAN())
   {
      return false;
   }
   if (theOrientMode != rect.theOrientMode)
      return false;
   
   toprs_float64  ulx = toprs::max(rect.left_up().x,left_up().x);
   toprs_float64  lrx = toprs::min(rect.right_down().x,right_down().x);
   toprs_float64  uly, lry;
   bool rtn=false;
   if (theOrientMode == TOPRS_LEFT_HANDED)
   {
      uly  = toprs::max(rect.left_up().y,left_up().y);
      lry  = toprs::min(rect.right_down().y,right_down().y);
      rtn = ((ulx <= lrx) && (uly <= lry));
   }
   else
   {
      uly  = toprs::max(rect.left_down().y,left_down().y);
      lry  = toprs::min(rect.right_up().y,right_up().y);
      rtn = ((ulx <= lrx) && (uly <= lry));
   }
      
   return (rtn);
}


bool toprsDRect::completely_within(const toprsDRect& rect) const
{
   if(hasNAN() || rect.hasNAN())
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


void toprsDRect::stretchOut()
{
   set_ulx(floor(ul.x));
   set_drx(ceil(dr.x));

   if (theOrientMode == TOPRS_LEFT_HANDED)
   {
      set_uly(floor(ul.y));
      set_dry(ceil(dr.y));
   }
   else
   {
      set_uly(ceil(ul.y));
      set_dry(floor(dr.y));
   }
}

void toprsDRect::stretchToTileBoundary(const toprsDpt& widthHeight)
{
   toprsDpt ulp;
   toprsDpt drp;
   toprs_int32 evenDivision=0;

   if(theOrientMode == TOPRS_LEFT_HANDED)
   {
      ulp.x = ul.x;
      if( fmod(ul.x, widthHeight.x) != 0)
      {
         ulp.x = ((long)(ulp.x / widthHeight.x))*widthHeight.x;
         if(ulp.x > ul.x)
         {
            ulp.x -= widthHeight.x;
         }
      }
      ulp.y = ul.y;
      if( fmod(ul.y, widthHeight.y) != 0)
      {
         ulp.y = ((long)(ulp.y / widthHeight.y))*widthHeight.y;
         if(ulp.y > ul.y)
         {
            ulp.y -= widthHeight.y;
         }
      }
      
      evenDivision = fmod(dr.x, widthHeight.x) == 0;
      drp.x = dr.x;
      if(!evenDivision)
      {
         drp.x = ((long)((drp.x)/widthHeight.x)) * widthHeight.x;
         if(drp.x < dr.x)
         {
            drp.x += widthHeight.x;
         }
      }

      evenDivision = fmod(dr.y, widthHeight.y) == 0;
      drp.y = dr.y;
      if(!evenDivision)
      {
         drp.y = ((long)(drp.y/widthHeight.y)) * widthHeight.y;
         if(drp.y < dr.y)
         {
            drp.y += widthHeight.y;
         }
      }
   }
   else
   {
      ulp.x = ul.x;
      ulp.y = ul.y;
      if( !toprs::almostEqual(fmod(ul.x, widthHeight.x), 0.0))
      {
         ulp.x = ((long)(ulp.x/ widthHeight.x))*widthHeight.x;
         if(ulp.x > ul.x)
         {
            ulp.x -= widthHeight.x;
         }
      }
      if( !toprs::almostEqual((double)fmod(ul.y, widthHeight.y), 0.0) )
      {
         ulp.y = ((long)(ulp.y / widthHeight.y))*widthHeight.y;
         if(ulp.y < ul.y)
         {
            ulp.y += widthHeight.y;
         }
      }
      
      evenDivision = toprs::almostEqual( fmod(dr.x, widthHeight.x), 0.0);
      drp.x = dr.x;
      if(!evenDivision)
      {
         drp.x = ((long)(drp.x/widthHeight.x)) * widthHeight.x;
         if(drp.x < dr.x)
         {
            drp.x += widthHeight.x;
         }
      }

      evenDivision = toprs::almostEqual(fmod(dr.y, widthHeight.y), 0.0);
      drp.y = dr.y;
      if(!evenDivision)
      {
         drp.y = ((long)(drp.y/widthHeight.y)) * widthHeight.y;

         if(drp.y > dr.y)
         {
            drp.y -= widthHeight.y;
         }
      }
  }

   *this = toprsDRect(ulp, drp, theOrientMode);
}

const toprsDRect& toprsDRect::expand(const toprsDpt& padding)
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


bool toprsDRect::clip(toprsDpt &p1, toprsDpt &p2)const
{
   if(p1.isNAN() || p2.isNAN())
   {
      return false;
   }
   toprsDpt shift(-ul.x, -ul.y);

   toprsDpt tempShiftP1 = p1+shift;
   toprsDpt tempShiftP2 = p2+shift;
   double maxW = width()-1;
   double maxH = height()-1;
   if (clip_1d (&tempShiftP1.x, &tempShiftP1.y, 
                &tempShiftP2.x, &tempShiftP2.y, 
                maxW) == 0)
   {
      return false;
   }
   if(clip_1d (&tempShiftP1.y, 
               &tempShiftP1.x, 
               &tempShiftP2.y, 
               &tempShiftP2.x, maxH) == 0)
   {
      return false;
   }
   p1 = tempShiftP1-shift;
   p2 = tempShiftP2-shift;
   return true;
}


long toprsDRect::getCode(const toprsDpt& aPoint,
                         const toprsDRect& clipRect)
{
   long result=NONE; // initialize to inside rect
   
   if( (aPoint.x > clipRect.right_down().x) )
      result |= RIGHT;
   else if( (aPoint.x < clipRect.left_up().x) )
      result |= LEFT;

   if (clipRect.theOrientMode == TOPRS_LEFT_HANDED)
   {
      if( (aPoint.y < clipRect.left_up().y) )
         result |= TOP;
      else if( (aPoint.y > clipRect.right_down().y) )
         result |= BOTTOM;
   }
   else
   {
      if( (aPoint.y > clipRect.left_up().y) )
         result |= TOP;
      else if( (aPoint.y < clipRect.right_down().y) )
         result |= BOTTOM;
   }
      
   return result;
}


void toprsDRect::splitToQuad(toprsDRect& ulRect,
                             toprsDRect& urRect,
                             toprsDRect& lrRect,
                             toprsDRect& llRect)
{
   toprsDpt ulPt  = this->left_up();
   toprsDpt urPt  = this->right_up();
   toprsDpt lrPt  = this->right_down();
   toprsDpt llPt  = this->left_down();
   toprsIpt midPt = this->midPoint();
   
   ulRect = toprsDRect(ulPt.x,
                       ulPt.y,
                       midPt.x,
                       midPt.y,
                       theOrientMode);
   
   urRect = toprsDRect(midPt.x,
                       ulPt.y,
                       urPt.x,
                       midPt.y,
                       theOrientMode);
   
   if(theOrientMode  == TOPRS_LEFT_HANDED)
   {
      lrRect = toprsDRect(midPt.x,
                          midPt.y,
                          lrPt.x,
                          theOrientMode);
      llRect = toprsDRect(ulPt.x,
                          midPt.y,
                          midPt.x,
                          llPt.y,
                          theOrientMode);
   }
   else
   {       
      lrRect = toprsDRect(midPt.x,
                          midPt.y,
                          lrPt.x,
                          theOrientMode);
      llRect = toprsDRect(ulPt.x,
                          midPt.y,
                          midPt.x,
                          llPt.y,
                          theOrientMode);       
   }
   
}


toprsDRect toprsDRect::clipToRect(const toprsDRect& rect)const
{
   toprsDRect result;
   result.makeNAN();
   if(rect.hasNAN() || hasNAN())
   {

      return result;
   }
   
   if (theOrientMode != rect.theOrientMode)
      return (*this);

   double x0 = toprs::max(rect.left_up().x, left_up().x);
   double x1 = toprs::min(rect.right_down().x, right_down().x);
   double y0, y1;

   if (theOrientMode == TOPRS_LEFT_HANDED)
   {
      y0 = toprs::max(rect.left_down().y, left_down().y);
      y1 = toprs::min(rect.right_up().y, right_up().y);

      if( (x1 < x0) || (y1 < y0) )
         return result;
      else
         result = toprsDRect(x0, y0, x1, y1, theOrientMode);
   }
   else
   {
      y0 = toprs::max(rect.left_down().y,left_down().y);
      y1 = toprs::min(rect.right_up().y,right_up().y);
      if((x0 <= x1) && (y0 <= y1))
      {
         result = toprsDRect(x0, y1, x1, y0, theOrientMode);
      }
   }
   return result;
}

const toprsDRect& toprsDRect::operator=(const toprsIRect& rect)
{
   if(rect.isNAN())
   {
      makeNAN();
   }
   else
   {
      ul   = rect.left_up();
      ur   = rect.right_up();
      dr   = rect.right_down();
      dl   = rect.left_down();
      theOrientMode = rect.OrientMode();
   }
   
   return *this;
}


toprsDpt toprsDRect::findClosestEdgePointTo(const toprsDpt& arg_point) const
{
   double dXleft  = ul.x - arg_point.x;
   double dXright = dr.x - arg_point.x;
   double dYupper = ul.y - arg_point.y;
   double dYlower = dr.y - arg_point.y;

   toprsDpt edge_point (dr);

   if (dXleft*dXright < 0.0)
      edge_point.x = arg_point.x;
   else if (fabs(dXleft) < fabs(dXright))
      edge_point.x = ul.x;

   if (dYupper*dYlower < 0.0)
      edge_point.y = arg_point.y;
   else if (fabs(dYupper) < fabs(dYlower))
      edge_point.y = ul.y;

   return edge_point;
}

