static const char* NUMBER_VERTICES_KW = "number_vertices";
static const char* VERTEX_ORDER_KW    = "order";

static const int RECT_LEFT_EDGE   = 0;
static const int RECT_TOP_EDGE    = 1;
static const int RECT_RIGHT_EDGE  = 2;
static const int RECT_BOTTOM_EDGE = 3;

toprsPolygon::toprsPolygon()
   : theOrderingType(TOPRS_VERTEX_ORDER_UNKNOWN),
    theVertexList(),
    theCurrentVertex(0)
   
{}

toprsPolygon::toprsPolygon(const vector<toprsIpt>& polygon)
   :theOrderingType(TOPRS_VERTEX_ORDER_UNKNOWN),
   theVertexList(polygon.size()),
   theCurrentVertex(0)
   
{
   // Assign std::vector<toprsIpt> list to std::vector<toprsDpt> theVertexList.
   for (std::vector<toprsIpt>::size_type i = 0; i < polygon.size(); ++i)
   {
      theVertexList[i] = polygon[i];
   }
}

toprsPolygon::toprsPolygon(const vector<toprsDpt>& polygon)
   :theOrderingType(TOPRS_VERTEX_ORDER_UNKNOWN),
   theVertexList(polygon),
    theCurrentVertex(0)
{
}

//*****************************************************************************
//  CONSTRUCTOR: toprsPolygon(int numVertices, const toprsDpt* vertex_array)
//  
//*****************************************************************************
toprsPolygon::toprsPolygon(int numVertices, const toprsDpt* v)
   : theOrderingType(TOPRS_VERTEX_ORDER_UNKNOWN),
     theCurrentVertex(0)
     
{
   theVertexList.insert(theVertexList.begin(),
                        v, v+numVertices);
}

//*****************************************************************************
//  COPY CONSTRUCTOR: toprsPolygon(toprsPolygon)
//  
//*****************************************************************************
toprsPolygon::toprsPolygon(const toprsPolygon& polygon)
   :theCurrentVertex(0)
{
   *this = polygon;
}

//*****************************************************************************
//  CONSTRUCTOR: toprsPolygon(p1, p2, p3, p4)
//  
//  Provided for convenience. Does not imply the polygon is limited to four
//  vertices
//  
//*****************************************************************************
toprsPolygon::toprsPolygon(toprsDpt v1,
                           toprsDpt v2,
                           toprsDpt v3,
                           toprsDpt v4)
   : theOrderingType(TOPRS_VERTEX_ORDER_UNKNOWN),
     theVertexList(4),
     theCurrentVertex(0)
     
{
   theVertexList[0] = v1;
   theVertexList[1] = v2;
   theVertexList[2] = v3;
   theVertexList[3] = v4;
}

toprsPolygon::toprsPolygon(const toprsIRect& rect)
: theOrderingType(TOPRS_CLOCKWISE_ORDER),
  theVertexList(4),
  theCurrentVertex(0)
{
   theVertexList[0] = rect.ul();
   theVertexList[1] = rect.ur();
   theVertexList[2] = rect.lr();
   theVertexList[3] = rect.ll();
}

toprsPolygon::toprsPolygon(const toprsDRect& rect)
: theOrderingType(TOPRS_CLOCKWISE_ORDER),
theVertexList(4),
theCurrentVertex(0)
{
   theVertexList[0] = rect.ul();
   theVertexList[1] = rect.ur();
   theVertexList[2] = rect.lr();
   theVertexList[3] = rect.ll();
}


//*****************************************************************************
//  DESTRUCTOR: ~toprsPolygon
//  
//*****************************************************************************
toprsPolygon::~toprsPolygon()
{
}


//*************************************************************************************************
//! Returns polygon area. Negative indicates CW ordering of vertices (in right-handed coordinates
//*************************************************************************************************
double toprsPolygon::area()const
{
   double area = 0;
   toprs_uint32 i=0;
   toprs_uint32 j=0;
   toprs_uint32 size = (toprs_uint32)theVertexList.size();
   
   for (i=0;i<size;i++)
   {
      j = (i + 1) % (int)size;
      area += theVertexList[i].x * theVertexList[j].y;
      area -= theVertexList[i].y * theVertexList[j].x;
   }

   area /= 2;

   return area;
}

