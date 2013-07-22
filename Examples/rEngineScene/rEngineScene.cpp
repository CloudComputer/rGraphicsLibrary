#include <rEngine\rEngine.h>

#include <rEngine\Viewport.h>

#include <iostream>

#include <rEngine\Texture.h>

#include <tinyxml2\tinyxml2.h>
#include <Util\Directory.h>

#include <Util\Macros.h>

tinyxml2::XMLDocument doc;

void loadXML(rEngine *t,std::string filename){
	doc.LoadFile(filename.c_str());

	auto firstChild = doc.FirstChildElement("rEngine");
	if(firstChild){
		bool windowCreated = false;
		FOR_XML_ELE("window",firstChild,win){
			rWindow::ReadXML(t,win);
			windowCreated = true;
		}
		if(!windowCreated){
			std::cerr << "No window defined" << std::endl;
			exit(-1);
		}

		FOR_XML_ELE("texture",firstChild,tex){
			Texture::loadTexture(tex);
		}
		
		FOR_XML_ELE("scene",firstChild,scene){
			t->addScene(Scene::CreateScene(scene));
		}


	}
}

void init(rEngine *t,int argc, char **argv){


	std::string filename = "";
	if(argc == 1){
		if(Directory::exists("scene.xml"))
			filename = "scene.xml";
	}else{
		filename = std::string(argv[1]);
	}

	if(filename.length() == 0 || !Directory::exists(filename) ){
		std::cout << "Usage: " << argv[0] << " scenefile.xml" << std::endl;
		exit(-1);
	}
	loadXML(t,filename);
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);                      
	glClearDepth (1.0f);                                        
	glEnable (GL_DEPTH_TEST); 
	glEnable(GL_TEXTURE_2D);                                
	
}

rENGINE_MAIN(init)