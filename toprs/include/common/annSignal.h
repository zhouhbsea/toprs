#ifndef ANNSIGNAL_H
#define ANNSIGNAL_H
class InterfaceDelegation
{
public:
	virtual ~InterfaceDelegation() {};
	virtual void Run() = 0;
};

template<class T>
class ImplDelegation : public InterfaceDelegation
{
public:
	typedef void (T::*pF)(); // point to T member function pointer
	ImplDelegation(T* _p, pF _pF) : mP(_p), mPf(_pF) {}
	virtual void Run()
	{
       if (mP)  
		   (mP->*mPf)();
	}
private:
	T* mP;
	pF mPf;
};

class Signal
{
public:
	Signal() : mInterDel(NULL) {};
	void operator() ()
	{
		if(mInterDel)  mInterDel->Run();
	}
	template<class T, class F>
	void ConnectSlot(T& recv, F pF)
	{
		if (mInterDel) delete mInterDel;
		mInterDel = new ImplDelegation<T>(&recv, pF);
	}
private:
	InterfaceDelegation* mInterDel;
};

#endif