#include <iostream>
#include "connection\toprsConnAbleFilter.h"

template<>
toprsConnAble<toprsConnAbleFilter>* toprsConnAble<toprsConnAbleFilter>::theInstance = NULL;

toprsConnAble<toprsConnAbleFilter>*  toprsConnAbleFilter::getInstance()
{
	if (theInstance == nullptr)
	{
		theInstance = new toprsConnAbleFilter;
	}	
	return theInstance;
}
