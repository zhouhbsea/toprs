#ifndef TOPRSFILENAMEH
#define TOPRSFILENAMEH
#include<string>
#include"common\toprsConstants.h"
#include"utility\toprsString.h"
class toprsLocalTm;

class TOPRSDLLEXPORT toprsFilename : public std::string
{
public:
   enum AccessModes
   {
      TOPRS_READ_WRITE = 6,
      TOPRS_READ       = 4,
      TOPRS_WRITE      = 2,
      TOPRS_EXE	       = 1, // The PC docs say that this is ignored
      TOPRS_EXIST      = 0
   };
   toprsFilename();
   toprsFilename(const toprsFilename& src);
   toprsFilename(const std::string& src);
   toprsFilename(const char* src);
   template <class Iter> toprsFilename(Iter s, Iter e);
   static const toprsFilename NIL;
   bool operator == (const toprsFilename& rhs)const;
   bool operator == (const std::string& rhs)const;
   bool operator == (const char* rhs)const;
   void convertBackToForwardSlashes();
   void convertForwardToBackSlashes();
   bool setTimes(toprsLocalTm* accessTime,
                 toprsLocalTm* modTime,
                 toprsLocalTm* createTime)const;
   bool getTimes(toprsLocalTm* accessTime,
                 toprsLocalTm* modTime,
                 toprsLocalTm* createTime)const;
   bool touch()const;
   
   /**
    * Method to do file name expansion.
    *
    * Like: ~/docs will be expanded to /home/user/docs
    *
    * @return The expanded file name.
    *
    * @note METHOD IS NOT COMPLETE YET.
    */
   toprsFilename expand() const;
   
   // Methods to test toprsFilename for various states.
   bool        exists()       const;
   bool        isFile()       const;
   bool        isDir()        const;
   bool        isReadable()   const;
   bool        isWriteable()  const;
   bool        isExecutable() const;
   toprs_int64 fileSize()     const;
   
   // Methods to access parts of the toprsFilename.

   /**
    * @return "tif" if file is "/data/images/t1.tif".
    *
    * @note The '.' (dot) is not returned.
    */
   std::string   ext() const;

   /**
    * @return @return "/data/images" if file is "/data/images/t1.tif".
    */
   toprsFilename path() const;

   /**
    * @return @return "c:" if file is "c:\data\images\t1.tif".
    */
   toprsFilename drive() const;
   
   /**
    * @return @return "t1.tif" if file is "/data/images/t1.tif".
    */
   toprsFilename file() const;

   /**
    * @return @return "t1" if file is "/data/images/t1.tif".
    */
   toprsFilename fileNoExtension() const;

   /**
    * @return @return "/data/images/t1" if file is "/data/images/t1.tif".
    *
    * @note  The '.' (dot) is not returned.
    */
   toprsFilename noExtension() const;

   /**
    * Sets the extension of a file name.
    * Given: 
    * toprsFilename f = "foo.jpg";
    * f.setExtension(".tif");
    * f now equals "foo.tif"
    * @param e Extension to add or replace.
    *
    * @returns a reference to this.
    */
   toprsFilename& setExtension(const std::string& e);

   /**
	* Sets the file path and drive.
	* Input: the drive to be set (this should come from the user's preferences)
	* Example: "x:"
	* Given: "/filepath/file.ext"
    * @return @return "c:\filepath\file.ext".
    */
   toprsFilename& setDrive(const std::string& d);
   toprsFilename& setPath(const std::string& p);
   toprsFilename& setFile(const std::string& f);
   
   void split(std::string& drivePart,
              std::string& pathPart,
              std::string& filePart,
              std::string& extPart)const;
   
   void merge(const std::string& drivePart,
              const std::string& pathPart,
              const std::string& filePart,
              const std::string& extPart);
   /*!
    * Returns file appended onto this string. Path seperator is always placed
    * between this and file.  Returns file if this string is empty.
    * Given: this = /foo  and file  = bar output  = /foo/bar
    * Given: this = /foo/ and file  = bar output  = /foo/bar
    */
   toprsFilename dirCat(const toprsFilename& file) const;

   /*!
    */
   bool createDirectory(bool recurseFlag=true,
                        int perm=0775)const;

   /*!
    * @brief Removes pathname from filesystem if supported by platform.
    *
    * If pathname is a directory it will only be removed if empty. If supported
    * will set errno and output equivalent string if one occurs.
    *
    * @return true on success, false on error.
    */
   static bool remove(const toprsFilename& pathname);

   /**
    * Will use a wildcard remove.  NOTE:  This is in
    * a regular expression format so if you want all
    * files with prefix myfile to be erased then you would give
    * it <path>/myfile.*
    *
    * The . here is a regular expression '.' character which says any character
    * and the * "is any number of"
    * 
    */ 
   static bool wildcardRemove(const toprsFilename& pathname);

   bool rename(const toprsFilename& destFile, bool overwriteDestinationFlag=true)const;
   
   bool remove()const;
   bool wildcardRemove()const;

   /**
    * @brief Copies this file to output file.
    *
    * Copies this to output file. If output file is a directory, this->file() is
    * appended to output file.
    * 
    * @param outputFile File name to copy to.
    * 
    * @return True on success, false on error.
    */
   bool copyFileTo(const toprsFilename& ouputFile) const;

   /**
    * @brief Checks whether file name is relative or absolute.
    * 
    * @return true if file name has a relative path, false if absolute.
    *
    * @note This will return true if file name is empty.
    */
   bool isRelative() const;
   
   /**
    * @brief Method to check if expansion is needed.
    *
    * This checks if file isRelative() first if isRelative is false (has
    * absolute path) it then walks the file and looks for '$'.
    * 
    * @return true if file name is relative or has environment vars.
    *
    * @note This will return false if file name is empty.
    */
   bool needsExpansion() const;

   /** @return The path separator. */
   char getPathSeparator() const;
   
protected:

   void convertToNative();

   /*!
    * since windows uses \ for path separation
    * and unix / we need to be consistent.
    */
   static const char thePathSeparator;

};
#endif
