#include <fstream>;
#include <omp.h>

#include <OpenGLHelpers/OpenGLInfo.h>
#include <Util/StopClock.h>

#include <gl/glfw.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Image\Image.h>
#include <Image\BMPWriter.h>

#include <Geometry\Clustering\Clusterer.h>

#include <string>
#include <iostream>

#include <Geometry\Mesh\IndexedMesh.h>

#include <Physics\SpringDamperSystem.h>

#include <Geometry\CSG\MarchingTetrahedra.h>

#include <Volume\ScalarField.h>

#include <Math\Plane.h>

#include "RBF\RBF.h"

struct SourceFile{
	char *path;
	int w,h,d;
};

const SourceFile files[] = {
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000013-orig_SC-467x255x124.raw",467,255,124}, //0
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000013-ENH-1_SC-467x255x124.raw",467,255,124}, //1
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000013-ENH-2_SC-467x255x124.raw",467,255,124}, //2
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000013-ENH-3_SC-467x255x124.raw",467,255,124}, //3

	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000016-orig_SC-478x247x79x22.raw",478,247,79}, //4
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000016-ENH-1_SC-478x247x79x22.raw",478,247,79}, //5
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000016-ENH-2_SC-478x247x79x22.raw",478,247,79}, //6
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000016-ENH-3_SC-478x247x79x22.raw",478,247,79}, //7

	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000030_orig_SC-679x307x174.raw",679,307,174}, //8
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000030_ENH-1_SC-679x307x174.raw",679,307,174}, //9
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000030_ENH-2_SC-679x307x174.raw",679,307,174}, //10
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000030_ENH-3_SC-679x307x174.raw",679,307,174}, //11

	{"C:/Thesis Data/ContextVision/new volume/I0000027_SC--467x255x101.raw",467,255,101}, //12
	{"C:/Thesis Data/ContextVision/new volume/I0000027_TEMP-1-ENH-3-467x255x101.raw",467,255,101}, //13
	{"C:/Thesis Data/ContextVision/new volume/I0000027_TEMP-1-ENH-3_ENH-1-467x255x101.raw",467,255,101}, //14

	{"C:/Thesis Data/ContextVision/2012-12-10/I0000027-enh3_SC-fLOOD-sc-0-516x244x124.raw",516,244,124}, //15
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000027-enh3_SC-fLOOD-sc-0-n516x244x124.raw",516,244,124}, //16
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000027-enh3_SC-fLOOD-sc-0_2-516x244x124.raw",516,244,124}, //17
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000027-enh3_SC-fLOOD-sc-0_3-516x244x124.raw",516,244,124}, //18
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000027-enh3_SC-fLOOD-sc-0_4-516x244x124.raw",516,244,124}, //19
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000027_SC516x244x124.raw",516,244,124}, //20

	{"C:/Thesis Data/ContextVision/2012-12-10/I0000029-ENH3_SC-fLOOD-sc-1-688x404x124.raw",688,404,124}, //21
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000029-ENH3_SC-fLOOD-sc-1-n688x404x124.raw",688,404,124}, //22
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000029-ENH3_SC-fLOOD-sc-1_2-688x404x124.raw",688,404,124}, //23
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000029-ENH3_SC-fLOOD-sc-1_3-688x404x124.raw",688,404,124}, //24
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000029-ENH3_SC-fLOOD-sc-1_4-688x404x124.raw",688,404,124}, //25
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000029_SC-688x404x124.raw",688,404,124}, //26

	{"C:/Thesis Data/ContextVision/2012-12-10/I0000042-ENH3_SC-fLOOD-sc-0-516x244x124.raw",516,244,124}, //27
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000042-ENH3_SC-fLOOD-sc-0-n516x244x124.raw",516,244,124}, //28
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000042-ENH3_SC-fLOOD-sc-0_2-516x244x124.raw",516,244,124}, //29
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000042-ENH3_SC-fLOOD-sc-0_3-516x244x124.raw",516,244,124}, //30
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000042-ENH3_SC-fLOOD-sc-0_4-516x244x124.raw",516,244,124}, //31
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000042_SC516x244x124.raw",516,244,124}, //32

	{"C:/Thesis Data/ContextVision/2012-12-10/I0000047-ENH3_SC-fLOOD-sc-2-624x327x124.raw" ,624,327,124}, //33
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000047-ENH3_SC-fLOOD-sc-2-n624x327x124.raw",624,327,124}, //34
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000047-ENH3_SC-fLOOD-sc-2_2-624x327x124.raw",624,327,124}, //35
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000047-ENH3_SC-fLOOD-sc-2_3-624x327x124.raw",624,327,124}, //36
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000047_SC624x327x124.raw",624,327,124}, //37

	{"C:/Thesis Data/ContextVision/2012-12-10/I0000048-ENH3_SC-fLOOD-sc-3-n645x432x124.raw",645,432,124}, //38
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000048-ENH3_SC-fLOOD-sc-3_2_645x432x124.raw",645,432,124}, //39
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000048-ENH3_SC-fLOOD-sc-3_3_645x432x124.raw",645,432,124}, //40
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000048-ENH3_SC-fLOOD-sc-3_4_645x432x124.raw",645,432,124}, //41
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000048-ENH3_SC-fLOOD-sc-3_645x432x124.raw",645,432,124}, //42
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000048_SC645x432x124.raw",645,432,124} //43

};
const unsigned int numDatasets = 44;