void toprsPolygon::roundToIntegerBounds(bool compress)
{
   int i = 0;
   for(i = 0; i < (int)theVertexList.size(); ++i)
   {
      theVertexList[i] = toprsIpt(theVertexList[i]);
   }

  if(compress&&theVertexList.size())
   {
      vector<toprsDpt> polyLine;
      
      polyLine.push_back(theVertexList[0]);
      toprsDpt testPt = theVertexList[0];
      for(i=1; i < (int)theVertexList.size(); ++i)
      {
         if(testPt!=theVertexList[i])
         {
            testPt = theVertexList[i];
            polyLine.push_back(testPt);
         }
      }
      if(polyLine.size() == 1)
      {
         polyLine.push_back(polyLine[0]);
      }
      
      if(theVertexList.size() == 1)
      {
         polyLine.push_back(testPt);
      }
      theVertexList    = polyLine;
      theCurrentVertex = 0;
   }
}

toprsDpt toprsPolygon::midPoint()const
{
   int upper = (int)theVertexList.size();
   toprsDpt result(0.0, 0.0);
   int i = 0;

   if(!upper)
   {
      result.makeNan();
   }
   else
   {
      for(i = 0; i < upper; ++i)
      {
         result.x+=theVertexList[i].x;
         result.y+=theVertexList[i].y;
      }
      result.x/=(double)upper;
      result.y/=(double)upper;
   }
   
   return result;
}

bool toprsPolygon::hasNans()const
{
   int upper = (int)theVertexList.size();
   int i = 0;

   for(i = 0; i < upper; ++i)
   {
      if(theVertexList[i].hasNans())
      {
         return true;
      }
   }

   return false;
}

void toprsPolygon::getIntegerBounds(toprs_int32& minX,
   toprs_int32& minY,
   toprs_int32& maxX,
   toprs_int32& maxY)const
{
   toprs_int32 npoly = (toprs_int32)theVertexList.size();
   int i = 0;

   if(npoly)
   {
      minX = (toprs_int32)floor(theVertexList[0].x);
      maxX = (toprs_int32)ceil(theVertexList[0].x);
      minY = (toprs_int32)floor(theVertexList[0].y);
      maxY = (toprs_int32)ceil(theVertexList[0].y);

      for(i =1; i < npoly; ++i)
      {
         minX = std::min((toprs_int32)floor(theVertexList[i].x),
            (toprs_int32)minX);
         maxX = std::max((toprs_int32)ceil(theVertexList[i].x),
            (toprs_int32)maxX);
         minY = std::min((toprs_int32)floor(theVertexList[i].y),
            (toprs_int32)minY);
         maxY = std::max((toprs_int32)ceil(theVertexList[i].y),
            (toprs_int32)maxY);
      }
   }
   else
   {
      minX = TOPRS_INT_NAN;
      minY = TOPRS_INT_NAN;
      maxX = TOPRS_INT_NAN;
      maxY = TOPRS_INT_NAN;
   }
}

void toprsPolygon::getFloatBounds(toprs_float64& minX,
                                  toprs_float64& minY,
                                  toprs_float64& maxX,
                                  toprs_float64& maxY) const
{
   toprs_int32 npoly = (toprs_int32)theVertexList.size();
   int i = 0;

   if(npoly)
   {
      minX = floor(theVertexList[0].x);
      maxX = ceil(theVertexList[0].x);
      minY = floor(theVertexList[0].y);
      maxY = ceil(theVertexList[0].y);

      for(i =1; i < npoly; ++i)
      {
         minX = std::min<double>(floor(theVertexList[i].x), minX);
         maxX = std::max<double>(ceil(theVertexList[i].x),  maxX);
         minY = std::min<double>(floor(theVertexList[i].y), minY);
         maxY = std::max<double>(ceil(theVertexList[i].y),  maxY);
      }
   }
   else
   {
      minX = toprs::nan();
      minY = toprs::nan();
      maxX = toprs::nan();
      maxY = toprs::nan();
   }
}

bool toprsPolygon::clipToRect(vector<toprsPolygon>& result,
                              const toprsDRect& rect)const
{
   result.clear();
   toprsPolyArea2d p1(*this);
   toprsPolyArea2d p2(rect.ul(), rect.ur(), rect.lr(), rect.ll());
   
   p1&=p2;

   p1.getVisiblePolygons(result);

   return (result.size() > 0);
}   

