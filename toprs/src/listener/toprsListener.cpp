#include <listener/toprsListener.h>


toprsListener::toprsListener()
:theListenerEnableFlag(true)
{}

toprsListener::~toprsListener()
{
}

void toprsListener::processEvent(toprsEvent& /* event */)
{
}

void toprsListener::enableListener()
{
	theListenerEnableFlag = true;
}

void toprsListener::disableListener()
{
	theListenerEnableFlag = false;
}

void toprsListener::setListenerEnableFlag(bool flag)
{
	theListenerEnableFlag = flag;
}

bool toprsListener::isListenerEnabled()const
{
	return theListenerEnableFlag;
}

bool toprsListener::getListenerEnableFlag()const
{
	return theListenerEnableFlag;
}
