#include <rEngine\rEngine.h>

#include <rEngine\Viewport.h>

#include <iostream>

#include <rEngine\Texture.h>

#include <tinyxml2\tinyxml2.h>
#include <Util\Directory.h>

#include <Util\Macros.h>
#include <Util\Logger.h>

tinyxml2::XMLDocument doc;

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