#ifndef TOPRSLISTENERMANAGER_H
#define TOPRSLISTENERMANAGER_H
#include <list>

class toprsListener;
class toprsEvent;

class toprsListenerManager
{
public:
   /*!
    * Default constructor and needs no initialization.
    */
   toprsListenerManager();

   /*!
    * Default destructor.  Note the Manager doesn't own the listeners
    * and will not delete them
    */
   ~toprsListenerManager();

   /*!
    * Traverses through all listeners and fires an event to them.
    * if any listener sets the consumed flag on the event it
    * will stop traversing the list.
    */
   void fireEvent(toprsEvent& event);
   
   /*!
    * Appends the listener onto the list.
    */
   bool addListener(toprsListener* listener);

   /*!
    * Finds and removes the listener.
    */
   bool removeListener(toprsListener* listener);

   /*!
    * Searches the list and sees if a listener is found
    */
  bool findListener(toprsListener* listener);
  
protected:
  toprsListenerManager(const toprsListenerManager& rhs);//can not copy
  
  /*!
   *  Holds the list of listeners.
   */
  std::list<toprsListener*> theListenerList;
  bool theEnabledFlag;
  mutable std::list<toprsListener*> theDelayedAdd;
  mutable std::list<toprsListener*> theDelayedRemove;
  mutable bool theFireEventFlag;
};

#endif
