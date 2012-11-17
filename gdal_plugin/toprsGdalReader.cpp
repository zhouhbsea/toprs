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

int toprsGadlReader::getNumberOfLines( int resLevel /*= 0*/ ) const
{
	return 0;
}

int toprsGadlReader::getNumberOfSamples( int resLevel /*= 0*/ ) const
{
	int result = 0;
	if ( isOpen() )
	{

			int x, y;
			//getMaxSize(reduced_res_level, x, y);
			result = x;

	}
	return result;
}

toprsIRect toprsGadlReader::getImageRectangle( int reduced_res_level /*= 0*/ ) const
{
	return toprsIRect(0,0,0,0);
}


