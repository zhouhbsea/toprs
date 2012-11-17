#include <iostream>
#include "base/toprsImg.h"

toprsImg::toprsImg( const toprsImg &rhs )
	:theNumberOfBands(rhs.theNumberOfBands),
	theWidth(rhs.theWidth),
	theHeight(rhs.theHeight),
	theDataType(rhs.theDataType),
	theOrigin(rhs.theOrigin),
	theIndexedFlag(rhs.theIndexedFlag),
	theNullPixelV(rhs.theNullPixelV),
	theMinPixelV(rhs.theMinPixelV),
	theMaxPixelV(rhs.theMaxPixelV),
	theAlphaChannelV(rhs.theAlphaChannelV)
{

}

const toprsImg& toprsImg::operator=( const toprsImg &rhs )
{
	if (this != &rhs)
	{
		 theNumberOfBands  =  rhs.theNumberOfBands;
		 theWidth  =  rhs.theWidth;
		 theHeight  =  rhs.theHeight;
		 theDataType = rhs.theDataType;
		 theOrigin = rhs.theOrigin;
		 theIndexedFlag = rhs.theIndexedFlag;
		 theNullPixelV = rhs.theNullPixelV;
		 theMinPixelV = rhs.theMinPixelV;
		 theMaxPixelV = rhs.theMaxPixelV;
		 theAlphaChannelV = rhs.theAlphaChannelV;
	}
	return *this;
}

int toprsImg::w() const
{
	return theWidth;
}

int toprsImg::h() const
{
	return theHeight;
}

void toprsImg::setWidth( int width )
{
	theWidth = width;
}


void toprsImg::setHeight(int height)
{
	theHeight = height;
}

void toprsImg::setWH( int width, int height )
{
	theWidth  = width;
	theHeight = height;
}

void toprsImg::setImgRect( const toprsIRect& rect )
{
	if (rect.hasNAN())
	{
		return;
	}
	int wi = w();
	int hi = h();
	int nw = rect.width();
	int nh = rect.height();

	setOrigin(rect.left_up());
	setWH(nw,nh);
	if ((wi*hi != nw*nh) && (theDataObjectStatus != TOPRS_NULL))
	{
		initialize();
	}
}

void toprsImg::setNullPix( toprs_float64 null_pix )
{
	if (!theNumberOfBands) return;
	theNullPixelV.resize(theNumberOfBands);
	for (int i = 0; i < theNumberOfBands; ++ i)
	{
		theNullPixelV[i] = null_pix;
	}
}

void toprsImg::setNullPix( toprs_float64 null_pix, int band )
{
	if (!theNumberOfBands || band > theNumberOfBands)
	{
		return;
	}
	if (theNullPixelV.size() != theNumberOfBands)
	{
		initializeNullDefault();
	}
	theNullPixelV[band] = null_pix;
}

void toprsImg::setNullPix( const toprs_float64* nullPixArray, int numberOfValues )
{
	if (!nullPixArray || !theNumberOfBands)
	{
		return;
	}
	if (theNullPixelV.size() != theNumberOfBands)
	{
		initializeNullDefault();
	}
	for(int i = 0; (i < numberOfValues) && (i < theNumberOfBands); ++i)
	{
		theNullPixelV[i] = nullPixArray[i];
	}
}

void toprsImg::setMinPix( toprs_float64 min_pix )
{
	if (!theNumberOfBands) return;
	theMinPixelV.resize(theNumberOfBands);
	for (int i = 0; i < theNumberOfBands; ++ i)
	{
		theMinPixelV[i] = min_pix;
	}
}

void toprsImg::setMinPix( toprs_float64 min_pix, int band )
{
	if (!theNumberOfBands || band > theNumberOfBands)
	{
		return;
	}
	if (theMinPixelV.size() != theNumberOfBands)
	{
		initializeMinDefault();
	}
	theMinPixelV[band] = min_pix;
}

void toprsImg::setMinPix( const toprs_float64* minPixArray,int numberOfValues )
{
	if (!minPixArray || !theNumberOfBands)
	{
		return;
	}
	if (theMinPixelV.size() != theNumberOfBands)
	{
		initializeMinDefault();
	}
	for(int i = 0; (i < numberOfValues) && (i < theNumberOfBands); ++i)
	{
		theMinPixelV[i] = minPixArray[i];
	}
}

void toprsImg::setMaxPix( toprs_float64 max_pix )
{
	if (!theNumberOfBands) return;
	theMinPixelV.resize(theNumberOfBands);
	for (int i = 0; i < theNumberOfBands; ++ i)
	{
		theMaxPixelV[i] = max_pix;
	}
}

void toprsImg::setMaxPix( toprs_float64 max_pix, int band )
{
	if (!theNumberOfBands || band > theNumberOfBands)
	{
		return;
	}
	if (theMaxPixelV.size() != theNumberOfBands)
	{
		initializeMaxDefault();
	}
	theMaxPixelV[band] = max_pix;
}

void toprsImg::setMaxPix( const toprs_float64* maxPixArray, int numberOfValues )
{
	if (!maxPixArray || !theNumberOfBands)
	{
		return;
	}
	if (theMaxPixelV.size() != theNumberOfBands)
	{
		initializeMaxDefault();
	}
	for(int i = 0; (i < numberOfValues) && (i < theNumberOfBands); ++i)
	{
		theMaxPixelV[i] = maxPixArray[i];
	}
}

void toprsImg::initializeNullDefault()
{
	if (!theNumberOfBands) return;
	theNullPixelV.resize(theNumberOfBands);
	toprs_float64 val = toprs::defaultNull(getDataType());
	for (int i = 0; i < theNumberOfBands; ++ i)
	{
		theNullPixelV[i] = val;
	}
}

void toprsImg::initializeMinDefault()
{
	if (!theNumberOfBands) return;
	theNullPixelV.resize(theNumberOfBands);
	toprs_float64 val = toprs::defaultMin(getDataType());
	for (int i = 0; i < theNumberOfBands; ++ i)
	{
		theMinPixelV[i] = val;
	}
}

void toprsImg::initializeMaxDefault()
{
	if (!theNumberOfBands) return;
	theNullPixelV.resize(theNumberOfBands);
	toprs_float64 val = toprs::defaultMax(getDataType());
	for (int i = 0; i < theNumberOfBands; ++ i)
	{
		theMaxPixelV[i] = val;
	}
}

