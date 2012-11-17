#ifndef TOPRSEVENT_H
#define TOPRSEVENT_H

#include "event\toprsEventId.h"

class toprsEvent
{
	public:
		toprsEvent(int id = TOPRS_EVENT_NULL);
		toprsEvent(const toprsEvent& rhs);

		long getId() const;
		void setId(long id);

		bool isConsumed() const;
		void setConsumedFlag(bool flag = true);
		void consume();

	protected:
		int theId;
		bool theIsConsumedFlag;
};

#endif
