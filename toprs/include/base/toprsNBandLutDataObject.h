#ifndef TOPRSNBANDLUTDATAOBJECTH
#define TOPRSNBANDLUTDATAOBJECTH
#include <vector>
#include <map>
#include <string>
#include "common/toprsConstants.h"
#include "common/toprsCommon.h"
class TOPRSDLLEXPORT toprsNBandLutDataObject 
{
public:
   typedef toprs_int32 LUT_ENTRY_TYPE;
   toprsNBandLutDataObject(toprs_uint32 numberOfEntries=0,
                           toprs_uint32 numberOfBands=0,
                           toprsDataType bandScalarType = TOPRS_UINT8,
                           toprs_int32 nullPixelIndex=-1);
   toprsNBandLutDataObject(const toprsNBandLutDataObject& lut);
   virtual ~toprsNBandLutDataObject();
   void create(toprs_uint32 numberOfEntries, toprs_uint32 numberOfBands=3);
   const LUT_ENTRY_TYPE* operator[](toprs_uint32 idx)const
   {
      if(theLut)
      {
         return &theLut[idx*theNumberOfBands];
      }
      
      return 0;
   }
   LUT_ENTRY_TYPE* operator[](toprs_uint32 idx)
   {
      if(theLut)
      {
         return &theLut[idx*theNumberOfBands];
      }
      
      return 0;
   }
   const LUT_ENTRY_TYPE* operator[](toprs_int32 idx)const
   {
      if(theLut)
      {
         return &theLut[idx*theNumberOfBands];
      }
      
      return 0;
   }
   LUT_ENTRY_TYPE* operator[](toprs_int32 idx)
   {
      if(theLut)
      {
         return &theLut[idx*theNumberOfBands];
      }
      
      return 0;
   }
   const LUT_ENTRY_TYPE* operator[](double normalizedIndex)const
   {
      int idx = int(normalizedIndex*theNumberOfEntries);
      if (idx < 0)
      {
         idx = 0;
      }
      return (*this)[idx];
   }
   LUT_ENTRY_TYPE* operator[](double normalizedIndex)
   {
      toprs_uint32 idx = int(normalizedIndex*(theNumberOfEntries-1));
      if (idx >= theNumberOfEntries)
      {
         idx = theNumberOfEntries-1;
      }
      return (*this)[idx];
   }
   
   bool hasNullPixelIndex()const
   {
      return (theNullPixelIndex >= 0);
   }
   void getMinMax(toprs_uint32 band, LUT_ENTRY_TYPE& minValue, LUT_ENTRY_TYPE& maxValue);
   toprs_int32 getFirstNullAlphaIndex() const;
   toprs_int32 getNullPixelIndex()const{return theNullPixelIndex;}
   void setNullPixelIndex(int idx){theNullPixelIndex = idx;}
   toprs_uint32 getNumberOfBands()const{return theNumberOfBands;}
   toprs_uint32 getNumberOfEntries()const{return theNumberOfEntries;}
   toprs_uint32 findIndex(toprs_int32* values) const;
   toprs_uint32 findIndex(toprs_int32* values, toprs_uint32 size) const;
   void clearLut();
   const toprsNBandLutDataObject& operator =(const toprsNBandLutDataObject& lut);
   bool operator ==(const toprsNBandLutDataObject& lut)const;
   bool open(const std::string& lutFile);
   std::vector<std::string> getEntryLabels(toprs_uint32 band);
   void setEntryLables(toprs_uint32 band, std::vector<std::string> entryLabels);

protected:

   LUT_ENTRY_TYPE *theLut;
   toprs_uint32    theNumberOfEntries;
   toprs_uint32    theNumberOfBands;
   toprsDataType   theBandScalarType;
   toprs_int32     theNullPixelIndex;
   std::map<toprs_uint32, std::vector<std::string> >  m_entryLabels;
};
#endif