//*****************************************************************************
//  METHOD: toprsPolygon::clipLineSegment(p1, p2)
//  
//  Implements Cyrus-Beck clipping algorithm as described in:
//  http://www.daimi.au.dk/~mbl/cgcourse/wiki/cyrus-beck_line-clipping_.html
//
//  Clips the line segment defined by the two endpoints provided. The
//  endpoints are modified as needed to represent the clipped line. Returns
//  true if intersection present.
//  
//*****************************************************************************
bool toprsPolygon::clipLineSegment(toprsDpt& P, toprsDpt& Q) const
{
   toprsDpt PQ (Q - P);
   double tE = 0.0;
   double tL = 1.0;
   toprsLine edge, edgeE, edgeL;
   bool intersected=false;
   double num, denom, t;
   toprs_uint32 npol = (toprs_uint32)theVertexList.size();

   checkOrdering();
   //***
   // clip the segment against each edge of the polygon
   //***
   toprs_uint32 i = 0;
   toprs_uint32 j = 0;
   for(i = 0, j = 1; i < npol;)
   {
      edge = toprsLine(theVertexList[i],
                       theVertexList[j]);
      
      toprsDpt normal = edge.normal();

      // Fix from CChuah@observera.com for counter clockwise polygons. (drb)
      if (theOrderingType == TOPRS_COUNTERCLOCKWISE_ORDER)
      {
         normal.x = -normal.x;
         normal.y = -normal.y;
      }
      
      denom = normal.x*PQ.x + normal.y*PQ.y;
      
      num = normal.x*(edge.theP1.x - P.x) + normal.y*(edge.theP1.y - P.y);
      
      if (denom < 0)
      {
         //***
         // Appears to be entering:
         //***
         t = num / denom;
         if (t > tE)
         {
            tE = t; //+ FLT_EPSILON;
            edgeE = edge;
         }
      }
      else if (denom > 0)
      {
         //***
         // Appears to be leaving:
         //***
         t = num / denom;
         if (t < tL)
         {
            tL = t;// - FLT_EPSILON;
            edgeL = edge;
         }
      }

      ++i;
      ++j;
      j%=npol;
   } 
   
   //***
   // Compute clipped end points:
   //***
   if(tL >= tE)
   {
       Q.x = P.x + tL*PQ.x;
       Q.y = P.y + tL*PQ.y;
       P.x += tE*PQ.x;
       P.y += tE*PQ.y;
       intersected = true;
   }
   
   return intersected;
}

/**
* METHOD: isRectWithin()
* Returns true if all the corner points of the given rect fit within.
*/
bool toprsPolygon::isRectWithin(const toprsIRect &rect) const 
{
    if(isPointWithin(rect.ul()) &&
       isPointWithin(rect.ur()) &&
       isPointWithin(rect.ll()) &&
       isPointWithin(rect.lr())) {
       	return true;
    }
    return false;
}
/**
* METHOD: isPolyWithin()
* Returns true if all the vertices of the given polygon fit within.
*/
bool toprsPolygon::isPolyWithin(const toprsPolygon &poly) const 
{
   bool ret=false;
   int numvertex=poly.getNumberOfVertices();
   if(getNumberOfVertices()>1 && numvertex) {
      ret=true;
      for(int v=0;v<numvertex;v++) {
         if(!isPointWithin(poly[v])) {
            ret=false;
            break;
         }
      }
   }
   return ret;
}

//*****************************************************************************
//  METHOD: toprsPolygon::pointWithin(const toprsDpt& point)
//  
//  Returns TRUE if point is inside polygon.
//  
//*****************************************************************************
bool toprsPolygon::isPointWithin(const toprsDpt& point) const
{

   int i, j, c = 0;
   int npol = (int)theVertexList.size();

   for (i = 0, j = npol-1; i < npol; j = i++)
   {
      if ((((theVertexList[i].y <= point.y) && (point.y < theVertexList[j].y)) ||
           ((theVertexList[j].y <= point.y) && (point.y < theVertexList[i].y))) &&
          (point.x < (theVertexList[j].x - theVertexList[i].x) * (point.y - theVertexList[i].y) /
           (theVertexList[j].y - theVertexList[i].y) + theVertexList[i].x))
      {
         c = !c;
      }
   }

   if(!c) // check if on if not within
   {
      for (i = 0, j = npol-1; i < npol; j = i++)
      {
         if(toprsLine(theVertexList[i], theVertexList[j]).isPointWithin(point))
         {
            return true;
         }
      }
   }

   return (c!=0);
}

