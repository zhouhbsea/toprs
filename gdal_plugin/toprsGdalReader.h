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
	virtual std::shared_ptr<toprsImg> getTile(const toprsIRect& tileRect, int resLevel=0);

	virtual void close() ;
	virtual bool isOpen()const  = 0;
	virtual int getNumberOfLines(int resLevel = 0) const;
	virtual int getNumberOfSamples(int resLevel = 0) const;
	virtual int getNumberOfInputBands() const;
	virtual int getNumberOfOutputBands() const;
    virtual toprsIRect getImageRectangle(int reduced_res_level = 0) const;

	virtual toprsDataType getOutputScalarType() const;
	

	virtual bool setOutputBandList(const std::vector<int>& band_list);





	GDALDriverH getDriver() { return theDriver; }
	toprsDataType getInputScalarType() const;


private:
	std::shared_ptr<toprsImg> getTileBlockRead(const toprsIRect& tileRect,int resLevel);

	std::string filterSubDatasetsString( const std::string& subString );

	void loadIndexTo3BandTile(const toprsIRect& clipRect,int aGdalBandStart = 1,int anToprsBandStart = 0);
	template<class InputType, class OutputType>
	void loadIndexTo3BandTileTemplate(InputType in,	OutputType out,
		                              const toprsIRect& clipRect,
		                              int aGdalBandStart = 1,
		                              int anToprsBandStart = 0);
	
	bool isIndexTo3Band(int bandNumber = 1)const;
	bool isIndexTo1Band(int bandNumber = 1)const;
	int getIndexBandOutputNumber(int bandNumber)const;
	bool isIndexed(int aGdalBandNumber = 1)const;
	void populateLut();
	void computeMinMax();
	void getMaxSize(int resLevel,int& maxX,	int& maxY)const;
	GDALRasterBandH resolveRasterBand( int resLevel,int aGdalBandIndex )const;

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
	int                         theEntryNumberToRender;
	std::vector<std::string>    theSubDatasets;
	bool                        theIsComplexFlag;
	bool                        theAlphaChannelFlag;
	bool                        m_preservePaletteIndexesFlag;
	std::vector<int>            m_outputBandList;
	bool                        m_isBlocked;
};
#endif