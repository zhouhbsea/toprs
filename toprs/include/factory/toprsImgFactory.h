#ifndef TOPRSIMGFACTORYH
#define TOPRSIMGFACTORYH
//#include <mutex>
#include "base/toprsImg.h"
#include "imagesource/toprsImageSource.h"
class TOPRSDLLEXPORT toprsImgFactory
{
public:
	virtual ~toprsImgFactory();
	static toprsImgFactory* instance();
	virtual std::shared_ptr<toprsImg> create(toprsDataType scalar,int bands = 1)const;
	virtual std::shared_ptr<toprsImg> create(toprsDataType scalar,int bands,int width,int height)const;
	virtual std::shared_ptr<toprsImg> create(int bands,	toprsImageSource* inputSource)const;
	virtual std::shared_ptr<toprsImg> create(toprsImageSource* inputSource)const;
protected:
	toprsImgFactory(); // hide
	toprsImgFactory(const toprsImgFactory&){}//hide
	void operator = (toprsImgFactory&){}// hide
	static toprsImgFactory* theInstance;
	//static std::mutex theInstanceMutex;
};
#endif