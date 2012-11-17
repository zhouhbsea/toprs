#include "event\toprsEvent.h"

toprsEvent::toprsEvent(int id)
	:theId(id),
	theIsConsumedFlag(false)
{
}


toprsEvent::toprsEvent(const toprsEvent& rhs)
	:theId(rhs.theId),
	theIsConsumedFlag(rhs.theIsConsumedFlag)
{
}


long toprsEvent::getId() const
{
   return theId;
}

void toprsEvent::setId(long id)
{
   theId = id;
}

bool toprsEvent::isConsumed() const
{
   return theIsConsumedFlag;
}

void toprsEvent::setConsumedFlag(bool flag)
{
   theIsConsumedFlag = flag;
}

void toprsEvent::consume()
{
   setConsumedFlag(true);
}