void toprsImg::computeMinMaxPix( std::vector<toprs_float64>& minBands, std::vector<toprs_float64>& maxBands ) const
{
	if ((getDataObjectStatus() == TOPRS_NULL) ||
		(getDataObjectStatus() == TOPRS_EMPTY))
	{
		return;
	}
	switch(theDataType)
	{
	case TOPRS_UINT8:
		{
			computeMinMaxPix((toprs_uint8)0,
				minBands,
				maxBands);
			break;
		}
	case TOPRS_SINT8:
		{
			computeMinMaxPix((toprs_sint8)0,
				minBands,
				maxBands);
			break;

		}
	case TOPRS_USHORT11:
	case TOPRS_UINT16:
		{
			computeMinMaxPix((toprs_uint16)0,
				minBands,
				maxBands);
			break;
		}
	case TOPRS_SINT16:
		{
			computeMinMaxPix((toprs_sint16)0,
				minBands,
				maxBands);
			break;
		}
	case TOPRS_UINT32:
		{
			computeMinMaxPix((toprs_uint32)0,
				minBands,
				maxBands);
			break;
		}
	case TOPRS_SINT32:
		{
			computeMinMaxPix((toprs_sint32)0,
				minBands,
				maxBands);
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		{
			computeMinMaxPix((toprs_float32)0.0,
				minBands,
				maxBands);
			break;
		}
	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		{
			computeMinMaxPix((toprs_float64)0.0,
				minBands,
				maxBands);
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			// Shouldn't hit this.
			std::cout
				<< "toprsImg::computeMinPix Unsupported scalar type!"
				<< std::endl;
			break;
		}
	}
}

template <class T>
void toprsImg::computeMinMaxPix( T dummyValue,std::vector<toprs_float64>& minBands,std::vector<toprs_float64>& maxBands ) const
{
	const toprs_uint32 BANDS = getBands();
	const toprs_uint32 SPB   = getSizePerBand(); 

	if( minBands.size() != BANDS ||
		maxBands.size() != BANDS )
	{
		minBands.resize(BANDS);
		maxBands.resize(BANDS);
		for (int band = 0; band < BANDS; ++band)
		{
			// Set min to max and max to min for starters.
			minBands[band] = getMaxPix(band);
			maxBands[band] = getMinPix(band);
		}
	}

	for(int band = 0; band < BANDS; ++band)
	{
		const T* bandBuffer = (const T*)getBuf(band);
		if(bandBuffer)
		{
			const T NP   = static_cast<T>(getNullPix(band));
			toprs_float64 currentMin = minBands[band];
			toprs_float64 currentMax = maxBands[band];
			for(int offset = 0; offset < SPB; ++offset)
			{
				T p = bandBuffer[offset];
				if(p != NP)
				{
					if(p < currentMin)
					{
						currentMin = p;
					}
					else if(p > currentMax)
					{
						currentMax = p;
					}
				}
			}
			minBands[band] = currentMin;
			maxBands[band] = currentMax;
		}
	}
}

void toprsImg::computeMinMaxNullPix(std::vector<toprs_float64>& minBands, std::vector<toprs_float64>& maxBands, std::vector<toprs_float64>& nullBands ) const
{
	if ((getDataObjectStatus() == TOPRS_NULL) ||
		(getDataObjectStatus() == TOPRS_EMPTY))
	{
		return;
	}
	switch(theDataType)
	{
	case TOPRS_UINT8:
		{
			computeMinMaxNullPix((toprs_uint8)0,
				minBands,
				maxBands,
				nullBands);
			break;
		}
	case TOPRS_SINT8:
		{
			computeMinMaxNullPix((toprs_sint8)0,
				minBands,
				maxBands,
				nullBands);
			break;

		}
	case TOPRS_USHORT11:
	case TOPRS_UINT16:
		{
			computeMinMaxNullPix((toprs_uint16)0,
				minBands,
				maxBands,
				nullBands);
			break;
		}
	case TOPRS_SINT16:
		{
			computeMinMaxNullPix((toprs_sint16)0,
				minBands,
				maxBands,
				nullBands);
			break;
		}
	case TOPRS_UINT32:
		{
			computeMinMaxNullPix((toprs_uint32)0,
				minBands,
				maxBands,
				nullBands);
			break;
		}
	case TOPRS_SINT32:
		{
			computeMinMaxNullPix((toprs_sint32)0,
				minBands,
				maxBands,
				nullBands);
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		{
			computeMinMaxNullPix((toprs_float32)0.0,
				minBands,
				maxBands,
				nullBands);
			break;
		}
	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		{
			computeMinMaxNullPix((toprs_float64)0.0,
				minBands,
				maxBands,
				nullBands);
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			// Shouldn't hit this.
			std::cout
				<< "toprsImg::computeMinPix Unsupported scalar type!"
				<< std::endl;
			break;
		}
	}
}

template <class T>
void toprsImg::computeMinMaxNullPix( T dummyValue,
	                               std::vector<toprs_float64>& minBands,
	                               std::vector<toprs_float64>& maxBands,
								   std::vector<toprs_float64>& nullBands) const
{
	const toprs_uint32 BANDS = getBands();
	const toprs_uint32 SPB   = getSizePerBand(); 
	T NP = static_cast<T>(toprs::defaultNull(getDataType()));

	if( minBands.size() != BANDS ||
		maxBands.size() != BANDS ||
		nullBands.size() != BANDS)
	{
		minBands.resize(BANDS);
		maxBands.resize(BANDS);
		nullBands.resize(BANDS);
		for (int band = 0; band < BANDS; ++band)
		{
			// Set min to max and max to min for starters.
			minBands[band] = getMaxPix(band);
			maxBands[band] = getMinPix(band);
			nullBands[band] = getMaxPix(band);
		}
	}

	for(int band = 0; band < BANDS; ++band)
	{
		const T* bandBuffer = (const T*)getBuf(band);
		if(bandBuffer)
		{
			toprs_float64 currentMin = minBands[band];
			toprs_float64 currentMax = maxBands[band];
			toprs_float64 currentNull = nullBands[band];
			for(int offset = 0; offset < SPB; ++offset)
			{
				T p = bandBuffer[offset];

				if(p != NP)
				{

					if (p > currentMax)
					{
						currentMax = p;
					}
					if(p < currentMin)
					{
						if (p > currentNull)
						{
							currentMin = p;
						}
						else if(p < currentNull)
						{
							currentMin = currentNull;
						}
						currentNull = p;
					}
				}
			}
			minBands[band] = currentMin;
			maxBands[band] = currentMax;
			nullBands[band]= currentNull;
		}
	}
}
toprs_float64 toprsImg::getPix( const toprsIpt& pt, int band /*= 0*/ ) const
{
	toprsIpt relative(pt.x - theOrigin.x,
		              pt.y - theOrigin.y);
	return getPix(w() * relative.y + relative.x, band);

}

toprs_float64 toprsImg::getPix( int offset, int band /*= 0*/ ) const
{
	switch(theDataType)
	{
	case TOPRS_UINT8:
		{
			const toprs_uint8* buf = getUcharBuf(band);
			if(buf)
			{
				return (toprs_float64)buf[offset];
			}
		}
	case TOPRS_SINT8:
		{
			const toprs_sint8* buf = static_cast<const toprs_sint8*>(getBuf(band));
			if(buf)
			{
				return (toprs_float64)buf[offset];
			}
		}
	case TOPRS_USHORT11:
	case TOPRS_UINT16:
		{
			const toprs_uint16* buf = getUshortBuf(band);
			if(buf)
			{
				return (toprs_float64)buf[offset];
			}
			break;
		}
	case TOPRS_SINT16:
		{
			const toprs_sint16* buf = getSshortBuf(band);
			if(buf)
			{
				return (toprs_float64)buf[offset];
			}
			break;
		}
	case TOPRS_SINT32:
		{
			const toprs_sint32* buf = static_cast<const toprs_sint32*>(getBuf(band));
			if(buf)
			{
				return (toprs_float64)buf[offset];
			}
			break;
		}
	case TOPRS_UINT32:
		{
			const toprs_uint32* buf = static_cast<const toprs_uint32*>(getBuf(band));
			if(buf)
			{
				return (toprs_float64)buf[offset];
			}
			break;
		}
	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		{
			const toprs_float64* buf = getDoubleBuf(band);
			if(buf)
			{
				return (toprs_float64)buf[offset];
			}      
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		{
			const toprs_float32* buf = getFloatBuf(band);
			if(buf)
			{
				return (toprs_float64)buf[offset];
			}
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		// Shouldn't hit this.
		std::cout
			<< "toprsImg::fill Unsupported scalar type!"
			<< std::endl;   
	}

	return 0.0;
}

void toprsImg::fill( int band, toprs_float64 val )
{
	void* s = getBuf(band);
	if (s == nullptr) return;
    int size_in_pixels = w()*h();
	switch (getDataType())
	{
	case TOPRS_UINT8:
		{
			toprs_uint8* p = getUcharBuf(band);
			toprs_uint8 np = static_cast<toprs_uint8>(val);
			for (int i=0; i<size_in_pixels; i++) p[i] = np;
			break;
		}
	case TOPRS_SINT8:
		{
			toprs_sint8* p = static_cast<toprs_sint8*>(getBuf(band));
			toprs_sint8 np = static_cast<toprs_sint8>(val);
			for (int i=0; i<size_in_pixels; i++) p[i] = np;

			break;
		}
	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		{
			toprs_uint16* p = getUshortBuf(band);
			toprs_uint16 np = static_cast<toprs_uint16>(val);
			for (toprs_uint32 i=0; i<size_in_pixels; i++) p[i] = np;

			break;
		}
	case TOPRS_SINT16:
		{
			toprs_sint16* p = getSshortBuf(band);
			toprs_sint16 np = static_cast<toprs_sint16>(val);
			for (toprs_uint32 i=0; i<size_in_pixels; i++) p[i] = np;

			break;
		}
	case TOPRS_UINT32:
		{
			toprs_uint32* p = static_cast<toprs_uint32*>(getBuf(band));
			toprs_uint32 np = static_cast<toprs_uint32>(val);
			for (toprs_uint32 i=0; i<size_in_pixels; i++) p[i] = np;

			break;
		}
	case TOPRS_SINT32:
		{
			toprs_sint32* p = static_cast<toprs_sint32*>(getBuf(band));
			toprs_sint32 np = static_cast<toprs_sint32>(val);
			for (toprs_uint32 i=0; i<size_in_pixels; i++) p[i] = np;

			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		{
			toprs_float32* p = getFloatBuf(band);
			toprs_float32 np = static_cast<toprs_float32>(val);
			for (toprs_uint32 i=0; i<size_in_pixels; i++) p[i] = np;

			break;
		}
	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		{
			toprs_float64* p = getDoubleBuf(band);
			toprs_float64 np = static_cast<toprs_float64>(val);
			for (toprs_uint32 i=0; i<size_in_pixels; i++) p[i] = np;

			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
         std::cout << 
			 "toprsImg::makeBlank File"
			 <<__FILE__
			 << "line \n"
			 << __LINE__
			 <<"Invalid scalar type: "
		     << getDataType();
		return;
	}
	setDataObjectStatus(TOPRS_EMPTY);
}

void toprsImg::fill( toprs_float64 val )
{
	int valueNullCount = 0;
	for(int i = 0; i < getBands(); ++i)
	{
		if (val == theNullPixelV[i])
		{
			++valueNullCount;
		}
		fill(i, val);
	}
	if (valueNullCount==0)
	{
		setDataObjectStatus(TOPRS_FULL);
	}
	else if(valueNullCount==getBands())
	{
		setDataObjectStatus(TOPRS_EMPTY);
	}
	else
	{
		setDataObjectStatus(TOPRS_PARTIAL);
	}
}

bool toprsImg::isNull( int offset ) const
{
	int numberOfBands = getBands();
	int band = 0;
	if(!getBuf()) return true;
	switch(getDataType())
	{
	case TOPRS_UINT8:
		{
			for(band = 0; band < numberOfBands; ++band)  
			{
				const toprs_uint8* buf = static_cast<const toprs_uint8*>(getBuf(band))+offset;
				if((*buf) != (toprs_uint8)getNullPix(band))
				{
					return false;
				}
			}
			break;
		}
	case TOPRS_SINT8:
		{
			for(band = 0; band < numberOfBands; ++band)  
			{
				const toprs_sint8* buf = static_cast<const toprs_sint8*>(getBuf(band))+offset;
				if((*buf) != (toprs_uint8)getNullPix(band))
				{
					return false;
				}
			}
			break;
		}
	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		{
			for(band = 0; band < numberOfBands; band++)
			{
				const toprs_uint16* buf = static_cast<const toprs_uint16*>(getBuf(band))+offset;
				if((*buf) != (toprs_uint16)getNullPix(band))
				{
					return false;
				}
			}
			break;
		}
	case TOPRS_SINT16:
		{
			for(band = 0; band < numberOfBands; band++)
			{
				const toprs_sint16* buf = static_cast<const toprs_sint16*>(getBuf(band))+offset;
				if((*buf) != (toprs_sint16)getNullPix(band))
				{
					return false;
				}
			}
			break;
		}
	case TOPRS_UINT32:
		{
			for(band = 0; band < numberOfBands; band++)
			{
				const toprs_uint32* buf = static_cast<const toprs_uint32*>(getBuf(band))+offset;
				if((*buf) != (toprs_uint32)getNullPix(band))
				{
					return false;
				}
			}
			break;
		}
	case TOPRS_SINT32:
		{
			for(band = 0; band < numberOfBands; band++)
			{
				const toprs_sint32* buf = static_cast<const toprs_sint32*>(getBuf(band))+offset;
				if((*buf) != (toprs_sint32)getNullPix(band))
				{
					return false;
				}
			}
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
		{
			for(band = 0; band < numberOfBands; band++)
			{
				const toprs_float32* buf = static_cast<const toprs_float32*>(getBuf(band))+offset;
				if((*buf) != (toprs_float32)getNullPix(band))
				{
					return false;
				}
			}
			break;
		}
	case TOPRS_NORMALIZED_DOUBLE:
		{
			for(band = 0; band < numberOfBands; band++)
			{
				const toprs_float64* buf = static_cast<const toprs_float64*>(getBuf(band))+offset;
				if((*buf) != 0.0)
				{
					return false;
				}
			}
			break;
		}
	case TOPRS_FLOAT64:
		{
			for(band = 0; band < numberOfBands; band++)
			{
				const toprs_float64* buf = static_cast<const toprs_float64*>(getBuf(band))+offset;
				if((*buf) != getNullPix(band))
				{
					return false;
				}
			}
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			std::cout
				<< "toprsImg::isNull Unsupported scalar type!"
				<< std::endl;
		}
	}
	return true;
}

bool toprsImg::isNull( int offset, int band ) const
{
	switch(getDataType())
	{
	case TOPRS_UINT8:
		{
			const toprs_uint8* buf =
				static_cast<const toprs_uint8*>(getBuf(band))+offset;

			if((*buf) != (toprs_uint8)getNullPix(band))
			{
				return false;
			}
			break;
		}
	case TOPRS_SINT8:
		{
			const toprs_sint8* buf =
				static_cast<const toprs_sint8*>(getBuf(band))+offset;

			if((*buf) != (toprs_sint8)getNullPix(band))
			{
				return false;
			}
			break;
		}
	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		{
			const toprs_uint16* buf =
				static_cast<const toprs_uint16*>(getBuf(band))+offset;
			if((*buf) != (toprs_uint16)getNullPix(band))
			{
				return false;
			}
			break;
		}
	case TOPRS_SINT16:
		{
			const toprs_sint16* buf =
				static_cast<const toprs_sint16*>(getBuf(band))+offset;
			if((*buf) != (toprs_sint16)getNullPix(band))
			{
				return false;
			}
			break;
		}
	case TOPRS_UINT32:
		{
			const toprs_uint32* buf =
				static_cast<const toprs_uint32*>(getBuf(band))+offset;
			if((*buf) != (toprs_uint32)getNullPix(band))
			{
				return false;
			}
			break;
		}
	case TOPRS_SINT32:
		{
			const toprs_sint32* buf =
				static_cast<const toprs_sint32*>(getBuf(band))+offset;
			if((*buf) != (toprs_sint32)getNullPix(band))
			{
				return false;
			}
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		{
			const toprs_float32* buf = static_cast<const toprs_float32*>(getBuf(band))+offset;
			if((*buf) != (toprs_float32)getNullPix(band))
			{
				return false;
			}
			break;
		}
	case TOPRS_FLOAT64:
	case TOPRS_NORMALIZED_DOUBLE:
		{
			const toprs_float64* buf = static_cast<const toprs_float64*>(getBuf(band))+offset;
			if((*buf) != getNullPix(band))
			{
				return false;
			}
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			//ERROR
			std::cout
				<< "toprsImg::isNull Unsupported scalar type!"
				<< std::endl;
		}
	}
	return true;
}

bool toprsImg::isNull(const toprsIpt& pt ) const
{
	int xNew = (pt.x - theOrigin.x);
	int yNew = (pt.y - theOrigin.y);
	if(xNew < 0 || xNew >= static_cast<int>(w()) ||
		yNew < 0 || yNew >= static_cast<int>(h()) )
	{
		return true;
	}
	int offset = w()*yNew + xNew;

	return isNull(offset);
}

bool toprsImg::isNull( const toprsIpt& pt, int band ) const
{
	int xNew = (pt.x - theOrigin.x);
	int yNew = (pt.y - theOrigin.y);
	if(xNew < 0 || xNew >= static_cast<int>(w()) ||
		yNew < 0 || yNew >= static_cast<int>(h()) )
	{
		return true;
	}
	int offset = w()*yNew + xNew;

	return isNull(offset, band);
}

void toprsImg::setNull( int offset )
{
	int  numberOfBands = getBands();
	int  band=0;
	switch(getDataType())
	{
	case TOPRS_UINT8:
		{
			for(band = 0; band < numberOfBands; ++band)  
			{
				toprs_uint8* buf = static_cast<toprs_uint8*>(getBuf(band))+offset;
				*buf       = (toprs_uint8)getNullPix(band);
			}
			break;
		}
	case TOPRS_SINT8:
		{
			for(band = 0; band < numberOfBands; ++band)  
			{
				toprs_sint8* buf = static_cast<toprs_sint8*>(getBuf(band))+offset;
				*buf       = (toprs_sint8)getNullPix(band);
			}
			break;
		}
	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		{
			for(band = 0; band < numberOfBands; band++)
			{
				toprs_uint16* buf = static_cast<toprs_uint16*>(getBuf(band))+
					offset;
				*buf = (toprs_uint16)getNullPix(band);
			}
			break;
		}
	case TOPRS_SINT16:
		{
			for(band = 0; band < numberOfBands; band++)
			{
				toprs_sint16* buf = static_cast<toprs_sint16*>(getBuf(band))+
					offset;
				*buf = (toprs_sint16)getNullPix(band);
			}
			break;
		}
	case TOPRS_UINT32:
		{
			for(band = 0; band < numberOfBands; band++)
			{
				toprs_uint32* buf = static_cast<toprs_uint32*>(getBuf(band))+
					offset;
				*buf = (toprs_uint32)getNullPix(band);
			}
			break;
		}
	case TOPRS_SINT32:
		{
			for(band = 0; band < numberOfBands; band++)
			{
				toprs_sint32* buf = static_cast<toprs_sint32*>(getBuf(band))+
					offset;
				*buf = (toprs_sint32)getNullPix(band);
			}
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		{
			for(band = 0; band < numberOfBands; band++)
			{
				toprs_float32* buf = static_cast<toprs_float32*>(getBuf(band))+offset;
				*buf = (toprs_float32)getNullPix(band);
			}
			break;
		}
	case TOPRS_FLOAT64:
	case TOPRS_NORMALIZED_DOUBLE:
		{
			for(band = 0; band < numberOfBands; band++)
			{
				toprs_float64* buf = static_cast<toprs_float64*>(getBuf(band))+offset;
				*buf = getNullPix(band);
			}
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			//ERROR
             std::cout
				<< "toprsImg::isNull Unsupported scalar type!"
				<< std::endl;
		}
	}
}

void toprsImg::setNull( int offset, int band )
{
	switch(getDataType())
	{
	case TOPRS_UINT8:
		{
			toprs_uint8* buf = static_cast<toprs_uint8*>(getBuf(band))+offset;
			*buf       = (toprs_uint8)getNullPix(band);
			break;
		}
	case TOPRS_SINT8:
		{
			toprs_sint8* buf = static_cast<toprs_sint8*>(getBuf(band))+offset;
			*buf       = (toprs_sint8)getNullPix(band);
			break;
		}
	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		{
			toprs_uint16* buf = static_cast<toprs_uint16*>(getBuf(band))+offset;
			*buf = (toprs_uint16)getNullPix(band);
			break;
		}
	case TOPRS_SINT16:
		{
			toprs_sint16* buf = static_cast<toprs_sint16*>(getBuf(band))+offset;
			*buf = (toprs_sint16)getNullPix(band);
			break;
		}
	case TOPRS_UINT32:
		{
			toprs_uint32* buf = static_cast<toprs_uint32*>(getBuf(band))+offset;
			*buf       = (toprs_uint32)getNullPix(band);
			break;
		}
	case TOPRS_SINT32:
		{
			toprs_sint32* buf = static_cast<toprs_sint32*>(getBuf(band))+offset;
			*buf       = (toprs_sint32)getNullPix(band);
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		{
			toprs_float32* buf = static_cast<toprs_float32*>(getBuf(band))+offset;
			*buf = (toprs_float32)getNullPix(band);
			break;
		}
	case TOPRS_FLOAT64:
	case TOPRS_NORMALIZED_DOUBLE:
		{
			toprs_float64* buf = static_cast<toprs_float64*>(getBuf(band))+offset;
			*buf = getNullPix(band);
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			//ERROR
			std::cout
				<< "toprsImg::isNull Unsupported scalar type!"
				<< std::endl;
		}
	}
}

void toprsImg::setNull( const toprsIpt& pt )
{
	int xNew = (pt.x - theOrigin.x);
	int yNew = (pt.y - theOrigin.y);

	if(xNew < 0 || xNew >= (int)w() ||
		yNew < 0 || yNew >= (int)h() )
	{
		return;
	}
	int offset = ((int)w())*yNew + xNew;

	setNull(offset);
}

void toprsImg::setNull( const toprsIpt& pt, int band )
{
	int xNew = (pt.x - theOrigin.x);
	int yNew = (pt.y - theOrigin.y);

	if(xNew < 0 || xNew >= (int)w() ||
		yNew < 0 || yNew >= (int)h() )
	{
		return;
	}
	int offset = ((int)w())*yNew + xNew;

	setNull(offset,band);
}

bool toprsImg::isValidBand( int band ) const
{
	return band < getBands();
}

toprsDataObjectStatus toprsImg::validate() const
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		{
			return validate(toprs_uint8(0));
		}
	case TOPRS_SINT8:
		{
			return validate(toprs_sint8(0));
		}

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		{
			return validate(toprs_uint16(0));
		}  
	case TOPRS_SINT16:
		{
			return validate(toprs_sint16(0));
		}

	case TOPRS_UINT32:
		{
			return validate(toprs_uint32(0));
		}  
	case TOPRS_SINT32:
		{
			return validate(toprs_sint32(0));
		}  
	case TOPRS_FLOAT32:
	case TOPRS_NORMALIZED_FLOAT:
		{
			return validate(toprs_float32(0.0));
		}

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		{
			return validate(toprs_float64(0.0));
		}  
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			setDataObjectStatus(TOPRS_STATUS_UNKNOWN);
			break;
		}
	}
	return TOPRS_STATUS_UNKNOWN;
}

template <class T>
toprsDataObjectStatus toprsImg::validate( T dummyTemplate ) const
{
	if (theDataBufferV.size() == 0)
	{
		setDataObjectStatus(TOPRS_NULL);
		return TOPRS_NULL;
	}
	int count = 0;
	const int SIZE            = getSize();
	const int SIZE_PER_BAND   = getSizePerBand();
	const int NUMBER_OF_BANDS = getBands();

	for(int band = 0; band < NUMBER_OF_BANDS; ++band)
	{
		const T NP = static_cast<T>(theNullPixelV[band]);
		const T* p = static_cast<const T*>(getBuf(band));

		for (int i = 0; i < SIZE_PER_BAND; ++i)
		{
			// check if the band is null
			if (p[i] != NP) ++count;         
		}
	}

	if (!count)
		setDataObjectStatus(TOPRS_EMPTY);
	else if (count == SIZE)
		setDataObjectStatus(TOPRS_FULL);
	else
		setDataObjectStatus(TOPRS_PARTIAL);
	return getDataObjectStatus();
}



toprsImg::PImg toprsImg::newNormalizedFloat() const
{
	PImg result(new toprsImg(
		TOPRS_NORMALIZED_FLOAT,
		this->getBands(),
		this->w(),
		this->h()));
	result->initialize();
	converToNormalizedFloat(result.get());
	return result;
}

void toprsImg::converToNormalizedFloat( toprsImg* result ) const
{
	if(!result)
	{
		return;
	}
	// make sure that the types and width and height are
	// good.
	if( (result->getDataType() != TOPRS_NORMALIZED_FLOAT) ||
		(result->getBands() != this->getBands())||
		(result->w() != this->w()) ||
		(result->h() != this->h())||
		(result->getDataObjectStatus() == TOPRS_NULL) ||
		(getDataObjectStatus() == TOPRS_NULL))
	{
		return;
	}
	copyTileToNormalizedBuffer((toprs_float32*)result->getBuf());//xizhi
	result->setDataObjectStatus(getDataObjectStatus());
}




void toprsImg::getNormalizedFloat( int offset, int bandNumber, toprs_float32& result ) const
{
	// Make sure that the types and width and height are good.
	if( (getDataObjectStatus() == TOPRS_NULL) &&
		(bandNumber < getBands()) )
	{
		return;
	}

	toprs_float32 delta =  theMaxPixelV[bandNumber] - theMinPixelV[bandNumber];

	switch (getDataType())
	{
	case TOPRS_UINT8:
		{
			const unsigned char* sourceBuf = getUcharBuf(bandNumber);
			result = (sourceBuf[offset] - theMinPixelV[bandNumber])/delta;
			break;
		}
	case TOPRS_SINT8:
		{
			const toprs_sint8* sourceBuf = static_cast<const toprs_sint8*>(getBuf(bandNumber));
			result = (sourceBuf[offset] - theMinPixelV[bandNumber])/delta;
			break;
		}
	case TOPRS_USHORT11:
	case TOPRS_UINT16:
		{
			const toprs_uint16* sourceBuf = getUshortBuf(bandNumber);
			result = (sourceBuf[offset] - theMinPixelV[bandNumber])/delta;
			break;
		}
	case TOPRS_SINT16:
		{
			const toprs_sint16* sourceBuf = getSshortBuf(bandNumber);
			result = (sourceBuf[offset] - theMinPixelV[bandNumber])/delta;
			break;
		}
	case TOPRS_UINT32:
		{
			const toprs_uint32* sourceBuf =
				static_cast<const toprs_uint32*>(getBuf(bandNumber));
			result = (sourceBuf[offset] - theMinPixelV[bandNumber])/delta;
			break;
		}
	case TOPRS_SINT32:
		{
			const toprs_sint32* sourceBuf = static_cast<const toprs_sint32*>(getBuf(bandNumber));
			result = (sourceBuf[offset] - theMinPixelV[bandNumber])/delta;
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		{
			const toprs_float32* sourceBuf = getFloatBuf(bandNumber);
			result = (sourceBuf[offset] - theMinPixelV[bandNumber])/delta;
			break;
		}
	case TOPRS_FLOAT64:
	case TOPRS_NORMALIZED_DOUBLE:
		{
			const toprs_float64* sourceBuf = getDoubleBuf(bandNumber);
			result = (sourceBuf[offset] - theMinPixelV[bandNumber])/delta;
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			// Shouldn't hit this.
			std::cout
				<< "toprsImg::setNormalizedFloat Unsupported scalar type!"
				<< std::endl;

			result = 0;
		}
	}
}

//void toprsImg::updateMinMax()
//{
//	std::vector<toprs_float64 > minV;
//	std::vector<toprs_float64 > maxV;
//	computeMinMaxPix(minV, maxV);
//	setMinPix((toprs_float64*)&(*(minV.begin())), theNumberOfBands);
//    setMaxPix((toprs_float64*)&(*(maxV.begin())), theNumberOfBands);
//}

//void toprsImg::updateMinMaxNull()
//{
//	std::vector<toprs_float64 > minV;
//	std::vector<toprs_float64 > maxV;
//	std::vector<toprs_float64 > nullV;
//	computeMinMaxNullPix(minV, maxV, nullV);
//	setMinPix((toprs_float64*)&(*(minV.begin())), theNumberOfBands);
//	setMaxPix((toprs_float64*)&(*(maxV.begin())), theNumberOfBands);
//	setNullPix((toprs_float64*)&(*(nullV.begin())), theNumberOfBands);
//}

void toprsImg::setNormalizedFloat( int offset, int bandNumber, toprs_float32 input ) 
{
	// Make sure that the types and width and height are good.
	if( (getDataObjectStatus() == TOPRS_NULL)&&
		(bandNumber < getBands()) )
	{
		return;
	}

	toprs_float32 delta =  theMaxPixelV[bandNumber] - theMinPixelV[bandNumber];
	switch (getDataType())
	{
	case TOPRS_UINT8:
		{
			toprs_uint8* sourceBuf = getUcharBuf(bandNumber);
			sourceBuf[offset] =
				static_cast<toprs_uint8>(theMinPixelV[bandNumber] +
				delta*input);

			break;
		}
	case TOPRS_SINT8:
		{
			toprs_sint8* sourceBuf = static_cast<toprs_sint8*>(getBuf(bandNumber));
			sourceBuf[offset] =
				static_cast<toprs_sint8>(theMinPixelV[bandNumber] +
				delta*input);
			break;
		}
	case TOPRS_USHORT11:
	case TOPRS_UINT16:
		{
			toprs_uint16* sourceBuf = getUshortBuf(bandNumber);
			sourceBuf[offset] =
				static_cast<toprs_uint16>(theMinPixelV[bandNumber] +
				delta*input);
			break;
		}
	case TOPRS_SINT16:
		{
			toprs_sint16* sourceBuf = getSshortBuf(bandNumber);
			sourceBuf[offset] =
				static_cast<toprs_sint16>(theMinPixelV[bandNumber] +
				delta*input);
			break;
		}
	case TOPRS_UINT32:
		{
			toprs_uint32* sourceBuf =
				static_cast<toprs_uint32*>(getBuf(bandNumber));
			sourceBuf[offset] =
				static_cast<toprs_uint32>(theMinPixelV[bandNumber] +
				delta*input);
			break;
		}
	case TOPRS_SINT32:
		{
			toprs_sint32* sourceBuf = static_cast<toprs_sint32*>(getBuf(bandNumber));
			sourceBuf[offset] =
				static_cast<toprs_sint32>(theMinPixelV[bandNumber] +
				delta*input);
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		{
			toprs_float32* sourceBuf = getFloatBuf(bandNumber);
			sourceBuf[offset] =
				static_cast<toprs_float32>(theMinPixelV[bandNumber] +
				delta*input);
			break;
		}
	case TOPRS_FLOAT64:
	case TOPRS_NORMALIZED_DOUBLE:
		{
			toprs_float64* sourceBuf = getDoubleBuf(bandNumber);
			sourceBuf[offset] =
				static_cast<toprs_float64>(theMinPixelV[bandNumber]
			+ delta*input);
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		// Shouldn't hit this.
		std::cout
			<< "toprsImg::setNormalizedFloat Unsupported scalar type!"
			<< std::endl;
	}
}

toprsImg::PImg toprsImg::newNormalizedDouble() const
{
	PImg result(new toprsImg(
		TOPRS_NORMALIZED_DOUBLE,
		this->getBands(),
		this->w(),
		this->h()));
	result->initialize();
	converToNormalizedDouble(result.get());
	return result;
}

void toprsImg::converToNormalizedDouble( toprsImg* result ) const
{
	if(!result)
	{
		return;
	}
	// make sure that the types and width and height are good.
	if( (result->getDataType() != TOPRS_NORMALIZED_DOUBLE) ||
		(result->getBands() != this->getBands())||
		(result->w() != this->w()) ||
		(result->h() != this->h())||
		(result->getDataObjectStatus() == TOPRS_NULL) ||
		(getDataObjectStatus() == TOPRS_NULL))
	{
		return;
	}

	copyTileToNormalizedBuffer((toprs_float64*)result->getBuf());//xizhi
	result->setDataObjectStatus(getDataObjectStatus());
}

void toprsImg::unNormalizedInput( toprsImg* normalizedInput )//should set min and max
{
	if((normalizedInput->getDataType() != TOPRS_NORMALIZED_FLOAT) &&
		(normalizedInput->getDataType() != TOPRS_NORMALIZED_DOUBLE) )
	{
		//ERROR
		return;
	}

	if(normalizedInput->getDataType() == TOPRS_NORMALIZED_DOUBLE)
	{
		copyNormalizedBufferToTile((toprs_float64*)normalizedInput->getBuf());
	}
	else
	{
		copyNormalizedBufferToTile((toprs_float32*)normalizedInput->getBuf());
	}
}

template <class T>
void toprsImg::copyNormalizedBufferToTile( T dummyTemplate, toprs_float32* buf )
{
	const toprs_uint32 SIZE  = getSizePerBand();
	const toprs_uint32 BANDS = getBands();

	for(int band = 0; band < BANDS; ++band)
	{
		const toprs_float64 MIN_PIX = getMinPix(band);
		const toprs_float64 MAX_PIX = getMaxPix(band);
		const toprs_float64 RANGE   = (MAX_PIX-MIN_PIX);
		const T NP                  = (T)getNullPix(band);

		toprs_float32* s = buf + (band*SIZE); // source
		T* d   = (T*)getBuf(band); // destination

		for(toprs_uint32 offset = 0; offset < SIZE; ++offset)
		{
			const toprs_float64 P = s[offset];
			if(P != 0.0)
			{
				toprs_float64 test = MIN_PIX + RANGE*P;
				if(test > MAX_PIX) test = MAX_PIX;
				d[offset] = (T)test;
			}
			else
			{
				d[offset] = NP;
			}
		}
	}
}

template <class T>
void toprsImg::copyNormalizedBufferToTile( T dummyTemplate, toprs_float64* buf )
{
	const toprs_uint32 SIZE  = getSizePerBand();
	const toprs_uint32 BANDS = getBands();

	for(int band = 0; band < BANDS; ++band)
	{
		const toprs_float64 MIN_PIX = getMinPix(band);
		const toprs_float64 MAX_PIX = getMaxPix(band);
		const toprs_float64 RANGE   = (MAX_PIX-MIN_PIX);
		const T NP                  = (T)getNullPix(band);
		toprs_float64* s = buf + (band*SIZE); // source
		T* d   = (T*)getBuf(band); // destination

		for(toprs_uint32 offset = 0; offset < SIZE; ++offset)
		{
			const toprs_float64 P = s[offset];
			if(P != 0.0)
			{
				toprs_float64 test = MIN_PIX + RANGE*P;
				if(test > MAX_PIX) test = MAX_PIX;
				d[offset] = (T)test;
			}
			else
			{
				d[offset] = NP;
			}
		}
	}
}


template <class T>
void toprsImg::copyNormalizedBufferToTile( T dummyTemplate, int band,toprs_float64* buf )
{
	const toprs_uint32  SIZE    = getSizePerBand();
	const toprs_float64 MIN_PIX = getMinPix(band);
	const toprs_float64 MAX_PIX = getMaxPix(band);
	const toprs_float64 RANGE   = (MAX_PIX-MIN_PIX);
	const T             NP      = (T)getNullPix(band);

	toprs_float64* s = buf; // source
	T* d   = (T*)getBuf(band); // destination

	for(int offset = 0; offset < SIZE; ++offset)
	{
		const toprs_float64 P = s[offset];
		if(P != 0.0)
		{
			toprs_float64 test = MIN_PIX + RANGE*P;
			if(test > MAX_PIX) test = MAX_PIX;
			d[offset] = (T)test;
		}
		else
		{
			d[offset] = NP;
		}
	}
}

template <class T>
void toprsImg::copyNormalizedBufferToTile( T dummyTemplate, int band,toprs_float32* buf )
{
	const toprs_uint32  SIZE    = getSizePerBand();
	const toprs_float64 MIN_PIX = getMinPix(band);
	const toprs_float64 MAX_PIX = getMaxPix(band);
	const toprs_float64 RANGE   = (MAX_PIX-MIN_PIX);
	const T             NP      = (T)getNullPix(band);

	toprs_float32* s = buf; // source
	T* d   = (T*)getBuf(band); // destination

	for(int offset = 0; offset < SIZE; ++offset)
	{
		const toprs_float64 P = s[offset];
		if(P != 0.0)
		{
			toprs_float64 test = MIN_PIX + RANGE*P;
			if(test > MAX_PIX) test = MAX_PIX;
			d[offset] = (T)test;
		}
		else
		{
			d[offset] = NP;
		}
	}
}
template <class T>
void toprsImg::copyTileToNormalizedBuffer(T /* dummyTemplate */,toprs_float32* buf) const
{
	const toprs_uint32 SIZE  = getSizePerBand();
	const toprs_uint32 BANDS = getBands();

	for(toprs_uint32 band = 0; band < BANDS; ++band)
	{
		const toprs_float64 MIN_PIX = getMinPix(band);
		const toprs_float64 MAX_PIX = getMaxPix(band);
		const toprs_float64 RANGE   = (MAX_PIX-MIN_PIX);
		const toprs_float64 NP      = getNullPix(band);

		const T* s = (T*)getBuf(band);  // source
		toprs_float32* d = (toprs_float32*)(buf + (band*SIZE));  // destination

		for(toprs_uint32 offset = 0; offset < SIZE; ++offset)
		{
			toprs_float64 p = s[offset];
			if(p != NP)
			{
				if( p == MIN_PIX)
				{
					d[offset] = TOPRS_DEFAULT_MIN_PIX_NORM_FLOAT;
				}
				else
				{
					d[offset] = (p-MIN_PIX)/RANGE;
				}
			}
			else
			{
				d[offset] = 0.0;
			}
		}
	}   
}
template <class T>
void toprsImg::copyTileToNormalizedBuffer( T dummyTemplate, toprs_float64* buf ) const
{
	const toprs_uint32 SIZE  = getSizePerBand();
	const toprs_uint32 BANDS = getBands();

	for(int band = 0; band < BANDS; ++band)
	{
		const toprs_float64 MIN_PIX = getMinPix(band);
		const toprs_float64 MAX_PIX = getMaxPix(band);
		const toprs_float64 RANGE   = (MAX_PIX-MIN_PIX);
		const toprs_float64 NP      = getNullPix(band);

		const T* s = (T*)getBuf(band);  // source
		toprs_float64* d = (toprs_float64*)(buf + (band*SIZE));  // destination

		for(toprs_uint32 offset = 0; offset < SIZE; ++offset)
		{
			toprs_float64 p = s[offset];
			if(p != NP)
			{
				if( p == MIN_PIX)
				{
					d[offset] = TOPRS_DEFAULT_MIN_PIX_NORM_DOUBLE;
				}
				else
				{
					d[offset] = (p-MIN_PIX)/RANGE;
				}
			}
			else
			{
				d[offset] = 0.0;
			}
		}
	}   
}

template <class T>
void toprsImg::copyTileToNormalizedBuffer(T /* dummyTemplate */,int band,toprs_float64* buf) const
{
	const toprs_uint32  SIZE    = getSizePerBand();
	const toprs_float64 MIN_PIX = getMinPix(band);
	const toprs_float64 MAX_PIX = getMaxPix(band);
	const toprs_float64 RANGE   = (MAX_PIX-MIN_PIX);
	const toprs_float64 NP      = getNullPix(band);

	const T* s = (T*)getBuf(band);  // source
	toprs_float64* d = (toprs_float64*)(buf);  // destination

	for(toprs_uint32 offset = 0; offset < SIZE; ++offset)
	{
		toprs_float64 p = s[offset];
		if(p != NP)
		{
			if( p == MIN_PIX)
			{
				d[offset] = TOPRS_DEFAULT_MIN_PIX_NORM_DOUBLE;
			}
			else
			{
				d[offset] = (p-MIN_PIX)/RANGE;
			}
		}
		else
		{
			d[offset] = 0.0;
		}
	}   
}

template <class T>
void toprsImg::copyTileToNormalizedBuffer(T /* dummyTemplate */,int band,	toprs_float32* buf) const
{
	const toprs_uint32  SIZE    = getSizePerBand();
	const toprs_float64 MIN_PIX = getMinPix(band);
	const toprs_float64 MAX_PIX = getMaxPix(band);
	const toprs_float64 RANGE   = (MAX_PIX-MIN_PIX);
	const toprs_float64 NP      = getNullPix(band);

	const T* s = (T*)getBuf(band);  // source
	toprs_float32* d     = (toprs_float32*)(buf);  // destination

	for(toprs_uint32 offset = 0; offset < SIZE; ++offset)
	{
		toprs_float64 p = s[offset];
		if(p != NP)
		{
			if( p == MIN_PIX)
			{
				d[offset] = TOPRS_DEFAULT_MIN_PIX_NORM_FLOAT;
			}
			else
			{
				d[offset] = (p-MIN_PIX)/RANGE;
			}
		}
		else
		{
			d[offset] = 0.0;
		}
	}   
}

void toprsImg::copyTileToNormalizedBuffer( int band, toprs_float64* buf ) const
{
	if (!buf)
	{
		std::cout
			<< "toprsImg::copyTileToNormalizedBuffer ERROR:"
			<< "\nNull buffer passed to method!  Returning..." << std::endl;
		return;
	}

	if (getDataObjectStatus() == TOPRS_NULL)
	{
		std::cout
			<< "toprsImg::copyTileToNormalizedBuffer ERROR:"
			<< "\nThis object is null! Returning..." << std::endl;
		return;
	}

	switch(getDataType())
	{
	case TOPRS_NORMALIZED_DOUBLE:
		{
			memmove(buf, getBuf(band), getDataSizeInBytes());
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
		{
			toprs_uint32 upperBound = w()*h()*getBands();
			const toprs_float32* inputBuf = static_cast<const toprs_float32*>(getBuf(band));
			for(toprs_uint32 offset = 0; offset < upperBound; ++offset)
			{
				buf[offset] = inputBuf[offset];
			}
			break;
		}
	case TOPRS_FLOAT32:
		{
			copyTileToNormalizedBuffer((toprs_float32)0, band, buf);
			break;
		}
	case TOPRS_FLOAT64:
		{
			copyTileToNormalizedBuffer((toprs_float64)0, band, buf);
			break;
		}
	case TOPRS_UINT8:
		{
			copyTileToNormalizedBuffer((toprs_uint8)0, band, buf);
			break;

		}
	case TOPRS_SINT8:
		{
			copyTileToNormalizedBuffer((toprs_sint8)0, band, buf);
			break;

		}
	case TOPRS_USHORT11:
	case TOPRS_UINT16:
		{
			copyTileToNormalizedBuffer((toprs_uint16)0,band, buf);
			break;
		}
	case TOPRS_SINT16:
		{
			copyTileToNormalizedBuffer((toprs_sint16)0,band, buf);
			break;
		}
	case TOPRS_UINT32:
		{
			copyTileToNormalizedBuffer((toprs_uint32)0,band, buf);
			break;
		}
	case TOPRS_SINT32:
		{
			copyTileToNormalizedBuffer((toprs_sint32)0, band, buf);
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			std::cout
				<< "NOTICE:  copyTileToNormalizedBuffer not implemented yet"
				<< std::endl;
		}

	}  // End of "switch(getDataType())"
}


bool toprsImg::isPointtWithin( const toprsIpt& point ) const
{
	return ((point.x >= theOrigin.x)&&
		(point.y >= theOrigin.y)&&
		((point.x - theOrigin.x)<static_cast<int>(theWidth))&&
		((point.y - theOrigin.y)<static_cast<int>(theHeight)));
}

bool toprsImg::isPointtWithin( int x, int y ) const
{
	return ((x >= theOrigin.x)&&
		(y >= theOrigin.y)&&
		((x - theOrigin.x) < static_cast<int>(theWidth))&&
		((y - theOrigin.y) < static_cast<int>(theWidth)));

}

void toprsImg::stretchMinMax()
{
	if ( (getDataObjectStatus() != TOPRS_NULL) &&
		(getDataObjectStatus() != TOPRS_EMPTY) )
	{
		switch (getDataType())
		{
		case TOPRS_UINT8:
			{
				stretchMinMax(toprs_uint8(0));
				return;
			}  
		case TOPRS_SINT8:
			{
				stretchMinMax(toprs_sint8(0));
				return;
			}  
		case TOPRS_UINT16:
		case TOPRS_USHORT11:
			{
				stretchMinMax(toprs_uint16(0));
				return;
			}  
		case TOPRS_SINT16:
			{
				stretchMinMax(toprs_sint16(0));
				return;
			}  
		case TOPRS_UINT32:
			{
				stretchMinMax(toprs_uint32(0));
				return;
			}
		case TOPRS_SINT32:
			{
				stretchMinMax(toprs_sint32(0));
				return;
			}  
		case TOPRS_FLOAT32:
		case TOPRS_NORMALIZED_FLOAT:
			{
				stretchMinMax(toprs_float32(0.0));
				return;
			}  
		case TOPRS_NORMALIZED_DOUBLE:
		case TOPRS_FLOAT64:
			{
				stretchMinMax(toprs_float64(0.0));
				return;
			}  
		case TOPRS_DATA_UNKNOWN:
		default:
			{
				setDataObjectStatus(TOPRS_STATUS_UNKNOWN);
				std::cout << 
					"toprsImg::stretchMinMax Fill"
					<<__FILE__
					<< "line \n"
					<< __LINE__; 
				break;
			}
		}
	}
}

template <class T> void toprsImg::stretchMinMax(T /* dummyTemplate */)
{
	const toprs_uint32 BANDS  = getBands();
	const toprs_uint32 SPB    = getSizePerBand();

	// scalar min
	const toprs_float64 S_MIN = toprs::defaultMin(getDataType());

	// scalar max
	const toprs_float64 S_MAX = toprs::defaultMax(getDataType());

	// scalar range
	const toprs_float64 S_RNG = S_MAX-S_MIN+1.0;

	for(int band = 0; band < BANDS; ++band)
	{
		T* s = static_cast<T*>(getBuf(band));

		if (s)
		{
			const toprs_float64 T_NUL = theNullPixelV[band]; // tile null
			const toprs_float64 T_MIN = theMinPixelV[band];  // tile min
			const toprs_float64 T_MAX = theMaxPixelV[band];  // tile max
			const toprs_float64 T_RNG = (T_MAX-T_MIN)+1;           // tile range
			const toprs_float64 SPP = S_RNG / T_RNG; // stretch per pixel

			for(toprs_uint32 i = 0; i < SPB; ++i)
			{
				if (s[i] != T_NUL)
				{
					toprs_float64 p = s[i];
					if (p <= T_MIN)
					{
						p = S_MIN;
					}
					else if (p >= T_MAX)
					{
						p = S_MAX;
					}
					else
					{
						p = (p - T_MIN + 1.0) * SPP + S_MIN - 1.0;
					}
					s[i] = toprs::round<T>(p);
				}
			}
		}
	} 
}

void toprsImg::computeAlphaChannel()
{
	if ( getDataObjectStatus() != TOPRS_NULL )
	{
		switch (getDataType())
		{
		case TOPRS_UINT8:
			{
				computeAlphaChannel(toprs_uint8(0));
				return;
			}  
		case TOPRS_SINT8:
			{
				computeAlphaChannel(toprs_sint8(0));
				return;
			}  
		case TOPRS_UINT16:
		case TOPRS_USHORT11:
			{
				computeAlphaChannel(toprs_uint16(0));
				return;
			}  
		case TOPRS_SINT16:
			{
				computeAlphaChannel(toprs_sint16(0));
				return;
			}  
		case TOPRS_UINT32:
			{
				computeAlphaChannel(toprs_uint32(0));
				return;
			}
		case TOPRS_SINT32:
			{
				computeAlphaChannel(toprs_sint32(0));
				return;
			}  
		case TOPRS_FLOAT32:
		case TOPRS_NORMALIZED_FLOAT:
			{
				computeAlphaChannel(toprs_float32(0.0));
				return;
			}  
		case TOPRS_NORMALIZED_DOUBLE:
		case TOPRS_FLOAT64:
			{
				computeAlphaChannel(toprs_float64(0.0));
				return;
			}  
		case TOPRS_DATA_UNKNOWN:
		default:
			{
				setDataObjectStatus(TOPRS_STATUS_UNKNOWN);
				std::cout << 
					"toprsImg::computeAlphaChannel"
					<<__FILE__
					<< "line \n"
					<< __LINE__; 
				break;
			}
		}
	}
}

template <class T>
void toprsImg::computeAlphaChannel(T dummyTemplate)
{
	const toprs_uint32 SPB = getSizePerBand();
	const toprs_uint8  ANP = 0;   // Alpha Null Pixel
	const toprs_uint8  AVP = 255; // Alpha Valid Pixel

	// Allocate the alpha channel if not already.
	if (theAlphaChannelV.size() != SPB)
	{
		theAlphaChannelV.resize(SPB);
	}

	if (getDataObjectStatus() == TOPRS_FULL)
	{
		memset( static_cast<void*>(&theAlphaChannelV.front()),
			static_cast<int>(AVP),
			static_cast<int>(SPB) );
	}
	else if (getDataObjectStatus() == TOPRS_EMPTY)
	{
		// Start with alpha being set to 0.  
		memset( static_cast<void*>(&theAlphaChannelV.front()),
			static_cast<int>(ANP),
			static_cast<int>(SPB) );
	}
	else // Partial must check each pixel.
	{
		const toprs_uint32 BANDS = getBands();

		std::vector<T> null_pix(BANDS);
		std::vector<const T*> buf(BANDS); // Pointer to pixel data.
		int band;
		for(band = 0; band < BANDS; ++band)
		{
			buf[band] = static_cast<const T*>(getBuf(band));
			null_pix[band] = static_cast<T>(theNullPixelV[band]);
		}

		for(toprs_uint32 i = 0; i < SPB; ++i)
		{
			//---
			// Start with alpha being set to 0.  If any band has a valid pixel set
			// to 255 and break from band loop.
			//---
			theAlphaChannelV[i] = ANP;
			for(band = 0; band < BANDS; ++band)
			{
				if (buf[band][i] != null_pix[band])
				{
					theAlphaChannelV[i] = AVP;
					break;
				}
			}
		}
	}
}



bool toprsImg::isWithin( int x, int y )
{
	return ((x >= theOrigin.x) &&
		(x <  theOrigin.x + static_cast<toprs_int32>(theWidth)) &&
		(y >= theOrigin.y) &&
		(y <  theOrigin.y + static_cast<toprs_int32>(theHeight)));
}

void toprsImg::copyLine( const void* src, int lineNumber, int lineStartSample, int lineStopSample, toprsInterleaveType lineInterleave )
{
	switch(theDataType)
	{
	case TOPRS_UINT8:
		{
			copyLine((toprs_uint8)0, src, lineNumber, lineStartSample,
				lineStopSample, lineInterleave);
			break;
		}
	case TOPRS_SINT8:
		{
			copyLine((toprs_sint8)0, src, lineNumber, lineStartSample,
				lineStopSample, lineInterleave);
			break;

		}
	case TOPRS_USHORT11:
	case TOPRS_UINT16:
		{
			copyLine((toprs_uint16)0, src, lineNumber, lineStartSample,
				lineStopSample, lineInterleave);
			break;
		}
	case TOPRS_SINT16:
		{
			copyLine((toprs_sint16)0, src, lineNumber, lineStartSample,
				lineStopSample, lineInterleave);
			break;
		}
	case TOPRS_UINT32:
		{
			copyLine((toprs_uint32)0, src, lineNumber, lineStartSample,
				lineStopSample, lineInterleave);
			break;
		}
	case TOPRS_SINT32:
		{
			copyLine((toprs_sint32)0, src, lineNumber, lineStartSample,
				lineStopSample, lineInterleave);
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		{
			copyLine((toprs_float32)0.0, src, lineNumber, lineStartSample,
				lineStopSample, lineInterleave);
			break;
		}
	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		{
			copyLine((toprs_float64)0.0, src, lineNumber, lineStartSample,
				lineStopSample, lineInterleave);
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			// Shouldn't hit this.
			std::cout
				<< "toprsImg::copyLine Unsupported scalar type!"
				<< std::endl;
			break;
		}
	}
}

template <class T>
void toprsImg::copyLine(T dummyTemplate,const void* src, int lineNumber, int lineStartSample, int lineStopSample, toprsInterleaveType lineInterleave)
{
	if (src)
	{
		const toprsIRect RECT = getImgRect();

		// Check for intersect:
		if ( ( lineNumber       >= RECT.left_up().y)     &&
			( lineNumber       <= RECT.right_down().y)     &&
			( lineStartSample  <  lineStopSample)  &&
			( lineStartSample  <= RECT.right_down().x)     &&
			( lineStopSample   >= RECT.left_up().x) )
		{
			const toprs_int32 BANDS = static_cast<toprs_int32>(theNumberOfBands);
			const toprs_int32 START_SAMP =
				(lineStartSample > RECT.left_up().x)?lineStartSample:RECT.left_up().x;
			const toprs_int32 STOP_SAMP  =
				(lineStopSample  < RECT.right_down().x)?lineStopSample:RECT.right_down().x;
			const toprs_int32 SAMPS = STOP_SAMP - START_SAMP + 1;

			std::vector<T*> d(BANDS);

			toprs_int32 band;
			for (band = 0; band < BANDS; ++band)
			{
				d[band] = static_cast<T*>(getBuf(band));

				// Position at start sample.
				d[band] +=  (lineNumber - RECT.left_up().y) * RECT.width() + (START_SAMP - RECT.left_up().x);
			}

			if (lineInterleave == TOPRS_BIP)
			{
				const T* S = static_cast<const T*>(src); // Source buffer:

				// Position at start sample.
				S += (START_SAMP - lineStartSample) * BANDS;

				toprs_int32 srcOffset = 0;
				for (toprs_int32 samp = 0; samp < SAMPS; ++samp)
				{
					for (band = 0; band < BANDS; ++band)
					{
						d[band][samp] = S[srcOffset++];
					}
				}
			}
			else
			{
				const toprs_int32 W = lineStopSample - lineStartSample + 1;
				std::vector<const T*> S(BANDS);
				for (band = 0; band < BANDS; ++band)
				{
					S[band] = static_cast<const T*>(src) + (START_SAMP - lineStartSample);
					if (band)
					{
						S[band] += band * W; // Move to line.
					}
				}

				for (band = 0; band < BANDS; ++band)
				{
					for (toprs_int32 samp = 0; samp < SAMPS; ++samp)
					{
						d[band][samp] = S[band][samp];
					}
				}
			}

		} // intersect check

	} // if (src)

}

void toprsImg::setIndexedFlag( bool flag )
{
	theIndexedFlag = flag;
}

bool toprsImg::getIndexedFlag() const
{
	return theIndexedFlag;
}

void toprsImg::copyTileToNormalizedBuffer( toprs_float32* buf ) const
{
	if (!buf)
	{
		std::cout
			<< "toprsImg::copyNormalizedBufferToTile ERROR:"
			<< "\nNull buffer passed to method!  Returning..." << std::endl;
		return;
	}
	if (getDataObjectStatus() == TOPRS_NULL)
	{
		std::cout
			<< "toprsImg::copyNormalizedBufferToTile ERROR:"
			<< "\nThis object is null! Returning..." << std::endl;
		return;
	}
	switch(getDataType())
	{
	case TOPRS_NORMALIZED_DOUBLE:
		{
			toprs_uint32 upperBound = w()*h()*getBands();
			const toprs_float32* inputBuf = static_cast<const toprs_float32*>(getBuf());
			for(toprs_uint32 offset = 0; offset < upperBound; ++offset)
			{
				buf[offset] = inputBuf[offset];
			}
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
		{
			memmove(buf, getBuf(),getDataSizeInBytes());//xizhi
			break;
		}
	case TOPRS_FLOAT32:
		{
			copyTileToNormalizedBuffer((toprs_float32)0,  buf);//xizhi
			break;
		}
	case TOPRS_FLOAT64:
		{
			copyTileToNormalizedBuffer((toprs_float64)0, buf);//xizhi
			break;
		}
	case TOPRS_UINT8:
		{
			copyTileToNormalizedBuffer((toprs_uint8)0, buf);//xizhi
			break;

		}
	case TOPRS_SINT8:
		{
			copyTileToNormalizedBuffer((toprs_sint8)0,  buf);//xizhi
			break;

		}
	case TOPRS_USHORT11:
	case TOPRS_UINT16:
		{
			copyTileToNormalizedBuffer((toprs_uint16)0,  buf);//xizhi
			break;
		}
	case TOPRS_SINT16:
		{
			copyTileToNormalizedBuffer((toprs_sint16)0,  buf);//xizhi
			break;
		}
	case TOPRS_UINT32:
		{
			copyTileToNormalizedBuffer((toprs_uint32)0, buf);//xizhi
			break;
		}
	case TOPRS_SINT32:
		{
			copyTileToNormalizedBuffer((toprs_sint32)0,  buf);//xizhi
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			std::cout
				<< "copyTileToNormalizedBuffer\n"
				<< "Unknown scalar type!" << std::endl;
		}

	}  // End of "switch(getDataType())".
}

void toprsImg::copyTileToNormalizedBuffer( toprs_float64* buf ) const
{
	if (!buf)
	{
		std::cout
			<< "toprsImg::copyTileToNormalizedBuffer ERROR:"
			<< "\nNull buffer passed to method!  Returning..." << std::endl;
		return;
	}

	if (getDataObjectStatus() == TOPRS_NULL)
	{
		std::cout
			<< "toprsImg::copyTileToNormalizedBuffer ERROR:"
			<< "\nThis object is null! Returning..." << std::endl;
		return;
	}

	switch(getDataType())
	{
	case TOPRS_NORMALIZED_DOUBLE:
		{
			memmove(buf, getBuf(), getDataSizeInBytes());
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
		{
			toprs_uint32 upperBound = w()*h()*getBands();
			const toprs_float32* inputBuf = static_cast<const toprs_float32*>(getBuf());
			for(toprs_uint32 offset = 0; offset < upperBound; ++offset)
			{
				buf[offset] = inputBuf[offset];
			}
			break;
		}
	case TOPRS_FLOAT32:
		{
			copyTileToNormalizedBuffer((toprs_float32)0, buf);
			break;
		}
	case TOPRS_FLOAT64:
		{
			copyTileToNormalizedBuffer((toprs_float64)0, buf);
			break;
		}
	case TOPRS_UINT8:
		{
			copyTileToNormalizedBuffer((toprs_uint8)0, buf);
			break;

		}
	case TOPRS_SINT8:
		{
			copyTileToNormalizedBuffer((toprs_sint8)0, buf);
			break;

		}
	case TOPRS_USHORT11:
	case TOPRS_UINT16:
		{
			copyTileToNormalizedBuffer((toprs_uint16)0, buf);
			break;
		}
	case TOPRS_SINT16:
		{
			copyTileToNormalizedBuffer((toprs_sint16)0, buf);
			break;
		}
	case TOPRS_UINT32:
		{
			copyTileToNormalizedBuffer((toprs_uint32)0, buf);
			break;
		}
	case TOPRS_SINT32:
		{
			copyTileToNormalizedBuffer((toprs_sint32)0, buf);
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			std::cout
				<< "NOTICE:  copyTileToNormalizedBuffer not implemented yet"
				<< std::endl;
		}

	}  // End of "switch(getScalarType())
}



void toprsImg::copyTileToNormalizedBuffer(int band,	toprs_float32* buf)const
{
	if (!buf)
	{
		std::cout
			<< "toprsImg::copyTileBandToNormalizedBuffer ERROR:"
			<< "\nNull buffer passed to method!  Returning..." << std::endl;
		return;
	}

	if (getDataObjectStatus() == TOPRS_NULL)
	{
		std::cout
			<< "toprsImg::copyTileBandToNormalizedBuffer ERROR:"
			<< "\nThis object is null! Returning..." << std::endl;
		return;
	}

	switch(getDataType())
	{
	case TOPRS_NORMALIZED_DOUBLE:
		{
			toprs_uint32 upperBound = w()*h();
			const toprs_float32* inputBuf = static_cast<const toprs_float32*>(getBuf(band));
			for(toprs_uint32 offset = 0; offset < upperBound; ++offset)
			{
				buf[offset] = inputBuf[offset];
			}
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
		{
			memmove(buf, getBuf(band), getSizePerBandInBytes());
			break;
		}
	case TOPRS_FLOAT32:
		{
			copyTileToNormalizedBuffer((toprs_float32)0, band, buf);
			break;
		}
	case TOPRS_FLOAT64:
		{
			copyTileToNormalizedBuffer((toprs_float64)0, band, buf);
			break;
		}
	case TOPRS_UINT8:
		{
			copyTileToNormalizedBuffer((toprs_uint8)0, band, buf);
			break;

		}
	case TOPRS_SINT8:
		{
			copyTileToNormalizedBuffer((toprs_sint8)0, band, buf);
			break;

		}
	case TOPRS_USHORT11:
	case TOPRS_UINT16:
		{
			copyTileToNormalizedBuffer((toprs_uint16)0, band, buf);
			break;
		}
	case TOPRS_SINT16:
		{
			copyTileToNormalizedBuffer((toprs_sint16)0, band, buf);
			break;
		}
	case TOPRS_UINT32:
		{
			copyTileToNormalizedBuffer((toprs_uint32)0, band, buf);
			break;
		}
	case TOPRS_SINT32:
		{
			copyTileToNormalizedBuffer((toprs_sint32)0, band, buf);
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			std::cout
				<< "toprsImg::copyTileBandToNormalizedBuffer ERROR:"
				<< " Unknown scalar type"
				<< std::endl;
		}

	}  // End of "switch(getDataType())"
}
void toprsImg::copyNormalizedBufferToTile(toprs_float64* buf)
{
	if (!buf)
	{
		std::cout
			<< "toprsImg::copyNormalizedBufferToTile ERROR:"
			<< "\nNull buffer passed to method!  Returning..." << std::endl;
		return;
	}

	if (getDataObjectStatus() == TOPRS_NULL)
	{
		std::cout
			<< "toprsImg::copyNormalizedBufferToTile ERROR:"
			<< "\nThis object is null! Returning..." << std::endl;
		return;
	}

	switch(getDataType())
	{
	case TOPRS_NORMALIZED_DOUBLE:
		{
			memmove(getBuf(), buf, getDataSizeInBytes());
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
		{
			toprs_uint32 upperBound = w()*h()*getBands();
			toprs_float32* inputBuf = static_cast<toprs_float32*>(getBuf());
			for(toprs_uint32 offset = 0; offset < upperBound; ++offset)
			{
				inputBuf[offset] = static_cast<toprs_float32>(buf[offset]);
			}
			break;
		}
	case TOPRS_FLOAT32:
		{
			copyNormalizedBufferToTile((toprs_float32)0, buf);
			break;
		}
	case TOPRS_FLOAT64:
		{
			copyNormalizedBufferToTile((toprs_float64)0, buf);
			break;
		}
	case TOPRS_UINT8:
		{
			copyNormalizedBufferToTile((toprs_uint8)0, buf);
			break;

		}
	case TOPRS_SINT8:
		{
			copyNormalizedBufferToTile((toprs_sint8)0, buf);
			break;

		}
	case TOPRS_USHORT11:
	case TOPRS_UINT16:
		{
			copyNormalizedBufferToTile((toprs_uint16)0, buf);
			break;
		}
	case TOPRS_SINT16:
		{
			copyNormalizedBufferToTile((toprs_sint16)0, buf);
			break;
		}
	case TOPRS_UINT32:
		{
			copyNormalizedBufferToTile((toprs_uint32)0, buf);
			break;
		}
	case TOPRS_SINT32:
		{
			copyNormalizedBufferToTile((toprs_sint32)0, buf);
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			std::cout
				<< "toprsImg::copyNormalizedBufferToTile\n"
				<< "Unknown scalar type!" << std::endl;
		}

	} // End of "switch(getDataType())".
}

void toprsImg::copyNormalizedBufferToTile(int band,	toprs_float64* buf)
{
	if (!buf)
	{
		std::cout
			<< "toprsImg::copyNormalizedBufferToTile ERROR:"
			<< "\nNull buffer passed to method!  Returning..." << std::endl;
		return;
	}

	if (getDataObjectStatus() == TOPRS_NULL)
	{
		std::cout
			<< "toprsImg::copyNormalizedBufferToTile ERROR:"
			<< "\nThis object is null! Returning..." << std::endl;
		return;
	}

	if(band < getBands())
	{
		switch(getDataType())
		{
		case TOPRS_NORMALIZED_DOUBLE:
			{
				memmove(getBuf(band), buf, getSizePerBandInBytes());
				break;
			}
		case TOPRS_NORMALIZED_FLOAT:
			{
				toprs_uint32 upperBound = w()*h();
				toprs_float32* inputBuf = static_cast<toprs_float32*>(getBuf(band));
				for(toprs_uint32 offset = 0; offset < upperBound; ++offset)
				{
					inputBuf[offset] = static_cast<toprs_float32>(buf[offset]);
				}
				break;
			}
		case TOPRS_FLOAT32:
			{
				copyNormalizedBufferToTile((toprs_float32)0, band, buf);
				break;
			}
		case TOPRS_FLOAT64:
			{
				copyNormalizedBufferToTile((toprs_float64)0, band, buf);
				break;
			}
		case TOPRS_UINT8:
			{
				copyNormalizedBufferToTile((toprs_uint8)0, band, buf);
				break;

			}
		case TOPRS_SINT8:
			{
				copyNormalizedBufferToTile((toprs_sint8)0, band, buf);
				break;

			}
		case TOPRS_USHORT11:
		case TOPRS_UINT16:
			{
				copyNormalizedBufferToTile((toprs_uint16)0, band, buf);
				break;
			}
		case TOPRS_SINT16:
			{
				copyNormalizedBufferToTile((toprs_sint16)0, band, buf);
				break;
			}
		case TOPRS_UINT32:
			{
				copyNormalizedBufferToTile((toprs_uint32)0, band, buf);
				break;
			}
		case TOPRS_SINT32:
			{
				copyNormalizedBufferToTile((toprs_sint32)0, band, buf);
				break;
			}
		case TOPRS_DATA_UNKNOWN:
		default:
			{
				std::cout
					<< "toprsImg::copyNormalizedBufferToTile\n"
					<< "Unknown scalar type." << std::endl;
			}

		} // End of "switch(getDataType())".
	}
}

void toprsImg::copyNormalizedBufferToTile(toprs_float32* buf)
{
	if (!buf)
	{
		std::cout
			<< "toprsImg::copyNormalizedBufferToTile ERROR:"
			<< "\nNull buffer passed to method!  Returning..." << std::endl;
		return;
	}

	if (getDataObjectStatus() == TOPRS_NULL)
	{
		std::cout
			<< "toprsImg::copyNormalizedBufferToTile ERROR:"
			<< "\nThis object is null! Returning..." << std::endl;
		return;
	}

	switch(getDataType())
	{
	case TOPRS_NORMALIZED_DOUBLE:
		{
			toprs_uint32 upperBound = w()*h()*getBands();
			toprs_float64* inputBuf = static_cast<toprs_float64*>(getBuf());
			for(toprs_uint32 offset = 0; offset < upperBound; ++offset)
			{
				inputBuf[offset] = buf[offset];
			}
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
		{
			memmove(getBuf(), buf, getDataSizeInBytes());
			break;
		}
	case TOPRS_FLOAT32:
		{
			copyNormalizedBufferToTile((toprs_float32)0, buf);
			break;
		}
	case TOPRS_FLOAT64:
		{
			copyNormalizedBufferToTile((toprs_float64)0, buf);
			break;
		}
	case TOPRS_UINT8:
		{
			copyNormalizedBufferToTile((toprs_uint8)0, buf);
			break;

		}
	case TOPRS_SINT8:
		{
			copyNormalizedBufferToTile((toprs_sint8)0, buf);
			break;

		}
	case TOPRS_USHORT11:
	case TOPRS_UINT16:
		{
			copyNormalizedBufferToTile((toprs_uint16)0, buf);
			break;
		}
	case TOPRS_SINT16:
		{
			copyNormalizedBufferToTile((toprs_sint16)0, buf);
			break;
		}
	case TOPRS_UINT32:
		{
			copyNormalizedBufferToTile((toprs_uint32)0, buf);
			break;
		}
	case TOPRS_SINT32:
		{
			copyNormalizedBufferToTile((toprs_sint32)0, buf);
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			std::cout
				<< "toprsImgcopyNormalizedBufferToTile\n"
				<< "Unknown scalar type!" << std::endl;
		}

	}  // End of "switch(getDataType())".
}

void toprsImg::copyNormalizedBufferToTile(int band,	toprs_float32* buf)
{
	if (!buf)
	{
		std::cout
			<< "toprsImg::copyNormalizedBufferToTile ERROR:"
			<< "\nNull buffer passed to method!  Returning..." << std::endl;
		return;
	}

	if (getDataObjectStatus() == TOPRS_NULL)
	{
		std::cout
			<< "toprsImg::copyNormalizedBufferToTile ERROR:"
			<< "\nThis object is null! Returning..." << std::endl;
		return;
	}

	switch(getDataType())
	{
	case TOPRS_NORMALIZED_DOUBLE:
		{
			toprs_uint32 upperBound = w()*h();
			toprs_float64* inputBuf = static_cast<toprs_float64*>(getBuf(band));
			for(toprs_uint32 offset = 0; offset < upperBound; ++offset)
			{
				inputBuf[offset] = buf[offset];
			}
			break;
		}
	case TOPRS_NORMALIZED_FLOAT:
		{
			memmove(getBuf(band), buf, getSizePerBandInBytes());
			break;
		}
	case TOPRS_FLOAT32:
		{
			copyNormalizedBufferToTile((toprs_float32)0, band, buf);
			break;
		}
	case TOPRS_FLOAT64:
		{
			copyNormalizedBufferToTile((toprs_float64)0, band, buf);
			break;
		}
	case TOPRS_UINT8:
		{
			copyNormalizedBufferToTile((toprs_uint8)0, band, buf);
			break;

		}
	case TOPRS_SINT8:
		{
			copyNormalizedBufferToTile((toprs_sint8)0, band, buf);
			break;

		}
	case TOPRS_USHORT11:
	case TOPRS_UINT16:
		{
			copyNormalizedBufferToTile((toprs_uint16)0, band, buf);
			break;
		}
	case TOPRS_SINT16:
		{
			copyNormalizedBufferToTile((toprs_sint16)0, band, buf);
			break;
		}
	case TOPRS_UINT32:
		{
			copyNormalizedBufferToTile((toprs_uint32)0, band, buf);
			break;
		}
	case TOPRS_SINT32:
		{
			copyNormalizedBufferToTile((toprs_sint32)0, band, buf);
			break;
		}
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			std::cout
				<< "toprsImg::copyNormalizedBufferToTile\n"
				<< "Unknown scalar type!" << std::endl;
		}

	}  // End of "switch(getDataType())".
}
const void* toprsImg::getBuf() const
{
	if (theDataBufferV.size() > 0)
	{
		return static_cast<const void*>(&theDataBufferV.front());
	}
	return 0;
}

void* toprsImg::getBuf()
{
	if (theDataBufferV.size() > 0)
	{
		return static_cast< void*>(&theDataBufferV.front());
	}
	return 0;
}

const void* toprsImg::getBuf( int band ) const
{
	const toprs_uint8* b = static_cast<const toprs_uint8*>(getBuf());

	if (isValidBand(band) && b != nullptr)
	{
		b += (band * getSizePerBandInBytes());
		return static_cast<const void*>(b);
	}
	return 0;
}

void* toprsImg::getBuf( int band )
{
	toprs_uint8* b = static_cast<toprs_uint8*>(getBuf());

	if (isValidBand(band) && b != nullptr)
	{
		b += (band * getSizePerBandInBytes());
		return static_cast<void*>(b);
	}
	return 0;
}

const toprs_uint8* toprsImg::getUcharBuf() const
{
	if (theDataType == TOPRS_UINT8)
	{
		return static_cast<const toprs_uint8*>(getBuf());
	}
	return 0;
}

toprs_uint8* toprsImg::getUcharBuf()
{
	if (theDataType == TOPRS_UINT8)
	{
		return static_cast< toprs_uint8*>(getBuf());
	}
	return 0;
}

const toprs_uint8* toprsImg::getUcharBuf( int band ) const
{
	if (theDataType == TOPRS_UINT8)
	{
		return static_cast<const toprs_uint8*>(getBuf(band));
	}
	return 0;
}

toprs_uint8* toprsImg::getUcharBuf( int band )
{
	if (theDataType == TOPRS_UINT8)
	{
		return static_cast< toprs_uint8*>(getBuf(band));
	}
	return 0;
}

const toprs_uint16* toprsImg::getUshortBuf() const
{
	if (theDataType == TOPRS_UINT16 ||
		theDataType == TOPRS_USHORT11)
	{
		return static_cast<const toprs_uint16*>(getBuf());
	}
	return 0;
}

toprs_uint16* toprsImg::getUshortBuf()
{
	if (theDataType == TOPRS_UINT16 ||
		theDataType == TOPRS_USHORT11)
	{
		return static_cast<toprs_uint16*>(getBuf());
	}
	return 0;
}

const toprs_uint16* toprsImg::getUshortBuf( int band ) const
{
	if (theDataType == TOPRS_UINT16 ||
		theDataType == TOPRS_USHORT11)
	{
		return static_cast<const toprs_uint16*>(getBuf(band));
	}
	return 0;
}

toprs_uint16* toprsImg::getUshortBuf( int band )
{
	if (theDataType == TOPRS_UINT8)
	{
		return static_cast<toprs_uint16*>(getBuf(band));
	}
	return 0;
}

const toprs_sint16* toprsImg::getSshortBuf() const
{
	if (theDataType == TOPRS_SINT16)
	{
		return static_cast<const toprs_sint16*>(getBuf());
	}
	return 0;
}

toprs_sint16* toprsImg::getSshortBuf()
{
	if (theDataType == TOPRS_SINT16)
	{
		return static_cast<toprs_sint16*>(getBuf());
	}
	return 0;
}

const toprs_sint16* toprsImg::getSshortBuf( int band ) const
{
	if (theDataType == TOPRS_SINT16)
	{
		return static_cast<const toprs_sint16*>(getBuf(band));
	}
	return 0;
}

toprs_sint16* toprsImg::getSshortBuf( int band )
{
	if (theDataType == TOPRS_UINT16 ||
		theDataType == TOPRS_USHORT11)
	{
		return static_cast<toprs_sint16*>(getBuf(band));
	}
	return 0;
}

const toprs_float32* toprsImg::getFloatBuf() const
{
	if (theDataType == TOPRS_FLOAT32 ||
		theDataType == TOPRS_NORMALIZED_FLOAT)
	{
		return static_cast<const toprs_float32*>(getBuf());
	}
	return 0;
}

toprs_float32* toprsImg::getFloatBuf()
{
	if (theDataType == TOPRS_FLOAT32 ||
		theDataType == TOPRS_NORMALIZED_FLOAT)
	{
		return static_cast<toprs_float32*>(getBuf());
	}
	return 0;
}

const toprs_float32* toprsImg::getFloatBuf( int band ) const
{
	if (theDataType == TOPRS_FLOAT32 ||
		theDataType == TOPRS_NORMALIZED_FLOAT)
	{
		return static_cast<const toprs_float32*>(getBuf(band));
	}
	return 0;
}

toprs_float32* toprsImg::getFloatBuf( int band )
{
	if (theDataType == TOPRS_FLOAT32 ||
		theDataType == TOPRS_NORMALIZED_FLOAT)
	{
		return static_cast<toprs_float32*>(getBuf(band));
	}
	return 0;
}

const toprs_float64* toprsImg::getDoubleBuf() const
{
	if (theDataType == TOPRS_FLOAT64 ||
		theDataType == TOPRS_NORMALIZED_DOUBLE)
	{
		return static_cast<const toprs_float64*>(getBuf());
	}
	return 0;
}

toprs_float64* toprsImg::getDoubleBuf()
{
	if (theDataType == TOPRS_FLOAT64 ||
		theDataType == TOPRS_NORMALIZED_DOUBLE)
	{
		return static_cast< toprs_float64*>(getBuf());
	}
	return 0;
}

const toprs_float64* toprsImg::getDoubleBuf( int band ) const
{
	if (theDataType == TOPRS_FLOAT64 ||
		theDataType == TOPRS_NORMALIZED_DOUBLE)
	{
		return static_cast<const toprs_float64*>(getBuf(band));
	}
	return 0;
}

toprs_float64* toprsImg::getDoubleBuf( int band )
{
	if (theDataType == TOPRS_FLOAT64 ||
		theDataType == TOPRS_NORMALIZED_DOUBLE)
	{
		return static_cast<toprs_float64*>(getBuf(band));
	}
	return 0;
}

int toprsImg::getSize() const
{
	return getSizePerBand()*getBands();
}

int toprsImg::getSizePerBand() const
{
	return w() * h();
}

int toprsImg::getSizePerBandInBytes() const
{
	return w()*h()*getDataSizeInBytes();
}

int toprsImg::getDataSizeInBytes() const
{
	return toprs::dataSizeInBytes(getDataType());
}

int toprsImg::getSizeInBytes() const
{
	return getSizePerBandInBytes()*getBands();
}

void toprsImg::makeBlank()
{
	if ( (theDataBufferV.size() == 0) || (getDataObjectStatus() == TOPRS_EMPTY) )
	{
		return; // nothing to do...
	}

	switch (getDataType())
	{
	case TOPRS_UINT8:
		{
			makeBlank(toprs_uint8(0));
			return;
		}  
	case TOPRS_SINT8:
		{
			makeBlank(toprs_sint8(0));
			return;
		}  
	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		{
			makeBlank(toprs_uint16(0));
			return;
		}  
	case TOPRS_SINT16:
		{
			makeBlank(toprs_sint16(0));
			return;
		}  
	case TOPRS_UINT32:
		{
			makeBlank(toprs_uint32(0));
			return;
		}
	case TOPRS_SINT32:
		{
			makeBlank(toprs_sint32(0));
			return;
		}  
	case TOPRS_FLOAT32:
	case TOPRS_NORMALIZED_FLOAT:
		{
			makeBlank(toprs_float32(0.0));
			return;
		}  
	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		{
			makeBlank(toprs_float64(0.0));
			return;
		}  
	case TOPRS_DATA_UNKNOWN:
	default:
		{
			setDataObjectStatus(TOPRS_STATUS_UNKNOWN);
			std::cout << 
				"toprsImg::makeBlank File"
				<<__FILE__
				<< "line \n"
				<< __LINE__
				<<"Invalid scalar type: "
				<< getDataType();
			break;
		}
	}
}



template <class T> void toprsImg::makeBlank(T /* dummyTemplate */ )
{
	// Note: Empty buffer or already TOPRS_EMPTY checked in public method.

	const int BANDS = getBands();
	const int SPB   = getSizePerBand();
	int sizePerBandInBytes = getSizePerBandInBytes();
	for(int band = 0; band < BANDS; ++band)
	{
		const T NP = static_cast<T>(theNullPixelV[band]);
		if(NP == 0.0)
		{
			// do an optimize byte fill
			memset(getBuf(band), '\0', sizePerBandInBytes);
		}
		else
		{
			// slower assign call
			T* p = static_cast<T*>(getBuf(band));
			for (toprs_uint32 i = 0; i < SPB; ++i)
			{
				p[i] = NP;
			}
		}
	}
	setDataObjectStatus(TOPRS_EMPTY);
}

void toprsImg::initialize()
{
	if (theDataBufferV.size() != getDataSizeInBytes())
	{
		theDataBufferV.resize(getDataSizeInBytes());
		setDataObjectStatus(TOPRS_STATUS_UNKNOWN);
	}
	if (theDataBufferV.size() > 0)
	{
		makeBlank();  // Make blank will set the status.
	}
	
}



void toprsImg::setValue( int x, int y,toprs_float64 color )
{
   if(theDataBufferV.size() > 0 && isWithin(x, y))
   {
      int band=0;

      //***
      // Compute the offset into the buffer for (x,y).  This should always
      // come out positive.
      //***
      toprs_uint32 ux = static_cast<toprs_uint32>(x - theOrigin.x);
      toprs_uint32 uy = static_cast<toprs_uint32>(y - theOrigin.y);
      
      toprs_uint32 offset = uy * theWidth + ux;
      
      switch (getDataType())
      {
         case TOPRS_UINT8:
         {
            for(band = 0; band < theNumberOfBands; band++)
            {
               unsigned char* buf = static_cast<unsigned char*>(getBuf(band))+
                  offset;
               *buf = (unsigned char)color;
            }
            break;
         }
         case TOPRS_SINT8:
         {
            for(band = 0; band < theNumberOfBands; band++)
            {
               toprs_sint8* buf = static_cast<toprs_sint8*>(getBuf(band))+
                  offset;
               *buf = (toprs_sint8)color;
            }
            break;
         }
         case TOPRS_UINT16:
         case TOPRS_USHORT11:
         {
            for(band = 0; band < theNumberOfBands; band++)
            {
               toprs_uint16* buf = static_cast<toprs_uint16*>(getBuf(band))+
                  offset;
               *buf = (toprs_uint16)color;
            }
            break;
         }
         case TOPRS_SINT16:
         {
            for(band = 0; band < theNumberOfBands; band++)
            {
               signed short* buf = static_cast<signed short*>(getBuf(band))+
                  offset;
               *buf = (signed short)color;
            }
            break;
         }
         case TOPRS_UINT32:
         {
            for(band = 0; band < theNumberOfBands; band++)
            {
               toprs_uint32* buf = static_cast<toprs_uint32*>(getBuf(band))+
                  offset;
               *buf = (toprs_uint32)color;
            }
            break;
         }
         case TOPRS_SINT32:
         {
            for(band = 0; band < theNumberOfBands; band++)
            {
               toprs_sint32* buf = static_cast<toprs_sint32*>(getBuf(band))+
                  offset;
               *buf = (toprs_sint32)color;
            }
            break;
         }
         case TOPRS_NORMALIZED_FLOAT:
         case TOPRS_FLOAT32:
         {
            for(band = 0; band < theNumberOfBands; band++)
            {
               toprs_float32* buf = static_cast<toprs_float32*>(getBuf(band))+offset;
               *buf = (toprs_float32)color;
            }
            break;
         }
         case TOPRS_FLOAT64:
         case TOPRS_NORMALIZED_DOUBLE:
         {
            for(band = 0; band < theNumberOfBands; band++)
            {
               toprs_float64* buf = static_cast<toprs_float64*>(getBuf(band))+offset;
               *buf = color;
            }
            break;
         }
         case TOPRS_DATA_UNKNOWN:
         default:
         {
            //ERROR
            std::cout
               << "toprsImg::setValue Unsupported scalar type!"
               << std::endl;
            
         }
         
      } // End of:  switch (getDataType())
   }
}

void toprsImg::loadBand( const void* src, const toprsIRect& src_rect, int band )
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		loadBandTemplate(toprs_uint8(0), src, src_rect, band);
		return;

	case TOPRS_SINT8:
		loadBandTemplate(toprs_sint8(0), src, src_rect, band);
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		loadBandTemplate(toprs_uint16(0), src, src_rect, band);
		return;

	case TOPRS_SINT16:
		loadBandTemplate(toprs_sint16(0), src, src_rect, band);
		return;

	case TOPRS_UINT32:
		loadBandTemplate(toprs_uint32(0), src, src_rect, band);
		return;

	case TOPRS_SINT32:
		loadBandTemplate(toprs_sint32(0), src, src_rect, band);
		return;

	case TOPRS_FLOAT32:
	case TOPRS_NORMALIZED_FLOAT:
		loadBandTemplate(toprs_float32(0), src, src_rect, band);
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		loadBandTemplate(toprs_float64(0), src, src_rect, band);
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"toprsImg::loadband File"
			<<__FILE__
			<< "line \n"
			<< __LINE__;

		return;
	}
}

void toprsImg::loadBand( const void* src, const toprsIRect& src_rect, const toprsIRect& clip_rect, int band )
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		loadBandTemplate(toprs_uint8(0), src, src_rect, clip_rect, band);
		return;

	case TOPRS_SINT8:
		loadBandTemplate(toprs_sint8(0), src, src_rect, clip_rect, band);
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		loadBandTemplate(toprs_uint16(0), src, src_rect, clip_rect, band);
		return;

	case TOPRS_SINT16:
		loadBandTemplate(toprs_sint16(0), src, src_rect, clip_rect, band);
		return;

	case TOPRS_UINT32:
		loadBandTemplate(toprs_uint32(0), src, src_rect, clip_rect, band);
		return;

	case TOPRS_SINT32:
		loadBandTemplate(toprs_sint32(0), src, src_rect, clip_rect, band);
		return;

	case TOPRS_FLOAT32:
	case TOPRS_NORMALIZED_FLOAT:
		loadBandTemplate(toprs_float32(0), src, src_rect, clip_rect, band);
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		loadBandTemplate(toprs_float64(0), src, src_rect, clip_rect, band);
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"toprsImg::loadband File"
			<<__FILE__
			<< "line \n"
			<< __LINE__;
    
		return;
	}
}


template <class T> void toprsImg::loadBandTemplate(T, // dummy template variable
	const void* src,
	const toprsIRect& src_rect,
	int band)
{
	const toprsIRect img_rect = getImgRect();

	// Get the clip rect.
	const toprsIRect clip_rect = img_rect.clipToRect(src_rect);


	loadBandTemplate(T(0), src, src_rect, clip_rect, band);
}

template <class T> void toprsImg::loadBandTemplate(T, // dummy template variable
	const void* src,
	const toprsIRect& src_rect,
	const toprsIRect& clip_rect,
	int band)
{

	// Check the pointer.
	if (!src)
	{
		// Set the error...
		std::cout << 
			"NULL pointer passed to method"
			<<__FILE__
			<< "line \n"
			<< __LINE__;
		return;
	}

	// Check the band.
	if (!isValidBand(band))
	{
		std::cout << 
			"Invalid band"
			<<__FILE__
			<< "line \n"
			<< __LINE__;
		return;
	}

	const toprsIRect img_rect = getImgRect();

	// Check for intersect.
	if ( ! img_rect.intersects(src_rect) )
	{
		return; // Nothing to do here.
	}

	// Check the clip rect.
	if (!clip_rect.completely_within(img_rect))
	{
		return;
	}

	// Check the status and allocate memory if needed.
	if (getDataObjectStatus() == TOPRS_NULL) initialize();

	// Get the width of the buffers.
	toprs_uint32 s_width = src_rect.width();
	toprs_uint32 d_width = w();

	const T* s = static_cast<const T*>(src);
	T* d = static_cast<T*>(getBuf(band));

	// Move the pointers to the first valid pixel.
	s += (clip_rect.left_up().y - src_rect.left_up().y) * s_width +
		clip_rect.left_up().x - src_rect.left_up().x;

	d += (clip_rect.left_up().y - img_rect.left_up().y) * d_width +
		clip_rect.left_up().x - img_rect.left_up().x;

	// Copy the data.
	toprs_uint32 clipHeight = clip_rect.height();
	toprs_uint32 clipWidth  = clip_rect.width();

	for (toprs_uint32 line = 0; line < clipHeight; ++line)
	{
		for (toprs_uint32 sample = 0; sample < clipWidth; ++sample)
		{
			d[sample] = s[sample];
		}

		s += s_width;
		d += d_width;
	}
}

void toprsImg::loadTile( const void* src, const toprsIRect& src_rect, toprsInterleaveType il_type )
{
	switch (il_type)
	{
	case TOPRS_BIP:
		loadTileFromBip(src, src_rect);
		return;
	case TOPRS_BIL:
		loadTileFromBil(src, src_rect);
		return;
	case TOPRS_BSQ:
		loadTileFromBsq(src, src_rect);
		return;
	default:
		std::cout
			<< "toprsImg::loadTile ERROR:  unsupported interleave type!"
			<< std::endl;
   
		return;
	} // End of "switch (type)"
}

void toprsImg::loadTile( const void* src, const toprsIRect& src_rect, const toprsIRect& clip_rect,toprsInterleaveType il_type )
{
	switch (il_type)
	{
	case TOPRS_BIP:
		loadTileFromBip(src, src_rect, clip_rect);
		return;
	case TOPRS_BIL:
		loadTileFromBil(src, src_rect, clip_rect);
		return;
	case TOPRS_BSQ:
		loadTileFromBsq(src, src_rect, clip_rect);
		return;
	default:
		std::cout
			<< "toprsImg::loadTile ERROR:  unsupported interleave type!"
			<< std::endl;
		return;
	} // End of "switch (type)"
}

void toprsImg::nullTileAlpha( const toprs_uint8* src, const toprsIRect& src_rect, bool multiplyAlphaFlag /*= false*/ )
{

	const toprsIRect img_rect = getImgRect();

	// Get the clip rect.
	const toprsIRect clip_rect = img_rect.clipToRect(src_rect);

	nullTileAlpha(src, src_rect, clip_rect, multiplyAlphaFlag);
}

void toprsImg::nullTileAlpha( const toprs_uint8* src, const toprsIRect& src_rect, const toprsIRect& clip_rect, bool multiplyAlphaFlag /*= false*/ )
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		nullTileAlphaTemplate(toprs_uint8(0), src, src_rect, clip_rect, multiplyAlphaFlag);
		return;

	case TOPRS_SINT8:
		nullTileAlphaTemplate(toprs_sint8(0), src, src_rect, clip_rect, multiplyAlphaFlag);
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		nullTileAlphaTemplate(toprs_uint16(0), src, src_rect, clip_rect, multiplyAlphaFlag);
		return;

	case TOPRS_SINT16:
		nullTileAlphaTemplate(toprs_sint16(0), src, src_rect, clip_rect, multiplyAlphaFlag);
		return;

	case TOPRS_UINT32:
		nullTileAlphaTemplate(toprs_uint32(0), src, src_rect, clip_rect, multiplyAlphaFlag);
		return;

	case TOPRS_SINT32:
		nullTileAlphaTemplate(toprs_sint32(0), src, src_rect, clip_rect, multiplyAlphaFlag);
		return;

	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		nullTileAlphaTemplate(toprs_float32(0), src, src_rect, clip_rect, multiplyAlphaFlag);
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		nullTileAlphaTemplate(toprs_float64(0), src, src_rect, clip_rect, multiplyAlphaFlag);
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"toprsImg::nullTileAlpha File"
			<<__FILE__
			<< "line \n"
			<< __LINE__;
		return;
	}
}

template <class T> void toprsImg::nullTileAlphaTemplate(
	T,
	const toprs_uint8* src,
	const toprsIRect& src_rect,
	const toprsIRect& clip_rect,
	bool multiplyAlphaFlag)
{
	static const char  MODULE[] = "toprsImg::nullTileAlphaTemplate";

	// Check the pointer.
	if (!src)
	{
		// Set the error...
		std::cout << 
			"toprsImg::nullTileAlpha File"
			<<__FILE__
			<< "line \n"
			<< __LINE__;

		return;
	}

	const toprsIRect img_rect = getImgRect();

	// Check for intersect.
	if ( ! img_rect.intersects(src_rect) )
	{
		return; // Nothing to do here.
	}

	// Check the clip rect.
	if (!clip_rect.completely_within(img_rect))
	{
		return;
	}

	// Check the status and allocate memory if needed.
	if (getDataObjectStatus() == TOPRS_NULL) initialize();

	// Get the width of the buffers.
	toprs_uint32 num_bands = getBands();
	toprs_uint32 s_width = src_rect.width();
	toprs_uint32 d_width = w();
	int band;
	toprs_uint32 destinationOffset = (clip_rect.left_up().y - img_rect.left_up().y) * d_width +
		(clip_rect.left_up().x - img_rect.left_up().x);
	toprs_uint32 destinationIndex = destinationOffset;
	toprs_uint32 sourceOffset = (clip_rect.left_up().y - src_rect.left_up().y) * s_width +
		(clip_rect.left_up().x - src_rect.left_up().x);
	toprs_uint32 sourceIndex = sourceOffset;
	toprs_uint32 clipHeight = clip_rect.height();
	toprs_uint32 clipWidth = clip_rect.width();

	if(!multiplyAlphaFlag)
	{
		// Copy the data.
		for (band=0; band<num_bands; band++)
		{
			T* destinationBand = static_cast<T*>(getBuf(band));
			destinationIndex = destinationOffset;
			sourceIndex = sourceOffset;
			T nullPix = (T)getNullPix(band);
			T minPix = (T)getMinPix(band);
			for (toprs_uint32 line = 0; line < clipHeight; ++line)
			{
				for (toprs_uint32 sample = 0; sample < clipWidth; ++sample)
				{
					if(src[sourceIndex+sample] == 0)
					{
						destinationBand[destinationIndex + sample] = nullPix;
					}
					else if(destinationBand[destinationIndex + sample] == nullPix)
					{
						destinationBand[destinationIndex + sample] = minPix;
					}
				}
				sourceIndex += s_width;
				destinationIndex += d_width;
			}
		}
	}
	else
	{
		toprs_float64 normalizer = 1.0/255.0;
		// Copy the data.
		for (band=0; band<num_bands; band++)
		{
			T* destinationBand = static_cast<T*>(getBuf(band));
			destinationIndex = destinationOffset;
			sourceIndex = sourceOffset;
			T nullPix = (T)getNullPix(band);
			T minPix = (T)getMinPix(band);
			for (toprs_uint32 line = 0; line < clipHeight; ++line)
			{
				for (toprs_uint32 sample = 0; sample < clipWidth; ++sample)
				{
					if(src[sourceIndex+sample] != 255)
					{
						if(src[sourceIndex+sample] != 0)
						{
							destinationBand[destinationIndex + sample] = (T)(destinationBand[destinationIndex + sample]*
								(src[sourceIndex+sample]*normalizer));
							if(destinationBand[destinationIndex + sample] == nullPix)
							{
								destinationBand[destinationIndex + sample] = minPix;
							}
						}
						else
						{
							destinationBand[destinationIndex + sample] = nullPix;
						}
					}
					else if(destinationBand[destinationIndex + sample] == nullPix)
					{
						destinationBand[destinationIndex + sample] = minPix;
					}
				}
				sourceIndex += s_width;
				destinationIndex += d_width;
			}
		}

	}
}

void toprsImg::loadTileFromBip(const void* src,
	const toprsIRect& src_rect)
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		loadTileFromBipTemplate(toprs_uint8(0), src, src_rect);
		return;

	case TOPRS_SINT8:
		loadTileFromBipTemplate(toprs_sint8(0), src, src_rect);
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		loadTileFromBipTemplate(toprs_uint16(0), src, src_rect);
		return;

	case TOPRS_SINT16:
		loadTileFromBipTemplate(toprs_sint16(0), src, src_rect);
		return;

	case TOPRS_UINT32:
		loadTileFromBipTemplate(toprs_uint32(0), src, src_rect);
		return;

	case TOPRS_SINT32:
		loadTileFromBipTemplate(toprs_sint32(0), src, src_rect);
		return;

	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		loadTileFromBipTemplate(toprs_float32(0), src, src_rect);
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		loadTileFromBipTemplate(toprs_float64(0), src, src_rect);
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"toprsImg::loadTileFromBip File"
			<<__FILE__
			<< "line \n"
			<< __LINE__;
  
		return;
	}
}

void toprsImg::loadTileFromBip(const void* src,
	const toprsIRect& src_rect,
	const toprsIRect& clip_rect)
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		loadTileFromBipTemplate(toprs_uint8(0), src, src_rect, clip_rect);
		return;

	case TOPRS_SINT8:
		loadTileFromBipTemplate(toprs_sint8(0), src, src_rect, clip_rect);
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		loadTileFromBipTemplate(toprs_uint16(0), src, src_rect, clip_rect);
		return;

	case TOPRS_SINT16:
		loadTileFromBipTemplate(toprs_sint16(0), src, src_rect, clip_rect);
		return;

	case TOPRS_UINT32:
		loadTileFromBipTemplate(toprs_uint32(0), src, src_rect, clip_rect);
		return;

	case TOPRS_SINT32:
		loadTileFromBipTemplate(toprs_sint32(0), src, src_rect, clip_rect);
		return;

	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		loadTileFromBipTemplate(toprs_float32(0), src, src_rect, clip_rect);
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		loadTileFromBipTemplate(toprs_float64(0), src, src_rect, clip_rect);
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"toprsImg::loadTileFromBip File"
			<<__FILE__
			<< "line \n"
			<< __LINE__;
    
		return;
	}
}

void toprsImg::loadTileFromBil(const void* src,
	const toprsIRect& src_rect)
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		loadTileFromBilTemplate(toprs_uint8(0), src, src_rect);
		return;

	case TOPRS_SINT8:
		loadTileFromBilTemplate(toprs_sint8(0), src, src_rect);
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		loadTileFromBilTemplate(toprs_uint16(0), src, src_rect);
		return;

	case TOPRS_SINT16:
		loadTileFromBilTemplate(toprs_sint16(0), src, src_rect);
		return;

	case TOPRS_UINT32:
		loadTileFromBilTemplate(toprs_uint32(0), src, src_rect);
		return;

	case TOPRS_SINT32:
		loadTileFromBilTemplate(toprs_sint32(0), src, src_rect);
		return;

	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		loadTileFromBilTemplate(toprs_float32(0), src, src_rect);
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		loadTileFromBilTemplate(toprs_float64(0), src, src_rect);
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"toprsImg::loadTileFromBil File"
			<<__FILE__
			<< "line \n"
			<< __LINE__;
      
		return;
	}
}

void toprsImg::loadTileFromBil(const void* src,
	const toprsIRect& src_rect,
	const toprsIRect& clip_rect)
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		loadTileFromBilTemplate(toprs_uint8(0), src, src_rect, clip_rect);
		return;

	case TOPRS_SINT8:
		loadTileFromBilTemplate(toprs_sint8(0), src, src_rect, clip_rect);
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		loadTileFromBilTemplate(toprs_uint16(0), src, src_rect, clip_rect);
		return;

	case TOPRS_SINT16:
		loadTileFromBilTemplate(toprs_sint16(0), src, src_rect, clip_rect);
		return;

	case TOPRS_UINT32:
		loadTileFromBilTemplate(toprs_uint32(0), src, src_rect, clip_rect);
		return;

	case TOPRS_SINT32:
		loadTileFromBilTemplate(toprs_sint32(0), src, src_rect, clip_rect);
		return;

	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		loadTileFromBilTemplate(toprs_float32(0), src, src_rect, clip_rect);
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		loadTileFromBilTemplate(toprs_float64(0), src, src_rect, clip_rect);
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"toprsImg::loadTileFromBil File"
			<<__FILE__
			<< "line \n"
			<< __LINE__;
   
		return;
	}
}

void toprsImg::loadTileFromBsq(const void* src,
	const toprsIRect& src_rect)
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		loadTileFromBsqTemplate(toprs_uint8(0), src, src_rect);
		return;

	case TOPRS_SINT8:
		loadTileFromBsqTemplate(toprs_sint8(0), src, src_rect);
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		loadTileFromBsqTemplate(toprs_uint16(0), src, src_rect);
		return;

	case TOPRS_SINT16:
		loadTileFromBsqTemplate(toprs_sint16(0), src, src_rect);
		return;

	case TOPRS_UINT32:
		loadTileFromBsqTemplate(toprs_uint32(0), src, src_rect);
		return;

	case TOPRS_SINT32:
		loadTileFromBsqTemplate(toprs_sint32(0), src, src_rect);
		return;

	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		loadTileFromBsqTemplate(toprs_float32(0), src, src_rect);
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		loadTileFromBsqTemplate(toprs_float64(0), src, src_rect);
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"toprsImg::loadTileFromBsq File"
			<<__FILE__
			<< "line \n"
			<< __LINE__;
   
		return;
	}
}

