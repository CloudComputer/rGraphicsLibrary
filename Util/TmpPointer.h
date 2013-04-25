#ifndef _TMPPOINTER_H_
#define _TMPPOINTER_H_



template <class T>
class TmpPointer{
	T *_t;
public:
	TmpPointer():_t(0){}
	TmpPointer(T* t):_t(t){}
	virtual ~TmpPointer(){delete _t;}

	TmpPointer &operator=(T* t){
		if(_t != 0 && _t != t)
			delete _t;
		_t = t;
		return *this;
	}
	
	T *get(){return _t;}
	T *get()const{return _t;}

	T &operator*(){return *_t;}
	T *operator->(){return _t;}
};

#endif