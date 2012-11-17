#ifndef TOPRSIMGH
#define TOPRSIMGH
#include <vector>
#include <memory>
#include "common/toprsConstants.h"
#include "common/toprsCommon.h"
#include "base/toprsIpt.h"
#include "base/toprsIRect.h"

class toprsImg
{
public:
	typedef std::shared_ptr<toprsImg> PImg;
	enum OverwriteBandRule
   {
      COLOR_DISCREPANCY = 0,
      COLOR_DISCREPANCY_OF_ANY_FROM_DEST = 1,
      COLOR_DISCREPANCY_OF_ALL_FROM_DEST = 2,
      COLOR_EQUALITY_OF_ANY_TO_SRC = 3,
      COLOR_EQUALITY_OF_ALL_TO_SRC = 4,
      NULL_RULE = 5
   };

	toprsImg() : theNumberOfBands(1),
	             theDataType(TOPRS_UINT8),
	             theOrigin(0,0),
	             theIndexedFlag(false),
	             theNullPixelV(0),
	             theMinPixelV(0),
	             theMaxPixelV(0),
	             theAlphaChannelV(0)
	{
	//initialize tile size
	}
	toprsImg(toprsDataType type, int bands = 1)
	           :theNumberOfBands(bands),
			    theDataType(type),
			    theOrigin(0,0),
			    theIndexedFlag(false),
			    theNullPixelV(0),
			    theMinPixelV(0),
			    theMaxPixelV(0),
			    theAlphaChannelV(0)
	{
		//initialize tile size;
	}

	toprsImg(toprsDataType type, int bands, int width, int height)
				:theNumberOfBands(bands),
				theWidth(width),
				theHeight(height),
				theDataType(type),
				theOrigin(0,0),
				theIndexedFlag(false),
				theNullPixelV(0),
				theMinPixelV(0),
				theMaxPixelV(0),
				theAlphaChannelV(0)
	{
		//initialize tile size;
	}
	toprsImg(const toprsImg &rhs);
	toprsImg(toprsImg &&rhs){};

	const toprsImg& operator=(const toprsImg &rhs);
	int w()const;
	int h()const;
	void setWidth(int width);
	void setHeight(int height);
	void setWH(int width, int height);

	toprsIRect getImgRect()const;
	void setImgRect(const toprsIRect& rect);

	int getBands()const {return theNumberOfBands;};
	void setBands(int bands){ theNumberOfBands = bands;};

	void setOrigin(const toprsIpt& origin) {theOrigin = origin;};
	const toprsIpt& getOrigin() const {return theOrigin; }

	void setDataType(toprsDataType type){theDataType = type;};
	toprsDataType getDataType()const { return theDataType; }

	void setDataObjectStatus(toprsDataObjectStatus status)const{theDataObjectStatus = status;};
	toprsDataObjectStatus getDataObjectStatus() const {return theDataObjectStatus; }

	int getHashID() const;

	void setNullPix(toprs_float64 null_pix);
	void setNullPix(toprs_float64 null_pix, int band);
	void setNullPix(const toprs_float64* nullPixArray, int numberOfValues);
	void setMinPix(toprs_float64 min_pix);
	void setMinPix(toprs_float64 min_pix, int band);
	void setMinPix(const toprs_float64* minPixArray,int numberOfValues);
	void setMaxPix(toprs_float64 max_pix);
	void setMaxPix(toprs_float64 max_pix, int band);
	void setMaxPix(const toprs_float64* maxPixArray, int numberOfValues);

	void initializeNullDefault();
	void initializeMinDefault();
	void initializeMaxDefault();


	const toprs_float64* getNullPix()const;
	const toprs_float64* getMinPix()const;
	const toprs_float64* getMaxPix()const;
	toprs_float64        getNullPix(int band) const;
	toprs_float64        getMinPix(int band)  const;
    toprs_float64        getMaxPix(int band)  const;

	void computeMinMaxPix(std::vector<toprs_float64>& minBands,
		                  std::vector<toprs_float64>& maxBands)const;
	void computeMinMaxNullPix(std::vector<toprs_float64>& minBands,
		                  std::vector<toprs_float64>& maxBands,
						  std::vector<toprs_float64>& nullBands)const;