void toprsImg::loadTileFromBsq(const void* src,
	const toprsIRect& src_rect,
	const toprsIRect& clip_rect)
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		loadTileFromBsqTemplate(toprs_uint8(0), src, src_rect, clip_rect);
		return;

	case TOPRS_SINT8:
		loadTileFromBsqTemplate(toprs_sint8(0), src, src_rect, clip_rect);
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		loadTileFromBsqTemplate(toprs_uint16(0), src, src_rect, clip_rect);
		return;

	case TOPRS_SINT16:
		loadTileFromBsqTemplate(toprs_sint16(0), src, src_rect, clip_rect);
		return;

	case TOPRS_UINT32:
		loadTileFromBsqTemplate(toprs_uint32(0), src, src_rect, clip_rect);
		return;

	case TOPRS_SINT32:
		loadTileFromBsqTemplate(toprs_sint32(0), src, src_rect, clip_rect);
		return;

	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		loadTileFromBsqTemplate(toprs_float32(0), src, src_rect, clip_rect);
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		loadTileFromBsqTemplate(toprs_float64(0), src, src_rect, clip_rect);
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"toprsImg::loadTileFromBsq File"
			<<__FILE__
			<< "line \n"
			<< __LINE__;
    
		return;
	}
}


template <class T>
void toprsImg::loadTileFromBipTemplate(T, // dummy template variable
	const void* src,
	const toprsIRect& src_rect)
{
	const toprsIRect img_rect = getImgRect();

	// Get the clip rect.
	const toprsIRect clip_rect = img_rect.clipToRect(src_rect);

	loadTileFromBipTemplate(T(0), src, src_rect, clip_rect);
}

