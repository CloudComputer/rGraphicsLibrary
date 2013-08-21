#include <fstream>;

#include <OpenGLHelpers/FBO.h>
#include <OpenGLHelpers/OpenGLInfo.h>
#include <OpenGLHelpers/AttribPusher.h>
#include <Util/StopClock.h>

#include <Geometry/Mesh/IndexedMesh.h>

#include <gl/glfw.h>

#include <OpenEXRHelpers/OpenEXRIncludes.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include "OpenGLHelpers/Shader.h"

StopClock s(true);

Mesh *m;
ShaderProgram *pgm;
RGBABuffer buffer;
GLuint tex;

std::string env_map_filename;

bool mouse0State = false;
float rx = 0,ry = 0,scale = 1;
int samples = 1;


std::vector<Light> lights;


float minExp = 0.0;
float maxExp = 0.4;
float gamma = 0.4;
float diffusion = 0.5;

void draw2(){
	chkGLErr();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::lookAt(glm::vec3(2,1,1.8),glm::vec3(0,0,0),glm::vec3(0,1,0))));
	glRotatef(ry,1,0,0);
	glRotatef(rx,0,1,0);
	glScalef(scale,scale,scale);

	pgm->bind();
	pgm->setTexture("env_light_tex",GL_TEXTURE_RECTANGLE_NV,0,tex);
	pgm->setUniform("env_light_tex_size",buffer.dim.x,buffer.dim.y);
	pgm->setUniform("env_number_of_samples",samples*samples);
	pgm->setUniform("env_gamma",gamma);
	pgm->setUniform("env_minExp",minExp);
	pgm->setUniform("env_maxExp",maxExp);
	pgm->setUniform("env_diffuse",diffusion);
	
	/*std::vector<Triangle> faces = m->getFaces();
	std::vector<Triangle>::iterator face;
	glColor3f(1,1,1);
	glBegin(GL_TRIANGLES);
	for(face = faces.begin();face != faces.end();++face){
		glNormal3fv(glm::value_ptr(face->v0->normal));
		glVertex3fv(glm::value_ptr(face->v0->position));

		glNormal3fv(glm::value_ptr(face->v1->normal));
		glVertex3fv(glm::value_ptr(face->v1->position));

		glNormal3fv(glm::value_ptr(face->v2->normal));
		glVertex3fv(glm::value_ptr(face->v2->position));
	}
	glEnd();*/
	static_cast<IndexedMesh*>(m)->draw();
	pgm->unbind();

	chkGLErr();
	glfwSwapBuffers();
}


void draw(){
	chkGLErr();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::lookAt(glm::vec3(2,1,1.8),glm::vec3(0,0,0),glm::vec3(0,1,0))));
	glRotatef(ry,1,0,0);
	glRotatef(rx,0,1,0);
	
	chkGLErr();
	/*for(int i = 0;i<lights.size();i++){
		lights[i].set();
	}*/
	glScalef(scale,scale,scale);
	
	float lum = 0;
	chkGLErr();
	{
		AttribPusher ___a(GL_LIGHTING_BIT);
		glPointSize(4.0f);
		chkGLErr();
		glDisable(GL_LIGHTING);
		chkGLErr();
		glBegin(GL_POINTS);
		for(int i = 0;i<lights.size();i++){
			/*glColor4fv(glm::value_ptr(glm::normalize(lights[i].getDiffuseColor())));
			glVertex3fv(glm::value_ptr(lights[i].getPosition()));
			*/
			if(i<64)
				lum += lights[i].getDiffuseColor().x * 0.2125 +lights[i].getDiffuseColor().y * 0.7454 +lights[i].getDiffuseColor().z * 0.0721;
			//std::cout << lights[i].getPosition().x << " " << lights[i].getPosition().y << " " << lights[i].getPosition().z << std::endl;
		}

		//std::cout << std::endl << std::endl;
		glEnd();
	}
	
	chkGLErr();

	pgm->bind();
	pgm->setUniform("num_lights",glm::min(64,(int)lights.size()));chkGLErr();
	pgm->setUniform("material.ambient",glm::vec4(0,0,0,1));chkGLErr();
	pgm->setUniform("material.diffuse",glm::vec4(1,1,1,1));chkGLErr();
	pgm->setUniform("material.specular",glm::vec4(1,1,1,1));chkGLErr();
	pgm->setUniform("material.shininess",20.0f);chkGLErr();
	pgm->setUniform("lum",lum);chkGLErr();
	chkGLErr();

	for(int i = 0;i<lights.size()&&i<64;i++){
		char name[256];
		sprintf(name,"lights[%d].ambient",i);
		pgm->setUniform(name,lights[i].getAmbientColor());
		sprintf(name,"lights[%d].diffuse",i);
		pgm->setUniform(name,lights[i].getDiffuseColor());
		sprintf(name,"lights[%d].specular",i);
		pgm->setUniform(name,lights[i].getSpecularColor());
		sprintf(name,"lights[%d].radius",i);
		pgm->setUniform(name,lights[i].getRadius());
		/*sprintf(name,"lights[%d].constantAtt",i);
		pgm->setUniform(name,lights[i].getConstantAttenuation());
		sprintf(name,"lights[%d].linearAtt",i);
		pgm->setUniform(name,lights[i].getLinearAttenuation());
		sprintf(name,"lights[%d].quadraticAtt",i);
		pgm->setUniform(name,lights[i].getQuadraticAttenuation());*/
		sprintf(name,"lights[%d].position",i);
		pgm->setUniform(name,lights[i].getPosition());
		chkGLErr();
	}


	
	float y = m->getBoundingAABB().getPosition(glm::vec3(0,0,0)).y;
	glColor3f(1,1,1);
	//glBegin(GL_QUADS);
	//glNormal3f(0,1,0);
	//glVertex3f(-5,y,-5);
	//glVertex3f(+5,y,-5);
	//glVertex3f(+5,y,+5);
	//glVertex3f(-5,y,+5);

	//glEnd();

	/*std::vector<Triangle> faces = m->getFaces();
	std::vector<Triangle>::iterator face;
	*/
	glColor3f(1,1,1);
	static_cast<IndexedMesh*>(m)->draw();
	/*glBegin(GL_TRIANGLES);
	for(face = faces.begin();face != faces.end();++face){
		glNormal3fv(glm::value_ptr(face->v0->normal));
		glVertex3fv(glm::value_ptr(face->v0->position));

		glNormal3fv(glm::value_ptr(face->v1->normal));
		glVertex3fv(glm::value_ptr(face->v1->position));

		glNormal3fv(glm::value_ptr(face->v2->normal));
		glVertex3fv(glm::value_ptr(face->v2->position));
	}
	glEnd();*/
	pgm->unbind();

	chkGLErr();
	glfwSwapBuffers();
}


