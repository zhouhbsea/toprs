#include <iostream>
#include "imagesource\toprsImageSource.h"
#include "connection\toprsConnection.h"

bool toprsImageSource::initialize()
{
	return true;
}

toprsInterfaceImageSource::PImg toprsImageSource::getTile(const toprsIRect& rect,  int resLevel)
{
    if(!theConnection)
	 initialize();

    toprsInterfaceImageSource* inter = theConnection->getInput(0);
    if(inter)
    {
	    return inter->getTile(rect, resLevel);
    }
    else
    {
	    std::cout << "Can not find the Input object!" <<std::endl;
        return NULL;
    }
}

toprsInterfaceImageSource::PImg toprsImageSource::getTile( const toprsIpt& origin, int resLevel /*= 0*/ )
{
	toprsIRect tileRect(origin.x,
		origin.y,
		origin.x + getTileWidth()  - 1,
		origin.y + getTileHeight() - 1);

	return getTile(tileRect, resLevel);
}

bool toprsImageSource::getTile( toprsImg* result, int resLevel /*= 0*/ )
{
	bool status = true;

	if (result)
	{
		toprsIRect tileRect = result->getImgRect();

		PImg id = getTile(tileRect, resLevel);
		if (id)
		{
			*result = *id;// changed to move 
		}
		else
		{
			status = false;
		}
	}
	return status;
}



toprsImageSource::toprsImageSource(const toprsImageSource& rhs)
{
}

const toprsImageSource& toprsImageSource::operator=(const toprsImageSource&)
{
    return *this;
}

bool toprsImageSource::canConnectInputTo( toprsInterfaceImageSource* input )
{
	return true;
}

void toprsImageSource::connectInputTo( toprsInterfaceImageSource* input )
{
	if(theConnection == nullptr)
		initialize();

	theConnection->connectMyInputTo(std::shared_ptr<toprsInterfaceImageSource>(input),false,false);

}

void toprsImageSource::connectInputEvent( toprsEvent& event )
{

}

toprsDataType toprsImageSource::getOutputScalarType() const
{
	toprsInterfaceImageSource* inter = theConnection->getInput(0);
	if(inter)
	{
		return inter->getOutputScalarType();
	}
	return TOPRS_DATA_UNKNOWN;
}

int toprsImageSource::getNumberOfOutputBands() const
{
	return getNumberOfInputBands();
}

void toprsImageSource::getOutputBandList( std::vector<int>& bandList ) const
{
	bandList.clear();
	const int BANDS = getNumberOfOutputBands();
	for (int band = 0; band < BANDS; ++band)
	{
		bandList.push_back(band);
	}
}

int toprsImageSource::getTileWidth() const
{
	toprsImageSource* inter = static_cast<toprsImageSource*>(theConnection->getInput(0));
	if(inter)
	{
		return inter->getTileWidth();
	}
    toprsIpt tileSize;
	tileSize.x = TOPRS_DEFAULT_TILE_WIDTH;
	tileSize.y = TOPRS_DEFAULT_TILE_HEIGHT;
	//toprs::defaultTileSize(tileSize);

	return tileSize.x;
}

int toprsImageSource::getTileHeight() const
{
	toprsImageSource* inter = static_cast<toprsImageSource*>(theConnection->getInput(0));

	if(inter)
	{
		return inter->getTileHeight();
	}
	toprsIpt tileSize;
	tileSize.x = TOPRS_DEFAULT_TILE_WIDTH;
	tileSize.y = TOPRS_DEFAULT_TILE_HEIGHT;
	//toprs::defaultTileSize(tileSize);

	return tileSize.y;
}

double toprsImageSource::getNullPixelValue( int band/*=0*/ ) const
{
	toprsImageSource* inter = static_cast<toprsImageSource*>(theConnection->getInput(0));

	if(inter)
	{
		return inter->getNullPixelValue();
	}
	return toprs::defaultNull(getOutputScalarType());
}

double toprsImageSource::getMinPixelValue( int band/*=0*/ ) const
{
	toprsImageSource* inter = static_cast<toprsImageSource*>(theConnection->getInput(0));

	if(inter)
	{
		return inter->getMinPixelValue();
	}
	return toprs::defaultMin(getOutputScalarType());
}

double toprsImageSource::getMaxPixelValue( int band/*=0*/ ) const
{
	toprsImageSource* inter = static_cast<toprsImageSource*>(theConnection->getInput(0));

	if(inter)
	{
		return inter->getMaxPixelValue();
	}

	return toprs::defaultMax(getOutputScalarType());
}

toprsIRect toprsImageSource::getBoundingRect( int resLevel/*=0*/ ) const
{
	toprsImageSource* inter = static_cast<toprsImageSource*>(theConnection->getInput(0));

	if(inter)
	{
		return inter->getBoundingRect();
	}
	toprsIRect rect;
	rect.makeNAN();
	return rect;

}

void toprsImageSource::getBoundingRect( toprsIRect& rect,int resLevel/*=0*/ ) const
{
	rect = getBoundingRect(resLevel);
}

bool toprsImageSource::isIndexedData() const
{
	toprsImageSource* inter = static_cast<toprsImageSource*>(theConnection->getInput(0));

	if(inter)
	{
		return inter->isIndexedData();
	}
	return false;
}

int toprsImageSource::getNumberOfDecimationLevels() const 
{
	toprsImageSource* inter = static_cast<toprsImageSource*>(theConnection->getInput(0));

	if(inter)
	{
		return inter->getNumberOfDecimationLevels();
	}
	return 0;
}

void toprsImageSource::getDecimationFactors( int resLevel, toprsDpt& result ) const
{
	toprsImageSource* inter = static_cast<toprsImageSource*>(theConnection->getInput(0));

	if(inter)
	{
		return inter->getDecimationFactors(resLevel, result);
	}
}

void toprsImageSource::getDecimationFactors( std::vector<toprsDpt>& decimations ) const
{
	toprsImageSource* inter = static_cast<toprsImageSource*>(theConnection->getInput(0));

	if(inter)
	{
		return inter->getDecimationFactors(decimations);
	}
}


//toprsInterfaceImageSource& operator >>(toprsInterfaceImageSource& left, toprsInterfaceImageSource& right)
//{
//
//}