template <class T>
void toprsImg::loadTileFromBipTemplate(T, // dummy template variable
	const void* src,
	const toprsIRect& src_rect,
	const toprsIRect& clip_rect)
{

	// Check the pointer.
	if (!src)
	{
		std::cout << 
			"toprsImg::loadTileFromBip File"
			<<__FILE__
			<< "line \n"
			<< __LINE__;

		return;
	}

	const toprsIRect img_rect = getImgRect();

	// Check for intersect.
	if ( ! img_rect.intersects(src_rect) )
	{
		return; // Nothing to do here.
	}

	// Check the clip rect.
	if (!clip_rect.completely_within(img_rect))
	{
		return;
	}

	// Check the status and allocate memory if needed.
	if (getDataObjectStatus() == TOPRS_NULL) initialize();

	// Get the width of the buffers.
	toprs_uint32 num_bands  = getBands();
	toprs_uint32 s_width    = (src_rect.width()) * num_bands;
	toprs_uint32 d_width    = w();
	int band       = 0;
	const T* s        = static_cast<const T*>(src);
	T** d             = new T*[num_bands];

	// Make destination pointers to each one.
	for (band=0; band<num_bands; band++)
	{
		d[band] = static_cast<T*>(getBuf(band));

		// Move the pointers to the first valid pixel.
		d[band] += (clip_rect.left_up().y - img_rect.left_up().y) * d_width +
			clip_rect.left_up().x - img_rect.left_up().x;
	}

	// Move the source pointer to the first valid pixel.
	s += (clip_rect.left_up().y - src_rect.left_up().y) * s_width +
		(clip_rect.left_up().x - src_rect.left_up().x) * num_bands;

	// Copy the data.
	toprs_uint32 clipHeight = clip_rect.height();
	toprs_uint32 clipWidth  = clip_rect.width();

	for (toprs_uint32 line = 0; line < clipHeight; ++line)
	{
		toprs_uint32 j = 0;
		for (toprs_uint32 sample = 0; sample < clipWidth; ++sample)
		{
			for (band=0; band<num_bands; band++)
			{
				d[band][sample] = s[j+band];
			}
			j += num_bands;
		}

		s += s_width;
		for (band=0; band<num_bands; band++)
		{
			d[band] += d_width;
		}
	}

	delete [] d;
}