//*****************************************************************************
//  METHOD: toprsPolygon::vertex(int)
//  
//  Returns the toprsDpt vertex given the index. Also initializes the current
//  edge (theCurrentEdge) to the edge corresponding to the index.
//  
//*****************************************************************************
bool toprsPolygon::vertex(int index, toprsDpt& tbd_vertex) const 
{
   if((index >= (int)theVertexList.size()) ||
      (index < 0))
   {
      return false;
   }

   tbd_vertex = theVertexList[index];
   theCurrentVertex = index;

   return true;
}

//*****************************************************************************
//  METHOD: toprsPolygon::nextVertex()
//  
//  Assigns the toprsDpt tbd_vertex following the current vertex. The current
//  vertex is initialized with a call to vertex(int), or after the last
//  vertex is reached (initialized to theFirstEdge. Returns false if no vertex
//  defined.
//
//*****************************************************************************
bool toprsPolygon::nextVertex(toprsDpt& tbd_vertex) const 
{
   ++theCurrentVertex;
   if(theCurrentVertex >= (toprs_int32)theVertexList.size())
   {
      return false;
   }
   tbd_vertex = theVertexList[theCurrentVertex];
   
   return true;
}

//*****************************************************************************
//  METHOD: operator=()
//  
//*****************************************************************************
const toprsPolygon& toprsPolygon::operator= (const toprsIRect& rect)
{
   theCurrentVertex = 0;
   theVertexList.resize(4);
   theVertexList[0] = rect.ul();
   theVertexList[1] = rect.ur();
   theVertexList[2] = rect.lr();
   theVertexList[3] = rect.ll();

   return *this;
}

const toprsPolygon& toprsPolygon::operator= (const toprsDRect& rect)
{
   theCurrentVertex = 0;
   theVertexList.resize(4);
   theVertexList[0] = rect.ul();
   theVertexList[1] = rect.ur();
   theVertexList[2] = rect.lr();
   theVertexList[3] = rect.ll();

   return *this;
}

const toprsPolygon&  toprsPolygon::operator=(const toprsPolygon& polygon)
{
   theVertexList    = polygon.theVertexList;
   theCurrentVertex = polygon.theCurrentVertex;
   theOrderingType  = polygon.theOrderingType;
   
   return *this;
}

const toprsPolygon& toprsPolygon::operator= (const vector<toprsDpt>& vertexList)
{
   theVertexList    = vertexList;
   theCurrentVertex = 0;
   theOrderingType = TOPRS_VERTEX_ORDER_UNKNOWN;
   
   return *this;
}

const toprsPolygon& toprsPolygon::operator=(const vector<toprsIpt>& vertexList)
{
   theVertexList.resize(vertexList.size());
   
   // Assign std::vector<toprsIpt> list to std::vector<toprsDpt> theVertexList.
   for (std::vector<toprsIpt>::size_type i = 0; i < vertexList.size(); ++i)
   {
      theVertexList[i] = vertexList[i];
   }
   
   theCurrentVertex = 0;
   theOrderingType = TOPRS_VERTEX_ORDER_UNKNOWN;
   
   return *this;
}

//*****************************************************************************
//  METHOD: operator==()
//  
//*****************************************************************************
bool toprsPolygon::operator==(const toprsPolygon& polygon) const
{
   if( (theVertexList.size() != polygon.theVertexList.size()))
   {
      return false;
   }
   if(!theVertexList.size() && polygon.theVertexList.size())
   {
      return true;
   }

   return (theVertexList == polygon.theVertexList);
}

const toprsPolygon& toprsPolygon::operator *=(const toprsDpt& scale)
{
   toprs_uint32 upper = (toprs_uint32)theVertexList.size();
   toprs_uint32 i = 0;
   for(i = 0; i < upper; ++i)
   {
      theVertexList[i].x*=scale.x;
      theVertexList[i].y*=scale.y;
   }
   
   return *this;
}

toprsPolygon toprsPolygon::operator *(const toprsDpt& scale)const
{
   toprsPolygon result(*this);

   toprs_uint32 upper = (toprs_uint32)theVertexList.size();
   toprs_uint32 i = 0;
   for(i = 0; i < upper; ++i)
   {
      result.theVertexList[i].x*=scale.x;
      result.theVertexList[i].y*=scale.y;
   }

   return result;
}


