#include <fstream>
#include <omp.h>


#include <Engine\DrawableObject.h>

#include <SuperEngine\SuperEngine.h>

#include <Util/Macros.h>
#include <Util/StopClock.h>
#include <Util/Random.h>

#include <gl/glfw.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Geometry\Clustering\Clusterer.h>

#include <string>
#include <iostream>

#include <Math\Plane.h>

#include <Base\XMLObjectHandler.h>

#include <Geometry\Mesh\IndexedMesh.h>

#include <Geometry\CSG\PointCloudInterpolation.h>
#include <Geometry\CSG\MarchingTetrahedra.h>

#include <Geometry\KDTree\KDTree.h>

class Renderer : public DrawableObject{
	std::vector<Vertex> points;
	std::vector<glm::vec3> surfPoints;
	K3DTree<Vertex> tree;

	IndexedMesh *mesh;
	
public:
	Renderer(){
		mesh = 0;
		auto modelMesh = static_cast<IndexedMesh*>(Mesh::LoadWavefront<IndexedMesh>(MODELS_DIR "/suzanne.obj"));
		float area = modelMesh->getArea();

		int numPoints = 2000;
		
		K3DTree<glm::vec3> pointCloud;
		glm::vec3 minP = modelMesh->getBoundingAABB().minPos();
		glm::vec3 maxP = modelMesh->getBoundingAABB().maxPos();
		minP.x = std::min(std::min(minP.x,minP.y),minP.z);
		maxP.x = std::max(std::max(maxP.x,maxP.y),maxP.z);
		minP.z = minP.y = minP.x;
		maxP.z = maxP.y = maxP.x;
		auto triangles = modelMesh->getFaces();
		for(auto tri = triangles.begin();tri!=triangles.end();++tri){
			float p = numPoints * (tri->area() / area);
			int points = std::floorf(p + 0.5);
			for(int i = 0;i<points;i++){
				auto p = Random::getRandomGenerator()->inTriangle(*tri);
				p -= minP;
				p /= (maxP-minP);
				surfPoints.push_back(p);
				pointCloud.insert(p,glm::vec3(0,0,0));
			}
		}

		std::vector<glm::vec3> closePoints;
		IT_FOR(pointCloud,point){
			auto nodes = pointCloud.findNNearest(point->getPosition(),10);
			closePoints.clear();
			IT_FOR(nodes,p){
				closePoints.push_back(glm::vec3((*p)->getPosition()[0],(*p)->getPosition()[1],(*p)->getPosition()[2]));
			}
			Plane plane(closePoints);
			point->get() = plane.getNormal();
		}

		
		//PointCloudInterpolation csg(pointCloud);

		/*CSGSphere s0(0.2,glm::vec3(0.5,0.5,0.5));
		CSGSphere s1(0.1,glm::vec3(0.7,0.5,0.5));
		CSGSphere s2(0.1,glm::vec3(0.3,0.5,0.5));
		
		CSGSphere s3(0.1,glm::vec3(0.5,0.7,0.5));
		CSGSphere s4(0.1,glm::vec3(0.5,0.3,0.5));
		
		CSGSphere s5(0.1,glm::vec3(0.5,0.5,0.7));
		CSGSphere s6(0.1,glm::vec3(0.5,0.5,0.3));
		
		CSGCombine  c0(&s1,&s2,Union);
		CSGCombine  c1(&s3,&s4,Union);
		CSGCombine  c2(&s5,&s6,Union);
		CSGCombine  c3(&c0,&c1,Union);
		CSGCombine  c4(&c2,&c3,Union);
		CSGCombine csg(&c4,&s0,Union);*/

		glm::vec3 _min(0,0,0),_max(1,1,1);
		int dim = 80;
		//auto m = MarchingTetrahedra::March<IndexedMesh>(&csg,dim,pointCloud);
		//mesh = dynamic_cast<IndexedMesh*>(m);
	}


	virtual void draw(){
		glTranslatef(-0.5,-0.5,-0.5);
		if(mesh)
			mesh->draw();
		if(mesh)
			return;
		glPointSize(2);
		glBegin(GL_POINTS);
		glColor3f(1,1,1);
		for(auto p = points.begin();p!=points.end();++p){
			glVertex3fv(glm::value_ptr(p->getPosition()));
		}
		glColor3f(1,0,0);
		for(auto p = surfPoints.begin();p!=surfPoints.end();++p){
			glVertex3fv(glm::value_ptr(*p));
		}
		glEnd();
	}


	virtual std::string toString()const{return "Renderer";}
};




void init(int argc,char **argv,Engine *t){
	try{
		static_cast<SuperGraphicsEngine*>(SuperEngine::getEngine()->getGraphicEngine())->addDrawableObject(new Renderer());
	}catch(...){
		exit(1);
	}
	if(argc == 2)
		exit(0);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 10.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat light_color[] = { 0.8, 0.8, 1.0 , 1.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE , light_color);
	glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
	glEnable ( GL_COLOR_MATERIAL ) ;

}

ENGINE_MAIN(SuperEngine::CreateEngine(),init)