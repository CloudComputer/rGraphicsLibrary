#define RBF_DEBUG

//#include "C:\Program Files (x86)\Visual Leak Detector\include\vld.h"

#include <Util\Directory.h>

#include <fstream>
#include <omp.h>

#include <OpenGLHelpers/OpenGLInfo.h>
#include <Util/StopClock.h>

#include <gl/glfw.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Image\Image.h>

#include <string>
#include <iostream>

#include <Geometry\Mesh\IndexedMesh.h>

#include <Geometry\CSG\MarchingTetrahedra.h>

#include <Volume\ScalarField.h>


#include <Volume\UltrasoundSurfacePointExtractor.h>
#include <Volume\UltrasoundVariationalClassification.h>

#include <Util\HTML.h>

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

std::vector<glm::vec4> surfacePoints;
	
int niceCases[]= {3,7,11,14,19,25,31,42};
const int numNiceCases = 8;
int _case = 3;

glm::ivec2 winSize;

bool mouse0State = false;
float ry = -90,rx = 90,scale = 1.5;

Mesh *mesh = 0;
int meshRes = 100;


bool runTests();
std::string test(ScalarField *v,const char *name,float alpha,float beta,float w,float iso,float uind,float xi,float threshold);

void draw(){
	chkGLErr();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::lookAt(glm::vec3(0,0,3),glm::vec3(0,0,0),glm::vec3(0,1,0))));
	glRotatef(ry,1,0,0);
	glRotatef(rx,0,1,0);
	glScalef(scale,scale,scale);
	
	glColor3f(1,1,1);
	if(mesh!=0)
		static_cast<IndexedMesh*>(mesh)->draw();

	glfwSwapBuffers();
	chkGLErr();
}


int prevScroll = 0;
void wheel(int i){
	scale *= 1+((i-prevScroll)*0.01);
	//threshold += i/255.0;
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
	
	chkGLErr();
}

glm::vec3 col(float v){
	if(v<-1) return glm::vec3(1,0,0);
	if(v>1) return glm::vec3(0,1,0);
	
	if(v==0) return glm::vec3(1,1,1);
	if(v<0) return glm::vec3(1-(1+v),1+v,1+v);
	return glm::vec3(1-v,v,1-v);
}


