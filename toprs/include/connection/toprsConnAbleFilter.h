#ifndef TOPRSCONABLEFILTER_H
#define TOPRSCONABLEFILTER_H
#include "connection\toprsConnAble.h"
class toprsConnAbleFilter:public toprsConnAble<toprsConnAbleFilter>
{
public:
	static toprsConnAble<toprsConnAbleFilter>* getInstance();
	template<class T>
	bool canConnectMyInputTo(T* inter)
	{
		return true;
	}
};
#endif