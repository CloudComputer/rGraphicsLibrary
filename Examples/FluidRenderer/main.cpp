
#include <fstream>;

#include <FluidSimulator\EulerianFluid.h>
#include <Math\Interpolator.h>
#include <Volume\VectorField.h>
#include <Volume\ScalarField.h>
#include <Geometry\Mesh.h>
#include <OpenGLHelpers/FBO.h>
#include <gl/glut.h>

#include <string>
#include <iostream>

void Raycast();


void idle(){}
void draw(){}
void keyDown(unsigned char key,int x,int y){}
void keyUp(unsigned char key,int x,int y){}
void mouseButton(int button,int state,int x,int y){}
void mouseMotion(int x,int y){}

void init(){

}

int main( int argc, char* argv[] )
{
	glutInit (&argc, argv);
	glutInitWindowSize (600, 400);
	glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("rGraphicsLibrary - Fluids  Example");

	if(glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to init glew" << std::endl;
		exit(1);
	}

	init();

	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mouseMotion);

	glutIdleFunc(idle);
	glutDisplayFunc(draw);

	glutMainLoop();
	return 0;
}


//
//sgct::Engine * gEngine;
//
//Mesh *m;
//Fluid *f;
//
//float rotationSpeed = 1;
//float rx=0,ry=0;
//float scale = 1;
//
//bool _front = true;
//
//void myDrawFun();
//void myPreSyncFun();
//void myEncodeFun();
//void myDecodeFun();
//void UpdateTexture();
//void initOGL();
//
//class AttribPusher{
//private:
//	GLbitfield _attrib;
//public:
//	AttribPusher(GLbitfield attrib){
//		_attrib = attrib;
//		glPushAttrib(_attrib);
//	}
//
//	~AttribPusher(){
//		glPopAttrib();
//	}
//
//};
//
//int mouseX = -1,mouseY = -1,scrollPos = 0;
//bool leftMouseButtonState = false;
//
//VectorField *vectorField;
//ScalarField *scalarField;
//
//GLuint texVol,locTexVol,locTexBack,locTexFront,vbo,locWinSize,locTexSize;
//FBO *fboFront;
//FBO *fboBack;
//
//GLfloat vertices[8*3] = {	0,0,0,
//							0,0,1,
//							0,1,0,
//							0,1,1,
//							1,0,0,
//							1,0,1,
//							1,1,0,
//							1,1,1};
//
//	GLfloat colors[8*3] = { 0,0,0, //0 black
//							0,0,1, //1 blue
//							0,1,0, //2 green
//							0,1,1, //3 cyan
//							1,0,0, //4 red
//							1,0,1, //5 magenta
//							1,1,0, //6 yellow
//							1,1,1};//7 white
//
//GLubyte indices[4*6] = {0,4,5,1,
//						0,2,6,4,
//						0,1,3,2,
//						7,6,2,3,
//						5,4,6,7,
//						5,7,3,1};
//
///**
//* Is called once for twice for every keypress, att press and at release
//* @param key case insensetive representation of key pressed (h and shift+h gives the same Integer, infact shift+h will call this function at press/release for both shift and h)
//* @param action wether the key was pressed or released
//*/
//void keyboardCallbackFunction(int key,int action){
//	if(key == 'F' && action)
//		_front = !_front;
//}
//
///**
//* Is called once for every keypress (after press and before release callbacks)
//* @param key case sensetive representation of key pressed (h and shift+h gives different Integers)
//* @param action unknown at the moment
//*/
//void charCallbackFunction(int key,int action){
//
//}
//
///**
//* Is called everytime the mouse is pressed/released.
//* @param button represent which button that was clicked
//* @param state represent the state of the button, zero is released and one is pressed
//*/
//void mouseButtonCallbackFunction(int button,int state){
//	if(button == GLFW_MOUSE_BUTTON_LEFT){
//		leftMouseButtonState = state;
//		gEngine->setMousePointerVisibility(!state);
//	}
//}
//
///**
//* called everytime the mouse is moved. [x,y] is the current coordinates of mouse relative to upper left corner of windows (viewpoert of window, not frames)
//* @param x current x-coordinate of mouse relative to left border of the window
//* @param y current yx-coordinate of mouse relative to upper border of the window
//*/
//void mousePosCallbackFunction(int x,int y){
//	int dx,dy;
//	if(mouseX == -1){
//		mouseX = x;
//		mouseY = y;
//	}
//	dx = x - mouseX;
//	dy = y - mouseY;
//	mouseX = x;
//	mouseY = y;
//	if(leftMouseButtonState){
//		rx += dx * rotationSpeed;
//		ry += dy * rotationSpeed;
//	}
//}
//
///**
//* 
//*/
//void mouseScrollCallbackFunction(int a){
//	int ds = a-scrollPos;
//	scrollPos = a;
//	scale *= 1+(ds*0.1);
//}
//
//double sq(double x,double y,double z){
//	return 1-sqrt(x*x + y*y + z*z)/sqrt(3.0);	
//}
//
//glm::vec3 sinCos(double x,double y,double z){
//	return glm::vec3(y,-x,0);
//}
//	
//
//
//void CreateData()
//{
//	m = Mesh::LoadWavefront("C:/Users/rickard/Dropbox/LIU/år 1/2011 VY P2 - Animation and Modeling/Objs/cow.obj");
//}
//
//

