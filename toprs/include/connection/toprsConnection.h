#ifndef TOPRSCONNECTION_H
#define TOPRSCONNECTION_H



#include <vector>
#include <algorithm>
#include <memory>

#include <event\toprsEvent.h>
#include <listener\toprsListenerManager.h>
#include "connection\toprsConnAble.h"


//class toprsVisitor;



template<class T>
class toprsConnection
{
public:
	typedef std::shared_ptr<T> Ptr;
	typedef std::vector<std::shared_ptr<T>> PV;
	typedef toprsConnection<T>  TConn;
	typedef std::shared_ptr<TConn> PConn;
public:
	class TNode
	{
	public:
		typedef std::shared_ptr<TNode> PNode;
		typedef std::vector<PNode> PNodeV;
	private:
		Ptr theOwnerP;
		PNodeV theInNodePV;
		PNodeV theOutNodePV;
	public:
		TNode() :theOwnerP(nullptr), theInNodePV(), theOutNodePV() {}
        explicit TNode(Ptr owner) : theOwnerP(owner), theInNodePV(), theOutNodePV() {}
		TNode(const TNode& rhs):theOwnerP(rhs.theOwnerP),theInNodePV(rhs.theInNodePV),theOutNodePV(rhs.theOutNodePV) {}
		TNode(TNode&& rhs):theOwnerP(std::move(rhs.theOwnerP)),
			theInNodePV(std::move(rhs.theInNodePV)),
			theOutNodePV(std::move(rhs.theOutNodePV))
		{
			for_each(theInNodePV.begin(), theInNodePV.end(), [&](PNode it){it->theOutNodePV.push_back(this);});
			for_each(theOutNodePV.begin(),theOutNodePV.end(),[&](PNode it){it->theInNodePV.push_back(this);});
		}

		~TNode()
		{
			theInNodePV.clear();
			theOutNodePV.clear();
		}
		friend class toprsConnection<T>;
	 };
public:
	
public:
   enum ConnectionDirectionType
   {
      CONNECTABLE_DIRECTION_NONE   = 0,
      CONNECTABLE_DIRECTION_INPUT  = 1,
      CONNECTABLE_DIRECTION_OUTPUT = 2
   };

   toprsConnection(Ptr owner = nullptr):theNodeP(new TNode(owner))
   {
	   std::cout<<theNodeP.use_count()<<std::endl;
	   theListnerManagerP = std::make_shared<toprsListenerManager>();
	   std::cout<<theListnerManagerP.use_count()<<std::endl;
	   owner->setConnection(this);
   };
   
   toprsConnection(const toprsConnection<T>& rhs);
   toprsConnection(toprsConnection<T>&& rhs);
   virtual ~toprsConnection();
   int getInDeg() const {return theNodeP->theInNodePV.size();}
   int getOutDeg() const {return theNodeP->theOutNodePV.size();}
   T*  getOwner()const {return theNodeP->theOwnerP.get();}
   void changeOwner(Ptr owner)
   {
	   theNodeP->theOwnerP = owner; 
	   theNodeP->theOwnerP->setConnection (this);
   }

   void changeOwner(T* owner)
   {
	   theNodeP->theOwnerP = std::shared_ptr(owner);
	   theNodeP->theOwnerP->setConnection(this);
   }


   bool isConnected(ConnectionDirectionType direction = CONNECTABLE_DIRECTION_INPUT)const;
    /**
     * Will connect the object passed in.
     */
   void connect(Ptr inter = nullptr);
   /**
    * Will disconnect the object passed in.
    */
   void disconnect(Ptr inter = nullptr);


   int connectMyInputTo(Ptr inter,
		                bool makeOutputConnection = true,
		                bool creatEventFlag = true);

   int findInputIndex(Ptr inter);
    /**
     * Will return the index_th Interface form the InputList
     */

   T*  getInput(int index)const;

   static PConn New(Ptr inter)
   { 
	   return std::make_shared<toprsConnection<T>>(inter);
	 //return std::shared_ptr(new toprsConnection<T>(inter));
   }


	template<class T>
    friend toprsConnection<T>& operator>>(toprsConnection<T>&left, T& right);
	/**
	* We will add a visitor interface for all connectable objects.
	*/
	//virtual void accept(toprsVisitor& visitor);

private:
	std::shared_ptr<TNode>                  theNodeP;
	std::shared_ptr<toprsListenerManager>   theListnerManagerP;
};


template<class T>
toprsConnection<T>& operator>>(toprsConnection<T>&left, T& right)
{

}

template<class T>
toprsConnection<T>::~toprsConnection()
{

}

template<class T>
toprsConnection<T>::toprsConnection(const toprsConnection<T>& rhs)
	:theListnerManagerP(rhs.theListnerManagerP),
	theNodeP(rhs.theNodeP)
{
}

template<class T>
toprsConnection<T>::toprsConnection(toprsConnection<T>&& rhs)
	:theListnerManagerP(std::move(rhs.theListnerManagerP)),
	theNodeP(std::move(rhs.theNodeP))
{
}


template<class T>
T* toprsConnection<T>::getInput(int index)const
{
	return theNodeP->theInNodePV[0]->theOwnerP.get();
}

template<class T>
int toprsConnection<T>::connectMyInputTo(Ptr inter,	bool makeOutputConnection,bool creatEventFlag)

{
	if(!inter) return -1;
	int index = findInputIndex(inter);
	if(index >=0 )return index;

	if(1)
	{
		PConn connection = inter->getConnection();//the connection must initial in T inter;
	
		if (connection == nullptr)
		{
			connection = TConn::New(inter);
		}
		theNodeP->theInNodePV.push_back(connection->theNodeP);
		connection->theNodeP->theOutNodePV.push_back(theNodeP);

	}
	return 0;
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
int toprsConnection<T>::findInputIndex(Ptr inter)
{
	//auto head = theInputList.begin();
	//auto tail = theInputList.end();
	//int i = 0;
	//for_each(head,tail,[&](T* it){if(it!=inter)++i});
	//if (i == theInputList.size())
	//	return -1;
	//else
	//	return i;
	return -1;
}



#endif
