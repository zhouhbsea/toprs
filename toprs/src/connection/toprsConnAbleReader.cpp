#include <iostream>
#include "connection\toprsConnAbleReader.h"

template<>
toprsConnAble<toprsConnAbleReader>* toprsConnAble<toprsConnAbleReader>::theInstance = NULL;

toprsConnAble<toprsConnAbleReader>*  toprsConnAbleReader::getInstance()
{
	if (theInstance == nullptr)
	{
		theInstance = new toprsConnAbleReader;
	}	
	return theInstance;
}