	//void updateMinMax();
	//void updateMinMaxNull();

	toprs_float64 getPix(const toprsIpt& pt, int band = 0) const;
	toprs_float64 getPix(int offset, int band = 0) const;
	
	void fill(int band, toprs_float64 val);
	void fill(toprs_float64 val);

	bool isNull(int offset)const;
	bool isNull(int offset, int band)const;
	void setNull(int offset);
	void setNull(int offset, int band);

	bool isNull(const toprsIpt& pt)const;
	void setNull(const toprsIpt& pt);
	bool isNull(const toprsIpt& pt, int band)const;
	void setNull(const toprsIpt& pt, int band);

	bool   isValidBand(int band) const;
	bool   isValid() const;
	toprsDataObjectStatus validate() const;

	PImg newNormalizedFloat()const; //will take this tile and normalized it to a newly allocated floating point tile
	void getNormalizedFloat(int offset, int bandNumber, toprs_float32& result) const;
	void setNormalizedFloat(int offset, int bandNumber, toprs_float32 input) ;


	void converToNormalizedFloat(toprsImg* result) const;

	PImg newNormalizedDouble()const; //will take this tile and normalized it to a newly allocated floating point tile
	void converToNormalizedDouble(toprsImg* result) const;

	void unNormalizedInput(toprsImg* normalizedInput);//covert the normalized input to this tile's type


	toprs_float64 computeAverageBandValue(int band = 0)const;
	toprs_float64 computeMeanSquaredError(toprs_float64 meanValue, int band = 0) const;

	bool hasAlpha() const;
    const toprs_uint8* getAlphaBuf() const;
    toprs_uint8* getAlphaBuf();

	//return the relate data  by the data type
    const void* getBuf() const;
    const toprs_uint8*    getUcharBuf()  const;
    const toprs_uint16*   getUshortBuf() const;
    const toprs_sint16*   getSshortBuf() const;
    const toprs_float32*  getFloatBuf()  const;
    const toprs_float64*  getDoubleBuf() const;
    
	void* getBuf();
    toprs_uint8*    getUcharBuf();
    toprs_uint16*   getUshortBuf();
    toprs_sint16*   getSshortBuf();
    toprs_float32*  getFloatBuf();
    toprs_float64*  getDoubleBuf();

    const void*           getBuf(int band) const;
    const toprs_uint8*    getUcharBuf(int band) const;
    const toprs_uint16*   getUshortBuf(int band) const;
    const toprs_sint16*   getSshortBuf(int band) const;
    const toprs_float32*  getFloatBuf(int band) const;
    const toprs_float64*  getDoubleBuf(int band) const;

    void*           getBuf(int band);
    toprs_uint8*    getUcharBuf(int band);
    toprs_uint16*   getUshortBuf(int band);
    toprs_sint16*   getSshortBuf(int band);
    toprs_float32*  getFloatBuf(int band);
    toprs_float64*  getDoubleBuf(int band);

	int getSize() const;//by pixel
    int getSizePerBand()const;
    int getSizePerBandInBytes() const;
    int getDataSizeInBytes()const;
    int getSizeInBytes() const;

    void makeBlank();//Initialize data to null pixel values
    void initialize();

	void copyTileToNormalizedBuffer(toprs_float64* buf) const;// buf getSize()*sizeof(double)
	void copyTileToNormalizedBuffer(toprs_float32* buf)const;

	void copyTileToNormalizedBuffer(int band, toprs_float64* buf) const;// buf getSize()*sizeof(double)
	void copyTileToNormalizedBuffer(int band, toprs_float32* buf)const;

	void copyNormalizedBufferToTile(toprs_float64* buf);
	void copyNormalizedBufferToTile(toprs_float32* buf);

	void copyNormalizedBufferToTile(int band, toprs_float64* buf);
	void copyNormalizedBufferToTile(int band, toprs_float32* buf);

	bool isWithin(int x, int y);
	void setValue(int x, int y,toprs_float64 color);

