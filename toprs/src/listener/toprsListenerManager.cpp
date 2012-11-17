#include <algorithm>
#include <listener/toprsListener.h>
#include <event/toprsEvent.h>
#include <listener/toprsListenerManager.h>


toprsListenerManager::toprsListenerManager()
{
   theFireEventFlag = false;
}


toprsListenerManager::~toprsListenerManager()
{
   theListenerList.clear();
}

void toprsListenerManager::fireEvent(toprsEvent& event)
{
   // only process the event if it has not been consumed.
   if(event.isConsumed())
   {
      return;
   }
   theFireEventFlag = true;

   std::list<toprsListener*>::iterator currentIterator = theListenerList.begin();
   
   while(currentIterator != theListenerList.end())
   {
      // only fire if the event is not consumed
      if(!event.isConsumed())
      {
         if(*currentIterator)
         {
            if(theDelayedRemove.end()==std::find(theDelayedRemove.begin(),
                                                 theDelayedRemove.end(),
                                                 (*currentIterator)))
            {
               if((*currentIterator)->isListenerEnabled())
               {
                  (*currentIterator)->processEvent(event);
               }
            }
         }
      }
      else
      {
         // the event is now consumed so stop propagating.
         theFireEventFlag = false;
         break;
      }
      ++currentIterator;
   }
   
   theFireEventFlag = false;
   
   if(theDelayedAdd.size())
   {
      for(std::list<toprsListener*>::iterator current = theDelayedAdd.begin();
          current != theDelayedAdd.end();++current)
      {
         addListener(*current);
      }
      theDelayedAdd.clear();
   }
   
   if(theDelayedRemove.size())
   {
      for(std::list<toprsListener*>::iterator current = theDelayedRemove.begin();
          current != theDelayedRemove.end();++current)
      {
         removeListener(*current);
      }
      theDelayedRemove.clear();
   }
}

bool toprsListenerManager::addListener(toprsListener* listener)
{
   if(theFireEventFlag)
   {
      theDelayedAdd.push_back(listener);
   }
   else
   {
      
      if(!findListener(listener))
      {
         theListenerList.push_back(listener);
      }
   }
   
   return true;
}

bool toprsListenerManager::removeListener(toprsListener* listener)
{
   
   if(theFireEventFlag)
   {
      theDelayedRemove.push_back(listener);
      return true;
   }
   
   std::list<toprsListener*>::iterator current=theListenerList.begin();
   while(current!=theListenerList.end())
   {
      if( (*current) == listener)
      {
         current = theListenerList.erase(current);
         break; // Should only be in list once.
      }
      else
      {
         ++current;
      }
   }
   
   return true;
}

bool toprsListenerManager::findListener(toprsListener* listener)
{
   bool result = false;

   if(listener)
   {
      std::list<toprsListener*>::iterator currentIter =
         std::find(theListenerList.begin(),
                   theListenerList.end(),
                   listener);
      result = (currentIter != theListenerList.end());
   }
   
   return result;
}