PointCluster *cluster;
std::vector<glm::vec4> surfacePoints;
	
int niceCases[]= {3,7,11,14,19,25,31,42};
const int numNiceCases = 8;
int _case = 3;

StopClock s(true);

glm::ivec2 winSize;

//float threshold = 98.0/255;
float threshold = 0.4;

ScalarField *vol = 0;
KDTree<Vertex,3,float>* points;
std::vector<PointCluster> clusters;

bool mouse0State = false;
float ry = -90,rx = 90,scale = 1.5;


RBFSystem *rbfs[6];
Mesh *meshes[] = {0,0,0,0,0,0};
unsigned int currentMesh = 0;
int meshRes = 50;

BoundingAABB aabb(glm::vec3(0,0,0),glm::vec3(0,0,0));

void draw(){
	chkGLErr();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::lookAt(glm::vec3(0,0,3),glm::vec3(0,0,0),glm::vec3(0,1,0))));
	glRotatef(ry,1,0,0);
	glRotatef(rx,0,1,0);
	glScalef(scale,scale,scale);

	glColor3f(1,1,1);
	aabb.draw();


	//KDTree<Vertex,3,float>::NodeIterator point;
	////std::vector<PointCluster>::iterator cluster;
	//srand(0);
	////for(cluster = clusters.begin();cluster != clusters.end();++cluster){
	//	float r,g,b;
	//	r = rand() / float(RAND_MAX);
	//	g = rand() / float(RAND_MAX);
	//	b = rand() / float(RAND_MAX);
	//	r = (r + 0.5) / 1.5;
	//	g = (g + 0.5) / 1.5;
	//	b = (b + 0.5) / 1.5;
	//	glColor3f(r,g,b);
	//	glBegin(GL_POINTS);
	//	/*for(point = cluster->getPoints()->begin();point != cluster->getPoints()->end();++point){
	//		glVertex3fv(point->getPosition());
	//	}*/
	//	glEnd();
	////}

	//float s = 1;
	//glBegin(GL_LINES);
	//glColor3f(1,0,0);
	//glVertex3f(0,0,0);
	//glVertex3f(s,0,0);
	//
	//glColor3f(0,1,0);
	//glVertex3f(0,0,0);
	//glVertex3f(0,s,0);
	//
	//glColor3f(0,0,1);
	//glVertex3f(0,0,0);
	//glVertex3f(0,0,s);


	//glEnd();

	
	glColor3f(1,1,1);
	if(meshes[currentMesh]!=0)
		static_cast<IndexedMesh*>(meshes[currentMesh])->draw();

	/*glPointSize(1);
	glDisable(GL_LIGHTING);
	glBegin(GL_POINTS);
	for(auto p = surfacePoints.begin();p != surfacePoints.end();++p){
		glColor3f(0,0,1);
		if(p->w < -0.00001)
			glColor3f(1,0,0);
		else if(p->w > 0.000001)
			glColor3f(0,1,0);
		glVertex3f(p->x,p->y,p->z);
	}
	glEnd();*/
	glEnable(GL_LIGHTING);


	glfwSwapBuffers();
	chkGLErr();
}