	void loadBand(const void* src, const toprsIRect& src_rect, int band);
	void loadBand(const void* src, const toprsIRect& src_rect, const toprsIRect& clip_rect, int band);
	void loadTile(const void* src, const toprsIRect& src_rect, toprsInterleaveType il_type);
	void loadTile(const void* src, const toprsIRect& src_rect, const toprsIRect& clip_rec,toprsInterleaveType il_type);


	void nullTileAlpha(const toprs_uint8* src,
		               const toprsIRect& src_rect, 
					   bool multiplyAlphaFlag = false);
	void nullTileAlpha(const toprs_uint8* src,
		               const toprsIRect& src_rect, 
		               const toprsIRect& clip_rect,
		               bool multiplyAlphaFlag = false);		

	void loadTile(const toprsImg* src);//if we load the tile of different types, we should process by normalizing and then unnormalizing.
	void loadTileFrom1Band(const toprsImg* data);
	void loadTileFrom1Band(const void* src, const toprsIRect& src_rect);

	void loadShortBand(const void* src,const toprsIRect& src_rect, int band, bool swap_bytes=false);

	void unloadBand( void* dest,int src_band, int dest_band, const toprsIRect& dest_rect,
		             toprsInterleaveType il_type=TOPRS_BSQ,OverwriteBandRule ow_type=NULL_RULE) const;
	void unloadBand( void* dest,int src_band, int dest_band, const toprsIRect& dest_rect, const toprsIRect& clip_rect,
		             toprsInterleaveType il_type=TOPRS_BSQ,OverwriteBandRule ow_type=NULL_RULE) const;

	void unloadBand(void* dest, const toprsIRect& dest_rect, int src_band) const;
	void unloadBand(void* dest, const toprsIRect& dest_rect, const toprsIRect& clip_rect, int src_band) const;

	void unloadTile(void* dest, const toprsIRect& dest_rect, toprsInterleaveType il_type ) const;
	void unloadTile(void* dest, const toprsIRect& dest_rect, const toprsIRect& clip_rect, toprsInterleaveType il_type) const;

	void unloadTileToBipAlpha(void* dest, const toprsIRect& dest_rect, const toprsIRect& clip_rect) const;
	bool isPointtWithin(const toprsIpt& point) const;
	bool isPointtWithin(int x, int y) const;

	void stretchMinMax();

	void computeAlphaChannel();// for each pixel if any band value is not null then alpha will be 255, else 0;

	/**
    * @brief Method to copy a single line to the tile.
    *
    * This will copy a line to the tile.  It is assumed that all bands are in
    * the buffer "src".  Only pixels within the tile rect will be copied.
    * Note that all coordinates are relative to the tile coordinates.
    * Clipping will be performed on pixels outside of the tile rectangle.
    * No fill is added if a partial tile line is copied so callers
    * should do a toprsImg::makeBlank if the whole tile is not to be
    * stuffed.  toprsImg::validate should be called after all lines
    * are copied if in doubt of a full tile.
    *
    * @param src The source buffer or line to copy containing all bands.
    *
    * @param lineNumber Line number to copy.
    *
    * @param lineStartSample The start sample of the source buffer relative
    * to tile coordinates.
    *
    * @param lineStopSample The stop sample of the source buffer relative
    * to tile coordinates.
    *
    * @param lineInterleave Interleave of src buffer.  If toprs_BIP assumed
    * rgbrgbrgb; else, red row, green row, blue row.
    *
    */
   void copyLine(const void* src, int lineNumber, int lineStartSample, int lineStopSample, toprsInterleaveType lineInterleave);
   void setIndexedFlag(bool flag);
   bool getIndexedFlag() const;

   template <class T>
   void copyLine(T dummyTemplate,const void* src, int lineNumber, int lineStartSample, int lineStopSample, toprsInterleaveType lineInterleav);

   template <class T>
   void stretchMinMax(T dummyTemplate);

   template <class T>
   void computeAlphaChannel(T dummyTemplate);  

   template <class T>
   toprsDataObjectStatus validate(T dummyTemplate) const;

   template <class T>
   void  makeBlank(T dummyTemplate);
   template <class T>
      toprs_float64 computeMeanSquaredError(T dummyTemplate,
                                            toprs_float64 meanValue,
                                            int bandNumber) const;

