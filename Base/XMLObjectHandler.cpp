#include "XMLObjectHandler.h"


XMLObjectHandler* XMLObjectHandler::instance = 0;

XMLObjectHandler *XMLObjectHandler::Handler(){
	if(instance == 0)
		instance = new XMLObjectHandler();
	return instance;
}


XMLObjectHandler::XMLObjectHandler(){
	init();
}

void XMLObjectHandler::registerFactory(std::string name,Factory *f){
	_factories[name] = f;
}

XMLObjectHandler::~XMLObjectHandler(){
	tinyxml2::XMLNode *node;
	
}

void XMLObjectHandler::init(){
	
}


void XMLObjectHandler::save(XMLObject* obj,std::string filename){
	tinyxml2::XMLDocument doc;
	doc.InsertFirstChild(doc.NewDeclaration());
	auto root = doc.NewElement("rGraphicsLibraryXMLFile");
	doc.InsertEndChild(root);
	obj->save(root);

	doc.SaveFile(filename.c_str());


}
void XMLObjectHandler::save(std::vector<XMLObject*> &objects,std::string filename){
	tinyxml2::XMLDocument doc;
	doc.InsertFirstChild(doc.NewDeclaration());
	auto root = doc.NewElement("rGraphicsLibraryXMLFile");
	doc.InsertEndChild(root);
	for(auto obj = objects.begin();obj!=objects.end();++obj){
		(*obj)->save(root);
	}

	doc.SaveFile(filename.c_str());
	
}

void XMLObjectHandler::read(std::vector<XMLObject*> &objects,std::string filename){
	tinyxml2::XMLDocument doc;
	doc.LoadFile(filename.c_str());

	auto firstChild = doc.FirstChildElement("rGraphicsLibraryXMLFile");
	if(firstChild){
		tinyxml2::XMLElement *e = firstChild->FirstChildElement();
		while(e){
			objects.push_back(_factories[e->Name()]->create(e));
			e = e->NextSiblingElement();
		}
	}
}