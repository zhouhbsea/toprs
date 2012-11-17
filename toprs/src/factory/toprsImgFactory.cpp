#include <iostream>
#include "factory/toprsImgFactory.h"


toprsImgFactory* toprsImgFactory::theInstance = nullptr;
//std::mutex toprsImgFactory::theInstanceMutex;
toprsImgFactory* toprsImgFactory::instance()
{
	//theInstanceMutex.lock();
	if(!theInstance)
	{
		theInstance = new toprsImgFactory;
	}
	//theInstanceMutex.unlock();
	return theInstance;
}

toprsImgFactory::toprsImgFactory()
{
	theInstance = nullptr;
}

toprsImgFactory::~toprsImgFactory()
{
	if (theInstance)
	{
		theInstance = nullptr;
	}
}

std::shared_ptr<toprsImg> toprsImgFactory::create( toprsDataType scalar,int bands /*= 1*/ ) const
{
	toprsIpt tileSize;
	tileSize.x = TOPRS_DEFAULT_TILE_WIDTH;
	tileSize.y = TOPRS_DEFAULT_TILE_HEIGHT;
	//toprs::defaultTileSize(tileSize);
	int width = tileSize.x;
	int height = tileSize.y;
	bands  = (bands>0)?bands:1;
	scalar = scalar != TOPRS_DATA_UNKNOWN?scalar:TOPRS_UINT8;
	std::shared_ptr<toprsImg> result;
	result.reset(new toprsImg(scalar, bands, width, height));
	return result;
}

std::shared_ptr<toprsImg> toprsImgFactory::create( toprsDataType scalar,int bands,int width,int height ) const
{
	toprsIpt tileSize;
	tileSize.x = TOPRS_DEFAULT_TILE_WIDTH;
	tileSize.y = TOPRS_DEFAULT_TILE_HEIGHT;
	//toprs::defaultTileSize(tileSize);
	width = width > 0 ? width: tileSize.x;
	height = height > 0 ? height : tileSize.y;
	bands  = (bands>0)?bands:1;
	scalar = scalar != TOPRS_DATA_UNKNOWN?scalar:TOPRS_UINT8;
	std::shared_ptr<toprsImg> result;
	result.reset(new toprsImg(scalar, bands, width, height));
	return result;
}

std::shared_ptr<toprsImg> toprsImgFactory::create( int bands, toprsImageSource* inputSource ) const
{
	std::shared_ptr<toprsImg> result;
	if (inputSource)
	{
		toprsDataType scalar = inputSource->getOutputScalarType();
		int width  = inputSource->getTileWidth();
		int height = inputSource->getTileHeight();

		result = create(scalar, bands, width, height);
		if ( result)
		{
			for(int band = 0; band < bands; ++band)
			{
				result->setMinPix(inputSource->getMinPixelValue(band),   band);
				result->setMaxPix(inputSource->getMaxPixelValue(band),   band);
				result->setNullPix(inputSource->getNullPixelValue(band), band);
			}
		}
	}
	else
	{
		std::cout
			<< "create::create ERROR:"
			<< "\nNULL input source!" << std::endl;
	}

	return result;
}

std::shared_ptr<toprsImg> toprsImgFactory::create( toprsImageSource* inputSource ) const
{
	std::shared_ptr<toprsImg> result;
	if (inputSource)
	{
		toprsDataType scalar = inputSource->getOutputScalarType();

		int bands = inputSource->getNumberOfOutputBands();
		int width  = inputSource->getTileWidth();
		int height = inputSource->getTileHeight();

		result = create(scalar, bands, width, height);
		if ( result)
		{
			for(int band = 0; band < bands; ++band)
			{
				result->setMinPix(inputSource->getMinPixelValue(band),   band);
				result->setMaxPix(inputSource->getMaxPixelValue(band),   band);
				result->setNullPix(inputSource->getNullPixelValue(band), band);
			}
		}
	}
	else
	{
		std::cout
			<< "create::create ERROR:"
			<< "\nNULL input source!" << std::endl;
	}

	return result;
}

