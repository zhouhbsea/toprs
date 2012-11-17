#include <event/toprsEventConnection.h>



toprsConnectionEvent::toprsConnectionEvent(long id)
    theDirectionType(TOPRS_DIRECTION_UNKNOWN)
{
}

toprsConnectionEvent::toprsConnectionEvent(
   long id,
   const std::vector<toprsConnection*>& newList,
   const std::vector<toprsConnection*>& oldList,
   toprsConnectionDirectionType whichDirection)
   : toprsEvent(object, id),
     theNewObjectList(newList),
     theOldObjectList(oldList),
     theDirectionType(whichDirection)
{
}

toprsConnectionEvent::toprsConnectionEvent(
   toprsObject* object,
   long id,
   toprsConnection* newConnectableObject,
   toprsConnection* oldConnectableObject,
   toprsConnectionDirectionType whichDirection)
   : toprsEvent(object, id),
     theDirectionType(whichDirection)
{
   if(newConnectableObject)
   {
      theNewObjectList.push_back(newConnectableObject);
   }
   if(oldConnectableObject)
   {
      theOldObjectList.push_back(oldConnectableObject);
   }
}
   
toprsConnectionEvent::toprsConnectionEvent(const toprsConnectionEvent& rhs)
   : toprsEvent(rhs),
     theNewObjectList(rhs.theNewObjectList),
     theOldObjectList(rhs.theOldObjectList),
     theDirectionType(rhs.theDirectionType)
{
}

toprsObject* toprsConnectionEvent::dup()const
{
   return new toprsConnectionEvent(*this);
}

void toprsConnectionEvent::setDirection(
   toprsConnectionDirectionType direction)
{
   theDirectionType   = direction;
}

toprsConnectionEvent::toprsConnectionDirectionType toprsConnectionEvent::getDirection()const
{
   return theDirectionType;
}

toprs_uint32 toprsConnectionEvent::getNumberOfNewObjects()const
{
   return theNewObjectList.size();
}

toprs_uint32 toprsConnectionEvent::getNumberOfOldObjects()const
{
   return theOldObjectList.size();
}

toprsConnection* toprsConnectionEvent::getOldObject(toprs_uint32 i)const
{
   if(i < getNumberOfOldObjects())
   {
      return theOldObjectList[i];
   }
   
   return (toprsConnection*)NULL;
}

toprsConnection* toprsConnectionEvent::getNewObject(toprs_uint32 i)const
{
   if(i < getNumberOfNewObjects())
   {
      return theNewObjectList[i];
   }
   
   return (toprsConnection*)NULL;
}

bool toprsConnectionEvent::isDisconnect()const
{
   return (getId()==TOPRS_EVENT_CONNECTION_DISCONNECT);
}

bool toprsConnectionEvent::isConnect()const
{
   return (getId()==TOPRS_EVENT_CONNECTION_CONNECT);
}

bool toprsConnectionEvent::isInputDirection()const
{
   return ((long)theDirectionType  & (long)TOPRS_INPUT_DIRECTION);
}

bool toprsConnectionEvent::isOutputDirection()const
{
   return ((long)theDirectionType & TOPRS_OUTPUT_DIRECTION);
}

