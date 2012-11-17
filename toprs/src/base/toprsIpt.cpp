#include <iostream>
#include <sstream>

#include "base/toprsIpt.h"
#include "base/toprsDpt.h"

toprsIpt::toprsIpt(const toprsDpt& pt)
{
	if(pt.hasNAN())
	{
		makeNAN();
	}
	else
	{
		x = toprs::round<int>(pt.x);
		y = toprs::round<int>(pt.y);
	}
}

const toprsIpt& toprsIpt::operator=(const toprsDpt& pt)
{
	if(pt.hasNAN())
	{
		makeNAN();
	}
	else
	{
		x = toprs::round<int>(pt.x);
		y = toprs::round<int>(pt.y);
	}
	return *this;
}

std::ostream& toprsIpt::print(std::ostream& os) const
{
	os << "( ";

	if (x != TOPRS_INT_NAN)
	{
		os << x;
	}
	else
	{
		os << "NAN";
	}

	os << ", ";

	if (y != TOPRS_INT_NAN)
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

std::ostream& operator<<(std::ostream& os, const toprsIpt& pt)
{
	return pt.print(os);
}

std::string toprsIpt::toString() const
{
	std::ostringstream os;
	os << "(";

	if (x != TOPRS_INT_NAN)
	{
		os << x;
	}
	else
	{
		os << "NAN";
	}

	os << ",";

	if (y != TOPRS_INT_NAN)
	{
		os << y;
	}
	else
	{
		os << "NAN";
	}

	os << ")";

	return os.str();
}


void toprsIpt::toPoint(const std::string& s)
{
	std::istringstream is(s);
	is >> *this;
}

std::istream& operator>>(std::istream& is, toprsIpt &pt)
{
	//---
	// Expected input format:
	// ( 30, -90 )
	//   -x- -y-
	//---

	// Start with a nan point.
	//pt.makeNAN();

	//// Check the stream.
	//if (!is) return is;

	//const int SZ = 64; // Handle real big number...
	//std::string os;
	//char buf[SZ];

	////---
	//// X SECTION:
	////---

	//// Grab data up to the first comma.
	//is.get(buf, SZ, ',');

	//if (!is) return is;

	//// Copy to toprs string.
	//os = buf;

	//// Get rid of the '(' if there is any.
	//std::string::size_type pos = os.find('(');
	//if (pos != std::string::npos)
	//{
	//	os.erase(pos, 1);
	//}   
	//if (os.contains("NAN") == false)
	//{
	//	pt.x = os.toInt32();
	//}
	//else
	//{
	//	pt.x = TOPRS_INT_NAN;
	//}

	////---
	//// Y SECTION:
	////---

	//// Grab the data up to the ')'
	//is.get(buf, SZ, ')');

	//if (!is) return is;

	//// Copy to toprs string.
	//os = buf;

	//// Get rid of the ',' if there is any.
	//pos = os.find(',');
	//if (pos != std::string::npos)
	//{
	//	os.erase(pos, 1);
	//}

	//if (os.contains("nan") == false)
	//{
	//	pt.y = os.toInt32();
	//}
	//else
	//{
	//	pt.y = TOPRS_INT_NAN;
	//}

	//// Gobble the trailing ")".
	//char c = 0;
	//while (c != ')')
	//{
	//	is.get(c);
	//	if (!is) break;
	//}

	//// Finished
	return is;
}



bool toprsIpt::isEqualto( const toprsIpt& rhs ) const
{
	return ((x==rhs.x)&&(y==rhs.y));
}