int prevScroll = 0;
void wheel(int i){
	if(glfwGetKey(GLFW_KEY_LCTRL) == GLFW_PRESS){
		gamma += (i-prevScroll)*0.1;
		gamma = glm::clamp(gamma,0.0f,1.0f);
	}else if(glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS){
		minExp += (i-prevScroll)*0.1; 
		minExp = glm::clamp(minExp,0.0f,1.0f);
	}else if(glfwGetKey(GLFW_KEY_LALT) == GLFW_PRESS){
		maxExp += (i-prevScroll)*0.1;
		maxExp = glm::clamp(maxExp,0.0f,1.0f);
	}else{
		scale *= 1+((i-prevScroll)*0.01);
	}
	std::cout << "Gamma: " << gamma << std::endl;
	std::cout << "minExp: " << minExp << std::endl;
	std::cout << "maxExp: " << maxExp << std::endl << std::endl;

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


void resize(int width,int height){
	chkGLErr();
	glMatrixMode(GL_PROJECTION);
	chkGLErr();
	
	float ar = width;
	ar /= height;
	glLoadMatrixf(glm::value_ptr(glm::perspective(45.0f, ar,0.1f,100.0f)));
	chkGLErr();

	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,width,height);
	chkGLErr();
}

void key(int button, int state){
	if(button == '1' && state){samples = 1;}
	if(button == '2' && state){samples = 2;}
	if(button == '3' && state){samples = 4;}
	if(button == '4' && state){samples = 8;}
	if(button == '5' && state){samples = 12;}
	if(button == '6' && state){samples = 14;}
	if(button == '7' && state){samples = 18;}
	if(button == '8' && state){samples = 24;}
	if(button == 'E' && state){diffusion += 0.05;}
	if(button == 'D' && state){diffusion -= 0.05;}
	if(button == 'R' && state){
		ShaderProgram::reReadFunctionSubbs();
		pgm = ShaderProgram::CreateShaderProgramFromSources(GLSL_DIR "/EnvironmentLighting.vert" ,GLSL_DIR "/EnvironmentLighting.frag");
	}
	diffusion = glm::clamp(diffusion,0.0f,1.0f);
	std::cout << "Number of samples: " << samples * samples << std::endl;
	std::cout << "Diffusion: " << diffusion  << std::endl;
}


void init(){
	chkGLErr();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_RECTANGLE_NV);
	glEnable(GL_TEXTURE_2D);

	m = Mesh::LoadWavefront<IndexedMesh>( MODELS_DIR "/cow.obj");
	//m = Mesh::LoadWavefront<IndexedMesh>( MODELS_DIR "/ico_sphere1.obj");
	
	pgm = ShaderProgram::CreateShaderProgramFromSources(GLSL_DIR "/EnvironmentLighting.vert" ,GLSL_DIR "/EnvironmentLighting.frag");


	chkGLErr();
	buffer = OpenEXRReader::readRGBA(env_map_filename);

	lights =  LightExtractor::ExtractLights(buffer,6,false,2.3);
	/*Light l(0);
	l.setPosition(glm::vec3(0,0,-1));
	lights.push_back(l);
*/
	chkGLErr();
	//tex = OpenEXRTexture::getAsTexture(buffer);

	chkGLErr();


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

	glDisable(GL_CULL_FACE);


	
	glClearColor(0,0,0,0);
	chkGLErr();
}

int main( int argc, char* argv[] )
{
	if (glfwInit() != GL_TRUE){
		std::cout << "Could not init glfw"<< std::endl;
		return 2;
	}

	if (glfwOpenWindow(600, 400, 8, 8, 8, 8, 32, 0, GLFW_WINDOW) != GL_TRUE){
		std::cout << "Could not create window"<< std::endl;
		return 3;
	}
	glfwSetWindowTitle ("rGraphicsLibrary - Mesh Example");

	if(glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to init glew" << std::endl;
		exit(1);
	}
	chkGLErr();
	OpenGLInfo::printOGLInformation();
	
	chkGLErr();

	if(argc > 1){
		env_map_filename = argv[1];
	}else{
		env_map_filename = EXR_IMAGES_DIR "/grace-new.exr";
	}

	init();

	glfwSetWindowSizeCallback(GLFWwindowsizefun(resize));
	glfwSetKeyCallback(GLFWkeyfun(key));

	glfwSetMouseButtonCallback(GLFWmousebuttonfun(mouseButton));
	glfwSetMousePosCallback(GLFWmouseposfun(mouseMotion));
	glfwSetMouseWheelCallback(GLFWmousewheelfun(wheel));

	while(true){
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
			break;

		draw();
	}
	return 0;
}

