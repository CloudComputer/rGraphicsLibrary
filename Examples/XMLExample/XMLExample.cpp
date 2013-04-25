#include <Base\XMLObjectHandler.h>


#include <iostream>

class TestObject : public XMLObject{
	
public:
	TestObject(){}
	~TestObject(){}
	virtual std::string toString()const{
		std::stringstream ss;
		ss << a << " ";
		ss << b << " ";
		ss << c << " ";
		ss << d << " ";
		ss << e << " ";
		ss << f << " ";
		ss << g;
		return ss.str();
	}
	
	float a,b,c,d,e,f,g;

	virtual void save(tinyxml2::XMLNode *parent){
		
		auto element = parent->GetDocument()->NewElement("TestObject");
		parent->InsertEndChild(element);

		element->SetAttribute("a",a);
		element->SetAttribute("b",b);
		element->SetAttribute("c",c);
		element->SetAttribute("d",d);
		element->SetAttribute("e",e);
		element->SetAttribute("f",f);
		element->SetAttribute("g",g);
	}

};


class TestFactory : public Factory{
public:
	TestFactory():Factory("TestObject"){};

	virtual std::string toString()const{return "TestFactory";}

	virtual XMLObject* create(tinyxml2::XMLElement *xmlelement){
		auto t = new TestObject();
		t->a = xmlelement->FloatAttribute("a");
		t->b = xmlelement->FloatAttribute("b");
		t->c = xmlelement->FloatAttribute("c");
		t->d = xmlelement->FloatAttribute("d");
		t->e = xmlelement->FloatAttribute("e");
		t->f = xmlelement->FloatAttribute("f");
		t->g = xmlelement->FloatAttribute("g");
		return t;
	}
};







int main(int argc,char **argv){
	TestFactory tf;


	TestObject t;
	t.a = 1;
	t.b = 2;
	t.c = 3;
	t.d = 4;
	t.e = 5;
	t.f = 6;
	t.g = 7;
	
	std::cout << t << std::endl;

	XMLObjectHandler::Handler()->save(&t,"testSave1.xml");


	std::vector<XMLObject*> objects;
	objects.push_back(&t);
	objects.push_back(&t);
	XMLObjectHandler::Handler()->save(objects,"testSave2.xml");
	

	objects.clear();
	XMLObjectHandler::Handler()->read(objects,"testSave2.xml");
	
	std::cout << objects[0] << std::endl;

	return 0;
}