template <class T>
void toprsImg::loadTileFromBilTemplate(T, // dummy template variable
	const void* src,
	const toprsIRect& src_rect)
{
	const toprsIRect img_rect = getImgRect();

	// Get the clip rect.
	const toprsIRect clip_rect = img_rect.clipToRect(src_rect);

	loadTileFromBilTemplate(T(0), src, src_rect, clip_rect);
}

template <class T>
void toprsImg::loadTileFromBilTemplate(T, // dummy template variable
	const void* src,
	const toprsIRect& src_rect,
	const toprsIRect& clip_rect)
{
	// Check the pointer.
	if (!src)
	{
		std::cout << 
			"toprsImg::loadTileFromBil File"
			<<__FILE__
			<< "line \n"
			<< __LINE__;

		return;
	}

	const toprsIRect img_rect = getImgRect();

	// Check for intersect.
	if ( ! img_rect.intersects(src_rect) )
	{
		return; // Nothing to do here.
	}

	// Check the clip rect.
	if (!clip_rect.completely_within(img_rect))
	{
		return;
	}

	// Check the status and allocate memory if needed.
	if (getDataObjectStatus() == TOPRS_NULL) initialize();

	// Get the width of the buffers.
	toprs_uint32 num_bands = getBands();
	toprs_uint32 s_width = (src_rect.right_down().x - src_rect.left_up().x + 1);
	toprs_uint32 d_width = w();
	int band = 0;
	toprs_uint32 d_offset = (clip_rect.left_up().y - img_rect.left_up().y) * d_width +
		clip_rect.left_up().x - img_rect.left_up().x;

	const T* s = static_cast<const T*>(src);

	// Get the number of bands and make destination pointers to each one.
	T** d = new T*[num_bands];

	for (band=0; band<num_bands; band++)
	{
		d[band] = static_cast<T*>(getBuf(band));

		// Move the pointers to the first valid pixel.
		d[band] += d_offset;
	}

	// Move the source pointer to the first valid pixel.
	s += (clip_rect.left_up().y - src_rect.left_up().y) * s_width * num_bands +
		clip_rect.left_up().x - src_rect.left_up().x;

	// Copy the data.
	toprs_uint32 clipHeight = clip_rect.height();
	toprs_uint32 clipWidth  = clip_rect.width();

	for (toprs_uint32 line = 0; line < clipHeight; ++line)
	{
		for (band = 0; band < num_bands; ++band)
		{
			for (toprs_uint32 sample = 0; sample < clipWidth; ++sample)
			{
				d[band][sample] = s[sample];
			}
			s       += s_width;
			d[band] += d_width;
		}
	}

	delete [] d;
}

