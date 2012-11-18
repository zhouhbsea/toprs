#include"utility\toprsString.h"
#include<iostream>
#include <algorithm>
using std::string;
using std::vector;

namespace toprs {

  string toString(toprs_int32 x) {
    char str[100];
    sprintf(str, "%d", x);
    return str;
  }

  int toInt(const string& val) {
    int result;
    char trash;
    int num = sscanf(val.c_str(), "%d%c", &result, &trash);
    return result;
  }

  float toFloat(const string& val) {
    float result;
    char trash;
    int num = sscanf(val.c_str(), "%f%c", &result, &trash);
    return result;
  }

  bool toBool(const string& val) {
    if (val == "true") {
      return true;
    } else if (val == "false") {
      return false;
    } else {
      std::cout << "toBool error.";
    }
  }

  string toUpper(const string& str) {// {{{
	  string ustr (str.size(),' ');
	  string::iterator it2=ustr.begin();
	  for ( string::const_iterator it1=str.begin();
		  it1!=str.end(); ++it1, ++it2) 
		  *it2 = toupper(*it1);
	  //transform(str.begin(), str.end(), ustr.begin(), toupper);
	  return ustr;
  }// }}

   void toUpper( std::string& str ) {
      string::iterator it = str.begin();
	  while(it != str.end()) {
		  *it = toupper(*it);
		  ++it;
	  }

   }

   void toLower( std::string& str ) {
	   string::iterator it = str.begin();
	   while(it != str.end()) {
		   *it = tolower(*it);
		   ++it;
	   }

   }

  string toLower(const string& str) {// {{{
	  string ustr (str.size(),' ');
	  string::iterator it2=ustr.begin();
	  for ( string::const_iterator it1=str.begin();
		  it1!=str.end(); ++it1, ++it2) 
		  *it2 = tolower(*it1);
	  //transform(str.begin(), str.end(), ustr.begin(), toupper);
	  return ustr;
  }// }}

  string& warptext(string& str, const unsigned int width) {// {{{
	  if (width < 1)
		  return str;

	  typedef string::size_type INT;
	  INT EMPTY = string::npos;
	  const char space=' ';
	  const char newline='\n';

	  INT end = str.size();
	  INT beg = 0;
	  while (end > width) {
		  INT c = str.substr(beg,width).find_last_of(newline);
		  if (c != EMPTY) {
			  beg += c+1;
			  end -= c+1;
			  continue;
		  }
		  INT pos = str.substr(beg,width).find_last_of(space);

		  if (pos == EMPTY) {
			  INT s = str.substr(beg).find_first_of(space);
			  INT n = str.substr(beg).find_first_of(newline);

			  INT t = EMPTY;
			  if (s != EMPTY && n!= EMPTY)
				  t = std::min(s,n);
			  else if (s != EMPTY)
				  t = s;
			  else if (n != EMPTY)
				  t = n;

			  if (t == EMPTY)
				  break;
			  str[beg+t] = newline;
			  beg += t+1;
			  end -= t+1;
			  continue;
		  } else {
			  str[beg+pos] = newline;
			  beg += pos+1;
			  end -= pos+1;
		  }
	  }
	  return str;
  }// }}}



  vector<string> splitString(const std::string& str,
			     const char* separator) {
    vector<string> result;
    string::size_type prev_pos=0;
    string::size_type pos=0;
    while ((pos = str.find_first_of(separator, prev_pos)) != string::npos) {
      if (prev_pos < pos) {
	result.push_back(str.substr(prev_pos, pos-prev_pos));
      }
      prev_pos = pos + 1;
    }
    if (prev_pos < str.size()) {
      result.push_back(str.substr(prev_pos));
    }
    return result;
  }

  string quoteString(const string& str,
                     const char* deliminators) {
    
    string result(str);
    for(int i=result.length() -1; i >= 0; --i) {
      char ch = result[i];
      if (!isprint(ch) ||
          ch == '\\' || 
          strchr(deliminators, ch)) {
        switch (ch) {
        case '\\':
          result.replace(i, 1, "\\\\");
          break;
        case '\t':
          result.replace(i, 1, "\\t");
          break;
        case '\n':
          result.replace(i, 1, "\\n");
          break;
        case ' ':
          result.replace(i, 1, "\\s");
          break;
        default:
          char buff[4];
          sprintf(buff, "\\%02x", static_cast<unsigned char>(result[i]));
          result.replace(i, 1, buff);
        }
      }
    }
    return result;
  }

