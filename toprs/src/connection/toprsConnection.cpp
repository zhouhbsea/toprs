#include <event\toprsEvent.h>
#include <connection\toprsConnection.h>
#include <listener\toprsListenerManager.h>

//template<class T>
//toprsConnection<T>::toprsConnection<T>( T* owner /* =nullptr*/ )
//	:theListnerManager(nullptr),
//	theOwner(owner)
//{
//	
//};

//template<class T>
//toprsConnection::~toprsConnection()
//{
//	//toprsObjectDestructingEvent event(this);
//	//theListnerManager->fireEvent(event);
//	//disconnect();
//}

template<class T>
void toprsConnection<T>::changeOwner(T* owner)
{
	theOwner = owner;
}

template<class T>
bool toprsConnection<T>::isConnected(ConnectionDirectionType direction = CONNECTABLE_DIRECTION_INPUT)const
{
    if(direction & CONNECTABLE_DIRECTION_INPUT)
    {
	    
    }
    if(direction & CONNECTABLE_DIRECTION_OUTPUT)
    {
    }
    return true;
}

template<class T>
int toprsConnection<T>::findInputIndex(T* inter)
{
	auto it = theInputList.begin();
	auto ed = theInputList.end();
	int i;
	for (i = 0; it!= ed; ++it,++i)
	{
		if (it == inter)
		{
			return i;
		}
	}
	return i;
}




