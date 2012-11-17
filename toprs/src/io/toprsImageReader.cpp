#include "io/toprsImageReader.h"
#include "base/toprsNBandLutDataObject.h"



toprsIRect toprsImageReader::getImageRectangle( int resLevel /*= 0*/ ) const
{
	toprsIRect result;

	if( isOpen() /*&& /*isValidRLevel(resLevel)*/ )
	{
		int lines   = getNumberOfLines(resLevel);
		int samples = getNumberOfSamples(resLevel);
		if( !lines || !samples )
		{
			result.makeNAN();
		}
		else
		{
			result = toprsIRect(0, 0, samples-1, lines-1);
		}
	}
	else
	{
		result.makeNAN();
	}

	return result;
}

toprsIRect toprsImageReader::getBoundingRect( int resLevel /*= 0*/ ) const
{
	return getImageRectangle(resLevel);
}

void toprsImageReader::getDecimationFactor( int resLevel, toprsDpt& result ) const
{
	if (resLevel < theDecimationFactors.size())
		result = theDecimationFactors[resLevel];
	else
		result.makeNAN();
}

void toprsImageReader::getDecimationFactors(std::vector<toprsDpt>& decimations ) const
{
	decimations = theDecimationFactors;
}

void toprsImageReader::setFilename( const std::string filename )
{
	theImageFile = filename;
}

const std::string& toprsImageReader::getFilename() const
{
	return theImageFile;
}

bool toprsImageReader::canConnectMyInputTo( int inputIndex,const toprsInterfaceImageSource* object ) const
{
	return false;
}



double toprsImageReader::getMinPixelValue( int band/*=0*/ ) const
{
	if(theData.getBands())
	{
		return theData.getMinPix(band);
	}
	return toprs::defaultMin(getOutputScalarType());
}

double toprsImageReader::getMaxPixelValue( int band/*=0*/ ) const
{
	if(theData.getBands())
	{
		return theData.getMaxPix(band);
	}
	return toprs::defaultMax(getOutputScalarType());
}

double toprsImageReader::getNullPixelValue( int band/*=0*/ ) const
{
	if(theData.getBands())
	{
		return theData.getNullPix(band);
	}
	return toprs::defaultNull(getOutputScalarType());
}

void toprsImageReader::setMinPixelValue( int band, const toprs_float64& pix )
{
	 theData.setMinPix(pix, band);
}

void toprsImageReader::setMaxPixelValue( int band, const toprs_float64& pix )
{
	theData.setMaxPix(pix, band);
}

void toprsImageReader::setNullPixelValue( int band, const toprs_float64& pix )
{
	theData.setNullPix(pix, band);
}

bool toprsImageReader::open( const std::string& imageFile )
{
	if(isOpen())
	{
		close();
	}

	setFilename(imageFile);

	return open();
}

void toprsImageReader::close()
{
	theDecimationFactors.clear();
}

std::shared_ptr<toprsNBandLutDataObject> toprsImageReader::getLut() const
{
	return theLut;
}

bool toprsImageReader::hasLut() const
{
	return theLut.use_count() != 0;
}

bool toprsImageReader::hasData() const
{
	return theData.isValid();
}

void toprsImageReader::completeOpen()
{
	theData.setDataType(getOutputScalarType());
}






