#ifndef TOPRSGDALREADERH
#define TOPRSGDALREADERH
#include <vector>
#include <string>
#include "gdal/gdal.h"
#include "io/toprsImageReader.h"
class toprsImg;
class TOPRSDLLEXPORT toprsGadlReader : public toprsImageReader
{
public:
	toprsGadlReader();
	virtual ~toprsGadlReader();
	virtual std::string getName()const;
	virtual std::string className()const;
	virtual bool open();
	virtual void close() ;
	virtual bool isOpen()const  = 0;
	virtual int getNumberOfLines(int resLevel = 0) const = 0;
	virtual int getNumberOfSamples(int resLevel = 0) const = 0;
	std::string filterSubDatasetsString(const std::string& subString);
	virtual int getNumberOfInputBands() const;
	virtual int getNumberOfOutputBands() const;
    virtual toprsIRect getImageRectangle(int reduced_res_level = 0) const;

	virtual toprsDataType getOutputScalarType() const;
	toprsDataType getInputScalarType() const;


	bool isIndexTo3Band(int bandNumber = 1)const;
	bool isIndexTo1Band(int bandNumber = 1)const;
	int getIndexBandOutputNumber(int bandNumber)const;
	bool isIndexed(int aGdalBandNumber = 1)const;

	void populateLut();
	void computeMinMax();


private:
	GDALDatasetH        theDataset;
	GDALDriverH         theDriver;

	std::shared_ptr<toprsImg> theTile;
	std::shared_ptr<toprsImg> theSingleBandTile;
	std::vector<toprs_uint8>    theGdalBuffer;
	toprsIRect                  theImageBound;
	mutable GDALDataType        theGdtType;
	mutable GDALDataType        theOutputGdtType;
	double*                     theMinPixValues;
	double*                     theMaxPixValues;
	double*                     theNullPixValues;
	toprs_uint32                theEntryNumberToRender;
	std::vector<std::string>    theSubDatasets;
	bool                        theIsComplexFlag;
	bool                        theAlphaChannelFlag;
	bool                        m_preservePaletteIndexesFlag;
	std::vector<toprs_uint32>        m_outputBandList;
	bool                        m_isBlocked;
};
#endif