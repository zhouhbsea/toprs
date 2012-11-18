#include <algorithm>
#include "toprsGdalReader.h"
#include "base/toprsNBandLutDataObject.h"
#include "factory/toprsImgFactory.h"
#include "gdal/gdal_priv.h"
#include "gdal/cpl_string.h"

#pragma comment (lib, "gdal_i.lib")

toprsGadlReader::toprsGadlReader():
                toprsImageReader()
{

};

toprsGadlReader::~toprsGadlReader()
{
     close();
}

std::string toprsGadlReader::getName() const
{
	return std::string("gdal reader");
}

std::string toprsGadlReader::className() const
{
	return std::string("toprsGadlReader");
}

void toprsGadlReader::close()
{
	if(theDataset)
	{
		GDALClose(theDataset);
		theDataset = 0;
	}

	theTile = 0;
	theSingleBandTile = 0;

	if(theMinPixValues)
	{
		delete [] theMinPixValues;
		theMinPixValues = 0;
	}
	if(theMaxPixValues)
	{
		delete [] theMaxPixValues;
		theMaxPixValues = 0;
	}
	if(theNullPixValues)
	{
		delete [] theNullPixValues;
		theNullPixValues = 0;
	}
	//deleteRlevelCache();

	m_preservePaletteIndexesFlag = false;
	theAlphaChannelFlag = false;
	theIsComplexFlag = false;
}

bool toprsGadlReader::open()
{
	if(isOpen())
	{
		close();
	}
	std::string driverNameTmp;

	if (theSubDatasets.size() == 0)
	{
		// Note:  Cannot feed GDALOpen a NULL string!
		if (theImageFile.size())
		{
			theDataset = GDALOpen(theImageFile.c_str(), GA_ReadOnly); 
			if( theDataset == 0 )
			{
				return false;
			}
		}
		else
		{
			return false;
		}


		// Check if it is nitf data for deciding whether or not open each sub-dataset
		//This will be removed eventually when toprs can handle 2GB nitf file.
		GDALDriverH driverTmp = GDALGetDatasetDriver(theDataset);
		bool isNtif = false;
		if (driverTmp != 0)
		{
			driverNameTmp = std::string(GDALGetDriverShortName(driverTmp));
			std::transform(driverNameTmp.begin(), driverNameTmp.end(), driverNameTmp.begin(), [&](char ch){return toupper(ch);});
			if (driverNameTmp == "NITF")
			{
				isNtif = true;
			}
		}

		// Check for sub data sets...
		char** papszMetadata = GDALGetMetadata( theDataset, "SUBDATASETS" );
		if( CSLCount(papszMetadata) > 0 )
		{
			theSubDatasets.clear();

			for( int i = 0; papszMetadata[i] != 0; ++i )
			{
				std::string os = papszMetadata[i];
				if (os.find("_NAME=") != std::string::npos)
				{
					//Sub sets have already been set. Open each sub-dataset really slow down the process
					//specially for hdf data which sometimes has over 100 sub-datasets. Since following code
					//only for ntif cloud checking, so only open each sub-dataset here if the dataset is
					//nitf. This will be removed eventually when toprs can handle 2GB nitf file. 
					//Otherwise open a sub-dataset when setCurrentEntry() gets called.
					if (isNtif)
					{
						GDALDatasetH subDataset = GDALOpen(filterSubDatasetsString(os).c_str(),
							GA_ReadOnly);
						if ( subDataset != 0 )
						{
							// "Worldview ingest should ignore subimages when NITF_ICAT=CLOUD"
							// Hack: Ignore NITF subimages marked as cloud layers.
							std::string nitfIcatTag( GDALGetMetadataItem( subDataset, "NITF_ICAT", "" ) );
							if ( nitfIcatTag.find("CLOUD") == std::string::npos )
							{
								theSubDatasets.push_back(filterSubDatasetsString(os));
							}
						}
						GDALClose(subDataset);
					}
					else
					{
						theSubDatasets.push_back(filterSubDatasetsString(os));
					}
				}
			}
			//---
			// Have multiple entries.  We're going to default to open the first
			// entry like cidcadrg.
			//---
			close();

			theDataset = GDALOpen(theSubDatasets[theEntryNumberToRender].c_str(),
				GA_ReadOnly);
			if (theDataset == 0)
			{

				return false;
			}
		}  // End of has subsets block.

	}  // End of "if (theSubdatasets.size() == 0)"
	else
	{
		// Sub sets have already been set.
		theDataset = GDALOpen(theSubDatasets[theEntryNumberToRender].c_str(),
			GA_ReadOnly);
		if (theDataset == 0)
		{
			return false;
		}
	}

	// Set the driver.
	theDriver = GDALGetDatasetDriver( theDataset );

	if(!theDriver) return false;


	theGdtType = GDT_Byte;
	theOutputGdtType = GDT_Byte;

	if(getNumberOfInputBands() < 1 )
	{
		if(CSLCount(GDALGetMetadata(theDataset, "SUBDATASETS")))
		{
			std::cout
				<< "torsGdalReader::open WARNING:"
				<< "\nHas multiple sub datasets and need to set the data before"
				<< " we can get to the bands" << std::endl;
		}

		close();
		std::cout
			<< "torsGdalReader::open WARNING:"
			<< "\nNo band data present in torsGdalReader::open" << std::endl;
		return false;
	}

	toprs_int32 i = 0;
	GDALRasterBandH bBand = GDALGetRasterBand( theDataset, 1 );
	theGdtType  = GDALGetRasterDataType(bBand);


	char** papszMetadata = GDALGetMetadata( bBand, NULL );
	if (CSLCount(papszMetadata) > 0)
	{
		for(int i = 0; papszMetadata[i] != NULL; i++ )
		{
			std::string metaStr = papszMetadata[i];

			if (metaStr.find("AREA_OR_POINT") != std::string::npos)
			{
				//std::string pixel_is_point_or_area = metaStr.split("=")[1];
				//pixel_is_point_or_area.downcase();
				//if (pixel_is_point_or_area.contains("area"))
				//	thePixelType = TOPRS_PIXEL_IS_AREA;
				break;
			}
		}
	}

	if(!isIndexed(1))
	{
		for(i = 0; i  < GDALGetRasterCount(theDataset); ++i)
		{
			if(theGdtType != GDALGetRasterDataType(GDALGetRasterBand( theDataset,i+1 )))
			{
				std::cout
					<< "torsGdalReader::open WARNING"
					<< "\nWe currently do not support different scalar type bands."
					<< std::endl;
				close();
				return false;
			}

		}
	}
	theOutputGdtType = theGdtType;
	switch(theGdtType)
	{
	case GDT_CInt16:
		{
			//          theOutputGdtType = GDT_Int16;
			theIsComplexFlag = true;
			break;
		}
	case GDT_CInt32:
		{
			//          theOutputGdtType = GDT_Int32;
			theIsComplexFlag = true;
			break;
		}
	case GDT_CFloat32:
		{
			//          theOutputGdtType = GDT_Float32;
			theIsComplexFlag = true;
			break;
		}
	case GDT_CFloat64:
		{
			//          theOutputGdtType = GDT_Float64;
			theIsComplexFlag = true;
			break;
		}
	default:
		{
			theIsComplexFlag = false;
			break;
		}
	}

	if((std::string(GDALGetDriverShortName( theDriver )) == "PNG")&&
		(getNumberOfInputBands() == 4))
	{
		theAlphaChannelFlag = true;
	}
	populateLut();
	computeMinMax();
	completeOpen();

	theTile = toprsImgFactory::instance()->create(this);
	theSingleBandTile = toprsImgFactory::instance()->create(getInputScalarType(), 1);

	if ( m_preservePaletteIndexesFlag )
	{
		theTile->setIndexedFlag(true);
		theSingleBandTile->setIndexedFlag(true);
	}

	theTile->initialize();
	theSingleBandTile->initialize();
	theGdalBuffer.resize(0);
	if(theIsComplexFlag)
	{
		theGdalBuffer.resize(theSingleBandTile->getSizePerBandInBytes()*2);
	}

	theImageBound = toprsIRect(0
		,0
		,GDALGetRasterXSize(theDataset)-1
		,GDALGetRasterYSize(theDataset)-1);
	int xSize=0, ySize=0;
	GDALGetBlockSize(GDALGetRasterBand( theDataset, 1 ),
		&xSize,
		&ySize);
 
	if (driverNameTmp.find("JPIP")!= std::string::npos || driverNameTmp.find("JP2")!= std::string::npos)
	{
		m_isBlocked = ((xSize > 1)&&(ySize > 1));
	}
	else
	{
		m_isBlocked = false;
	}
	//if(m_isBlocked)
	//{
	//	setRlevelCache();
	//}
	return true;
}