ScalarField* sf = 0;
int main( int argc, char* argv[] )
{
	if(argc == 2)
		_case = atoi(argv[1]);
	if (glfwInit() != GL_TRUE){
		std::cout << "Could not init glfw"<< std::endl;
		return 2;
	}

	if (glfwOpenWindow(1920, 1080, 8, 8, 8, 8, 32, 0, GLFW_WINDOW) != GL_TRUE){
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

	init();

	glfwSetWindowSizeCallback(GLFWwindowsizefun(resize));

	glfwSetKeyCallback(GLFWkeyfun(keyboard));

	glfwSetMouseButtonCallback(GLFWmousebuttonfun(mouseButton));
	glfwSetMousePosCallback(GLFWmouseposfun(mouseMotion));
	glfwSetMouseWheelCallback(GLFWmousewheelfun(wheel));

	bool run = true;
	
	//run = true;
	while(run){
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
			break;
		run = runTests();
		//draw();
		//break;
	}	

	if(sf != 0)
		delete sf;
	if(mesh!=0)
		delete mesh;
	imFormatRemoveAll();

	return 0;
}

struct testParams{
	 float alpha, beta, w, iso, uind, xi, threshold;
	 std::string name;
};

int testRun = 0;
std::vector<testParams> tests;
HTML html;
bool runTests(){
	if(tests.empty()){
		html.appendToHead("<link rel=\"stylesheet\" type=\"text/css\" href=\"testruns.css\" />");

		sf = ScalarField::ReadFromRawfile(files[niceCases[_case]].path,files[niceCases[_case]].w,files[niceCases[_case]].h,files[niceCases[_case]].d);

		float da,db,dw,di,du,dxi,dt;
		da = 0.3;
		db = 0.01;
		dw = 0.3;
		di = 0.1;
		du = 0.3;
		dxi = 0.3;
		dt = 0.3;

		float b = 0.03; //as mentioned in the paper
		float w = 0.6;
		float uind = 0.8;
		//for(float b = 0			;	b<0.05			;	b+=db) //5
		//for(float w = 0.1			;	w<=1			;	w+=dw) //3
		for(float threshold = 0.3	;	threshold<uind	;	threshold+=dt) //2
		for(float xi = 0.1			;	xi<=1			;	xi+=dxi) //3
		for(float iso = 0.4			;	iso<=0.6		;	iso+=di) //4
		for(float a = 0.1			;	a+b<=1			;	a+=da) //2
		//for(float uind = 0.7		;	uind<=1			;	uind+=du)
		{
			std::stringstream name;
			name << "testrun" << tests.size();
			
			testParams test;
			test.name = name.str();
			test.alpha = a;
			test.beta = b;
			test.w = w;
			test.iso = iso;
			test.uind = uind;
			test.xi = xi;
			test.threshold = threshold;

			tests.push_back(test);
		}

		tests.clear();

		testParams test;
		test.name = "real test";
		test.alpha = 0.6;
		test.beta = 0.03;
		test.w = 0.6;
		test.iso = 0.6;
		test.uind = 0.95;
		test.xi = 0.8;	
		test.threshold = 0.3;

		tests.push_back(test);

		std::cout << tests.size() << " tests created" << std::endl;
		DBG("Test cretead");
	}

	html.appendToBody(test(sf,tests[testRun].name.c_str(),tests[testRun].alpha,tests[testRun].beta,tests[testRun].w,tests[testRun].iso,tests[testRun].uind,tests[testRun].xi,tests[testRun].threshold));
	DBG("Test run completed");
	html.save("index.html");
	DBG("HTML Saved");

	testRun++;
	return tests.size() > testRun;
}



std::string test(ScalarField *v,const char *name,float alpha,float beta,float w,float iso,float uind,float xi,float threshold){
	try{
		Directory::createFolder(name);
		char screnshot[255];
		{
			std::stringstream filename;
			filename << name << "/screenshot.jpg";
			strcpy(screnshot,filename.str().c_str());
		}	
		DBG("starting test");
		TmpPointer<UltrasoundVariationalClassification> tmp = new UltrasoundVariationalClassification(v,alpha,beta,w,iso,uind,xi);
		DBG("US Classified");
		
		TmpPointer<ScalarField> blured = tmp->blur();
		CSGScalarField csg(blured.get(),threshold);
		DBG("CSGScalarField created");
		
		for(int i = 0;i<tmp->getDimensions().z;i++){
			IM_Pointer img = UltrasoundSurfacePointExtractor::getSlice(tmp.get(),Z_AXIS,i);
			std::stringstream ss;
			ss << name << "/slice_" << i << ".jpg";
			IM_SaveImage(img.get(),ss.str().c_str(),"JPEG");
		}

		mesh = MarchingTetrahedra::March<IndexedMesh>(&csg,tmp->getBoundingAABB(),tmp->getDimensions()/2);
		DBG("Mesh Created");
	
		DBG("Slices saved");

	
	
		draw();
		draw();
		DBG("Mesh drawn");
		IM_Pointer scren = IM_GetScreenshot();
		DBG("Screenshot taken");
		IM_SaveImage(scren.get(),screnshot,"JPEG");
		DBG("Screenshot saved");
		delete mesh;
		mesh = 0;
		DBG("Mesh deleted");

		auto vmin = tmp->getMin();
		auto vmax = tmp->getMax();

		std::stringstream ss;
		ss << "<div class=\"testrun\" id=\"" << name << "\" >"<<std::endl;
		ss << "<img width=\"250\" src=\"" << screnshot <<  "\" />" << std::endl;
		ss << "<table>" << std::endl;
		ss << "\t<tr><th colspan=\"2\">" << name << "</th></tr>" << std::endl;
		ss << "\t<tr><td>&alpha;</td><td>" << alpha << "</td></tr>" << std::endl;
		ss << "\t<tr><td>&beta;</td><td>" << beta << "</td></tr>" << std::endl;
		ss << "\t<tr><td>&gamma;</td><td>" << 1-alpha-beta << "</td></tr>" << std::endl;
		ss << "\t<tr><td>&omega;</td><td>" << w << "</td></tr>" << std::endl;
		ss << "\t<tr><td>v<sub>iso</sub></td><td>" << iso << "</td></tr>" << std::endl;
		ss << "\t<tr><td>u<sub>ind</sub></td><td>" << uind << "</td></tr>" << std::endl;
		ss << "\t<tr><td>&delta;</td><td>" << xi << "</td></tr>" << std::endl;
		ss << "\t<tr><td>Threshold</td><td>" << threshold << "</td></tr>" << std::endl;
		ss << "\t<tr><td>Volume min/max</td><td>" << vmin<< "/" << vmax << "</td></tr>" << std::endl;
		ss << "</table></div>" << std::endl << std::endl;
		DBG("HTML Created");
		return ss.str();
	}catch(...){
		std::stringstream ss;
		ss << "<div class=\"testrun\" id=\"" << name << "\" >"<<std::endl;
		ss << name << "failed, due to memory or something";
		ss << "</div>" << std::endl << std::endl;
		DBG("HTML Created");
		mesh = 0;
		return ss.str();
	}
}