#include "io/toprsImageReader.h"
#include "base/toprsNBandLutDataObject.h"



toprsIRect toprsImageReader::getImageRectangle( int resLevel /*= 0*/ ) const
{
	toprsIRect result;

	if( isOpen() && isValidRLevel(resLevel) )
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
	//theOverview = 0;//xizhi
	//theGeometry = 0;
	theValidImageVertices.clear();
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

bool toprsImageReader::isBandSelector() const
{
	return true;
}

bool toprsImageReader::isValidRLevel( int resLevel ) const
{
	bool result = false;

	const int LEVELS = getNumberOfDecimationLevels();

	if ( !theStartingResLevel) // Not an overview.
	{
		result = (resLevel < LEVELS);
	}
	else  // Used as overview.
	{
		if (resLevel >= theStartingResLevel)
		{
			//---
			// Adjust the res level to be zero based for this overview before
			// checking.
			//---
			result = ( (resLevel - theStartingResLevel) < LEVELS);
		}
	}

	return result;
}

int toprsImageReader::getNumberOfDecimationLevels() const
{
	int result = 1; // Add r0
	//if (theOverview.valid())
	//{
	//	result += theOverview->getNumberOfDecimationLevels();//xizhi
	//}
	return result;
}

int toprsImageReader::getNumberOfReducedResSets() const
{
	 return getNumberOfDecimationLevels();
}

bool toprsImageReader::setOutputToInputBandList()
{
	return false;
}

bool toprsImageReader::setOutputBandList( const std::vector<int>& band_list )
{
	bool result = false;
	if ( isBandSelector() )
	{
		std::vector<int> bandList;
		toprsImageSource::getOutputBandList( bandList );
		result = setOutputBandList( bandList );
	}
	return result;
}

bool toprsImageReader::setOutputBandList( const std::vector<int>& inBandList, std::vector<int>& outBandList )
{
	bool result = false;

	const int INPUT_BANDS  = getNumberOfInputBands();
	const int OUTPUT_BANDS = inBandList.size();

	if ( INPUT_BANDS && OUTPUT_BANDS )
	{
		result = true;
		outBandList.resize( OUTPUT_BANDS );
		for ( int band = 0; band < OUTPUT_BANDS; ++band )
		{
			if ( inBandList[band] < INPUT_BANDS )
			{
				outBandList[band] = inBandList[band];
			}
			else // Out of range...
			{
				result = false;
				break;
			}
		}
		//xizhi
		//if ( result && theOverview.valid() )
		//{
		//result = theOverview->setOutputBandList( inBandList );
		//}
	}

	if ( result == false )
	{
		toprsImageSource::getOutputBandList( outBandList ); // Set to identity.
	}

	return result;
}






