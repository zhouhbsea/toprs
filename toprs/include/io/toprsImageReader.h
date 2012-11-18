#ifndef TOPRSIMAGEREADERH
#define TOPRSIMAGEREADERH
#include <string>
#include <vector>
#include "imagesource/toprsImageSource.h"
class toprsNBandLutDataObject;

class TOPRSDLLEXPORT toprsImageReader : public toprsImageSource
{
public:
	toprsImageReader(){};// = default;
	virtual ~toprsImageReader(){};// = default;

	//virtual bool openOverview();
	virtual bool open() = 0;
	virtual bool open(const std::string& imageFile) ;
	virtual void close() ;
	virtual bool isOpen()const  = 0;
	virtual int getNumberOfLines(int resLevel = 0) const = 0;
	virtual int getNumberOfSamples(int resLevel = 0) const = 0;
	virtual toprsIRect getImageRectangle(int resLevel = 0) const; 
	virtual toprsIRect getBoundingRect(int resLevel = 0) const;
	virtual void getDecimationFactor(int resLevel, toprsDpt& result) const;
    virtual void getDecimationFactors(std::vector<toprsDpt>& decimations) const;
    virtual int getNumberOfDecimationLevels() const;
    virtual int getNumberOfReducedResSets()const;
    virtual void setFilename(const std::string filename);
    virtual const std::string& getFilename()const;
	virtual bool isValidRLevel(int resLevel) const;
	bool canConnectMyInputTo(int inputIndex,const toprsInterfaceImageSource* object) const;
	virtual bool isBandSelector() const;

	virtual bool setOutputToInputBandList();
	virtual bool setOutputBandList(const std::vector<int>& band_list);
	virtual bool setOutputBandList(const std::vector<int>& inBandList,	std::vector<int>& outBandList);

	virtual int getImageTileWidth() const = 0;
	virtual int getImageTileHeight() const = 0;
	virtual double getMinPixelValue(int band=0)const;
	virtual double getMaxPixelValue(int band=0)const;
	virtual double getNullPixelValue(int band=0)const;
	virtual void setMinPixelValue(int band, const toprs_float64& pix);
	virtual void setMaxPixelValue(int band, const toprs_float64& pix);
	virtual void setNullPixelValue(int band, const toprs_float64& pix);
	virtual std::shared_ptr<toprsNBandLutDataObject> getLut() const;
	virtual bool hasLut() const;
	virtual bool hasData() const;
	

	
protected:
	virtual void completeOpen();
	std::string theImageFile;
	std::string theOverviewFile;
	std::string theSupplementaryDirectory;
	std::vector<toprsIpt> theValidImageVertices;
	toprsImg theData;
	std::vector<toprsDpt> theDecimationFactors;
	std::shared_ptr<toprsNBandLutDataObject> theLut;
	int theStartingResLevel;
};
#endif