template <class T>
void toprsImg::loadTileFromBsqTemplate(T, // dummy template variable
	const void* src,
	const toprsIRect& src_rect)
{
	const toprsIRect img_rect = getImgRect();

	// Get the clip rect.
	const toprsIRect clip_rect = img_rect.clipToRect(src_rect);

	loadTileFromBsqTemplate(T(0), src, src_rect, clip_rect);
}

template <class T>
void toprsImg::loadTileFromBsqTemplate(T, // dummy template variable
	const void* src,
	const toprsIRect& src_rect,
	const toprsIRect& clip_rect)
{
	static const char  MODULE[] = "toprsImg::loadTileFromBsq";

	// Check the pointer.
	if (!src)
	{
		// Set the error...
		std::cout << 
			"toprsImg::loadTileFromBsq File"
			<<__FILE__
			<< "line \n"
			<< __LINE__;

		return;
	}

	const toprsIRect img_rect = getImgRect();

	// Check for intersect.
	if ( ! img_rect.intersects(src_rect) )
	{
		return; // Nothing to do here.
	}

	// Check the clip rect.
	if (!clip_rect.completely_within(img_rect))
	{
		return;
	}

	// Check the status and allocate memory if needed.
	if (getDataObjectStatus() == TOPRS_NULL) initialize();

	// Get the width of the buffers.
	toprs_uint32 num_bands = getBands();
	toprs_uint32 s_width = src_rect.width();
	toprs_uint32 d_width = w();
	toprs_uint32 s_band_offset = s_width * src_rect.height();

	const T* s = static_cast<const T*>(src);

	int band;
	toprs_uint32 destinationOffset = (clip_rect.left_up().y - img_rect.left_up().y) * d_width +
		(clip_rect.left_up().x - img_rect.left_up().x);
	toprs_uint32 destinationIndex = destinationOffset;
	toprs_uint32 sourceOffset = (clip_rect.left_up().y - src_rect.left_up().y) * s_width +
		(clip_rect.left_up().x - src_rect.left_up().x);
	toprs_uint32 sourceIndex = sourceOffset;
	toprs_uint32 clipHeight = clip_rect.height();
	toprs_uint32 clipWidth = clip_rect.width();

	// Copy the data.
	for (band=0; band<num_bands; band++)
	{
		T* destinationBand = static_cast<T*>(getBuf(band));
		destinationIndex = destinationOffset;
		sourceIndex = sourceOffset + s_band_offset*band;

		for (toprs_uint32 line = 0; line < clipHeight; ++line)
		{
			for (toprs_uint32 sample = 0; sample < clipWidth; ++sample)
			{
				destinationBand[destinationIndex + sample]
				= s[sourceIndex+sample];
			}
			sourceIndex += s_width;
			destinationIndex += d_width;
		}
	}
}

