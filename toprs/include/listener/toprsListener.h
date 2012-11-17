#ifndef TOPRSLISTENER_H
#define TOPRSLISTENER_H

class  toprsEvent;

/*!
* Base class for all listners.  Listners need to derive from this
* class and override the processEvent method.
*/
class  toprsListener
{
public:

	toprsListener();

	virtual ~toprsListener();

	/**
	* ProcessEvent.  The defaul is to do nothing.  Derived
	* classes need to override this class.
	*/
	virtual void processEvent(toprsEvent& event);

	void enableListener();

	void disableListener();

	void setListenerEnableFlag(bool flag);

	bool isListenerEnabled() const;

	bool getListenerEnableFlag() const;

protected:
	bool theListenerEnableFlag;
};

#endif