//int main(int argc , char ** argv){
//	gEngine = new sgct::Engine( argc, argv );
//	
//	//Bind your draw function to the render loop
//	gEngine->setDrawFunction( myDrawFun );
//	gEngine->setPreSyncFunction( myPreSyncFun );
//	sgct::SharedData::Instance()->setEncodeFunction(myEncodeFun);
//	sgct::SharedData::Instance()->setDecodeFunction(myDecodeFun);
//
//	gEngine->setInitOGLFunction(initOGL);
//
//
//	gEngine->setKeyboardCallbackFunction( keyboardCallbackFunction ); //arguments: int key, int action
//	gEngine->setCharCallbackFunction( charCallbackFunction ); //arguments: int character, int action
//	gEngine->setMouseButtonCallbackFunction( mouseButtonCallbackFunction ); //arguments: int button, int action
//	gEngine->setMousePosCallbackFunction( mousePosCallbackFunction ); //arguments: int x, int y
//	gEngine->setMouseScrollCallbackFunction( mouseScrollCallbackFunction ); //arguments: int pos
//
//	if( !gEngine->init() )
//	{
//		delete gEngine;
//		return EXIT_FAILURE;
//	}
//	CreateData();
//
//	// Main loop
//	gEngine->render();
// 
//	// Clean up
//	delete gEngine;
// 
//	// Exit program
//	exit( EXIT_SUCCESS );
//}
//
//void drawVectorField() 
//{
//	glm::ivec3 dim = vectorField->getDimensions();
//	float dx = 1.0 / (dim.x);
//	float dy = 1.0 / (dim.y);
//	float dz = 1.0 / (dim.z);
//	float s = dx;
//	if(s>dy) s = dy;
//	if(s>dz) s = dz;
//	s*=0.9;
//	glBegin(GL_LINES);
//	FOR(dim){
//		glm::vec3 v = vectorField->get(glm::ivec3(x,y,z));
//		glVertex3f(dx*x-0.5,dy*y-0.5,dz*z-0.5);
//		glVertex3f(dx*x-0.5 + v.x*s,dy*y-0.5 + v.y*s,dz*z-0.5 + v.z*s);
//	}
//	glEnd();
//}
//
//void drawColorCube() 
//{
//	glBindBuffer(GL_ARRAY_BUFFER,vbo);
//
//	glEnableClientState(GL_COLOR_ARRAY);
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(vertices)));
//	glVertexPointer(3, GL_FLOAT, 0, 0);
//
//	glDrawElements(GL_QUADS,6*4,GL_UNSIGNED_BYTE,indices);
//
//	glBindBuffer(GL_ARRAY_BUFFER,0);
//
//	//sgct::ShaderManager::Instance()->bindShader( "fluid" );
//
//}
//
//void renderFrontAndBackFaces() 
//{
//	int curFBO[1],v[1];
//	glGetIntegerv(GL_DRAW_BUFFER0,(GLint *)v);
//	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT,curFBO);
//
//
//	fboFront->clear();
//	fboBack->clear();
//
//	fboFront->bind();
//	glClearDepth(1);
//	glDepthFunc(GL_LESS);
//	glClear(GL_DEPTH_BUFFER_BIT);
//	drawColorCube();
//	fboFront->unbind();
//
//	fboBack->bind();
//	glClearDepth(0);
//	glDepthFunc(GL_GEQUAL);
//	glClear(GL_DEPTH_BUFFER_BIT);
//	drawColorCube();
//	fboBack->unbind();
//
//	glBindFramebuffer(GL_FRAMEBUFFER, curFBO[0]);
//	glDrawBuffer(v[0]);
//}
////
////glm::vec3 N(-1,0,0);
////glm::vec3 V(-1,1,1);
////glm::vec3 R = glm::reflect(-V,N);
////glm::vec3 up(N),side(0,0,1);
////
////std::vector<glm::vec4> directions;
////void buildBRDF(){
////	V = glm::normalize(V);
////
////	float deg = acos(glm::dot(R,N))*180/3.14;
////
////	if(deg!=0){
////		side = glm::cross(R,N);
////		//up = glm::cross(side,V);
////	}
////
////	for(int i = 0;i<40;i++){
////		
////
////		float azimuth,elevation,r0,r1;
////		r0 = rand();
////		r1 = rand();
////		r0 /= RAND_MAX;
////		r1 /= RAND_MAX;
////		azimuth   = 360*r0;
////		elevation = acos(pow((double)r1,1.0/(1+10))) * 90;
////		elevation /= 3.1415;
////
////		glm::vec4 dir(R,0);
////		glm::mat4x4 rot;
////		//rot = glm::rotate(rot,-deg,side);
////		rot = glm::rotate(rot,azimuth,R);
////		rot = glm::rotate(rot,elevation,side);
////		dir = rot * dir;
////
////		if(glm::dot(N,glm::vec3(dir.x,dir.y,dir.z))<0){
////			i--;
////		}else{
////			dir = glm::normalize(dir);
////			directions.push_back(dir);
////}
////
////	}
////}
////
////void drawBRDF() 
////{
////	glBegin(GL_LINES);
////	glColor3f(1,0,0);
////	glVertex3f(0,0,0);
////	glVertex3f(N.x,N.y,N.z);
////
////	glColor3f(0,1,0);
////	glVertex3f(0,0,0);
////	glVertex3f(V.x,V.y,V.z);
////
////	glColor3f(0,0,1);
////	glVertex3f(0,0,0);
////	glVertex3f(R.x,R.y,R.z);
////
////	glColor3f(0,1,1); //Cyan
////	glVertex3f(0,0,0);
////	glVertex3f(up.x,up.y,up.z);
////
////	glColor3f(1,0,1); //magenta
////	glVertex3f(0,0,0);
////	glVertex3f(side.x,side.y,side.z);
////
////
////	glColor3f(1,1,1);
////	for(int i = 0;i<directions.size();i++){
////		glVertex3f(0,0,0);
////		glVertex3f(directions[i].x,directions[i].y,directions[i].z);
////	}
////	glEnd();
////}
//
//void myDrawFun()
//{
//	glRotatef(ry,1,0,0);
//	glRotatef(rx,0,1,0);
//	glScalef(scale,scale,scale);
//
//	/*m->draw();
//	m->getBoundingAABB().draw();*/
//	
//	renderFrontAndBackFaces();
//	Raycast();
//
//	//*/
//	//drawVectorField();
//}
// 
//void myPreSyncFun()
//{
//	if(gEngine->getWindowPtr()->getHResolution() != fboFront->getSize().x ||
//		gEngine->getWindowPtr()->getVResolution() != fboFront->getSize().y ){
//			fboFront->setSize(gEngine->getWindowPtr()->getHResolution(),gEngine->getWindowPtr()->getVResolution());	
//			fboBack->setSize(gEngine->getWindowPtr()->getHResolution(),gEngine->getWindowPtr()->getVResolution());	
//	}
//
//	double dt = gEngine->getDt();
//	if(dt>0){
//		//f->update(dt/1000.0);
//		UpdateTexture();
//	}
//	
//}
// 
//void myEncodeFun()
//{
//}
// 
//void myDecodeFun()
//{
//}
//
//void createVBO() 
//{
//	for(int i = 0;i<8*3;i++){ //normalize and put 0.5 at origo
//		vertices[i] *= 2;
//		vertices[i] -= 1;
//	}
//
//	glGenBuffers(1,&vbo);
//	glBindBuffer(GL_ARRAY_BUFFER,vbo);
//	glBufferData(GL_ARRAY_BUFFER,sizeof(colors)+sizeof(vertices),0,GL_STATIC_DRAW);
//	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);
//	glBufferSubData(GL_ARRAY_BUFFER,sizeof(vertices),sizeof(colors),colors);
//	glBindBuffer(GL_ARRAY_BUFFER,0);
//}
//
//
//GLfloat *data = 0;
//void UpdateTexture() 
//{
//	glm::ivec3 dim = f->getDensityField()->getDimensions();
//	//float min = 99999,max = -9999,avg = 0;
//	if(data == 0)
//		data = (GLfloat*)malloc(dim.x*dim.y*dim.z*sizeof(GLfloat));
//	FOR(dim){
//		int id = (INDEX(f->getDensityField(),x,y,z));
//		data[id] = f->getDensityField()->get(glm::ivec3(x,y,z));
//		/*if(min>data[id]) min = data[id];
//		if(max<data[id]) max = data[id];
//		avg += data[id];*/
//	}
//
//	//avg /= dim.x*dim.y*dim.z;
//	//std::cout << min << " " << avg << " " << max << std::endl;
//
//	glBindTexture(GL_TEXTURE_3D,texVol);
//	glTexImage3D(GL_TEXTURE_3D,0,GL_R32F,dim.x,dim.y,dim.z,0,GL_RED,GL_FLOAT,data);
//	glBindTexture(GL_TEXTURE_3D,0);
//
//	//free(data);
//}
//
//void initOGL(){
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_TEXTURE_2D);
//	/*glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);*/
//	glClearDepth(1);
//	glEnable(GL_TEXTURE_3D);
//	/*glEnable(GL_CULL_FACE);
//	glFrontFace(GL_CW);*/
//	createVBO();
//
//	f = new Fluid(new LevelSet(Mesh::LoadWavefront("C:/Users/rickard/Dropbox/LIU/år 1/2011 VY P2 - Animation and Modeling/Objs/cow.obj"),glm::ivec3(32,32,32)));
//	f->addSource(new BoundingSphere(glm::vec3(0.2,0.8,0.2),0.1));
//	f->addSource(new BoundingSphere(glm::vec3(0.8,0.8,0.8),0.1));
//	f->addSource(new BoundingSphere(glm::vec3(0.2,0.8,0.8),0.1));
//	f->addSource(new BoundingSphere(glm::vec3(0.8,0.8,0.2),0.1));
//	f->addForce(glm::vec3(0,-0.9,0));
//
//	glGenTextures(1,&texVol);
//	glBindTexture(GL_TEXTURE_3D,texVol);
//	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_CLAMP);
//	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_CLAMP);
//	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_R,GL_CLAMP);
//	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//	glBindTexture(GL_TEXTURE_3D,0);
//	UpdateTexture();
//
//
//	fboFront = new FBO();
//	fboFront->setSize(gEngine->getWindowPtr()->getHResolution(),gEngine->getWindowPtr()->getVResolution());	
//	fboFront->init();
//	fboBack = new FBO();
//	fboBack->setSize(gEngine->getWindowPtr()->getHResolution(),gEngine->getWindowPtr()->getVResolution());	
//	fboBack->init();
//
//	sgct::ShaderManager::Instance()->addShader("fluid","shaders/fluid.vert","shaders/fluid.frag");
//
//	sgct::ShaderManager::Instance()->bindShader( "fluid" );
//	locTexVol = sgct::ShaderManager::Instance()->getShader( "fluid").getUniformLocation( "texVol" );
//	locTexBack = sgct::ShaderManager::Instance()->getShader( "fluid").getUniformLocation( "texBack" );
//	locTexFront = sgct::ShaderManager::Instance()->getShader( "fluid").getUniformLocation( "texFront" );
//	locWinSize = sgct::ShaderManager::Instance()->getShader( "fluid").getUniformLocation( "winSize" );
//	locTexSize = sgct::ShaderManager::Instance()->getShader( "fluid").getUniformLocation( "texSize" );
//	sgct::ShaderManager::Instance()->unBindShader();
//
//	sgct::TextureManager::Instance()->loadTexure("bild","bil.png",false,0);
//
//	//buildBRDF();
//}
//
//
//
//void Raycast() 
//{
//	glClearDepth(1);
//	glDepthFunc(GL_LESS);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//
//
//	sgct::ShaderManager::Instance()->bindShader("fluid");
//	glUniform2f(locWinSize,gEngine->getWindowPtr()->getHResolution(),gEngine->getWindowPtr()->getVResolution());
//	glUniform3f(locTexSize,f->getDensityField()->getDimensions().x,f->getDensityField()->getDimensions().y,f->getDensityField()->getDimensions().z);
//
//	glUniform1i(locTexVol,3);
//	glActiveTexture(GL_TEXTURE3);
//	glBindTexture(GL_TEXTURE_3D,texVol);
//
//	glUniform1i(locTexBack,0);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D,fboBack->getTexture());
//
//
//	glUniform1i(locTexFront,1);
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D,fboFront->getTexture());
//	drawColorCube();
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_3D,0);
//	glActiveTexture(GL_TEXTURE0);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D,0);
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D,0);
//	sgct::ShaderManager::Instance()->unBindShader();
//}