#include <iostream>
#include <iomanip>
#include <sstream>

#include "base/toprsDpt.h"
#include "base/toprsIpt.h"

toprsDpt::toprsDpt(const toprsIpt& pt)
   : x(pt.x), y(pt.y)
{
   if(pt.hasNAN())
   {
      makeNAN();
   }
}



const toprsDpt& toprsDpt::operator=(const toprsIpt& pt)
{
   if(pt.hasNAN())
   {
      makeNAN();
   }
   else
   {
      x = pt.x;
      y = pt.y;
   }
   return *this;
}


std::ostream& toprsDpt::print(std::ostream& os, int precision) const
{
   os << std::setiosflags(std::ios::fixed) << std::setprecision(precision)
      << "( ";

   if (toprs::isnan(x) == false)
   {
      os << x;
   }
   else
   {
      os << "NAN";
   }
   
   os << ", ";

   if (toprs::isnan(y) == false)
   {
      os << y;
   }
   else
   {
      os << "NAN";
   }

   os << " )";

   return os;
}

std::ostream& operator<<(std::ostream& os, const toprsDpt& pt)
{
   return pt.print(os);
}

bool toprsDpt::isEqualto( const toprsDpt& rhs ) const
{
   if(rhs.isNAN()&&isNAN()) return true;
   return (toprs::almostEqual(x, rhs.x)&&
           toprs::almostEqual(y, rhs.y));
}

std::string toprsDpt::toString(int precision) const
{
   std::ostringstream os;
   os << std::setprecision(precision);

   os << "(";
   if (toprs::isnan(x) == false)
   {
      os << x;
   }
   else
   {
      os << "NAN";
   }
   
   os << ",";
   
   if (toprs::isnan(y) == false)
   {
      os << y;
   }
   else
   {
      os << "NAN";
   }
   
   os << ")";
   
   //print(os, precision);
   return os.str();
}

void toprsDpt::toPoint(const std::string& s)
{
   std::istringstream is(s);
   is >> *this;
}



std::istream& operator>>(std::istream& is, toprsDpt &pt)
{
   ////---
   //// Expected input format:
   //// ( 30.00000000000000, -90.00000000000000 )
   ////   --------x--------  ---------y--------
   ////---

   //// Start with a nan point.
   //pt.makeNan();

   //// Check the stream.
   //if (!is) return is;
   //
   //const int SZ = 64; // Handle real big number...
   //toprsString os;
   //char buf[SZ];

   ////---
   //// X SECTION:
   ////---
   //
   //// Grab data up to the first comma.
   //is.get(buf, SZ, ',');

   //if (!is) return is;

   //// Copy to toprs string.
   //os = buf;

   //// Get rid of the '(' if there is any.
   //std::string::size_type pos = os.find('(');
   //if (pos != std::string::npos)
   //{
   //   os.erase(pos, 1);
   //}   

   //if (os.contains("nan") == false)
   //{
   //   pt.x = os.toFloat64();
   //}
   //else
   //{
   //   pt.x = toprs::nan();
   //}

   ////---
   //// Y SECTION:
   ////---
   //
   //// Grab the data up to the ')'
   //is.get(buf, SZ, ')');

   //if (!is) return is;

   //// Copy to toprs string.
   //os = buf;

   //// Get rid of the ',' if there is any.
   //pos = os.find(',');
   //if (pos != std::string::npos)
   //{
   //   os.erase(pos, 1);
   //}
   //
   //if (os.contains("nan") == false)
   //{
   //   pt.y = os.toFloat64();
   //}
   //else
   //{
   //   pt.y = toprs::nan();
   //}

   //// Gobble the trailing ")".
   //char c = '\0';
   //while (c != ')')
   //{
   //   is.get(c);
   //   if (!is) break;
   //}

   // Finished
   return is;
}