   /**
    * Templated computeAverageBandValue method.
    */
   template <class T>
      toprs_float64 computeAverageBandValue(T dummyTemplate,
                                            int bandNumber) const;
   
   void loadTileFromBip(const void* src, const toprsIRect& src_rect);
   void loadTileFromBil(const void* src, const toprsIRect& src_rect);
   void loadTileFromBsq(const void* src, const toprsIRect& src_rect);

   void loadTileFromBip(const void* src, const toprsIRect& src_rect, const toprsIRect& clip_rect);
   void loadTileFromBil(const void* src, const toprsIRect& src_rect, const toprsIRect& clip_rect);
   void loadTileFromBsq(const void* src, const toprsIRect& src_rect, const toprsIRect& clip_rect);

   void unloadTileToBip(void* dest,const toprsIRect& dest_rect,const toprsIRect& clip_rect) const;
   void unloadTileToBil(void* dest,const toprsIRect& dest_rect,const toprsIRect& clip_rect) const;
   void unloadTileToBsq(void* dest,const toprsIRect& dest_rect,const toprsIRect& clip_rect) const;

   void unloadBandToBsq( void* dest,int src_band, int dest_band, const toprsIRect& dest_rect, const toprsIRect& clip_rect,OverwriteBandRule ow_type=NULL_RULE)const;


   template <class T>
   void copyTileToNormalizedBuffer(T dummyTemplate, toprs_float64* buf) const;
   template <class T>
   void copyTileToNormalizedBuffer(T dummyTemplate, toprs_float32* buf)const;

   template <class T>
   void copyTileToNormalizedBuffer(T dummyTemplate, int band, toprs_float64* buf) const;
   template <class T>
   void copyTileToNormalizedBuffer(T dummyTemplate, int band, toprs_float32* buf)const;

   template <class T>
   void copyNormalizedBufferToTile(T dummyTemplate, toprs_float64* buf);
   template <class T>
   void copyNormalizedBufferToTile(T dummyTemplate, toprs_float32* buf);
   template <class T>
   void copyNormalizedBufferToTile(T dummyTemplate, int band,toprs_float64* buf);
   template <class T>
   void copyNormalizedBufferToTile(T dummyTemplate, int band,toprs_float32* buf);

   
   template <class T>
   void computeMinMaxPix(T dummyValue,std::vector<toprs_float64>& minBands,std::vector<toprs_float64>& maxBands) const;

   template <class T>
   void computeMinMaxNullPix(T dummyValue,
                            std::vector<toprs_float64>& minBands,
                            std::vector<toprs_float64>& maxBands,
                            std::vector<toprs_float64>& nulBands) const;
   
   template <class T>
   void loadBandTemplate(T, // dummy template variable
                                            const void* src,
                                            const toprsIRect& src_rect,
                                            int band);

   template <class T>
   void loadBandTemplate(T, // dummy template variable
                                            const void* src,
                                            const toprsIRect& src_rect,
                                            const toprsIRect& clip_rect,
                                            int band);

   template <class T> 
   void nullTileAlphaTemplate(T, const toprs_uint8* src, const toprsIRect& src_rect,const toprsIRect& clip_rect,  bool multiplyAlphaFlag=false);
   
   template <class T> 
   void loadTileFromBipTemplate(T, // dummy template variable
                                                   const void* src,
                                                   const toprsIRect& src_rect);

   template <class T>
   void loadTileFromBipTemplate(T, // dummy template variable
                                                   const void* src,
                                                   const toprsIRect& src_rect,
                                                   const toprsIRect& clip_rect);
   
   template <class T>
   void loadTileFromBilTemplate(T, // dummy template variable
                                                   const void* src,
                                                   const toprsIRect& src_rect);
   
   template <class T>
   void loadTileFromBilTemplate(T, // dummy template variable
                                                   const void* src,
                                                   const toprsIRect& src_rect,
                                                   const toprsIRect& clip_rect);
   
   template <class T>
   void loadTileFromBsqTemplate(T, // dummy template variable
                                                   const void* src,
                                                   const toprsIRect& src_rect);
   
