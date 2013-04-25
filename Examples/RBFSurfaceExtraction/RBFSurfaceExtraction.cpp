
#include <Base\XMLObjectHandler.h>

#include <SuperEngine\SuperEngine.h>
#include <SuperEngine\MeshRenderer.h>

#include <iostream>

#include <Geometry\Mesh\IndexedMesh.h>
#include <Geometry\CSG\MarchingTetrahedra.h>

#include <RBF\RBF.h>


void init(int argc,char **argv,Engine *t){
	if(argc!=3){
		std::cout << "Usage: " << argv[0] << " RBF.xml MESH_RESOLUTION";
		std::cout << "\tMESH_RESOLUTION = a positive integer defining the resolution of the mesh" << std::endl;
		exit(1);
	}
	std::vector<XMLObject*> objects;
	XMLObjectHandler::Handler()->read(objects,std::string(argv[1]));

	RBFSystem *rbf = 0;
	for(int i = 0;i<objects.size()&&rbf==0;i++){
		rbf = dynamic_cast<RBFSystem*>(objects[i]);	
	}
	if(rbf == 0){
		std::cout << "XML File " << argv[1] << "did not contain any RBF system defenitions" << std::endl;
		exit(2);
	}
	int meshRes = std::atoi(argv[2]);
	Mesh *mesh;
	{
		ScopeClock s("Mesh extracted: ");
		mesh = MarchingTetrahedra::March<IndexedMesh> (rbf,meshRes);
	}
	MeshRenderer *meshRenderer = new MeshRenderer();
	meshRenderer->buildFromMesh(mesh);
	static_cast<SuperGraphicsEngine*>(SuperEngine::getEngine()->getGraphicEngine())->addDrawableObject(meshRenderer);
}

ENGINE_MAIN(SuperEngine::CreateEngine(),init)