void toprsImg::loadTileFrom1Band(const void* src,
	const toprsIRect& src_rect)
{
	int bands = getBands();
	for(int band = 0; band < bands; ++band)
	{
		loadBand(src, src_rect, band);
	}
}

void toprsImg::loadTileFrom1Band(const toprsImg* src)
{
	if(!src) return;
	loadTileFrom1Band((const void*)src->getBuf(),
		src->getImgRect());
}

void toprsImg::loadShortBand(const void* src,
	const toprsIRect& src_rect,
	int band,
	bool swap_bytes)
{
	static const char  MODULE[] = "toprsImg::loadShortBand";

	// Check the pointer.
	if (!src)
	{
		// Set the error...
		std::cout << 
			MODULE
			<<__FILE__
			<< "line \n"
			<< __LINE__;

		return;
	}

	// Check the band.
	if (!isValidBand(band))
	{
		std::cout << 
			MODULE
			<<__FILE__
			<< "line \n"
			<< __LINE__;

		return;
	}

	const toprsIRect img_rect = getImgRect();

	// Check for intersect.
	if ( ! img_rect.intersects(src_rect) )
	{
		return; // Nothing to do here.
	}

	// Get the clip rect.
	const toprsIRect clip_rect = img_rect.clipToRect(src_rect);

	// Check the status and allocate memory if needed.
	if (getDataObjectStatus() == TOPRS_NULL) initialize();

	// Get the width of the buffers.
	toprs_uint32 s_width = src_rect.width();
	toprs_uint32 d_width = w();

	const toprs_uint16* s = static_cast<const toprs_uint16*>(src);
	toprs_uint16* d = getUshortBuf(band);

	// Move the pointers to the first valid pixel.
	s += (clip_rect.left_up().y - src_rect.left_up().y) * s_width +
		clip_rect.left_up().x - src_rect.left_up().x;

	d += (clip_rect.left_up().y - img_rect.left_up().y) * d_width +
		clip_rect.left_up().x - img_rect.left_up().x;

	// Copy the data.
	for (toprs_int32 line=clip_rect.left_up().y; line<=clip_rect.right_down().y; line++)
	{
		toprs_uint32 i = 0;
		for (toprs_int32 sample=clip_rect.left_up().x; sample<=clip_rect.right_down().x; sample++)
		{
			d[i] = (swap_bytes ? ( (s[i] << 8) | (s[i] >> 8) ) :
				s[i]);
		++i;
		}

		s += s_width;
		d += d_width;
	}
	validate();
}

void toprsImg::unloadBand(void* dest,
	const toprsIRect& dest_rect,
	int band) const
{
	unloadBand(dest, dest_rect, getImgRect(), band);
}

void toprsImg::unloadBand(void* dest,
	const toprsIRect& dest_rect,
	const toprsIRect& clip_rect,
	int band) const
{
	// Call the appropriate load method.
	switch (getDataType())
	{
	case TOPRS_UINT8:
		unloadBandTemplate(toprs_uint8(0), dest, dest_rect, clip_rect, band);
		return;

	case TOPRS_SINT8:
		unloadBandTemplate(toprs_sint8(0), dest, dest_rect, clip_rect, band);
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		unloadBandTemplate(toprs_uint16(0), dest, dest_rect, clip_rect, band);
		return;

	case TOPRS_SINT16:
		unloadBandTemplate(toprs_sint16(0), dest, dest_rect, clip_rect, band);
		return;

	case TOPRS_UINT32:
		unloadBandTemplate(toprs_uint32(0), dest, dest_rect, clip_rect, band);
		return;

	case TOPRS_SINT32:
		unloadBandTemplate(toprs_sint32(0), dest, dest_rect, clip_rect, band);
		return;

	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		unloadBandTemplate(toprs_float32(0), dest, dest_rect, clip_rect, band);
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		unloadBandTemplate(toprs_float64(0), dest, dest_rect, clip_rect, band);
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"Unknown data"
			<<__FILE__
			<< "line \n"
			<< __LINE__;    
		return;
	}
}

void toprsImg::unloadBand( void* dest,
	int src_band,
	int dest_band,
	const toprsIRect& dest_rect,
	toprsInterleaveType il_type,
	OverwriteBandRule ow_type) const
{
	unloadBand( dest, src_band, dest_band, dest_rect, getImgRect(), il_type, ow_type);
}

void toprsImg::unloadBand( void* dest,
	int src_band,
	int dest_band,
	const toprsIRect& dest_rect,
	const toprsIRect& clip_rect,
	toprsInterleaveType il_type,
	OverwriteBandRule ow_type) const
{
	static const char  MODULE[] = "toprsImg::unloadBand";

	if ( il_type == TOPRS_BSQ )
	{
		unloadBandToBsq( dest, src_band, dest_band, dest_rect, clip_rect, ow_type );
	}
	else
	{
		std::cout << 
			MODULE
			<<__FILE__
			<< "line \n"
			<< __LINE__;
	}
}




