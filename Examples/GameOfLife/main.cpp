#include <fstream>;

#include <OpenGLHelpers/OpenGLInfo.h>
#include <Util/StopClock.h>

#include <gl/glfw.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

#include <Physics\SpringDamperSystem.h>

StopClock s(true);

glm::ivec2 winSize;

glm::ivec2 dim(25,25);

bool **grid,**tmpGrid;


bool get(int x,int y){
	while(x<0) x+=dim.x;
	while(y<0) y+=dim.y;
	x %= dim.x;
	y %= dim.y;
	return grid[x][y];
}

void evolve(){
	for(int x = 0;x<dim.x;x++)for(int y = 0;y<dim.y;y++){
		int live = 0;
		live += get(x-1,y-1);
		live += get(x  ,y-1);
		live += get(x+1,y-1);

		live += get(x-1,y);
		live += get(x+1,y);

		live += get(x-1,y+1);
		live += get(x  ,y+1);
		live += get(x+1,y+1);
		if(get(x,y)){
			tmpGrid[x][y] = (live == 2 || live == 3);
		}else{
			tmpGrid[x][y] = live == 3;
		}
	}

	bool **tmp = grid;
	grid = tmpGrid;
	tmpGrid = tmp;
}

void draw(){
	chkGLErr();
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(1.0f/winSize.x,1.0f/winSize.y,0);
	
	float dx = winSize.x / (float) dim.x;
	float dy = winSize.y / (float) dim.y;
	glBegin(GL_QUADS);
	glColor3f(1,1,0);
	for(int x = 0;x<dim.x;x++)for(int y = 0;y<dim.y;y++)if(grid[x][y]){
		glVertex2f(x*dx,y*dy);	
		glVertex2f(x*dx+dx,y*dy);	
		glVertex2f(x*dx+dx,y*dy+dy);	
		glVertex2f(x*dx,y*dy+dy);	
	}
	glEnd();



	glfwSwapBuffers();
	chkGLErr();
}


void resize(int width,int height){
	chkGLErr();
	glMatrixMode(GL_PROJECTION);
	chkGLErr();
	
	winSize.x = width;
	winSize.y = height;
	glLoadMatrixf(glm::value_ptr(glm::ortho(0.f,1.f,0.f,1.f)));
	chkGLErr();
	

	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,width,height);
	chkGLErr();
}

void init(){
	chkGLErr();
	glClearColor(0,0,0,0);
	
	grid = new bool*[dim.x];
	tmpGrid = new bool*[dim.x];
	for(int i = 0;i<dim.x;i++){
		grid[i] = new bool[dim.y];
		tmpGrid[i] = new bool[dim.y];
		for(int j = 0;j<dim.y;j++){
			grid[i][j] = false; //rand() % 100 < 10;
			tmpGrid[i][j] = false;
		}
	}
	
	int o = 5;
	grid[4+o][1+o] = true;
	grid[5+o][1+o] = true;
	grid[8+o][1+o] = true;
	grid[9+o][1+o] = true;
	grid[10+o][1+o] = true;
	
	grid[1+o][2+o] = true;
	grid[2+o][2+o] = true;
	grid[3+o][2+o] = true;
	grid[5+o][2+o] = true;
	grid[7+o][2+o] = true;
	grid[9+o][2+o] = true;
	grid[10+o][2+o] = true;
	
	grid[1+o][3+o] = true;
	grid[2+o][3+o] = true;
	grid[3+o][3+o] = true;
	grid[4+o][3+o] = true;
	grid[5+o][3+o] = true;
	grid[8+o][3+o] = true;
	grid[9+o][3+o] = true;
	grid[10+o][3+o] = true;
	
	grid[1+o][4+o] = true;
	grid[3+o][4+o] = true;
	grid[5+o][4+o] = true;
	grid[7+o][4+o] = true;
	grid[9+o][4+o] = true;
	grid[11+o][4+o] = true;
	
	grid[2+o][5+o] = true;
	grid[4+o][5+o] = true;
	grid[6+o][5+o] = true;
	grid[8+o][5+o] = true;
	grid[9+o][5+o] = true;
	grid[10+o][5+o] = true;
	grid[11+o][5+o] = true;

	
	grid[5+o][6+o] = true;
	grid[6+o][6+o] = true;
	grid[7+o][6+o] = true;


	grid[1+o][7+o] = true;
	grid[2+o][7+o] = true;
	grid[3+o][7+o] = true;
	grid[4+o][7+o] = true;
	grid[6+o][7+o] = true;
	grid[8+o][7+o] = true;
	grid[10+o][7+o] = true;
	
	grid[1+o][8+o] = true;
	grid[3+o][8+o] = true;
	grid[5+o][8+o] = true;
	grid[7+o][8+o] = true;
	grid[9+o][8+o] = true;
	grid[11+o][8+o] = true;
	
	grid[2+o][9+o] = true;
	grid[3+o][9+o] = true;
	grid[4+o][9+o] = true;
	grid[7+o][9+o] = true;
	grid[8+o][9+o] = true;
	grid[9+o][9+o] = true;
	grid[10+o][9+o] = true;
	grid[11+o][9+o] = true;
	
	grid[2+o][10+o] = true;
	grid[3+o][10+o] = true;
	grid[5+o][10+o] = true;
	grid[7+o][10+o] = true;
	grid[9+o][10+o] = true;
	grid[10+o][10+o] = true;
	grid[11+o][10+o] = true;
	
	grid[2+o][11+o] = true;
	grid[3+o][11+o] = true;
	grid[4+o][11+o] = true;
	grid[7+o][11+o] = true;
	grid[8+o][11+o] = true;
	

	chkGLErr();
}

int main( int argc, char* argv[] )
{
	if (glfwInit() != GL_TRUE){
		std::cout << "Could not init glfw"<< std::endl;
		return 2;
	}

	if (glfwOpenWindow(600, 600, 8, 8, 8, 8, 32, 0, GLFW_WINDOW) != GL_TRUE){
		std::cout << "Could not create window"<< std::endl;
		return 3;
	}
	glfwSetWindowTitle ("rGraphicsLibrary - Game of Life");

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

	while(true){
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
			break;
		evolve();
		draw();

		glfwSleep(0.5);
		
	}
	return 0;
}