void toprsPolygon::reverseOrder()
{
   std::reverse(theVertexList.begin(), theVertexList.end());
   
   if(theOrderingType == TOPRS_COUNTERCLOCKWISE_ORDER)
   {
      theOrderingType = TOPRS_CLOCKWISE_ORDER;
   }
   else if(theOrderingType == TOPRS_CLOCKWISE_ORDER)
   {
      theOrderingType =  TOPRS_COUNTERCLOCKWISE_ORDER;
   }
   
}




toprsVertexOrdering toprsPolygon::checkOrdering()const
{
   if(theOrderingType == TOPRS_VERTEX_ORDER_UNKNOWN)
   {
      double areaValue = area();
      if(areaValue > 0)
      {
         theOrderingType = TOPRS_COUNTERCLOCKWISE_ORDER;
      }
      else if(areaValue <= 0)
      {
         theOrderingType = TOPRS_CLOCKWISE_ORDER;
      }
   }

   return theOrderingType;
}

void toprsPolygon::intersectEdge(toprsDpt& result,
                                 const toprsLine& segment,
                                 const toprsDRect& rect,
                                 int edge)
{
   toprsLine edgeLine;
   switch(edge)
   {
   case RECT_LEFT_EDGE:
   {
      edgeLine.theP1 = rect.ll();
      edgeLine.theP2 = rect.ul();
      break;
   }
   case RECT_TOP_EDGE:
   {
      edgeLine.theP1 = rect.ul();
      edgeLine.theP2 = rect.ur();
      break;
   }
   case RECT_RIGHT_EDGE:
   {
      edgeLine.theP1 = rect.ur();
      edgeLine.theP2 = rect.lr();
      break;
   }
   case RECT_BOTTOM_EDGE:
   {
      edgeLine.theP1 = rect.lr();
      edgeLine.theP2 = rect.ll();
      break;
   }
   }
   
   result = segment.intersectInfinite(edgeLine);
}

bool toprsPolygon::isInsideEdge(const toprsDpt& pt,
                                const toprsDRect& rect,
                                int edge)const
{
   switch(edge)
   {
   case RECT_LEFT_EDGE:
   {
      return (pt.x>rect.ul().x);
      break;
   }
   case RECT_TOP_EDGE:
   {
      if(rect.orientMode() == TOPRS_LEFT_HANDED)
      {
         return (pt.y > rect.ul().y);
      }
      else
      {
         return (pt.y < rect.ul().y);
      }
      break;
   }
   case RECT_RIGHT_EDGE:
   {
      return (pt.x<rect.lr().x);
      
      break;
   }
   case RECT_BOTTOM_EDGE:
   {
      if(rect.orientMode() == TOPRS_LEFT_HANDED)
      {
         return (pt.y < rect.lr().y);
      }
      else
      {
         return (pt.y > rect.lr().y);
      }
      break;
   }
   }
   return false;
}


   


