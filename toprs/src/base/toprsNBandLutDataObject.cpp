#include <iostream>
#include <sstream>
#include "base/toprsNBandLutDataObject.h"
toprsNBandLutDataObject::toprsNBandLutDataObject(toprs_uint32 numberOfEntries,
                                                 toprs_uint32 numberOfBands,
                                                 toprsDataType bandScalarType,
                                                 toprs_int32 nullPixelIndex)
   :theLut(0),
    theNumberOfEntries(0),
    theNumberOfBands(0),
    theBandScalarType(bandScalarType),
    theNullPixelIndex(nullPixelIndex)
{
   create(numberOfEntries, numberOfBands);
}

toprsNBandLutDataObject::toprsNBandLutDataObject(const toprsNBandLutDataObject& lut)
   :theLut(0),
    theNumberOfEntries(lut.theNumberOfEntries),
    theNumberOfBands(lut.theNumberOfBands),
    theBandScalarType(lut.theBandScalarType),
    theNullPixelIndex(lut.theNullPixelIndex),
    m_entryLabels(lut.m_entryLabels)
{
   *this = lut;
}

toprsNBandLutDataObject::~toprsNBandLutDataObject()
{
   if(theLut)
   {
      delete [] theLut;
      theLut = 0;
   }
   theNumberOfEntries = 0;
   theNumberOfBands   = 0;
   m_entryLabels.clear();
}

void toprsNBandLutDataObject::create(toprs_uint32 numberOfEntries,
                                     toprs_uint32 numberOfBands)
{
   if(theLut)
   {
      delete [] theLut;
      theLut = 0;
   }
   if(numberOfEntries&&numberOfBands)
   {
      theLut = new toprsNBandLutDataObject::LUT_ENTRY_TYPE[numberOfEntries*numberOfBands];
      theNumberOfEntries = numberOfEntries;
      theNumberOfBands   = numberOfBands;
   }
   else
   {
      theNumberOfEntries = 0;
      theNumberOfBands   = 0;
   }
}

toprs_uint32 toprsNBandLutDataObject::findIndex(
   toprsNBandLutDataObject::LUT_ENTRY_TYPE* bandValues)const
{
   return findIndex(bandValues, theNumberOfBands);
}

toprs_uint32 toprsNBandLutDataObject::findIndex(
   toprsNBandLutDataObject::LUT_ENTRY_TYPE* bandValues, toprs_uint32 size)const
{
   toprs_uint32  result = 0;

   if ( (theNumberOfEntries > 0) && (size <= theNumberOfBands) )
   {
      toprs_float64 distance = 1.0/DBL_EPSILON; 
      toprs_uint32 idx = 0;
      toprs_uint32 bandIdx = 0;
      toprsNBandLutDataObject::LUT_ENTRY_TYPE* lutPtr = theLut;

      for(idx = 0; idx < theNumberOfEntries; ++idx,lutPtr+=theNumberOfBands)
      {
         toprs_float64 sumSquare = 0.0;
         
         for(bandIdx = 0; bandIdx < size; ++bandIdx)
         {
            toprs_int64 delta = lutPtr[bandIdx] - bandValues[bandIdx];
            sumSquare += (delta*delta);
         }
         if((toprsNBandLutDataObject::LUT_ENTRY_TYPE)sumSquare == 0)
         {
            return idx;
         }
         else if( sumSquare < distance)
         {
            result = idx;
            distance = sumSquare;
         }
      }
   }

   return result;
}

void toprsNBandLutDataObject::clearLut()
{
   if(theLut)
   {
      memset(theLut, '\0', theNumberOfEntries*theNumberOfBands*sizeof(toprsNBandLutDataObject::LUT_ENTRY_TYPE));
   }
}

void toprsNBandLutDataObject::getMinMax(toprs_uint32 band,
                                        toprsNBandLutDataObject::LUT_ENTRY_TYPE& minValue,
                                        toprsNBandLutDataObject::LUT_ENTRY_TYPE& maxValue)
{
   minValue = 0;
   maxValue = 0;
   toprs_uint32 idx = 0;
   LUT_ENTRY_TYPE *bandPtr = theLut+band;
   if((band < theNumberOfBands)&&
      (theNumberOfEntries > 0))
   {
      minValue = theLut[band];
      maxValue = theLut[band];
      
      for(idx = 0; idx < theNumberOfEntries; ++idx,bandPtr+=theNumberOfBands)
      {
         if((toprs_int32)idx != theNullPixelIndex)
         {
            if(bandPtr[band] < minValue)
            {
               minValue = bandPtr[band];
            }
            if(bandPtr[band] > maxValue)
            {
               maxValue = bandPtr[band];
            }
         }
      }
   }
}

toprs_int32 toprsNBandLutDataObject::getFirstNullAlphaIndex() const
{
   toprs_int32 result = -1;
   if ( (theNumberOfBands == 4) &&  (theNumberOfEntries > 0) )
   {
      toprs_uint32 idx = 0;
      LUT_ENTRY_TYPE* bandPtr = theLut+3; // Index to the first alpha channel.
      for ( idx = 0; idx < theNumberOfEntries; ++idx, bandPtr+=theNumberOfBands )
      {
         if ( *bandPtr == 0 )
         {
            result = *bandPtr;
            break;
         }
      }
   }
   return result;
}

const toprsNBandLutDataObject& toprsNBandLutDataObject::operator =(const toprsNBandLutDataObject& lut)
{
   if(theNumberOfEntries != lut.theNumberOfEntries)
   {
      delete [] theLut;
      theLut = 0;
   }

   theNullPixelIndex = lut.theNullPixelIndex;
   theBandScalarType = lut.theBandScalarType;
   create(lut.theNumberOfEntries,
          lut.theNumberOfBands);

   if(theLut)
   {
      memcpy(theLut, lut.theLut, (theNumberOfEntries*theNumberOfBands)*sizeof(toprsNBandLutDataObject::LUT_ENTRY_TYPE));
   }

   return *this;
}

bool toprsNBandLutDataObject::operator ==(const toprsNBandLutDataObject& lut)const
{
   if(theNumberOfEntries != lut.theNumberOfEntries)
   {
      return false;
   }

   if(!theLut && !lut.theLut) return true;
   if(theNullPixelIndex != lut.theNullPixelIndex) return false;
   if(theBandScalarType != lut.theBandScalarType) return false;
   
   if(theLut&&lut.theLut)
   {
      return (memcmp(theLut, lut.theLut, theNumberOfEntries*theNumberOfBands*sizeof(toprsNBandLutDataObject::LUT_ENTRY_TYPE)) == 0);
   }
   return false;
}



std::vector<std::string> toprsNBandLutDataObject::getEntryLabels(toprs_uint32 band)
{
   std::map<toprs_uint32, std::vector<std::string> >::iterator it = m_entryLabels.find(band);
   if (it != m_entryLabels.end())
   {
      return it->second;
   }
   return std::vector<std::string>();
}

void toprsNBandLutDataObject::setEntryLables(toprs_uint32 band, std::vector<std::string> entryLabels)
{
   m_entryLabels[band] = entryLabels;
}
