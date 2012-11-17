#ifndef TOPRSCONABLEREADER_H
#define TOPRSCONABLEREADER_H
#include "connection\toprsConnAble.h"
class toprsConnAbleReader:public toprsConnAble<toprsConnAbleReader>
{
public:
	static toprsConnAble<toprsConnAbleReader>* getInstance();
	template<class T>
	bool canConnectMyInputTo(T* inter)
	{
		return false;
	}
};
#endif