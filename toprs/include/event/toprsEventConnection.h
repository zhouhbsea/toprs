
#ifndef TOPRSCONNECTIONEVENT_H
#define TOPRSCONNECTIONEVENT_H

#include <vector>

#include <event/toprsEventId.h>
#include <event/toprsEvent.h>
#include <connection/toprsConnection.h>

class TOPRSDLLEXPORT toprsConnectionEvent : public toprsEvent
{
public:
   enum toprsConnectionDirectionType
   {
      TOPRS_DIRECTION_UNKNOWN  = 0,
      TOPRS_INPUT_DIRECTION    = 1,
      TOPRS_OUTPUT_DIRECTION   = 2,
      TOPRS_INPUT_OUTPUT_DIRECTION = 8
   };
   
   toprsConnectionEvent(long id = TOPRS_EVENT_NULL);

   toprsConnectionEvent(long id,
                        const std::vector<toprsConnection*>& newList,
                        const std::vector<toprsConnection*>& oldList,
                        toprsConnectionDirectionType whichDirection);

   toprsConnectionEvent(toprsObject* object,
                        long id,
                        toprsConnection* newConnectableObject,
                        toprsConnection* oldConnectableObject,
                        toprsConnectionDirectionType whichDirection);
   
   toprsConnectionEvent(const toprsConnectionEvent& rhs);

  // toprsObject* dup()const;
   
   virtual void setDirection(toprsConnectionDirectionType direction);
   
   virtual toprsConnectionDirectionType getDirection()const;

   virtual toprs_uint32 getNumberOfNewObjects()const;
   
   virtual toprs_uint32 getNumberOfOldObjects()const;
   
   virtual toprsConnection* getOldObject(toprs_uint32 i=0)const;

   virtual toprsConnection* getNewObject(toprs_uint32 i=0)const;

   virtual bool isDisconnect()const;
   
   virtual bool isConnect()const;
      
   virtual bool isInputDirection()const;
   
   virtual bool isOutputDirection()const;
   
protected:
   std::vector<toprsConnection*> theNewObjectList;
   std::vector<toprsConnection*> theOldObjectList;
   toprsConnectionDirectionType         theDirectionType;

};

#endif