void creatMesh(int );

void loadPoints(){
	if(vol != 0)
		delete vol;
	StopClock sw;
	sw.start();
	
	//vol = ScalarField::ReadFromRawfile(files[niceCases[_case]].path,files[niceCases[_case]].w,files[niceCases[_case]].h,files[niceCases[_case]].d);
	TmpPointer<ScalarField> tmp = ScalarField::ReadFromRawfile(files[niceCases[_case]].path,files[niceCases[_case]].w,files[niceCases[_case]].h,files[niceCases[_case]].d);
	////TmpPointer<ScalarField> tm2 = tmp->blur();
	vol = tmp->blur();	
	

	std::cout << "Dataset loaded " << files[niceCases[_case]].path << ": " << sw.getFractionElapsedSeconds() << " seconds" << std::endl;
	sw.restart();

	//vol = tmp;

	points = vol->getSurfacePoints(threshold);
	std::cout << "Points extracted: " << points->size() << " number of points, " << sw.getFractionElapsedSeconds() << " seconds" << std::endl;
	sw.restart();

	glm::vec3 dir = vol->getBoundingAABB().getPosition(glm::vec3(1,1,1)) - vol->getBoundingAABB().getPosition(glm::vec3(0,0,0));
	dir /= vol->getDimensions();

	clusters = Clusterer::ClusterPoints(points,glm::length(dir) * 1,100);
	std::cout << "Points clustered: " << sw.getFractionElapsedSeconds() << " seconds" << std::endl;
	sw.restart();

	unsigned long _max = 0;
	std::cout << "Created " << clusters.size() << " clusters" << std::endl;
	for(int i = 0;i<clusters.size();i++){
		unsigned long size = clusters[i].getPoints()->size();
		std::cout << '\t' << "Cluster " << i << ": " << size << " points" << std::endl;
		if(size>_max){
			_max = size;
			cluster = &clusters[i];
		}
	}

	for(auto node = cluster->getPoints()->begin(); node != cluster->getPoints()->end() ; ++node){
		auto close = cluster->getPoints()->findNNearest(node->getPosition(),20);
		std::vector<glm::vec3> poses;
		for(auto c = close.begin(); c!=close.end();++c){
			poses.push_back(glm::vec3((*c)->getPosition()[0],(*c)->getPosition()[1],(*c)->getPosition()[2]));
		}
		Plane p(poses);
		node->get().setNormal(p.getNormal());
	}

	auto points = cluster->getPoints()->getAsVector();

	//points.clear();
	//float dist = 0.017;
	////dist = 0.019;
	//float p[] = {0,-1,0};
	//while(!cluster->getPoints()->empty()){
	//	auto node = cluster->getPoints()->findNearest(p);
	//	points.push_back(node->get());
	////	cluster->getPoints()->erase(node);
	//	auto nodes = cluster->getPoints()->findCloseTo(node->getPosition(),dist);
	//	for(auto n = nodes.begin();n!=nodes.end();++n){
	//		cluster->getPoints()->erase(*n);
	//	}
	//}
	
	
	//return;

	int s = points.size();
	
	//glm::vec3 v(0,-1,0);
	//for(int i = s-1;i>=0;i--){
	//	if(std::abs(points[i].getPosition().z) > 0.18 ||  std::abs(points[i].getPosition().x) > 0.3){
	//		points.erase(points.begin()+i);
	//	}
	//}
//*/


	s = points.size();
	float d = 0.01;
	int inc = 0.5 + (3.0f*s / 4000);
	std::cout << inc << std::endl;
	//inc = 1;
	surfacePoints.clear();
	//for(int i = 0;i<s;i += inc){
	for(int i = 0;i<points.size();i++){
		glm::vec4 p0,p1,p2,p3,p4;
		
		glm::vec3 n = points[i].getNormal();
		if(!(n.x == n.x))
			continue;
		glm::vec3 p = glm::vec3(points[i].getPosition());
		
		p0 = glm::vec4(p + (n * d), d);
		p1 = glm::vec4(p - (n * d),-d);
		p2 = glm::vec4(p,0);

		/*p3 = glm::vec4(p + n * (2*d), 2*d);
		p4 = glm::vec4(p - n * (2*d),-2*d);
*/


		if(i%2==0)
			surfacePoints.push_back(p0);
		else 
			surfacePoints.push_back(p1);
		surfacePoints.push_back(p2);
		/*surfacePoints.push_back(p3);
		surfacePoints.push_back(p4);*/
		
		
		aabb.minPos().x = std::min(aabb.minPos().x,p0.x);
		aabb.minPos().x = std::min(aabb.minPos().x,p1.x);
		aabb.minPos().y = std::min(aabb.minPos().y,p0.y);
		aabb.minPos().y = std::min(aabb.minPos().y,p1.y);
		aabb.minPos().z = std::min(aabb.minPos().z,p0.z);
		aabb.minPos().z = std::min(aabb.minPos().z,p1.z);
		
		aabb.maxPos().x = std::max(aabb.maxPos().x,p0.x);
		aabb.maxPos().x = std::max(aabb.maxPos().x,p1.x);
		aabb.maxPos().y = std::max(aabb.maxPos().y,p0.y);
		aabb.maxPos().y = std::max(aabb.maxPos().y,p1.y);
		aabb.maxPos().z = std::max(aabb.maxPos().z,p0.z);
		aabb.maxPos().z = std::max(aabb.maxPos().z,p1.z);

	}
	std::cout << surfacePoints.size() << std::endl;

}


