#ifndef _IDOBJECT_H_
#define _IDOBJECT_H_


#include <map>
#include <string>
#include <sstream>

class IDObject{
	static std::map<std::string,IDObject *> _objects; //defined in rEgnine.cpp

	std::string _id;

public:
	IDObject(){
		static int id = 1;
		std::stringstream ss;
		ss << "key_" << id++;
		setID(ss.str());
	}

	virtual ~IDObject(){
		_objects.erase(_objects.find(_id));
	}

	void setID(std::string id){
		auto it = _objects.find(_id);
		if(it != _objects.end()){
			_objects.erase(it);
		}
		_id = id;
		_objects[id] = this;
	}

	std::string getID(){
		return _id;
	}

	template<class T>
	static T* GetObjectS(std::string id){
		return static_cast<T*>(_objects[id]);
	}

	template<class T>
	static T* GetObjectD(std::string id){
		return dynamic_cast<T*>(_objects[id]);
	}

	static IDObject* GetObject(std::string id){
		return _objects[id];
	}

};

#endif