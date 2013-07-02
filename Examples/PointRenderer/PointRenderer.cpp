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

#include <RBF/RBFHelper.h>

#include <Geometry\KDTree\KDTree.h>

class PointRenderer : public DrawableObject{
	std::vector<Vertex> points;
	std::vector<glm::vec3> surfPoints;
	K3DTree<Vertex> tree;

	IndexedMesh *mesh;
	IndexedMesh *rbfMesh;



public:
	PointRenderer(){
		mesh = static_cast<IndexedMesh*>(Mesh::LoadWavefront<IndexedMesh>(MODELS_DIR "/cow.obj"));
		float area = mesh->getArea();

		int numPoints = 20000;

		auto triangles = mesh->getFaces();
		for(auto tri = triangles.begin();tri!=triangles.end();++tri){
			float p = numPoints * (tri->area() / area);
			int points = std::floorf(p + 0.5);
			for(int i = 0;i<points;i++){
				auto p = Random::getRandomGenerator()->inTriangle(*tri);
				surfPoints.push_back(p);
				//tree.insert(p,Vertex(p));
			}
		}


		RBFHelper rbfHelper;
		rbfHelper.setPoints(surfPoints);
		
		rbfHelper.hints.minPointsForIterativeFitting = 0;
		rbfHelper.hints.useCentersReduction = !true;
		rbfHelper.hints.centerReductionStartWith = 0.3;
		
		rbfHelper.hints.fastFitMinInnerSize = 2700;
		rbfHelper.hints.fastFitOuterSize = 0.01;
		rbfHelper.hints.minPointsForIterativeFitting = 0.001;
		rbfHelper.hints.fastFitCoarseGridSize = 220;


		rbfHelper.fit<GausianRBF>();
		rbfMesh = rbfHelper.getMesh<IndexedMesh>(40);

		std::cout << rbfMesh->getArea()  << " " << rbfMesh->getFaces().size() << " " << rbfMesh->getNumVertices() << " " << std::endl;

		/*for(auto p = tree.begin() ; p != tree.end(); ++p){
			auto nodes = tree.findNNearest(p->getPosition(),15);
			std::vector<glm::vec3> closePoints;
			

			IT_FOR(nodes,node){
				closePoints.push_back(glm::vec3((*node)->getPosition()[0],(*node)->getPosition()[1],(*node)->getPosition()[2]));
			}
			Plane plane(closePoints);
			p->getDataAsPointer()->setNormal(plane.getNormal());
			
		}*/

	}


	virtual void draw(){
		rbfMesh->draw();

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

		/*
		glBegin(GL_POINTS);
		
		for(auto p = tree.begin() ; p != tree.end(); ++p){
			glVertex3fv(p->getPosition());
		}
		glEnd();
		glBegin(GL_LINES);
		float normLength = 0.01;
		for(auto p = tree.begin() ; p != tree.end(); ++p){
			auto p0 = glm::vec3(p->get().getPosition());
			auto n = p->get().getNormal();
			auto p1 = p0 + normLength * n;
			
			glColor3f(1,0,0);
			glVertex3fv(glm::value_ptr(p0));
			
			glColor3f(0,0,1);
			glVertex3fv(glm::value_ptr(p1));
		}
		
		glEnd();*/
	}


	virtual std::string toString()const{return "PointRenderer";}
};




void init(int argc,char **argv,Engine *t){
	try{
		static_cast<SuperGraphicsEngine*>(SuperEngine::getEngine()->getGraphicEngine())->addDrawableObject(new PointRenderer());
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