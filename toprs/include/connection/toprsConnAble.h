#ifndef TOPRSCONABLE_H
#define TOPRSCONABLE_H
#include <iostream>


template<class Derive>
class toprsConnAble
{
public:
	toprsConnAble(){};
	virtual ~toprsConnAble(){};
    static toprsConnAble<Derive>* getInstance()
	{
		if (theInstance == nullptr)
		{
			return Derive::getInstance();
		}	
		return theInstance;
	};
	template<class T>
	bool canConnectMyInputTo(T* inter)
	{
		Derive* d = castToDerive();
		return d->canConnectMyInputTo(inter);
	}
protected:
	Derive* castToDerive()
	{
		return static_cast<Derive*> (this);
	}

	const Derive* castToDerive() const
	{
        return static_cast<Derive*> (this);
	}

protected:
	static toprsConnAble<Derive>* theInstance;
};

#endif