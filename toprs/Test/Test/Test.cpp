// Test.cpp : Defines the entry point for the console application.
//

//#include "toprsConnAble.h"
#include "connection\toprsConnAbleReader.h"
#include "connection\toprsConnAbleFilter.h"
#include "connection\toprsConnAble.h"

#include "connection/toprsConnection.h"
#include<vector>
#include <memory>

#include "base/toprsIpt.h"
#include "base/toprsDpt.h"

class testinter;



class testinter
{
public:
	testinter():theConnection(nullptr){};
	std::shared_ptr<toprsConnection<testinter> > theConnection;
	std::shared_ptr<toprsConnection<testinter> > getConnection() const{return theConnection;}
	void setConnection(std::shared_ptr<toprsConnection<testinter> > connection){theConnection  = connection;}
	void setConnection(toprsConnection<testinter >* connection){theConnection = std::shared_ptr<toprsConnection<testinter>>(connection);}
};


//
//template<class T,class F>
//bool testt(T*inter)
//{
//	//return true;
//	return toprsConnAble<F>::getInstance()->canConnectMyInputTo(inter);
//}
//template<class T>
//class Conn
//{
//public:
//	template<class F>
//	bool testt(T*inter)
//	{
//		//return true;
//		return toprsConnAble<F>::getInstance()->canConnectMyInputTo(inter);
//	}
//};


int main()
{
	//toprsImg img;
    int test = 5;
	
	toprsConnAble<toprsConnAbleReader> * pb = toprsConnAble<toprsConnAbleReader>::getInstance();
	toprsConnAble<toprsConnAbleFilter> * pd = toprsConnAble<toprsConnAbleFilter>::getInstance();
	////toprsConnAble * pc = toprsConnAble<toprsConnAbleReader>::getInstance();


	bool x = pb->canConnectMyInputTo(&test);

	bool y = pd->canConnectMyInputTo(&test);

	std::shared_ptr<testinter> temp(new testinter);
	std::shared_ptr<testinter> temp2 = std::make_shared<testinter>();

	toprsConnection<testinter> con(temp);
	con.connectMyInputTo(temp2);
	
	toprsConnection<testinter> con2(con);

	toprsConnection<testinter> con3(std::move(con));





	//bool z = con.testt<toprsConnAbleFilter>(&test);

	//toprsConnection<int> theConn(&test);

	//int one = 1;
	//int two =2;
	//int three = 3;

	//theConn.start();
   // theConn>>one>>two>>three;

	
	

	//bool z = pc->canConnectMyInputTo(&test);


	//return 0;
	//toprsConnAble<toprsConnAbleReader>* r = toprsConnAbleReader::getInstance();
	return 0;
}