float acc = 0.4*10e-5;
float smoothNess = 0.75;
int minInnerSize = 4000;
float outerSize = 0.01f;
int coarseGridSize = 30;
int maxIterations = 40;

void creatMesh(int id){
	//return;
	StopClock s1,s2;
	s1.start();
	float w = 0.5;
	//w = 0.01;
	w = id * 0.1 + 0.05;
	rbfs[id] = RBFSystem::FastFitting<Biharmonic>(surfacePoints,smoothNess,acc,minInnerSize,outerSize,coarseGridSize,maxIterations);
	/*switch(id){
	case 0:
		rbfs[id] =  RBFSystem::CreateFromPoints<Biharmonic>(surfacePoints,w);
		break;
	case 1:
		rbfs[id] =  RBFSystem::CreateFromPoints<Biharmonic>(surfacePoints,w);
		break;
	case 2:
		rbfs[id] =  RBFSystem::CreateFromPoints<Biharmonic>(surfacePoints,w);
		break;
	case 3:
		rbfs[id] =  RBFSystem::CreateFromPoints<Biharmonic>(surfacePoints,w);
		break;
	case 4:
		rbfs[id] =  RBFSystem::CreateFromPoints<Biharmonic>(surfacePoints,w);
		break;
	case 5:
		rbfs[id] =  RBFSystem::CreateFromPoints<ThinPlateSplineRBF>(surfacePoints,w);
		break;
	}*/
	s1.stop();
	s2.restart();
	CSGCache ccc(rbfs[id]);
	std::cout << "Mesh created: " << id << ", Fitting  time: " << s1.getFractionElapsedSeconds() << " sec";
	meshes[id] = MarchingTetrahedra::March<IndexedMesh>(&ccc,aabb,glm::ivec3(meshRes,meshRes,meshRes));

	std::cout << ", Eval/Surface extract time: " << s2.getFractionElapsedSeconds()   << " sec" << std::endl;
}