void toprsImg::unloadBandToBsq( void* dest,int src_band, int dest_band, const toprsIRect& dest_rect, const toprsIRect& clip_rect,OverwriteBandRule ow_type ) const
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		unloadBandToBsqTemplate(toprs_uint8(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type  );
		return;

	case TOPRS_SINT8:
		unloadBandToBsqTemplate(toprs_sint8(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type  );
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		unloadBandToBsqTemplate(toprs_uint16(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type  );
		return;

	case TOPRS_SINT16:
		unloadBandToBsqTemplate(toprs_sint16(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type  );
		return;

	case TOPRS_UINT32:
		unloadBandToBsqTemplate(toprs_uint32(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type  );
		return;

	case TOPRS_SINT32:
		unloadBandToBsqTemplate(toprs_sint32(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type  );
		return;

	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		unloadBandToBsqTemplate(toprs_float32(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type  );
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		unloadBandToBsqTemplate(toprs_float64(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type  );
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"toprsImg::unloadBandToBsq"
			<<__FILE__
			<< "line \n"
			<< __LINE__;     
		return;
	}
}
template <class T> void
	toprsImg::unloadBandToBsqTemplate(T,  // dummy template arg...
	void* dest,
	toprs_uint32 src_band,
	toprs_uint32 dest_band,
	const toprsIRect& dest_rect,
	const toprsIRect& clip_rect,
	OverwriteBandRule ow_type) const
{
	static const char  MODULE[] = "toprsImg::unloadBandToBsq";

	// Check the pointers.
	if (!dest)
	{
		std::cout
			<< MODULE << " ERROR:"
			<< "\nNULL pointer passed to method!  Returning..." << std::endl;
		return;
	}

	bool dataIsNull = false;
	if (getDataObjectStatus() == TOPRS_NULL)
	{
		dataIsNull = true;
	}

	toprsIRect img_rect = getImgRect();

	// Clip the clip_rect to the tile rect.
	toprsIRect output_clip_rect = clip_rect.clipToRect(img_rect);

	// Clip it again to the destination rect.
	output_clip_rect = dest_rect.clipToRect(output_clip_rect);

	// Check the output clip rect for intersection.
	if (output_clip_rect.hasNAN())
	{
		return;
	}
	if ( !(output_clip_rect.intersects(dest_rect)) )
	{
		return;
	}

	toprs_uint32 num_bands = getBands();
	toprs_uint32 band      = 0;
	if(!dataIsNull)
	{
		toprs_uint32 d_width       = dest_rect.right_down().x - dest_rect.left_up().x + 1;
		toprs_uint32 d_band_offset = d_width * (dest_rect.right_down().y-dest_rect.left_up().y+1);
		toprs_uint32 s_width  = w();
		toprs_uint32 s_offset = (output_clip_rect.left_up().y - img_rect.left_up().y) *
			s_width + (output_clip_rect.left_up().x - img_rect.left_up().x);

		T* d        = static_cast<T*>(dest);
		const T** s = new const T*[num_bands];

		// Grab a pointers to each one.
		for (band=0; band<num_bands; ++band)
		{
			s[band] = reinterpret_cast<const T*>(getBuf(band));

			// Move to first valid pixel.
			s[band] += s_offset;
		}

		// Move to first valid pixel.
		d += (output_clip_rect.left_up().y - dest_rect.left_up().y) * d_width +
			(output_clip_rect.left_up().x - dest_rect.left_up().x);

		toprs_uint32 d_dest_band_offset = dest_band * d_band_offset;
		toprs_uint32 d_buf_offset = 0;

		for (toprs_int32 line=output_clip_rect.left_up().y;
			line<=output_clip_rect.right_down().y; ++line)
		{
			toprs_int32 i=0;
			for (toprs_int32 samp=output_clip_rect.left_up().x;
				samp<=output_clip_rect.right_down().x; ++samp)
			{
				toprs_uint32 d_pixel_offset = d_buf_offset+i;
				toprs_uint32 d_dest_band_pixel_offset = d_pixel_offset + d_dest_band_offset;

				switch( ow_type )
				{
				case COLOR_DISCREPANCY:
				case COLOR_DISCREPANCY_OF_ANY_FROM_DEST:
					{
						T d_dest_band = d[d_dest_band_pixel_offset];

						for ( band=0; band<num_bands && band!=dest_band; ++band )
						{
							T d_other_band = d[d_pixel_offset + (band * d_band_offset)];

							// test for color discrepancy
							if ( d_other_band != d_dest_band )
							{
								d[d_dest_band_pixel_offset] = s[src_band][i];
								break;
							}
						}
					}
					break;

				case COLOR_DISCREPANCY_OF_ALL_FROM_DEST:
					{
						T d_dest_band = d[d_dest_band_pixel_offset];

						bool bFoundSameValue = false;
						for ( band=0; band<num_bands && band!=dest_band; ++band )
						{
							T d_other_band = d[d_pixel_offset + (band * d_band_offset)];

							// test for color sameness
							if ( d_other_band == d_dest_band )
							{
								bFoundSameValue = true;
								break;
							}
						}
						if ( bFoundSameValue == false )
						{
							d[d_dest_band_pixel_offset] = s[src_band][i];
						}
					}
					break;

				case COLOR_EQUALITY_OF_ANY_TO_SRC:
					{
						T d_src_band = s[src_band][i];

						for ( band=0; band<num_bands && band!=dest_band; ++band )
						{
							T d_other_band = d[d_pixel_offset + (band * d_band_offset)];

							// test for color discrepancy
							if ( d_other_band == d_src_band )
							{
								d[d_dest_band_pixel_offset] = s[src_band][i];
								break;
							}
						}
					}
					break;

				case COLOR_EQUALITY_OF_ALL_TO_SRC:
					{
						T d_src_band = s[src_band][i];

						bool bFoundDifferentValue = false;
						for ( band=0; band<num_bands && band!=dest_band; ++band )
						{
							T d_other_band = d[d_pixel_offset + (band * d_band_offset)];

							// test for color discrepancy
							if ( d_other_band != d_src_band )
							{
								bFoundDifferentValue = true;
								break;
							}
						}
						if ( bFoundDifferentValue == false )
						{
							d[d_dest_band_pixel_offset] = s[src_band][i];
						}
					}
					break;

				case NULL_RULE:
				default:
					{
						d[d_dest_band_pixel_offset] = s[src_band][i];
					}
					break;
				}

				++i;
			}

			d_buf_offset += d_width;
			s[src_band]  += s_width;
		}

		// Free up memory allocated for pointers.
		delete [] s;
	}
	else
	{
		toprs_uint32 d_width       = dest_rect.right_down().x - dest_rect.left_up().x + 1;
		toprs_uint32 d_band_offset = d_width * (dest_rect.right_down().y-dest_rect.left_up().y+1);

		toprs_uint8* d = static_cast<toprs_uint8*>(dest);

		// Move to first valid pixel.
		d += (output_clip_rect.left_up().y - dest_rect.left_up().y) * d_width +
			(output_clip_rect.left_up().x - dest_rect.left_up().x);

		for (band=0; band<num_bands; ++band)
		{
			toprs_uint8 np = static_cast<toprs_uint8>(theNullPixelV[band]);
			toprs_uint32 d_buf_offset = 0;

			for (toprs_int32 line=output_clip_rect.left_up().y;
				line<=output_clip_rect.right_down().y; ++line)
			{
				toprs_int32 i=0;
				for (toprs_int32 samp=output_clip_rect.left_up().x;
					samp<=output_clip_rect.right_down().x; ++samp)
				{
					d[d_buf_offset+i] = np;
					++i;
				}

				d_buf_offset += d_width;
			}
			d += d_band_offset;
		}
	}
}
template <class T>
void toprsImg::unloadBandTemplate(T, // dummy template variable
	void* dest,
	const toprsIRect& dest_rect,
	const toprsIRect& clip_rect,
	int band) const
{
	static const char  MODULE[] = "toprsImg::unloadBand";

	// Check the pointers.
	if (!dest)
	{
		std::cout << 
			MODULE
			<<__FILE__
			<< "line \n"
			<< __LINE__;   
		return;
	}

	if (getDataObjectStatus() == TOPRS_NULL)
	{
		std::cout << 
			MODULE
			<<__FILE__
			<< "line \n"
			<< __LINE__;   
		return;
	}

	toprsIRect img_rect = getImgRect();

	// Clip the clip_rect to the tile rect.
	toprsIRect output_clip_rect = clip_rect.clipToRect(img_rect);

	// Clip it again to the destination rect.
	output_clip_rect = dest_rect.clipToRect(output_clip_rect);

	// Check the output clip rect for intersection.
	if (!output_clip_rect.intersects(img_rect))
	{
		return;
	}
	if ( !(output_clip_rect.intersects(dest_rect)) )
	{
		return;
	}

	// Check for valid band.
	if (!isValidBand(band))
	{
		std::cout << 
			MODULE
			<<__FILE__
			<< "line \n"
			<< __LINE__
			<< std::endl;
		return;
	}

	toprs_uint32 buf_width = dest_rect.right_down().x - dest_rect.left_up().x + 1;

	// Get a pointer to the source band buffer.
	const T* s = reinterpret_cast<const T*>(getBuf(band));

	// Get the offset for each source band.
	toprs_uint32 src_offset = (output_clip_rect.left_up().y - img_rect.left_up().y) *
		w() + output_clip_rect.left_up().x - img_rect.left_up().x;

	// Move the pointers to the start.
	T* d = reinterpret_cast<T*>(dest);

	toprs_uint32 dest_offset = (output_clip_rect.left_up().y - dest_rect.left_up().y) *
		buf_width + output_clip_rect.left_up().x - dest_rect.left_up().x;

	d += dest_offset;
	s += src_offset;

	for (toprs_int32 line=output_clip_rect.left_up().y;
		line<=output_clip_rect.right_down().y; ++line)
	{
		toprs_uint32 i=0;
		for (toprs_int32 samp=clip_rect.left_up().x;
			samp<=output_clip_rect.right_down().x; ++samp)
		{
			d[i] = s[i];
			++i;
		}
		d += buf_width;
		s += w();
	}
}

template <class T> void
	toprsImg::unloadTileToBipTemplate(T, // dummy template variable
	void* dest,
	const toprsIRect& dest_rect,
	const toprsIRect& clip_rect) const
{
	static const char  MODULE[] = "toprsImg::unloadTileToBip";

	// Check the pointer.
	if (!dest)
	{
		std::cout << 
			MODULE
			<<__FILE__
			<< "line \n"
			<< __LINE__
		<< std::endl;

		return;
	}

	bool  dataIsNull = false;
	if (getDataObjectStatus() == TOPRS_NULL)
	{
		dataIsNull = true;
	}

	toprsIRect img_rect = getImgRect();

	// Clip the clip_rect to the tile rect.
	toprsIRect output_clip_rect = clip_rect.clipToRect(img_rect);

	// Clip it again to the destination rect.
	output_clip_rect = dest_rect.clipToRect(output_clip_rect);

	// Check the output clip rect for intersection.
	if (output_clip_rect.hasNAN())
	{
		return;
	}
	if ( !(output_clip_rect.intersects(dest_rect)) )
	{
		return;
	}

	toprs_int32 num_bands = getBands();

	toprs_int32 buf_width = dest_rect.width() * num_bands;

	if(!dataIsNull)
	{
		// Get the number of bands and grab a pointers to each one.
		const T** s = new const T*[num_bands];

		toprs_int32 band;
		toprs_int32 s_width = w();

		for (band=0; band<num_bands; band++)
		{
			s[band] = reinterpret_cast<const T*>(getBuf(band));
		}

		// Move the pointers to the start.
		T* d = reinterpret_cast<T*>(dest);      

		d += (output_clip_rect.left_up().y - dest_rect.left_up().y) * buf_width +
			(output_clip_rect.left_up().x - dest_rect.left_up().x) * num_bands;

		// Get the offset for each source band.
		toprs_int32 src_offset = (output_clip_rect.left_up().y - img_rect.left_up().y) *
			s_width + (output_clip_rect.left_up().x - img_rect.left_up().x);

		toprs_int32 output_clip_width  = output_clip_rect.width();
		toprs_int32 output_clip_height = output_clip_rect.height();

		for (band=0; band<(toprs_int32)getBands(); band++)
		{
			s[band] += src_offset;
		}

		toprs_int32 j;
		for (toprs_int32 line=0; line<output_clip_height; ++line)
		{
			j = 0;
			for (toprs_int32 samp=0; samp<output_clip_width; ++samp, j+=num_bands)
			{
				for (band=0; band<num_bands; ++band)
				{
					d[j+band] = s[band][samp];
				}
			}

			// increment to next line...
			d += buf_width;
			for (band=0; band<num_bands; ++band)
			{
				s[band] += s_width;
			}

		}
		delete [] s;
	}
	else
	{
		toprs_int32 band = 0;
		T* nulls = new T[num_bands];
		for(band = 0; band < num_bands; ++band)
		{
			nulls[band] = static_cast<T>(theNullPixelV[band]);
		}
		// Move the pointers to the start.
		T* d = reinterpret_cast<T*>(dest);

		d += (output_clip_rect.left_up().y - dest_rect.left_up().y) * buf_width +
			(output_clip_rect.left_up().x - dest_rect.left_up().x) * num_bands;

		for (toprs_int32 line=output_clip_rect.left_up().y;
			line<=output_clip_rect.right_down().y; ++line)
		{
			toprs_int32 i=0;
			toprs_int32 j=0;
			for (toprs_int32 samp=output_clip_rect.left_up().x;
				samp<=output_clip_rect.right_down().x; ++samp)
			{
				for (band=0; band<num_bands; ++band)
				{
					d[j+band] = nulls[band];
				}
				++i;  // increment to next pixel...
				j+= num_bands;
			}

			// increment to next line...
			d += buf_width;
		}
		delete [] nulls;
		nulls = 0;
	}
}


template <class T> void
	toprsImg::unloadTileToBipAlphaTemplate(T, // dummy template variable
	void* dest,
	const toprsIRect& dest_rect,
	const toprsIRect& clip_rect) const
{
	static const char  M[] = "toprsImg::unloadTileToBipAlphaTemplate";

	// Sanity checks:

	// Are we null:
	if (getDataObjectStatus() == TOPRS_NULL)
	{
		std::cout
			<< M << " ERROR:"
			<< "\nAttempting to copy null tile!  Returning..." << std::endl;
		return;
	}

	// Check the pointer.
	if (!dest)
	{
		std::cout
			<< M << " ERROR:"
			<< "\nNULL pointer passed to method!  Returning..." << std::endl;
		return;
	}

	if ( !hasAlpha() )
	{
		std::cout
			<< M << " ERROR:"
			<< "\nTile has no alpha!  Returning..." << std::endl;
		return; 
	}

	toprsIRect img_rect = getImgRect();

	// Clip the clip_rect to the tile rect.
	toprsIRect output_clip_rect = clip_rect.clipToRect(img_rect);

	// Clip it again to the destination rect.
	output_clip_rect = dest_rect.clipToRect(output_clip_rect);

	// Check the output clip rect for intersection.
	if (output_clip_rect.hasNAN())
	{
		return;
	}
	if ( !(output_clip_rect.intersects(dest_rect)) )
	{
		return;
	}

	const toprs_int32 NUM_DATA_BANDS = getBands();
	const toprs_int32 BANDS = NUM_DATA_BANDS + 1; // Has data plus alpha.
	const toprs_float64 MAX_PIX = static_cast<T>(getMaxPix(0));
	const toprs_float64 ALPHA_MAX_PIX = 255.00;
	const toprs_int32 S_WIDTH = w();
	const toprs_int32 D_WIDTH = dest_rect.width() * BANDS;
	const toprs_int32 OUTPUT_CLIP_WIDTH  = output_clip_rect.width();
	const toprs_int32 OUTPUT_CLIP_HEIGHT = output_clip_rect.height();

	bool uint8Flag = ( getDataType() == TOPRS_UINT8 );

	// Get the offset for each source band relative to start of clip rect.
	const toprs_int32 SRC_OFFSET = (output_clip_rect.left_up().y - img_rect.left_up().y) *
		S_WIDTH + (output_clip_rect.left_up().x - img_rect.left_up().x);

	// Get pointers to each one band of image data.
	std::vector<const T*> s(NUM_DATA_BANDS);
	toprs_int32 band;
	for (band = 0; band < NUM_DATA_BANDS; ++band)
	{
		s[band] = reinterpret_cast<const T*>(getBuf(band));

		// Move to start of clip rect.
		s[band] += SRC_OFFSET;
	}

	// Get pointer to the alpha channel:
	const toprs_uint8* a = getAlphaBuf();

	// Move to start of clip rect.
	a += SRC_OFFSET;

	// Pointer to the destination buffer.
	T* d = reinterpret_cast<T*>(dest); 

	// Move to start of clip rect.
	d += (output_clip_rect.left_up().y - dest_rect.left_up().y) * D_WIDTH +
		(output_clip_rect.left_up().x - dest_rect.left_up().x) * BANDS;


	// Loop to copy data:
	for (toprs_int32 line = 0; line < OUTPUT_CLIP_HEIGHT; ++line)
	{
		toprs_int32 j = 0;
		toprs_int32 alphaIdx = 0;
		for (toprs_int32 samp = 0; samp < OUTPUT_CLIP_WIDTH; ++samp, j +=BANDS)
		{
			// Copy the pixels:
			for (band = 0; band < NUM_DATA_BANDS; ++band)
			{
				d[j+band] = s[band][samp];
			}

			// Copy alpha channel converting to scalar type.
			if ( uint8Flag )
			{
				d[j+NUM_DATA_BANDS] = a[alphaIdx++];
			}
			else
			{
				d[j+NUM_DATA_BANDS] =
					static_cast<T>( (a[alphaIdx++]/ALPHA_MAX_PIX) * MAX_PIX );
			}
		}

		// Increment pointers to next line...
		d += D_WIDTH;
		for (band = 0; band < NUM_DATA_BANDS; ++band)
		{
			s[band] += S_WIDTH;
		}
		a += S_WIDTH;
	}
}

template <class T> void
	toprsImg::unloadTileToBilTemplate(T,  // dummy template arg...
	void* dest,
	const toprsIRect& dest_rect,
	const toprsIRect& clip_rect) const
{
	static const char  MODULE[] = "toprsImg::unloadTileToBil";

	// Check the pointers.
	if (!dest)
	{
		std::cout
			<< MODULE << " ERROR:"
			<< "\nNULL pointer passed to method!  Returning..." << std::endl;
		return;
	}
	bool dataIsNull = false;
	if (getDataObjectStatus() == TOPRS_NULL)
	{
		dataIsNull = true;
	}

	toprsIRect img_rect = getImgRect();

	// Clip the clip_rect to the tile rect.
	toprsIRect output_clip_rect = clip_rect.clipToRect(img_rect);

	// Clip it again to the destination rect.
	output_clip_rect = dest_rect.clipToRect(output_clip_rect);

	// Check the output clip rect for intersection.
	if (output_clip_rect.hasNAN())
	{
		return;
	}
	if ( !(output_clip_rect.intersects(dest_rect)) )
	{
		return;
	}

	toprs_uint32 num_bands = getBands();

	toprs_uint32 buf_width = dest_rect.width();

	if(!dataIsNull)
	{
		// Get the number of bands and grab a pointers to each one.
		const T** s = new const T*[num_bands];

		int band;

		for (band=0; band<num_bands; band++)
		{
			s[band] = reinterpret_cast<const T*>(getBuf(band));
		}

		// Get the offset for each source band.
		toprs_uint32 src_offset = (output_clip_rect.left_up().y - img_rect.left_up().y) *
			w() + (output_clip_rect.left_up().x - img_rect.left_up().x);

		// Move the pointers to the start.
		T* d = reinterpret_cast<T*>(dest);

		d += (output_clip_rect.left_up().y - dest_rect.left_up().y) *
			buf_width * num_bands +
			(output_clip_rect.left_up().x-dest_rect.left_up().x);

		for (band=0; band<num_bands; ++band)
		{
			s[band] += src_offset;
		}

		for (toprs_int32 line=output_clip_rect.left_up().y;
			line<=output_clip_rect.right_down().y; ++line)
		{
			for (band=0; band<num_bands; ++band)
			{
				toprs_uint32 i=0;
				for (toprs_int32 samp=output_clip_rect.left_up().x;
					samp<=output_clip_rect.right_down().x; ++samp)
				{
					d[i] = s[band][i];
					++i;
				}
				d       += buf_width;
				s[band] += w();
			}
		}
		// Free up memory allocated for pointers.
		delete [] s;
	}
	else
	{
		int band = 0;
		T* nulls = new T[num_bands];
		for(band = 0; band < num_bands; ++band)
		{
			nulls[band] = static_cast<T>(theNullPixelV[band]);
		}

		// Move the pointers to the start.
		T* d = reinterpret_cast<T*>(dest);

		d += (output_clip_rect.left_up().y - dest_rect.left_up().y) *
			buf_width * num_bands + (output_clip_rect.left_up().x - dest_rect.left_up().x);

		for (toprs_int32 line=output_clip_rect.left_up().y;
			line<=output_clip_rect.right_down().y; ++line)
		{
			for (band=0; band<num_bands; ++band)
			{
				toprs_uint32 i=0;
				for (toprs_int32 samp=output_clip_rect.left_up().x;
					samp<=output_clip_rect.right_down().x; ++samp)
				{
					d[i] = nulls[band];
					++i;
				}
				d += buf_width;
			}
		}
		delete [] nulls;
	}
}

void toprsImg::unloadTile(void* dest,
	const toprsIRect& dest_rect,
	toprsInterleaveType type) const
{
	unloadTile(dest, dest_rect, getImgRect(), type);
}

void toprsImg::unloadTile(void* dest,
	const toprsIRect& dest_rect,
	const toprsIRect& clip_rect,
	toprsInterleaveType type) const
{
	switch (type)
	{
	case TOPRS_BIP:
		unloadTileToBip(dest, dest_rect, clip_rect);
		return;
	case TOPRS_BIL:
		unloadTileToBil(dest, dest_rect, clip_rect);
		return;
	case TOPRS_BSQ:
		unloadTileToBsq(dest, dest_rect, clip_rect);
		return;
	default:
		std::cout << 
			"toprsImg::unloadTile\n"
			<<__FILE__
			<< "line \n"
			<< __LINE__; 
     
		return;
	}
}

void toprsImg::unloadTileToBip(void* dest,
	const toprsIRect& dest_rect,
	const toprsIRect& clip_rect) const
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		unloadTileToBipTemplate(toprs_uint8(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_SINT8:
		unloadTileToBipTemplate(toprs_sint8(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		unloadTileToBipTemplate(toprs_uint16(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_SINT16:
		unloadTileToBipTemplate(toprs_sint16(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_UINT32:
		unloadTileToBipTemplate(toprs_uint32(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_SINT32:
		unloadTileToBipTemplate(toprs_sint32(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		unloadTileToBipTemplate(toprs_float32(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		unloadTileToBipTemplate(toprs_float64(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"toprsImg::unloadTileToBip\n"
			<<__FILE__
			<< "line \n"
			<< __LINE__; 
  
		return;
	}
}


void toprsImg::unloadTileToBipAlpha(void* dest,
	const toprsIRect& dest_rect,
	const toprsIRect& clip_rect) const
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		unloadTileToBipAlphaTemplate(toprs_uint8(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_SINT8:
		unloadTileToBipAlphaTemplate(toprs_sint8(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		unloadTileToBipAlphaTemplate(toprs_uint16(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_SINT16:
		unloadTileToBipAlphaTemplate(toprs_sint16(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_UINT32:
		unloadTileToBipAlphaTemplate(toprs_uint32(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_SINT32:
		unloadTileToBipAlphaTemplate(toprs_sint32(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		unloadTileToBipAlphaTemplate(toprs_float32(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		unloadTileToBipAlphaTemplate(toprs_float64(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"toprsImg::unloadTileToBipAlpha\n"
			<<__FILE__
			<< "line \n"
			<< __LINE__; 

		return;
	}
}

void toprsImg::unloadTileToBil(void* dest,
	const toprsIRect& dest_rect,
	const toprsIRect& clip_rect) const
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		unloadTileToBilTemplate(toprs_uint8(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_SINT8:
		unloadTileToBilTemplate(toprs_sint8(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		unloadTileToBilTemplate(toprs_uint16(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_SINT16:
		unloadTileToBilTemplate(toprs_sint16(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_UINT32:
		unloadTileToBilTemplate(toprs_uint32(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_SINT32:
		unloadTileToBilTemplate(toprs_sint32(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		unloadTileToBilTemplate(toprs_float32(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		unloadTileToBilTemplate(toprs_float64(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"toprsImg::unloadTileToBil\n"
			<<__FILE__
			<< "line \n"
			<< __LINE__; 
		return;
	}
}

void toprsImg::unloadTileToBsq(void* dest,
	const toprsIRect& dest_rect,
	const toprsIRect& clip_rect) const
{
	switch (getDataType())
	{
	case TOPRS_UINT8:
		unloadTileToBsqTemplate(toprs_uint8(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_SINT8:
		unloadTileToBsqTemplate(toprs_sint8(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_UINT16:
	case TOPRS_USHORT11:
		unloadTileToBsqTemplate(toprs_uint16(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_SINT16:
		unloadTileToBsqTemplate(toprs_sint16(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_UINT32:
		unloadTileToBsqTemplate(toprs_uint32(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_SINT32:
		unloadTileToBsqTemplate(toprs_sint32(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_NORMALIZED_FLOAT:
	case TOPRS_FLOAT32:
		unloadTileToBsqTemplate(toprs_float32(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_NORMALIZED_DOUBLE:
	case TOPRS_FLOAT64:
		unloadTileToBsqTemplate(toprs_float64(0), dest, dest_rect, clip_rect);
		return;

	case TOPRS_DATA_UNKNOWN:
	default:
		std::cout << 
			"toprsImg::unloadTileToBsq\n"
			<<__FILE__
			<< "line \n"
			<< __LINE__; 
    
		return;
	}
}

template <class T> void
	toprsImg::unloadTileToBsqTemplate(T,  // dummy template arg...
	void* dest,
	const toprsIRect& dest_rect,
	const toprsIRect& clip_rect) const
{
	static const char  MODULE[] = "toprsImg::unloadTileToBsq";

	// Check the pointers.
	if (!dest)
	{
		std::cout
			<< MODULE << " ERROR:"
			<< "\nNULL pointer passed to method!  Returning..." << std::endl;
		return;
	}

	bool dataIsNull = false;
	if (getDataObjectStatus() == TOPRS_NULL)
	{
		dataIsNull = true;
	}

	toprsIRect img_rect = getImgRect();

	// Clip the clip_rect to the tile rect.
	toprsIRect output_clip_rect = clip_rect.clipToRect(img_rect);

	// Clip it again to the destination rect.
	output_clip_rect = dest_rect.clipToRect(output_clip_rect);

	// Check the output clip rect for intersection.
	if (output_clip_rect.hasNAN())
	{
		return;
	}
	if ( !(output_clip_rect.intersects(dest_rect)) )
	{
		return;
	}

	toprs_uint32 num_bands = getBands();
	int band      = 0;
	if(!dataIsNull)
	{
		toprs_uint32 d_width       = dest_rect.right_down().x - dest_rect.left_up().x + 1;
		toprs_uint32 d_band_offset = d_width * (dest_rect.right_down().y-dest_rect.left_up().y+1);
		toprs_uint32 s_width       = w();
		toprs_uint32 s_offset      = (output_clip_rect.left_up().y - img_rect.left_up().y) *
			s_width + (output_clip_rect.left_up().x - img_rect.left_up().x);

		T* d        = static_cast<T*>(dest);
		const T** s = new const T*[num_bands];

		// Grab a pointers to each one.
		for (band=0; band<num_bands; ++band)
		{
			s[band] = reinterpret_cast<const T*>(getBuf(band));

			// Move to first valid pixel.
			s[band] += s_offset;
		}

		// Move to first valid pixel.
		d += (output_clip_rect.left_up().y - dest_rect.left_up().y) * d_width +
			(output_clip_rect.left_up().x - dest_rect.left_up().x);

		for (band=0; band<num_bands; ++band)
		{
			toprs_uint32 d_buf_offset = 0;

			for (toprs_int32 line=output_clip_rect.left_up().y;
				line<=output_clip_rect.right_down().y; ++line)
			{
				toprs_int32 i=0;
				for (toprs_int32 samp=output_clip_rect.left_up().x;
					samp<=output_clip_rect.right_down().x; ++samp)
				{
					d[d_buf_offset+i] = s[band][i];
					++i;
				}

				d_buf_offset += d_width;
				s[band]      += s_width;
			}
			d += d_band_offset;
		}

		// Free up memory allocated for pointers.
		delete [] s;
	}
	else
	{
		toprs_uint32 d_width       = dest_rect.right_down().x - dest_rect.left_up().x + 1;
		toprs_uint32 d_band_offset = d_width * (dest_rect.right_down().y-dest_rect.left_up().y+1);

		toprs_uint8* d = static_cast<toprs_uint8*>(dest);

		// Move to first valid pixel.
		d += (output_clip_rect.left_up().y - dest_rect.left_up().y) * d_width +
			(output_clip_rect.left_up().x - dest_rect.left_up().x);

		for (band=0; band<num_bands; ++band)
		{
			toprs_uint8 np = static_cast<toprs_uint8>(theNullPixelV[band]);
			toprs_uint32 d_buf_offset = 0;

			for (toprs_int32 line=output_clip_rect.left_up().y;
				line<=output_clip_rect.right_down().y; ++line)
			{
				toprs_int32 i=0;
				for (toprs_int32 samp=output_clip_rect.left_up().x;
					samp<=output_clip_rect.right_down().x; ++samp)
				{
					d[d_buf_offset+i] = np;
					++i;
				}

				d_buf_offset += d_width;
			}
			d += d_band_offset;
		}
	}
}

toprs_uint8* toprsImg::getAlphaBuf()
{
	if (theAlphaChannelV.size() > 0)
	{
		return &theAlphaChannelV.front();
	}
	return 0;
}

const toprs_uint8* toprsImg::getAlphaBuf() const
{
	if (theAlphaChannelV.size() > 0)
	{
		return &theAlphaChannelV.front();
	}
	return 0;
}

bool toprsImg::isValid() const
{
    return theNumberOfBands > 0 && theDataType != TOPRS_DATA_UNKNOWN;;
}
