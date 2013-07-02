#include "Viewport.h"



Viewport::Viewport(rWindow *owner):ResizeListener(owner){
	topLeft = glm::ivec2(10,10);
	size = glm::ivec2(300,300);
	texture = 0;
}


Viewport::~Viewport(){
	
}

void Viewport::setTexture(std::string textureName){
	this->textureName = textureName;
	texture = 0;
}

void Viewport::draw(){
	if(texture==0){
		texture = Texture::getTextureID(textureName);
	}
	
	glViewport(topLeft.x,topLeft.y,size.x,size.y);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glOrtho(0,1,0,1,-1,1);
	

	glMatrixMode(GL_MODELVIEW);

	glBindTexture(GL_TEXTURE_2D,texture);

    glLoadIdentity();

	glBegin(GL_QUADS);
    glColor4f(1,1,1,1);
	glTexCoord2f(0,0);
    glVertex2f(0,0);

	glTexCoord2f(1,0);
	glVertex2f(1,0);

	glTexCoord2f(1,1);
	glVertex2f(1,1);

	glTexCoord2f(0,1);
	glVertex2f(0,1);

    glEnd();


	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
