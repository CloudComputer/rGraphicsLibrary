#include <Engine\DrawableObject.h>

#include <SuperEngine\SuperEngine.h>
#include <SuperEngine\MeshRenderer.h>

#include <iostream>

#include <Fractals\MengerSponge.h>

class TestObject{
	MengerSponge sponge;
	IndexedMesh *mesh;
	MeshRenderer *meshRender;
public:
	TestObject():sponge(false,2,false){
		mesh = new IndexedMesh();
		sponge.toMesh(mesh);
		std::cout << mesh->getFaces().size() << std::endl;
		mesh->removeDoubleTriangles(true);
		std::cout << mesh->getFaces().size() << std::endl;

		meshRender = new MeshRenderer();
		meshRender->buildFromMesh(mesh,false);
	}


	MeshRenderer *getRenderer(){return meshRender;}

	virtual void draw(){mesh->draw();}
	virtual std::string toString()const{return "TestObject";}
};




void init(int argc,char **argv,Engine *t){
	TestObject *test = new TestObject();
	static_cast<SuperGraphicsEngine*>(SuperEngine::getEngine()->getGraphicEngine())->addDrawableObject(test->getRenderer());
}

ENGINE_MAIN(SuperEngine::CreateEngine(),init)