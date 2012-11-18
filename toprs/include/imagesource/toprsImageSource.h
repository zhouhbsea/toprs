//**************************************************************
//Copyright (C) 2012 TOPRS Inc.
//
//Author: Huabing Zhou
//
//Data: 2012-8-21
//
//**************************************************************
#ifndef TOPRSIMAGESOURCE_H
#define TOPRSIMAGESOURCE_H


#include "interface/toprsInterfaceImageSource.h"
#include "connection/toprsConnection.h"

class toprsImageSource : public toprsInterfaceImageSource
{
public:
	toprsImageSource(){}; //= default;
	virtual ~toprsImageSource(){};// = default;
	virtual bool initialize();
	virtual PImg getTile(const toprsIpt& origin, int resLevel = 0);	
	virtual PImg getTile(const toprsIRect& rect, int resLevel = 0);	
	virtual bool getTile(toprsImg* result, int resLevel = 0);
	virtual int getNumberOfInputBands() const = 0;
	virtual int getNumberOfOutputBands() const;
	virtual void getOutputBandList(std::vector<int>& bandList) const;
	virtual toprsDataType getOutputScalarType() const;
	virtual int getTileWidth() const;
	virtual int getTileHeight() const;
	virtual bool canConnectInputTo(toprsInterfaceImageSource* input);
	virtual void connectInputTo(toprsInterfaceImageSource* input);

	virtual double getNullPixelValue(int band=0)const;
	virtual double getMinPixelValue(int band=0)const;
	virtual double getMaxPixelValue(int band=0)const;
	virtual toprsIRect getBoundingRect(int resLevel=0) const;
	virtual void getBoundingRect(toprsIRect& rect,int resLevel=0) const;
	virtual bool isIndexedData() const;

	//void getValidImageVertices(std::vector<toprsIpt>& validVertices,toprsVertexOrdering ordering,int resLevel)const
	virtual void getDecimationFactors(int resLevel, toprsDpt& result)const;
    virtual void getDecimationFactors(std::vector<toprsDpt>& decimations) const;
    virtual int getNumberOfDecimationLevels() const;
	   /** 
	   * listener Function
	   */
	virtual void connectInputEvent(toprsEvent& event);

    virtual toprsInterfaceImageSource* getInterface() { return this; }
    virtual const toprsInterfaceImageSource* getInterface() const { return this; }

	  // friend toprsInterfaceImageSource& operator >>(toprsInterfaceImageSource& left, toprsInterfaceImageSource& right);
protected:
    toprsImageSource(const toprsImageSource& rhs);// = delete;
    const toprsImageSource& operator= (const toprsImageSource&);// = delete;
};
#endif