   template <class T>
   void loadTileFromBsqTemplate(T, // dummy template variable
                                                   const void* src,
                                                   const toprsIRect& src_rect,
                                                   const toprsIRect& clip_rect);
   
   template <class T>
   void unloadTileToBipTemplate(T, // dummy template variable
                                                   void* dest,
                                                   const toprsIRect& dest_rect,
                                                   const toprsIRect& clip_rect) const;

   template <class T> 
   void unloadTileToBipAlphaTemplate(T, // dummy template variable
                                                        void* dest,
                                                        const toprsIRect& dest_rect,
                                                        const toprsIRect& clip_rect) const;

   template <class T>
   void unloadTileToBilTemplate(T, // dummy template variable
                                                   void* dest,
                                                   const toprsIRect& dest_rect,
                                                   const toprsIRect& clip_rect) const;

   template <class T>
   void unloadTileToBsqTemplate(T, // dummy template variable
                                                   void* dest,
                                                   const toprsIRect& dest_rect,
                                                   const toprsIRect& clip_rect) const;

   template <class T>
   void unloadBandTemplate(T, // dummy template variable
                                              void* dest,
                                              const toprsIRect& dest_rect,
                                              const toprsIRect& clip_rect,
                                              int band) const;

   template <class T>
   void unloadBandToBsqTemplate(T, // dummy template variable
                                                   void* dest,
                                                   toprs_uint32 src_band,
                                                   toprs_uint32 dest_band,
                                                   const toprsIRect& dest_rect,
                                                   const toprsIRect& clip_rect,
												   OverwriteBandRule ow_type) const;

private:
	toprsDataType         theDataType;
	
	int                   theNumberOfBands;

    toprsIpt              theOrigin;
	int                   theWidth;
	int                   theHeight;
	bool                  theIndexedFlag;
	mutable toprsDataObjectStatus theDataObjectStatus;
	std::vector<toprs_uint8>   theDataBufferV;
    std::vector<toprs_float64> theNullPixelV;
	std::vector<toprs_float64> theMinPixelV;
	std::vector<toprs_float64> theMaxPixelV;
	std::vector<toprs_uint8> theAlphaChannelV;
};



inline toprsIRect toprsImg::getImgRect() const
{
	toprsIpt lr(theOrigin.x + w() - 1, theOrigin.y + h() - 1);
	return toprsIRect(theOrigin, lr);
}

inline int toprsImg::getHashID() const
{
	const unsigned char *bufx = (unsigned char*)(&theOrigin.x);
	const unsigned char *bufy = (unsigned char*)(&theOrigin.y);
	return (int)(bufx[0]*101 + bufx[1]*103 +
		bufx[2]*107 + bufx[3]*109 +
		bufy[0]*139 + bufy[1]*149 +
		bufy[2]*151 + bufy[3]*157);
}

inline bool toprsImg::hasAlpha() const
{
	return (theAlphaChannelV.size()?true:false);
}



inline const toprs_float64* toprsImg::getNullPix()const
{
	return ( theNullPixelV.size() > 0 ? &theNullPixelV.front() : 0 );
}

inline const toprs_float64* toprsImg::getMinPix()const
{
	return ( theMinPixelV.size() > 0 ? &theMinPixelV.front() : 0 );
}

inline const toprs_float64* toprsImg::getMaxPix()const
{
	return ( theMaxPixelV.size() > 0 ? &theMaxPixelV.front() : 0 );
}

inline toprs_float64 toprsImg::getNullPix(int band) const
{
	return ( band < theNullPixelV.size() ? theNullPixelV[band] :
		toprs::defaultNull( getDataType() ) );
}

inline toprs_float64 toprsImg::getMinPix(int band) const
{
	return ( band < theMinPixelV.size() ? theMinPixelV[band] :
		toprs::defaultMin( getDataType() ) );
}

inline toprs_float64 toprsImg::getMaxPix(int band) const
{
	return ( band < theMaxPixelV.size() ? theMaxPixelV[band] :
		toprs::defaultMax( getDataType() ) );
}



#endif