int prevScroll = 0;
void wheel(int i){
	scale *= 1+((i-prevScroll)*0.01);
	threshold += i/255.0;
	//loadPoints();

	prevScroll = i;
}

void mouseButton(int button,int state,int x,int y){
if(button == 0)
	mouse0State = state;
}

int mouseX = -1,mouseY;
void mouseMotion(int x,int y){
	int dx,dy;
	if(mouseX == -1){
		mouseX = x;
		mouseY = y;
	}
	dx = x - mouseX;
	dy = y - mouseY;
	mouseX = x;
	mouseY = y;
	if(mouse0State){
		rx += dx * 1;
		ry += dy * 1;
	}
}

std::string datePan(int N,int a){
	std::stringstream ss;
	for(int i = a-1;a>=0;a--)
		if(N<std::powf(10,a))
			ss << "0";
	ss << N;
	return ss.str();
}

void keyboard(int button,int state){
	if(button == 'N' && state){
		_case++;
		if(_case>=numNiceCases){
			_case = 0;
		}
		loadPoints();
	}
	if(button == 'M' && state){
		_case--;
		if(_case<0){
			_case = numNiceCases-1;
		}
		loadPoints();
	}
	if(button == 'P' && state){
		time_t t = time(0);   // get time now
		struct tm * now = localtime( & t );
		std::stringstream ss;
		int day = now->tm_hour;
		int h = now->tm_mday;
		int min = now->tm_mday;
		int y = now->tm_mday;
		ss << "Screenshot_";
		ss << datePan(now->tm_year + 1900,4) << "-" << datePan(now->tm_mon +1,2) << "-" << datePan(day,2) << day; //data
		ss << "_";
		ss << datePan(h,2) << "-" << datePan(min,2) << "-" << datePan(now->tm_sec,2);
		ss << ".jpg";
		Image::TakeScreenShot(ss.str().c_str());
	}

	if(button == '1' && state){
		currentMesh = 0;
	}
	if(button == '2' && state){
		currentMesh = 1;
	}
	if(button == '3' && state){
		currentMesh = 2;
	}
	if(button == '4' && state){
		currentMesh = 3;
	}
	if(button == '5' && state){
		currentMesh = 4;
	}
	if(button == '6' && state){
		currentMesh = 5;
	}
}

void resize(int width,int height){
	chkGLErr();
	glMatrixMode(GL_PROJECTION);
	chkGLErr();
	
	winSize.x = width;
	winSize.y = height;
	float ar = width;
	ar /= height;
	glLoadMatrixf(glm::value_ptr(glm::perspective(45.0f, ar,0.1f,100.0f)));
	chkGLErr();
	

	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,width,height);
	chkGLErr();
}


void init(){
	chkGLErr();
	glClearColor(0,0,0,0);

	
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

	
	loadPoints();
	
	chkGLErr();
}

glm::vec3 col(float v){
	if(v<-1) return glm::vec3(1,0,0);
	if(v>1) return glm::vec3(0,1,0);
	
	if(v==0) return glm::vec3(1,1,1);
	if(v<0) return glm::vec3(1-(1+v),1+v,1+v);
	return glm::vec3(1-v,v,1-v);
	

}

void createImages(){
	return;
	std::cout << "Creating images" << std::endl;
	int w = 1024,h;
	glm::vec3 tmp = aabb.maxPos() - aabb.minPos();
	float ar = tmp.x / tmp.z;
	h = w * ar;
	float y = aabb.maxPos().y + aabb.minPos().y;
	y *= 0.5;
	//float *data = new float[w*h];
	for(int img = 0;img<5;img++){
		BMPWriter bmp(w,h);
		float mm = 999999,mx=-999999;
		for(int a = 0;a<w;a++)for(int b = 0;b<h;b++){
			float x = a;
			float z = b;
			x /= w;
			z /= h;
			float v = rbfs[img]->eval(glm::vec3(x,y,z));
			bmp.setPixel(a,b,col(v));
			//data[a+b*w] = v;
			if(v<mm) mm = v;
			if(v>mx) mx = v;
		}
		std::stringstream filename;
		filename << "img" << img << ".bmp" << " " << mm << " " << mx << std::endl;
		bmp.save(filename.str().c_str());

		std::cout << "Created bmp " << filename.str() << std::endl;
	}
	std::cout << "Images created" << std::endl;
}

