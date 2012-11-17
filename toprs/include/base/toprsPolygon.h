
#ifndef TOPRSPOLYGONH
#define TOPRSPOLYGONH
#include <iostream>
#include <vector>
using namespace std;

#include "base/toprsDpt.h"
#include "base/toprsIRect.h"
#include "base/toprsDRect.h"



class toprsLine;

class TOPRSDLLEXPORT toprsPolygon
{
public:
   toprsPolygon();
   toprsPolygon(const vector<toprsIpt>& polygon);
   toprsPolygon(const vector<toprsDpt>& polygon);
   toprsPolygon(int numVertices, const toprsDpt* vertex_array);

   toprsPolygon(const toprsPolygon& copy_this);
   
   /**
    * CONSTRUCTOR: Provided for convenience. Does not imply the polygon is
    * limited to four vertices:
    */
   toprsPolygon(toprsDpt v1,
                toprsDpt v2,
                toprsDpt v3,
                toprsDpt v4);
   
   toprsPolygon(const toprsIRect& rect);
   toprsPolygon(const toprsDRect& rect);

   ~toprsPolygon();

   toprsDpt& operator[](int index);
   
   const toprsDpt& operator[](int index)const;

   toprs_uint32 getVertexCount()const;
   
   toprs_uint32 getNumberOfVertices()const;

   //! Returns polygon area. Negative indicates CW ordering of vertices (in right-handed coordinates)
   double area()const;

   void getIntegerBounds(toprs_int32& minX,
                         toprs_int32& minY,
                         toprs_int32& maxX,
                         toprs_int32& maxY)const;
   void getFloatBounds(toprs_float64& minX,
                       toprs_float64& minY,
                       toprs_float64& maxX,
                       toprs_float64& maxY)const;

   void getBoundingRect(toprsIRect& rect)const;
   void getBoundingRect(toprsDRect& rect)const;

   /**
    * Initializes minRect with the minimum area rect (not-necessarily
    * aligned with axes) that bounds this polygon.
    *
    * @param minRect Polygon to initialize with the minimum rect.
    */
   void getMinimumBoundingRect(toprsPolygon& minRect) const;
   
   void roundToIntegerBounds(bool compress=false);

   void clear();

   void addPoint(const toprsDpt& pt);
   void addPoint(double x, double y);

   toprsDpt midPoint()const;
   
   /**
    * will sequence through the polygon and check to see if any values are NAN
    */
   bool hasNAN()const;

   const vector<toprsDpt>& getVertexList()const;
      
   /**
    * Uses the toprsPolyArea2d class for the intersection
    */
   bool clipToRect(vector<toprsPolygon>& result,
                   const toprsDRect& rect)const;

   
   /**
    * METHOD: clipLineSegment(p1, p2)
    * Implements Cyrus-Beck clipping algorithm as described in:
    * http://www.daimi.au.dk/~mbl/cgcourse/wiki/cyrus-beck_line-clipping_.html
    * Clips the line segment defined by thw two endpoints provided. The
    * endpoints are modified as needed to represent the clipped line. Returnes
    * true if intersection present.
    */
   bool clipLineSegment(toprsDpt& p1, toprsDpt& p2) const;

   /**
    * METHOD: pointWithin(toprsDpt)
    * Returns TRUE if point is inside polygon.
    */
   bool pointWithin(const toprsDpt& point) const;

   bool isPointWithin(const toprsDpt& point) const;

   /**
   * METHOD: isRectWithin()
   * Returns true if all the corner points of the given rect fit within.
   */
   bool isRectWithin(const toprsIRect &rect) const;

   /**
   * METHOD: isPolyWithin()
   * Returns true if all the vertices of the given polygon fit within.
   */
   bool isPolyWithin(const toprsPolygon &poly) const;

   /**
    * METHOD: vertex(index)
    * Returns the toprsDpt vertex given the index. Returns false if no vertex
    * defined.
    */
   bool vertex(int index, toprsDpt& tbd_vertex) const;

   /**
    * METHOD: nextVertex()
    * Assigns the toprsDpt tbd_vertex following the current vertex. The current
    * vertex is initialized with a call to vertex(int), or after the last
    * vertex is reached. Returns false if no vertex defined. Intended to be
    * when cycling through all vertices.
    */
   bool nextVertex(toprsDpt& tbd_vertex) const;

   void reverseOrder();
   /**
    * OPERATORS: (Some are inlined at bottom) 
    */
   const toprsPolygon& operator= (const toprsPolygon& copy_this);
   const toprsPolygon& operator= (const vector<toprsDpt>& vertexList);
   const toprsPolygon& operator= (const vector<toprsIpt>& vertexList);
   const toprsPolygon& operator= (const toprsIRect& rect);
   const toprsPolygon& operator= (const toprsDRect& rect);
   bool                operator==(const toprsPolygon& compare_this) const;
   bool                operator!=(const toprsPolygon& compare_this) const;

   const toprsPolygon& operator *=(const toprsDpt& scale);
   const toprsPolygon& operator *=(double scale);
   
   toprsPolygon operator *(const toprsDpt& scale)const;
   toprsPolygon operator *(double scale)const;
   
   void resize(toprs_uint32 newSize);

   toprsVertexOrdering checkOrdering()const;

   toprsVertexOrdering getOrdering()const;

protected:
   /**
   * METHOD: getCentroid()
   * Assigns the toprsDpt centroid the polygon.
   * Warning: centroid is not guaranteed to be inside the polygon!
    */
   void getCentroid(toprsDpt &centroid) const;

   /**
   * METHOD: shrink()
   * Shrinks the current polygon by inset, return true if success.
    */
   bool shrink(toprsPolygon &dest, double inset) const;
   
   /**
   * METHOD: remove()
   * Removes the vertex from the polygon.

    */
   void removeVertex(int vertex);
   /**
   * METHOD: removeSmallestContributingVertex()
   * Removes the vertex that contributes the smallest area to the polygon.
    */
   void removeSmallestContributingVertex();

   void intersectEdge(toprsDpt& result,
                      const toprsLine& segment,
                      const toprsDRect& rect,
                      int edge);
   
   bool isInsideEdge(const toprsDpt& pt,
                     const toprsDRect& rect,
                     int edge)const;

   /**
   * Assigns destPt the point that fits a circle of given radius inside the polygon vertex.
   * Warning: destPt is not guaranteed to be inside the polygon!
   * (you may not be able to fit a circle of the given radius inside the polygon)
   */
   void fitCircleInsideVertex(toprsDpt &destPt, unsigned int vertex, double radius) const;

   mutable toprsVertexOrdering theOrderingType;
   vector<toprsDpt> theVertexList;
   mutable toprs_int32 theCurrentVertex;
};

#endif /* End of "#ifndef toprsPolygon_HEADER" */