std::string toprsGadlReader::filterSubDatasetsString( const std::string& subString )
{
	//---
	// Skip up to and including '=' and filter out the quotes '"'.
	//---
	std::string s;
	bool atStart = false;
	for (int pos = 0; pos < subString.size(); ++pos)
	{
		if ( *(subString.begin()+pos) == '=')
		{
			atStart = true; // Start recording after this.
			continue;       // Skip the '='.
		}
		if (atStart)
		{
			s.push_back(*(subString.begin()+pos)); // Record the character.
		}
	}
	return s;
}

bool toprsGadlReader::isIndexed( int aGdalBandNumber /*= 1*/ ) const
{
	if(aGdalBandNumber <= GDALGetRasterCount(theDataset))
	{
		GDALRasterBandH band = GDALGetRasterBand(theDataset, aGdalBandNumber);
		if(!band) return false;
		if(GDALGetRasterColorInterpretation(band)==GCI_PaletteIndex)
		{
			return true;
		}
	}

	return false;
}

int toprsGadlReader::getIndexBandOutputNumber( int bandNumber ) const
{
	if(isIndexed(bandNumber))
	{
		GDALRasterBandH band = GDALGetRasterBand(theDataset, bandNumber);
		if(GDALGetRasterColorInterpretation(band)==GCI_PaletteIndex)
		{
			GDALColorTableH table = GDALGetRasterColorTable(band);
			GDALPaletteInterp interp = GDALGetPaletteInterpretation(table);
			switch(interp)
			{
			case GPI_CMYK:
			case GPI_RGB:
			case GPI_HLS:
				{
					return 3;
				}
			case GPI_Gray:
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

bool toprsGadlReader::isIndexTo1Band( int bandNumber /*= 1*/ ) const
{
	GDALRasterBandH band = GDALGetRasterBand(theDataset, bandNumber);
	if(GDALGetRasterColorInterpretation(band)==GCI_PaletteIndex)
	{
		GDALColorTableH table = GDALGetRasterColorTable(band);
		GDALPaletteInterp interp = GDALGetPaletteInterpretation(table);
		if(interp == GPI_Gray)
		{
			return true;
		}
	}
	return false;
}

bool toprsGadlReader::isIndexTo3Band( int bandNumber /*= 1*/ ) const
{
	GDALRasterBandH band = GDALGetRasterBand(theDataset, bandNumber);
	if(GDALGetRasterColorInterpretation(band)==GCI_PaletteIndex)
	{
		GDALColorTableH table = GDALGetRasterColorTable(band);
		GDALPaletteInterp interp = GDALGetPaletteInterpretation(table);
		if( (interp == GPI_RGB) ||
			(interp == GPI_HLS)||
			(interp == GPI_CMYK))
		{
			return true;
		}
	}

	return false;
}

void toprsGadlReader::computeMinMax()
{
	toprs_uint32 bands = GDALGetRasterCount(theDataset);

	if(theMinPixValues)
	{
		delete [] theMinPixValues;
		theMinPixValues = 0;
	}
	if(theMaxPixValues)
	{
		delete [] theMaxPixValues;
		theMaxPixValues = 0;
	}
	if(theNullPixValues)
	{
		delete [] theNullPixValues;
		theNullPixValues = 0;
	}
	if(isIndexTo3Band())
	{
		int i = 0;
		theMinPixValues  = new double[3];
		theMaxPixValues  = new double[3];
		theNullPixValues = new double[3];

		for(i = 0; i < 3; ++i)
		{
			theMinPixValues[i] = 1;
			theMaxPixValues[i] = 255;
			theNullPixValues[i] = 0;
		}
	}
	else if(isIndexTo1Band())
	{
		theMinPixValues  = new double[1];
		theMaxPixValues  = new double[1];
		theNullPixValues = new double[1];

		*theNullPixValues = 0;
		*theMaxPixValues = 255;
		*theMinPixValues = 1;
	}
	else
	{
		if(!theMinPixValues && !theMaxPixValues&&bands)
		{
			theMinPixValues = new double[bands];
			theMaxPixValues = new double[bands];
			theNullPixValues = new double[bands];
		}
		for(toprs_int32 band = 0; band < (toprs_int32)bands; ++band)
		{
			GDALRasterBandH aBand=0;

			aBand = GDALGetRasterBand(theDataset, band+1);

			int minOk=1;
			int maxOk=1;
			int nullOk=1;

			if(aBand)
			{
				if(hasData())
				{
					theMinPixValues[band] = theData.getMinPix(band);
					theMaxPixValues[band] = theData.getMaxPix(band);
					theNullPixValues[band] = theData.getNullPix(band);
				}
				else 
				{
					std::string driverName = theDriver ? GDALGetDriverShortName( theDriver ) : "";

					// Allow to rescale the image data 
					// to the min/max values found in the raster data only 
					// if it was not acquired with the following drivers:
					if ( driverName.find("JP2KAK") != std::string::npos  ||
						driverName.find("JPEG2000") != std::string::npos||
						driverName.find("NITF") != std::string::npos)
					{
						theMinPixValues[band] = toprs::defaultMin(getOutputScalarType());
						theMaxPixValues[band] = toprs::defaultMax(getOutputScalarType());
						theNullPixValues[band] = toprs::defaultNull(getOutputScalarType());
					}
					else
					{
						theMinPixValues[band]  = GDALGetRasterMinimum(aBand, &minOk);
						theMaxPixValues[band]  = GDALGetRasterMaximum(aBand, &maxOk);
						theNullPixValues[band] = GDALGetRasterNoDataValue(aBand, &nullOk);
					}
					if((!nullOk)||(theNullPixValues[band] < toprs::defaultNull(getOutputScalarType())))
					{
						theNullPixValues[band] = toprs::defaultNull(getOutputScalarType());
					}
				}
				if(!minOk||!maxOk)
				{
					theMinPixValues[band] = toprs::defaultMin(getOutputScalarType());
					theMaxPixValues[band] = toprs::defaultMax(getOutputScalarType());
				}
			}
			else
			{
				theMinPixValues[band] = toprs::defaultMin(getOutputScalarType());
				theMaxPixValues[band] = toprs::defaultMax(getOutputScalarType());
			}
		}
	}
}

void toprsGadlReader::populateLut()
{
	theLut.reset(); // toprsRefPtr not a leak.
	if(isIndexed(1)&&theDataset)
	{
		GDALColorTableH aTable = GDALGetRasterColorTable(GDALGetRasterBand( theDataset, 1 ));
		GDALPaletteInterp interp = GDALGetPaletteInterpretation(aTable);
		if(aTable && ( (interp == GPI_Gray) || (interp == GPI_RGB)))
		{
			GDALColorEntry colorEntry;
			int numberOfElements = GDALGetColorEntryCount(aTable);
			int idx = 0;
			if(numberOfElements)
			{
				// GPI_Gray Grayscale (in GDALColorEntry.c1)
				// GPI_RGB Red, Green, Blue and Alpha in (in c1, c2, c3 and c4)
				theLut.reset(new toprsNBandLutDataObject(numberOfElements,4,TOPRS_UINT8,-1));
				bool nullSet = false;
				for(idx = 0; idx < numberOfElements; ++idx)
				{
					switch(interp)
					{
					case GPI_RGB:
						{
							if(GDALGetColorEntryAsRGB(aTable, idx, &colorEntry))
							{
								(*theLut)[idx][0] = colorEntry.c1;
								(*theLut)[idx][1] = colorEntry.c2;
								(*theLut)[idx][2] = colorEntry.c3;
								(*theLut)[idx][3] = colorEntry.c4;

								if ( !nullSet )
								{
									if ( m_preservePaletteIndexesFlag )
									{
										// If preserving palette set the null to the fix alpha of 0.
										if ( (*theLut)[idx][3] == 0 )
										{
											theLut->setNullPixelIndex(idx);
											nullSet = true;
										}
									}
									else
									{
										//---
										// Not using alpha.
										// Since the alpha is currently not used, look for the null
										// pixel index and set if we find. If at some point the alpha
										// is taken out this can be removed.
										//---
										if ( ( (*theLut)[idx][0] == 0 ) &&
											( (*theLut)[idx][1] == 0 ) &&
											( (*theLut)[idx][2] == 0 ) )
										{
											theLut->setNullPixelIndex(idx);
											nullSet = true;
										}
									}
								}
							}
							else
							{
								(*theLut)[idx][0] = 0;
								(*theLut)[idx][1] = 0;
								(*theLut)[idx][2] = 0;
								(*theLut)[idx][3] = 0;

								// Look for the null pixel index and set if we find.
								if ( !nullSet )
								{
									if ( (*theLut)[idx][0] == 0 )
									{
										theLut->setNullPixelIndex(idx);
									}
								}
							}
							break;
						}
					case GPI_Gray:
						{
							const GDALColorEntry* constEntry =  GDALGetColorEntry(aTable, idx);
							if(constEntry)
							{
								(*theLut)[idx][0] = constEntry->c1;
							}
							else
							{
								(*theLut)[idx][0] = 0;
							}
							break;
						}
					default:
						{
							break;
						}
					}
				}
			}
		}

		toprs_uint32 rasterCount = GDALGetRasterCount(theDataset);
		for(toprs_uint32 aGdalBandIndex=1; aGdalBandIndex <= rasterCount; ++aGdalBandIndex)
		{
			GDALRasterBandH aBand = GDALGetRasterBand( theDataset, aGdalBandIndex );
			if (aBand)
			{
				GDALRasterAttributeTableH hRAT = GDALGetDefaultRAT(aBand);
				int colCount = GDALRATGetColumnCount(hRAT);
				for (toprs_int32 col = 0; col < colCount; col++)
				{
					const char* colName = GDALRATGetNameOfCol(hRAT, col);
					if (colName)
					{
						if (strcmp(colName, "Class_Names") == 0)
						{
							std::vector<std::string> entryLabels;
							toprs_int32 rowCount = GDALRATGetRowCount(hRAT);
							for (toprs_int32 row = 0; row < rowCount; row++)
							{
								const char* className = GDALRATGetValueAsString(hRAT, row, col);
								std::string entryLabel(className);
								entryLabels.push_back(entryLabel);
							}
							theLut->setEntryLables(aGdalBandIndex-1, entryLabels);
						}
					}
				}
			}
		}
	}
}

toprsDataType toprsGadlReader::getInputScalarType() const
{
	toprsDataType result = TOPRS_DATA_UNKNOWN;

	switch(theGdtType)
	{
	case GDT_Byte:
		{
			result = TOPRS_UINT8;
			break;
		}
	case GDT_UInt16:
		{
			result = TOPRS_USHORT16;
			break;
		}
	case GDT_Int16:
		{
			result = TOPRS_SSHORT16;
			break;
		}
	case GDT_UInt32:
		{
			result = TOPRS_UINT32;
			break;

		}
	case GDT_Int32:
		{
			int sizeType = GDALGetDataTypeSize(theGdtType)/8;
			if(sizeType == 2)
			{
				result = TOPRS_SSHORT16;
				theGdtType = GDT_Int16;
			}
			else
			{
				result = TOPRS_SINT32;
				theGdtType = GDT_Int32;
			}
			break;
		}
	case GDT_Float32:
		{
			result = TOPRS_FLOAT;
			break;
		}
	case GDT_Float64:
		{
			result = TOPRS_DOUBLE;
			break;
		}
	case GDT_CInt16:
		{
			result = TOPRS_SINT16;
			break;
		}
	case  GDT_CInt32:
		{
			result = TOPRS_SINT32;
			break;
		}
	case GDT_CFloat32:
		{
			result = TOPRS_FLOAT32;
			break;
		}
	case GDT_CFloat64:
		{
			result = TOPRS_FLOAT64;
			break;
		}
	default:
		break;
	}
	return result;
}

toprsDataType toprsGadlReader::getOutputScalarType() const
{
	toprsDataType result =  getInputScalarType();
	if (theLut.use_count() != 0)
	{
		result = TOPRS_UINT8;
	}
	return result;
}

int toprsGadlReader::getNumberOfInputBands() const
{
	if(isOpen())
	{
		if (m_outputBandList.size() > 0)
		{
			return (int) m_outputBandList.size();
		}

		if(isIndexed(1))
		{
			return getIndexBandOutputNumber(1);
		}

		if(!theIsComplexFlag)
		{
			return GDALGetRasterCount(theDataset);
		}
		return GDALGetRasterCount(theDataset)*2;
	}
	return 0;
}

int toprsGadlReader::getNumberOfOutputBands() const
{
	int result = 0;
	if( isIndexTo3Band() )
	{
		if ( m_preservePaletteIndexesFlag )
		{
			result = 1; // Passing palette indexes not expanded rgb values.
		}
		else
		{
			result = 3;
		}
	}
	else if (theAlphaChannelFlag)
	{
		result = 3;
	}
	else
	{
		// the number of outputs will be the same as the number of inputs
		result = getNumberOfInputBands();
	}
	return result;
}

int toprsGadlReader::getNumberOfLines( int reduced_res_level /*= 0*/ ) const
{
	int result = 0;
	if ( isOpen() && isValidRLevel(reduced_res_level) )
	{//xizhi
		//if(theOverview.valid() && theOverview->isValidRLevel(reduced_res_level))
		//{
		//	result = theOverview->getNumberOfLines(reduced_res_level);
		//}
		//else
		{
			int x, y;
			getMaxSize(reduced_res_level, x, y);
			result = y;
		}
	}

	return result;
}

int toprsGadlReader::getNumberOfSamples( int reduced_res_level /*= 0*/ ) const
{
	int result = 0;
	if ( isOpen() && isValidRLevel(reduced_res_level) )
	{//xizhi
		//if(theOverview.valid() && theOverview->isValidRLevel(reduced_res_level))
		//{
		//	result = theOverview->getNumberOfLines(reduced_res_level);
		//}
		//else
		{
			int x, y;
			getMaxSize(reduced_res_level, x, y);
			result = x; 
		}
	}
	return result;
}

toprsIRect toprsGadlReader::getImageRectangle( int reduced_res_level /*= 0*/ ) const
{
	toprsIRect result;
	result.makeNAN();

	int x, y;
	getMaxSize(reduced_res_level, x, y);
	if(x&&y)
	{
		return toprsIRect(0,
			0,
			x - 1,
			y - 1);
	}

	if (result.hasNAN())
	{
		return toprsImageReader::getImageRectangle(reduced_res_level);
	}

	return result;
}

void toprsGadlReader::getMaxSize( int resLevel,int& maxX, int& maxY ) const
{
	int aGdalBandIndex = 0;
	maxX = 0;
	maxY = 0;

	//if(theOverview.valid() && theOverview->isValidRLevel(resLevel))// xizhi
	{
		//toprsIRect rect = theOverview->getBoundingRect(resLevel);//xizhi
		toprsIRect rect = getBoundingRect(resLevel);
		if(!rect.hasNAN())
		{
			maxX = rect.width();
			maxY = rect.height();
		}
		return;
	}

	for(aGdalBandIndex=1;
		(int)aGdalBandIndex <= (int)GDALGetRasterCount(theDataset);
		++aGdalBandIndex)
	{
		GDALRasterBandH aBand = resolveRasterBand(resLevel, aGdalBandIndex);
		if(aBand)
		{
			maxY = toprs::max<int>((int)GDALGetRasterBandYSize(aBand), maxY);
			maxX = toprs::max<int>((int)GDALGetRasterBandXSize(aBand), maxX);
		}
		else
		{
			break;
		}
	}
}

GDALRasterBandH toprsGadlReader::resolveRasterBand( int resLevel,	int aGdalBandIndex ) const
{
	GDALRasterBandH aBand = GDALGetRasterBand( theDataset, aGdalBandIndex );

	if( resLevel > 0 )
	{
		int overviewIndex = resLevel-1;

		GDALRasterBandH overviewBand = GDALGetOverview( aBand, overviewIndex );
		if ( overviewBand )
		{
			aBand = overviewBand;
		}
	}
	return aBand;
}

bool toprsGadlReader::setOutputBandList( const std::vector<int>& outputBandList )
{
	m_outputBandList.clear();
	if (outputBandList.size())
	{
		toprs_uint32 bandCount = GDALGetRasterCount(theDataset);
		for (toprs_uint32 i = 0; i < outputBandList.size(); i++)
		{
			if (outputBandList[i] > bandCount)//check if it is a valid band
			{
				return false;
			}
		}
		m_outputBandList = outputBandList;  // Assign the new list.
		return true;
	}
	return false;
}

std::shared_ptr<toprsImg> toprsGadlReader::getTileBlockRead( const toprsIRect& tileRect,int resLevel )
{
	std::shared_ptr<toprsImg> result;
	toprsIRect imageBound = getBoundingRect(resLevel);
	theTile->setImgRect(tileRect);

	// Compute clip rectangle with respect to the image bounds.
	toprsIRect clipRect   = tileRect.clipToRect(imageBound);

	if (tileRect.completely_within(clipRect) == false)
	{
		// Not filling whole tile so blank it out first.
		theTile->makeBlank();
	}
	if(m_isBlocked)
	{
		int xSize=0, ySize=0;
		GDALGetBlockSize(resolveRasterBand( resLevel, 1 ),
			&xSize,
			&ySize);
		toprsIRect blockRect = clipRect;
		blockRect.stretchToTileBoundary(toprsIpt(xSize, ySize));
		blockRect = blockRect.clipToRect(getBoundingRect(resLevel));

		// we need to cache here
		//
		toprs_int64 x = blockRect.left_up().x;
		toprs_int64 y = blockRect.left_up().y;
		toprs_int64 maxx = blockRect.right_down().x;
		toprs_int64 maxy = blockRect.right_down().y;
		toprs_uint32 aGdalBandIndex   = 1;
		toprs_uint32 rasterCount = GDALGetRasterCount(theDataset);
		if (m_outputBandList.size() > 0)
		{
			rasterCount = m_outputBandList.size();
		}

		toprs_uint32 outputBandIndex = 0;
		for(;x < maxx;x+=xSize)
		{
			for(;y < maxy;y+=ySize)
			{
				toprsIpt origin(x,y);

				//std::shared_ptr<toprsImg> cacheTile = toprsAppFixedTileCache::instance()->getTile(m_rlevelBlockCache[resLevel], origin);//xizhi

				//if(!cacheTile.valid())
				//{
				//	toprsIRect rect(origin.x,origin.y,origin.x+xSize-1, origin.y+ySize-1);
				//	theSingleBandTile->setImgRect(rect);
				//	toprsIRect validRect = rect.clipToRect(imageBound);
				//	cacheTile = toprsImgFactory::instance()->create(this);
				//	cacheTile->setImgRect(validRect);
				//	cacheTile->initialize();                          
				//	for(toprs_uint32 aBandIndex =1; aBandIndex <= rasterCount; ++aBandIndex)
				//	{
				//		if (m_outputBandList.size() > 0)
				//		{
				//			aGdalBandIndex = m_outputBandList[outputBandIndex] + 1;
				//			outputBandIndex++;
				//		}
				//		else
				//		{
				//			aGdalBandIndex = aBandIndex;
				//		}
				//		GDALRasterBandH aBand = resolveRasterBand( resLevel, aGdalBandIndex );
				//		if ( aBand )
				//		{
				//			try{

				//				bool bReadSuccess =  (GDALReadBlock(aBand, x/xSize, y/ySize, theSingleBandTile->getBuf() )== CE_None) ? true : false;
				//				if(bReadSuccess)
				//				{
				//					//cacheTile->loadBand(theSingleBandTile->getBuf(), theSingleBandTile->getImgRect(), aBandIndex-1);
				//				}
				//			}
				//			catch(...)
				//			{
				//			}
				//		}
				//	}
				//	//cacheTile->validate();
				//	//toprsAppFixedTileCache::instance()->addTile(m_rlevelBlockCache[resLevel], cacheTile.get(), false);
				//}
				//theTile->loadTile(cacheTile->getBuf(), cacheTile->getImageRectangle(), TOPRS_BSQ);
				result = theTile;
			}
		}
	}
	if(result) result->validate();

	return result;
}

std::shared_ptr<toprsImg> toprsGadlReader::getTile( const toprsIRect& tileRect, int resLevel/*=0*/ )
{
	// This tile source bypassed, or invalid res level, return a blank tile.
	if (!theDataset)
	{
		return std::shared_ptr<toprsImg>();
	}

	// Check for intersect.
	toprsIRect imageBound = getBoundingRect(resLevel);
	if(!tileRect.intersects(imageBound))
	{
		theTile->setImgRect(tileRect);
		theTile->makeBlank();
		return theTile;
	}

	if(m_isBlocked)
	{
		return getTileBlockRead(tileRect, resLevel);
	}
	// Check for overview.
	if(resLevel)
	{
		if ( m_preservePaletteIndexesFlag )
		{
			// No mechanism for this coded for reduce resolution levels.
			std::cout << std::string("toprsGdalReader::getTile ERROR: Accessing reduced resolution level with the preservePaletteIndexesFlag set!") ;
			return std::shared_ptr<toprsImg>();//xizhi
		}

		//if(theOverview.valid() && theOverview->isValidRLevel(resLevel))//xizhi
		//{
		//	std::shared_ptr<toprsImg> tileData = theOverview->getTile(tileRect, resLevel);
		//	tileData->setDataType(getOutputScalarType());
		//	return tileData;
		//}

#if 1
		//---
		// Note: This code was shut off since we currently don't utilize gdal overviews.
		// toprsGdalReader::getNumberOfDecimationLevels has been fixed accordingly.
		
		//---
		//else if(GDALGetRasterCount(theDataset))
		//{
		//	GDALRasterBandH band = GDALGetRasterBand(theDataset, 1);
		//	if(static_cast<int>(resLevel) > GDALGetOverviewCount(band))
		//	{
		//		return std::shared_ptr<toprsImg>();
		//	}
		//}
#endif
	}

	// Set the rectangle of the tile.
	theTile->setImgRect(tileRect);

	// Compute clip rectangle with respect to the image bounds.
	toprsIRect clipRect   = tileRect.clipToRect(imageBound);

	theSingleBandTile->setImgRect(clipRect);

	if (tileRect.completely_within(clipRect) == false)
	{
		// Not filling whole tile so blank it out first.
		theTile->makeBlank();
	}

	// Always blank the single band tile.
	theSingleBandTile->makeBlank();

	toprs_uint32 anToprsBandIndex = 0;
	toprs_uint32 aGdalBandIndex   = 1;

	toprs_uint32 rasterCount = GDALGetRasterCount(theDataset);
	if (m_outputBandList.size() > 0)
	{
		rasterCount = (toprs_uint32) m_outputBandList.size();
	}

	toprs_uint32 outputBandIndex = 0;
	for(toprs_uint32 aBandIndex =1; aBandIndex <= rasterCount; ++aBandIndex)
	{
		if (m_outputBandList.size() > 0)
		{
			aGdalBandIndex = m_outputBandList[outputBandIndex] + 1;
			outputBandIndex++;
		}
		else
		{
			aGdalBandIndex = aBandIndex;
		}
		GDALRasterBandH aBand = resolveRasterBand( resLevel, aGdalBandIndex );
		if ( aBand )
		{
			bool bReadSuccess;
			if(!theIsComplexFlag)
			{
				bReadSuccess = (GDALRasterIO(aBand
					, GF_Read
					, clipRect.left_up().x
					, clipRect.left_up().y
					, clipRect.width()
					, clipRect.height()
					, theSingleBandTile->getBuf()
					, clipRect.width()
					, clipRect.height()
					, theOutputGdtType
					, 0 
					, 0 ) == CE_None) ? true : false;

				if ( bReadSuccess == true )
				{
					if(isIndexed(aGdalBandIndex))
					{
						if(isIndexTo3Band(aGdalBandIndex))
						{
							if ( m_preservePaletteIndexesFlag )
							{
								theTile->loadBand((void*)theSingleBandTile->getBuf(),
									clipRect, anToprsBandIndex);
								anToprsBandIndex += 1;
							}
							else
							{
								loadIndexTo3BandTile(clipRect, aGdalBandIndex, anToprsBandIndex);
								anToprsBandIndex+=3;
							}
						}
						else if(isIndexTo1Band(aGdalBandIndex))
						{ 
							// ??? Ignoring (drb)
							anToprsBandIndex += 1;
						}
					}
					else
					{
						if(theAlphaChannelFlag&&(aGdalBandIndex==rasterCount))
						{
							theTile->nullTileAlpha((toprs_uint8*)theSingleBandTile->getBuf(),
								theSingleBandTile->getImgRect(),
								clipRect,
								false);
						}
						else
						{
							// Note fix rectangle to represent theBuffer's rect in image space.
							theTile->loadBand((void*)theSingleBandTile->getBuf()
								, clipRect
								, anToprsBandIndex);
							++anToprsBandIndex;
						}
					}
				}
				else
				{
					++anToprsBandIndex;
				}
			} 
			else // matches if(!theIsComplexFlag){} 
			{
				bReadSuccess = (GDALRasterIO(aBand
					, GF_Read
					, clipRect.left_up().x
					, clipRect.left_up().y
					, clipRect.width()
					, clipRect.height()
					, &theGdalBuffer.front()
					, clipRect.width()
					, clipRect.height()
					, theOutputGdtType
					, 0
					, 0 ) == CE_None) ? true : false;
				if (  bReadSuccess == true )
				{
					toprs_uint32 byteSize = toprs::dataSizeInBytes(theSingleBandTile->getDataType());
					toprs_uint32 byteSize2 = byteSize*2;
					toprs_uint8* complexBufPtr = (toprs_uint8*)(&theGdalBuffer.front()); // start at first real part
					toprs_uint8* outBufPtr  = (toprs_uint8*)(theSingleBandTile->getBuf());
					toprs_uint32 idxMax = theSingleBandTile->w()*theSingleBandTile->h();
					toprs_uint32 idx = 0;
					for(idx = 0; idx < idxMax; ++idx)
					{
						memcpy(outBufPtr, complexBufPtr, byteSize);
						complexBufPtr += byteSize2;
						outBufPtr     += byteSize;
					}
					theTile->loadBand((void*)theSingleBandTile->getBuf()
						, clipRect
						, anToprsBandIndex);
					++anToprsBandIndex;
					complexBufPtr = (toprs_uint8*)(&theGdalBuffer.front()) + byteSize; // start at first imaginary part
					outBufPtr  = (toprs_uint8*)(theSingleBandTile->getBuf());
					for(idx = 0; idx < idxMax; ++idx)
					{
						memcpy(outBufPtr, complexBufPtr, byteSize);
						complexBufPtr += byteSize2;
						outBufPtr     += byteSize;
					}
					theTile->loadBand((void*)theSingleBandTile->getBuf()
						, clipRect
						, anToprsBandIndex);
					++anToprsBandIndex;
				}
				else
				{
					anToprsBandIndex += 2;
				}
			}
		}
	}

	theTile->validate();
	return theTile;
}

void toprsGadlReader::loadIndexTo3BandTile( const toprsIRect& clipRect,int aGdalBandStart /*= 1*/,int anToprsBandStart /*= 0*/ )
{
	// Typical case 16 bit indexes, eight bit out, NOT 16 bit out.
	toprsDataType inScalar  = getInputScalarType();
	toprsDataType outScalar = getOutputScalarType();

	if ( ( inScalar == TOPRS_UINT8 ) && ( outScalar == TOPRS_UINT8 ) )
	{
		loadIndexTo3BandTileTemplate(toprs_uint8(0), // input type
			toprs_uint8(0), // output type
			clipRect,
			aGdalBandStart,
			anToprsBandStart);
	}
	else if ( ( inScalar == TOPRS_UINT16 ) && ( outScalar == TOPRS_UINT8 ) )
	{
		loadIndexTo3BandTileTemplate(toprs_uint16(0), // input type
			toprs_uint8(0),  // output type
			clipRect,
			aGdalBandStart,
			anToprsBandStart);
	}

	else if ( ( inScalar == TOPRS_UINT16 ) && ( outScalar == TOPRS_UINT16 ) )
	{
		loadIndexTo3BandTileTemplate(toprs_uint16(0), // input type
			toprs_uint16(0),  // output type
			clipRect,
			aGdalBandStart,
			anToprsBandStart);
	}
	else if ( ( inScalar == TOPRS_SINT16 ) && ( outScalar == TOPRS_SINT16 ) )
	{
		loadIndexTo3BandTileTemplate(toprs_sint16(0), // input type
			toprs_sint16(0),  // output type
			clipRect,
			aGdalBandStart,
			anToprsBandStart);
	}
	else if ( ( inScalar == TOPRS_FLOAT32 ) && ( outScalar == TOPRS_FLOAT32 ) )
	{
		loadIndexTo3BandTileTemplate(toprs_float32(0.0), // input type
			toprs_float32(0.0),  // output type
			clipRect,
			aGdalBandStart,
			anToprsBandStart);
	}
	else if ( ( inScalar == TOPRS_FLOAT64 ) && ( outScalar == TOPRS_FLOAT64 ) )
	{
		loadIndexTo3BandTileTemplate(toprs_float64(0.0), // input type
			toprs_float64(0.0),  // output type
			clipRect,
			aGdalBandStart,
			anToprsBandStart);
	}
	else if ( ( inScalar == TOPRS_FLOAT64 ) && ( outScalar == TOPRS_UINT8 ) )
	{
		loadIndexTo3BandTileTemplate(toprs_float64(0.0), // input type
			toprs_uint8(0.0),  // output type
			clipRect,
			aGdalBandStart,
			anToprsBandStart);
	}
	else
	{
		std::cout
			<< __FILE__ << __LINE__
			<< " toprsGdalTileSource::loadIndexTo3BandTile WARNING!\n"
			<< "Unsupported scalar types:\nInupt scalar: " << inScalar
			<< "\nOutput scalar: " << outScalar
			<< std::endl;
	}
}


template<class InputType, class OutputType>
void toprsGadlReader::loadIndexTo3BandTileTemplate( InputType in, OutputType out, const toprsIRect& clipRect, int aGdalBandStart /*= 1*/, int anToprsBandStart /*= 0*/ )
{

	const InputType* s = reinterpret_cast<const InputType*>(theSingleBandTile->getBuf());
	GDALRasterBandH aBand=0;
	aBand = GDALGetRasterBand(theDataset, aGdalBandStart);
	GDALColorTableH table = GDALGetRasterColorTable(aBand);

	// Get the width of the buffers.
	toprs_uint32 s_width = theSingleBandTile->w();
	toprs_uint32 d_width = theTile->w();
	toprsIRect src_rect  = theSingleBandTile->getImgRect();
	toprsIRect img_rect  = theTile->getImgRect();

	// Move the pointers to the first valid pixel.
	s += (clipRect.left_up().y - src_rect.left_up().y) * s_width +
		clipRect.left_up().x - src_rect.left_up().x;
	toprs_uint32 clipHeight = clipRect.height();
	toprs_uint32 clipWidth  = clipRect.width();

	OutputType* d[3];
	d[0]= static_cast<OutputType*>(theTile->getBuf(anToprsBandStart));
	d[1]= static_cast<OutputType*>(theTile->getBuf(anToprsBandStart + 1));
	d[2]= static_cast<OutputType*>(theTile->getBuf(anToprsBandStart + 2));

	OutputType np[3];
	np[0] = (OutputType)theTile->getNullPix(0);
	np[1] = (OutputType)theTile->getNullPix(1);
	np[2] = (OutputType)theTile->getNullPix(2);

	OutputType minp[3];
	minp[0] = (OutputType)theTile->getMinPix(0);
	minp[1] = (OutputType)theTile->getMinPix(1);
	minp[2] = (OutputType)theTile->getMinPix(2);

	int offset = (clipRect.left_up().y - img_rect.left_up().y) * d_width +
		clipRect.left_up().x  - img_rect.left_up().x;
	d[0] += offset;
	d[1] += offset;
	d[2] += offset;

	// Copy the data.

	GDALPaletteInterp interp = GDALGetPaletteInterpretation(table);

	for (int line = 0; line < clipHeight; ++line)
	{
		for (int sample = 0; sample < clipWidth; ++sample)
		{
			GDALColorEntry entry;
			if(GDALGetColorEntryAsRGB(table, s[sample], &entry))
			{
				if(interp == GPI_RGB)
				{
					if(!entry.c4)
					{
						d[0][sample] = 0;
						d[1][sample] = 0;
						d[2][sample] = 0;
					}
					else
					{
#if 0 /* Code shut off to treat all indexes as valid. */
						d[0][sample] = entry.c1==np[0]?minp[0]:entry.c1;
						d[1][sample] = entry.c2==np[1]?minp[1]:entry.c2;
						d[2][sample] = entry.c3==np[2]?minp[2]:entry.c3;
#endif
						d[0][sample] = entry.c1;
						d[1][sample] = entry.c2;
						d[2][sample] = entry.c3;

					}
				}
				else
				{
					d[0][sample] = entry.c1;
					d[1][sample] = entry.c2;
					d[2][sample] = entry.c3;
				}
			}
			else
			{
				d[0][sample] = 0;
				d[1][sample] = 0;
				d[2][sample] = 0;
			}
		}

		s += s_width;
		d[0] += d_width;
		d[1] += d_width;
		d[2] += d_width;
	}

}