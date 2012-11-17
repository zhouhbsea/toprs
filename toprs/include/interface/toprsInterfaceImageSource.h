//**************************************************************
//Copyright (C) 2012 TOPRS Inc.
//
//Author: Huabing Zhou
//
//Data: 2012-8-21
//
//**************************************************************
#ifndef TOPRSINTERFACEIMAGESOURCE_H
#define TOPRSINTERFACEIMAGESOURCE_H

#include <vector>
#include <memory>
#include "connection/toprsConnection.h"
#include "base/toprsImg.h"



class toprsIRect;
class toprsEvent;


class toprsInterfaceImageSource
{
public:
	typedef toprsConnection<toprsInterfaceImageSource> TImgConn;
	typedef std::shared_ptr<TImgConn> PImgConn;
	typedef std::shared_ptr<toprsImg> PImg;
public:
	toprsInterfaceImageSource():theConnection(nullptr){};
	PImgConn getConnection() const{return theConnection;}
	void setConnection(PImgConn connection){theConnection  = connection;}
	void setConnection(TImgConn* connection){theConnection = PImgConn(connection);}
public:
	virtual ~toprsInterfaceImageSource(){};
	virtual bool initialize() = 0;
	virtual PImg getTile(const toprsIRect& rect, int resLevel = 0) = 0;	
	virtual toprsDataType getOutputScalarType() const = 0;
	/**
	 * Connection Function                                                                  
	 */
	virtual bool canConnectInputTo(toprsInterfaceImageSource* inter) = 0;
	virtual void connectInputTo(toprsInterfaceImageSource* input) = 0;
	/** 
	 * Listner Function
	 */
	virtual void connectInputEvent(toprsEvent& event) = 0;
protected:
    PImgConn    theConnection;
};

#endif