void toprsPolygon::getMinimumBoundingRect(toprsPolygon& minRect) const
{
   static const double MIN_STEP = (0.5)*M_PI/180.0;
   double angle_step = M_PI/8.0;  // initial rotation step size for min area search = 22.5 deg
   double theta;
   double best_theta = M_PI/4.0;  // Initial guess is 45 deg orientation
   double center_theta;
   double cos_theta, sin_theta;
   toprsPolygon rotatedPolygon(*this);
   toprsDpt xlatedVertex;
   toprsDpt rotatedVertex(0.0, 0.0);
   double min_x, min_y, max_x, max_y;
   double area;
   double min_area = 1.0/DBL_EPSILON;
   rotatedPolygon.theVertexList[0] = toprsDpt(0, 0);  // first vertex always at origin
   bool first_time = true;
   toprsDRect best_rect;
   static const bool TESTING = false;

   //***
   // Loop to converge on best orientation angle for bounding polygon:
   //***
   while (angle_step > MIN_STEP)
   {
      //***
      // Try four different rotations evenly centered about the current best guess:
      //***
      center_theta = best_theta;
      for (int i=0; i<5; i++)
      {
         //***
         // Check for i=2 (center angle) since already computed quantities for this in last iteration
         // unless this is first time through:
         //***
         if ((i != 2) || (first_time)) 
         {
            theta = center_theta + (i - 2.0)*angle_step;
            cos_theta = cos(theta);
            sin_theta = sin(theta);
            min_x = rotatedPolygon.theVertexList[0].x;
            min_y = rotatedPolygon.theVertexList[0].y;
            max_x = min_x;
            max_y = min_y;

            //***
            // Translate polygon to origin and rotate all vertices by current theta:
            //***
            for (unsigned int vertex=1; vertex < theVertexList.size(); vertex++)
            {
               xlatedVertex.x = theVertexList[vertex].x - theVertexList[0].x;
               xlatedVertex.y = theVertexList[vertex].y - theVertexList[0].y;
               rotatedVertex.x = cos_theta*xlatedVertex.x + sin_theta*xlatedVertex.y;
               rotatedVertex.y = cos_theta*xlatedVertex.y - sin_theta*xlatedVertex.x;
               rotatedPolygon.theVertexList[vertex] = rotatedVertex;

               //***
               // Latch max and mins of bounding rect:
               //***
               if (min_x > rotatedVertex.x) min_x = rotatedVertex.x;
               if (min_y > rotatedVertex.y) min_y = rotatedVertex.y;
               if (max_x < rotatedVertex.x) max_x = rotatedVertex.x;
               if (max_y < rotatedVertex.y) max_y = rotatedVertex.y;
            }

            if (TESTING)
            {
               toprsDpt v1 (cos_theta*min_x - sin_theta*max_y + theVertexList[0].x,
                            cos_theta*max_y + sin_theta*min_x + theVertexList[0].y);
               toprsDpt v2 (cos_theta*max_x - sin_theta*max_y + theVertexList[0].x,
                            cos_theta*max_y + sin_theta*max_x + theVertexList[0].y);
               toprsDpt v3 (cos_theta*max_x - sin_theta*min_y + theVertexList[0].x,
                            cos_theta*min_y + sin_theta*max_x + theVertexList[0].y);
               toprsDpt v4 (cos_theta*min_x - sin_theta*min_y + theVertexList[0].x,
                            cos_theta*min_y + sin_theta*min_x + theVertexList[0].y);
               cout << v1.x << "\t" << v1.y << endl;
               cout << v2.x << "\t" << v2.y << endl;
               cout << v3.x << "\t" << v3.y << endl;
               cout << v4.x << "\t" << v4.y << endl << endl;
            }

            //***
            // Establish bounding rect and area about rotated polygon:
            //***
            area = (max_x - min_x) * (max_y - min_y);
            if (area < min_area)
            {
               best_theta = theta;
               min_area = area;
               best_rect = toprsDRect(min_x, max_y, max_x, min_y, TOPRS_RIGHT_HANDED);
            }
         } // end if (i != 2 || first_time)
      }  // end for-loop over surrounding rotations

      //***
      // Adjust step size by half to repeat process:
      //***
      angle_step /= 2.0;
      first_time = false;

   } // end while loop for convergence

   //***
   // best_theta now contains optimum rotation of bounding rect. Need to apply reverse
   // rotation and translation of best_rect:
   //***
   cos_theta = cos(best_theta);
   sin_theta = sin(best_theta);
   toprsDpt v1 (cos_theta*best_rect.ul().x - sin_theta*best_rect.ul().y + theVertexList[0].x,
                cos_theta*best_rect.ul().y + sin_theta*best_rect.ul().x + theVertexList[0].y);
   toprsDpt v2 (cos_theta*best_rect.ur().x - sin_theta*best_rect.ur().y + theVertexList[0].x,
                cos_theta*best_rect.ur().y + sin_theta*best_rect.ur().x + theVertexList[0].y);
   toprsDpt v3 (cos_theta*best_rect.lr().x - sin_theta*best_rect.lr().y + theVertexList[0].x,
                cos_theta*best_rect.lr().y + sin_theta*best_rect.lr().x + theVertexList[0].y);
   toprsDpt v4 (cos_theta*best_rect.ll().x - sin_theta*best_rect.ll().y + theVertexList[0].x,
                cos_theta*best_rect.ll().y + sin_theta*best_rect.ll().x + theVertexList[0].y);
    
   if (TESTING)
   {
      cout << v1.x << "\t" << v1.y << endl;
      cout << v2.x << "\t" << v2.y << endl;
      cout << v3.x << "\t" << v3.y << endl;
      cout << v4.x << "\t" << v4.y << endl << endl;
   }

   //***
   // Assign return value rect:
   //***
   minRect.clear();
   minRect.addPoint(v1);
   minRect.addPoint(v2);
   minRect.addPoint(v3);
   minRect.addPoint(v4);

   // Make sure we are always returning a positive clockwise area.
   minRect.checkOrdering();
   if(minRect.getOrdering()==TOPRS_COUNTERCLOCKWISE_ORDER)
      minRect.reverseOrder();
   return;
}

