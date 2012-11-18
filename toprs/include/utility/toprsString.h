#ifndef TOPRSSTRINGH
#define TOPRSSTRINGH
#include <string>
#include <vector>
#include"common\toprsConstants.h"
namespace toprs
{
	 TOPRSDLLEXPORT std::string toString(toprs_int32 x); 
     TOPRSDLLEXPORT toprs_int32 toInt(const std::string& val); 
     TOPRSDLLEXPORT float toFloat(const std::string& val);
     TOPRSDLLEXPORT bool toBool(const std::string& val);
	 TOPRSDLLEXPORT std::string toUpper(const std::string& str);
	 TOPRSDLLEXPORT std::string toLower(const std::string& str);
     TOPRSDLLEXPORT void toUpper(std::string& str);
	 TOPRSDLLEXPORT void toLower(std::string& std);
     TOPRSDLLEXPORT toprs_uint64 getCurrentMillis(); 
     TOPRSDLLEXPORT std::vector<std::string> splitString(const std::string& str, const char* separator); 
     TOPRSDLLEXPORT std::string quoteString(const std::string& str, const char* deliminators);
     TOPRSDLLEXPORT std::string unquoteString(const std::string& str);

	 TOPRSDLLEXPORT int noCaseComparison(const std::string & s1, const std::string& s2);
	 TOPRSDLLEXPORT std::string& warptext(std::string& str, const unsigned int width);
	 TOPRSDLLEXPORT bool ifContains(const std::string& s1, const std::string s2);
	 TOPRSDLLEXPORT bool ifContains(const std::string& str, const char* ch);

	 TOPRSDLLEXPORT std::string trim(const std::string& str, const std::string& trim_str  = " \t\n\r");
	 TOPRSDLLEXPORT void trim(std::string& str, const std::string& trim_str  = " \t\n\r");
	 void beforePos(std::string& str,std::string::size_type pos);
	 void afterPos(std::string& str,std::string::size_type pos);



}
#endif