  string unquoteString(const string& str) {
    string result(str);
    string::size_type current = result.find('\\');
    while (current != string::npos) {
      if (current + 1 < result.size()) {
        char new_ch;
        int num_chars;
        if (isxdigit(result[current+1])) {
          num_chars = 2;
          //HADOOP_ASSERT(current + num_chars < result.size(),"escape pattern \\<hex><hex> is missing second digit in '" + str + "'");
          char sub_str[3];
          sub_str[0] = result[current+1];
          sub_str[1] = result[current+2];
          sub_str[2] = '\0';
          char* end_ptr = NULL;
          long int int_val = strtol(sub_str, &end_ptr, 16);
         // HADOOP_ASSERT(*end_ptr == '\0' && int_val >= 0, "escape pattern \\<hex><hex> is broken in '" + str + "'");
          new_ch = static_cast<char>(int_val);
        } else {
          num_chars = 1;
          switch(result[current+1]) {
          case '\\':
            new_ch = '\\';
            break;
          case 't':
            new_ch = '\t';
            break;
          case 'n':
            new_ch = '\n';
            break;
          case 's':
            new_ch = ' ';
            break;
          default:
            string msg("unknow n escape character '");
            msg += result[current+1];
            //HADOOP_ASSERT(false, msg + "' found in '" + str + "'");
          }
        }
        result.replace(current, 1 + num_chars, 1, new_ch);
        current = result.find('\\', current+1);
      } else {
        //HADOOP_ASSERT(false, "trailing \\ in '" + str + "'");
      }
    }
    return result;
  }

  int noCaseComparison(const string & s1, const string& s2) {// {{{
	  //stop when either string's end has been reached
	  for ( string::const_iterator it1=s1.begin(), it2=s2.begin();
		  (it1!=s1.end()) && (it2!=s2.end()); ++it1, ++it2) 
	  { 
		  // return -1 to indicate smaller than, 1 otherwise
		  if(toupper(*it1) != toupper(*it2)) //letters differ?
			  return (toupper(*it1)  < toupper(*it2)) ? -1 : 1; 
	  }
	  string::size_type size1=s1.size(), size2=s2.size();// cache lengths
	  //return -1,0 or 1 according to strings' lengths
	  if (size1==size2) 
		  return 0;
	  return (size1<size2) ? -1 : 1;
  }// }}}

  bool ifContains( const std::string& s1, const std::string s2 )
  {
	  return s1.find(s2)!=std::string::npos;
  }

  bool ifContains( const std::string& str, const char* ch )
  {
      return str.find(ch)!=std::string::npos;
  }

  string trim( const std::string& str, const std::string& trim_str /*= " \t\n\r"*/ )
  {
	  if(str.size() == 0) return str;
	  if(trim_str.empty()) return str;
	  string::const_iterator startPos = str.begin();
	  string::const_iterator endPos   = str.begin() + (str.size()-1);

	  while( ( startPos != str.end() ) &&
		  (std::find(trim_str.begin(),
		  trim_str.end(),
		  *startPos)!=trim_str.end()) ) ++startPos;

	  if(startPos == str.end())
	  {
		  return std::string("");
	  }

	  while( (endPos!=startPos)&& (std::find(trim_str.begin(),
		  trim_str.end(),
		  *endPos)!=trim_str.end())) --endPos;

	  return std::string(startPos, endPos+1);
  }

  void trim( std::string& str, const std::string& trim_str /*= " \t\n\r"*/ )
  {
	  if(str.size() == 0) return ;
	  if(trim_str.empty()) return ;
	  string::iterator startPos = str.begin();
	  string::iterator endPos   = str.begin() + (str.size()-1);

	  while( ( startPos != str.end() ) &&
		  (std::find(trim_str.begin(),
		  trim_str.end(),
		  *startPos)!=trim_str.end()) ) ++startPos;

	  if(startPos == str.end())
	  {
		  str = "";
	  }

	  while( (endPos!=startPos)&& (std::find(trim_str.begin(),
		  trim_str.end(),
		  *endPos)!=trim_str.end())) --endPos;

	  str =  std::string(startPos, endPos+1);
  }

  void beforePos( std::string& str,std::string::size_type pos )
  {
	  str.erase(pos, std::string::npos);
  }

  void afterPos( std::string& str,std::string::size_type pos )
  {
	  str.erase(0, pos+1);
  }
}