/**
* METHOD: remove() 
* Removes the vertex from the polygon.
*/
void toprsPolygon::removeVertex(int vertex)
{
   int numvertices=getNumberOfVertices();
   if(vertex>numvertices) {
      return;
   } else {
      vector<toprsDpt>::iterator it;
      int v=0;
      for(it=theVertexList.begin();it!=theVertexList.end();it++) {
         if(v++==vertex) {
            theVertexList.erase(it);
            break;
         }
      }
   }
}

/**
* METHOD: removeSmallestContributingVertex() 
* Removes the vertex that contributes the smallest area to the polygon.
*/
void toprsPolygon::removeSmallestContributingVertex()
{
   unsigned int numvertices=getNumberOfVertices();
   if (!numvertices)
      return;

   int smallest_vertex=-1,n1,n2;
   double smallest_area=1.0/DBL_EPSILON;
   toprsPolygon tmp;

   for(unsigned int v=0;v<numvertices;v++) {
      tmp.clear();
      if(v==0) {
         n1=numvertices-1;
         n2=1;
      } else if(v==numvertices-1) {
         n1=numvertices-2;
         n2=0;
      } else {
         n1=v-1;
         n2=v+1;
      }

      tmp.addPoint(theVertexList[n1]);
      tmp.addPoint(theVertexList[v]);
      tmp.addPoint(theVertexList[n2]);

      if(fabs(tmp.area())<smallest_area) {
         smallest_area=fabs(tmp.area());
         smallest_vertex=v;
      }
   }
   removeVertex(smallest_vertex);
}


toprsDpt& toprsPolygon::operator[](int index)
{
   return theVertexList[index];
}

const toprsDpt& toprsPolygon::operator[](int index)const
{
   return theVertexList[index];
}

toprs_uint32 toprsPolygon::getVertexCount()const
{
   return getNumberOfVertices();
}

toprs_uint32 toprsPolygon::getNumberOfVertices()const
{
   return (toprs_uint32)theVertexList.size();
}

void toprsPolygon::getBoundingRect(toprsIRect& rect)const
{
   toprs_int32 minX;
   toprs_int32 minY;
   toprs_int32 maxX;
   toprs_int32 maxY;
   getIntegerBounds(minX, minY, maxX, maxY);
   rect = toprsIRect(minX, minY, maxX, maxY);
}

void toprsPolygon::getBoundingRect(toprsDRect& rect)const
{
   toprs_float64 minX;
   toprs_float64 minY;
   toprs_float64 maxX;
   toprs_float64 maxY;
   getFloatBounds(minX, minY, maxX, maxY);
   rect = toprsDRect(minX, minY, maxX, maxY);
}

void toprsPolygon::clear()
{
   theVertexList.clear();
   theOrderingType = TOPRS_VERTEX_ORDER_UNKNOWN;
}

void toprsPolygon::addPoint(const toprsDpt& pt)
{
   theVertexList.push_back(pt);
   theOrderingType = TOPRS_VERTEX_ORDER_UNKNOWN;
}

void toprsPolygon::addPoint(double x, double y)
{
   theVertexList.push_back(toprsDpt(x, y));
   theOrderingType = TOPRS_VERTEX_ORDER_UNKNOWN;
}

const vector<toprsDpt>& toprsPolygon::getVertexList()const
{
   return theVertexList;
}

 bool toprsPolygon::pointWithin(const toprsDpt& point) const
{
   return isPointWithin(point);
}

const toprsPolygon& toprsPolygon::operator *=(double scale)
{
   return ((*this)*=toprsDpt(scale, scale));
}

toprsPolygon toprsPolygon::operator *(double scale)const
{
   return ((*this)*toprsDpt(scale, scale));
}