char *filename;
void takeScreen(){
	Image::TakeScreenShot(filename);
}

struct __counter{
	int c;
};
//
//float acc = 0.4*10e-5;
//float smoothNess = 0.75;
//int minInnerSize = 4000;
//float outerSize = 0.01f;
//int coarseGridSize = 30;
//int maxIterations = 40;

int main( int argc, char* argv[] )
{
	if(argc>=2) acc = atof(argv[1]);
	if(argc>=3) smoothNess = atof(argv[2]);
	if(argc>=4) minInnerSize = atoi(argv[3]);
	if(argc>=5) outerSize = atof(argv[4]);
	if(argc>=6) coarseGridSize = atoi(argv[5]);
	if(argc>=7) maxIterations = atoi(argv[6]);
	
	if(argc>=8) filename = argv[7];
	else filename = "unnamed.jpg";
	if (glfwInit() != GL_TRUE){
		std::cout << "Could not init glfw"<< std::endl;
		return 2;
	}

	if (glfwOpenWindow(600, 600, 8, 8, 8, 8, 32, 0, GLFW_WINDOW) != GL_TRUE){
		std::cout << "Could not create window"<< std::endl;
		return 3;
	}
	glfwSetWindowTitle ("rGraphicsLibrary - Ultrsound Surface Extraction");

	if(glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to init glew" << std::endl;
		exit(1);
	}
	chkGLErr();
	std::cout << "OpenGL Version: " << OpenGLInfo::getOpenGLVersion() << std::endl;
	std::cout << "Glew   Version: " << OpenGLInfo::getGlewVersion()   << std::endl;
	std::cout << "GLSL   Version: " << OpenGLInfo::getGLSLVersion()   << std::endl;
	std::cout << "OpenGL Vendor: "  << OpenGLInfo::getOpenGLVendor()   << std::endl;
	std::cout << "OpenGL Renderer: "<< OpenGLInfo::getOpenGLRenderer()   << std::endl;
	chkGLErr();

	double f[3] = {2,1,-1};
	Polynomial<> p(2,f);
	double d = p.evaluate(1);
	double d2 = p.evaluate(2);

	init();

	glfwSetWindowSizeCallback(GLFWwindowsizefun(resize));

	glfwSetKeyCallback(GLFWkeyfun(keyboard));

	glfwSetMouseButtonCallback(GLFWmousebuttonfun(mouseButton));
	glfwSetMousePosCallback(GLFWmouseposfun(mouseMotion));
	glfwSetMouseWheelCallback(GLFWmousewheelfun(wheel));

	__counter *c = new __counter;
	c->c = 5;
		draw();
	creatMesh(0);
		draw();
	//creatMesh(1);
	//	draw();
	//creatMesh(2);
	//creatMesh(3);
	//creatMesh(4);
	while(true){
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
			break;
		draw();
		//std::cout << c->c << std::endl;
		takeScreen();
		break;
	}	

	//int meshes = 5;
	//#pragma omp parallel num_threads(6) shared(c)
	//{
	//	if(omp_get_thread_num()!=0){
	//		creatMesh(omp_get_thread_num()-1);
	//		c->c--;
	//		if(c->c  == 0){
	//			createImages();
	//		}
	//	}else{
	//		while(true){
	//			if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
	//				break;
	//			draw();
	//			//std::cout << c->c << std::endl;
	//		}	
	//	}
	//}


	
	return 0;
}
