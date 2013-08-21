#include <rEngine\rEngine.h>

#include <rEngine\Viewport.h>

#include <iostream>

#include <rEngine\Texture.h>

#include <tinyxml2\tinyxml2.h>
#include <Util\Directory.h>

#include <Util\Macros.h>
#include <Util\Logger.h>

#include <IMHelpers\IMHelpers.h>

#include <rEngine\rObject.h>

tinyxml2::XMLDocument doc;

class ScreenShotFirstScreen : public rObject{
	bool first_;
	int test;
	std::string filename;
public:
	ScreenShotFirstScreen(std::string filename){ 
		first_ = true;
		this->filename = filename;
		LOG_INFO("ScreenShotFirstScreen created, a screenshot will be taken after first screen is rendered");
	}
	virtual void postDraw(Scene *s){
		if(first_){
			chkGLErr();
			first_ = false;
			s->screenshot(filename.c_str());
			chkGLErr();
		}
	}
};


class DieAfterFirstScreen : public rObject{
	bool first_;
public:
	DieAfterFirstScreen(){ 
		first_ = true;
		LOG_INFO("DieAfterFirstScreen created, Applcation will be closed after first screen has been rendered");
	}
	virtual void postDraw(Scene *s){
		if(first_){// Actually, close on second, to make sure everything has been rendered
			first_ = false;
			return;
		}
		LOG_INFO("Exiting application");
		exit(0);
		
	}
};

void loadXML(rEngine *t,std::string filename){
	doc.LoadFile(filename.c_str());

	auto firstChild = doc.FirstChildElement("rEngine");
	if(firstChild){
		bool windowCreated = false;
        FOR_XML_ELE("loging",firstChild,l){
            auto manager = LogManager::getLogManager();
            auto eConsole = l->FirstChildElement("console");
            auto eFile = l->FirstChildElement("file");
            if(eConsole){
                auto logger = new ConsolLogger();
                auto lvl = eConsole->Attribute("logLevel");
                auto timmer = eConsole->Attribute("useTimmers");
                if(lvl && strcmp(lvl,"debug") == 0) logger->setMinLogLevel(Debug);
                else if(lvl && strcmp(lvl,"info") == 0) logger->setMinLogLevel(Info);
                else if(lvl && strcmp(lvl,"warning") == 0) logger->setMinLogLevel(Warning);
                else if(lvl && strcmp(lvl,"error") == 0) logger->setMinLogLevel(Error);

                logger->setUseTimmers(timmer && strcmp(timmer,"on") == 0);
                manager->attatchLogger(logger);
            }
            if(eFile){
                auto filename = eFile->Attribute("file");
                if(filename){
                    auto logger = FileLogger::CreateLogger(filename);
                    if(logger){
                        auto lvl = eFile->Attribute("logLevel");
                        auto timmer = eFile->Attribute("useTimmers");
                        if(lvl && strcmp(lvl,"debug") == 0) logger->setMinLogLevel(Debug);
                        else if(lvl && strcmp(lvl,"info") == 0) logger->setMinLogLevel(Info);
                        else if(lvl && strcmp(lvl,"warning") == 0) logger->setMinLogLevel(Warning);
                        else if(lvl && strcmp(lvl,"error") == 0) logger->setMinLogLevel(Error);

                        logger->setUseTimmers(timmer && strcmp(timmer,"on") == 0);
                        manager->attatchLogger(logger);
                    }
                }
            }

            LOG_INFO("Logger initiated");
            //break;
        }
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
			auto s = Scene::CreateScene(scene);
			t->addScene(s);
			FOR_XML_ELE("ScreenShotFirstScreen",scene,e){
				auto src = e->Attribute("file");
				if(src)
					s->addObject(new ScreenShotFirstScreen(src));
				break;
			}
			FOR_XML_ELE("DieAfterFirstScreen",scene,e){
				s->addObject(new DieAfterFirstScreen());
				break;
			}
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
	LOG_INFO("XML File read completed " << filename);
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);                      
	glClearDepth (1.0f);                                        
	glEnable (GL_DEPTH_TEST); 
	glEnable(GL_TEXTURE_2D);                                
	
	//exit(0);
}

rENGINE_MAIN(init)