void toprsPolygon::resize(toprs_uint32 newSize)
{
   theVertexList.resize(newSize);
   theOrderingType  = TOPRS_VERTEX_ORDER_UNKNOWN;
   theCurrentVertex = 0;
}

toprsVertexOrdering toprsPolygon::getOrdering()const
{
   return theOrderingType;
}

bool toprsPolygon::operator!=(const toprsPolygon& compare_this) const
{
   return !(*this == compare_this);
}

ostream& operator<<(ostream& os, const toprsPolygon& polygon)
{
   polygon.print(os);
   return os;
}

/**
* METHOD: getCentroid() 
* Assigns the toprsDpt centroid the polygon.
* Warning: centroid is not guaranteed to be inside the polygon!
*/
void toprsPolygon::getCentroid(toprsDpt &centroid) const
{
   int numpts = (int)theVertexList.size();
   unsigned int next;
   double area=0,parea;

   centroid=toprsDpt(0,0);
   for(int i=0;i<numpts;i++) {
      if(i<numpts-1) {
         next=i+1;
      } else {
         next=0;
      }
      parea=theVertexList[i].x*theVertexList[next].y-theVertexList[next].x*theVertexList[i].y;
      area+=parea;
      centroid.x+=(theVertexList[i].x+theVertexList[next].x)*parea;
      centroid.y+=(theVertexList[i].y+theVertexList[next].y)*parea;
   }
   area=area/2.0;
   centroid=centroid/(area*6.0);
}

/**
* METHOD: fitCircleInsideVertex() 
* Assigns destPt the point that fits a circle of given radius inside the polygon vertex.
* Warning: destPt is not guaranteed to be inside the polygon!
* (you may not be able to fit a circle of the given radius inside the polygon)
*/
void toprsPolygon::fitCircleInsideVertex(toprsDpt &destPt, unsigned int vertex, double radius) const
{
   toprs_uint32 num_vertices=(int)theVertexList.size(),n1,n2;
   toprsDpt side1,side2,bisection,currpt;
   double length_out,side1_side2_cross;
   bool concave=true;

   // don't be doing that dude.
   if(num_vertices<3 || vertex>=num_vertices) {
      destPt=toprsDpt(0,0);
      return;
   }
 
   if(vertex==0) {
      n1=num_vertices-1;
      n2=vertex+1;
   } else if(vertex==num_vertices-1) {
      n1=num_vertices-2;
      n2=0;
   } else {
      n1=vertex-1;
      n2=vertex+1;
   }

   currpt=theVertexList[vertex];
   // get the side vectors
   side1=theVertexList[n1]-currpt;
   side2=theVertexList[n2]-currpt;

   // normalize the sides
   side1 = side1/side1.length();
   side2 = side2/side2.length();

   side1_side2_cross=side1.x*side2.y-side2.x*side1.y;

   checkOrdering();
   if(getOrdering()==TOPRS_COUNTERCLOCKWISE_ORDER) {
      if(side1_side2_cross<0)
         concave=false;
   } else { //clockwise
      if(side1_side2_cross>0)
         concave=false;
   }

   bisection = side1+side2;
   bisection = bisection/bisection.length();

   if(concave) {
      bisection=bisection*-1.0;
      length_out=radius;
   } else {
      double cos_theta=(side1.x*bisection.x+side1.y*bisection.y);
      length_out=radius/sqrt(1-cos_theta*cos_theta);
   }
   destPt=toprsDpt(currpt+bisection*length_out);
   return;
}


/**
//! Shrinks this polygon by radius. Effectively, circles of given radius are placed inside 
//! the vertices just tangent to the polygon edges (via fitCircleInsideVertex()). The new
//! polygon's vertices will be the center of these circles. Return true if success. 
*/
bool toprsPolygon::shrink(toprsPolygon &dest, double inset) const 
{
   int numpts = (int) theVertexList.size();
   toprsDpt pt;
   
   //don't let people shrink themselves, that isn't going to work
   if(&dest==this) return false;

   dest.clear();
   for(int i=0;i<numpts;i++) {
      fitCircleInsideVertex(pt,i,inset);
      dest.addPoint(pt);
   }
   if(isPolyWithin(dest)) {
      return true;
   } else {
      //return an empty polygon
      dest=toprsPolygon();
      return false;
   }
}



