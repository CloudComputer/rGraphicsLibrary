#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_


#include <glm/glm.hpp>
#include <glew\GL\glew.h>
#include <GLFW\glfw3.h>

#include "rWindow.h"
#include "Texture.h"

class rWindow;
class ResizeListener;

class Viewport : public ResizeListener{
protected:
	glm::ivec2 topLeft,size;
	GLuint texture;
	std::string textureName;
public:
	Viewport(rWindow *owner);
	virtual ~Viewport();

	virtual void onResize(glm::ivec2 newSize) = 0;

	void setTexture(std::string textureName);

	void draw();
};

class FullscreenViewport : public Viewport{
public:
	FullscreenViewport(rWindow *owner) : Viewport(owner){
		topLeft = glm::ivec2(0,0);
		size = owner->getSize();
	}
	virtual ~FullscreenViewport(){}
	virtual void onResize(glm::ivec2 newSize){size = newSize;}
};


class RelativeViewport : public Viewport{
	glm::vec2 relTopLeft,relSize;
public:
	RelativeViewport(rWindow *owner,glm::vec2 topLeft,glm::vec2 size) : 
	      Viewport(owner),
		  relTopLeft(topLeft),
		  relSize(size)
	 
	{
		onResize(owner->getSize());
	}
	virtual ~RelativeViewport(){}
	virtual void onResize(glm::ivec2 newSize){
		this->topLeft = relTopLeft * glm::vec2(newSize);
		this->size = relSize * glm::vec2(newSize);
	}
};

enum Anchorpoint{
	TOPLEFT,
	TOP,
	TOPRIGHT,
	LEFT,
	CENTER,
	RIGHT,
	BOTTOMLEFT,
	BOTTOM,
	BOTTOMRIGHT
};

class AnchoredViewport : public Viewport{
	Anchorpoint _anchorpoint;

public:
	AnchoredViewport(rWindow *owner,Anchorpoint anchorpoint,glm::ivec2 size) : 
	      Viewport(owner)
	 
	{
		_anchorpoint = anchorpoint;
		this->size = size;
		onResize(owner->getSize());
	}
	virtual ~AnchoredViewport(){}

	void setAnchorpoint(Anchorpoint point){_anchorpoint = point;onResize(owner->getSize());}

	virtual void onResize(glm::ivec2 newSize){
		glm::ivec2 mid = newSize / 2;
		glm::ivec2 half = size / 2;

		switch(_anchorpoint){
		case TOPLEFT:
			topLeft = glm::ivec2(0,newSize.y - size.y);
			break;
		case TOP:
			topLeft = glm::ivec2(mid.x - half.x,newSize.y - size.y);
			break;
		case TOPRIGHT:
			topLeft = glm::ivec2(newSize.x - size.x,newSize.y - size.y);
			break;
		case LEFT:
			topLeft = glm::ivec2(0,mid.y - half.y);
			break;
		case CENTER:
			topLeft = glm::ivec2(mid.x - half.x,mid.y - half.y);
			break;
		case RIGHT:
			topLeft = glm::ivec2(newSize.x -size.x,mid.y - half.y);
			break;
		case BOTTOMLEFT:
			topLeft = glm::ivec2(0,0);
			break;
		case BOTTOM:
			topLeft = glm::ivec2(mid.x - half.x,0);
			break;
		case BOTTOMRIGHT:
			topLeft = glm::ivec2(newSize.x-size.x,0);
			break;
		}
	}
};


class RelativeAnchoredViewport : public Viewport{
	Anchorpoint _anchorpoint;
	glm::vec2 relSize;
public:
	RelativeAnchoredViewport(rWindow *owner,Anchorpoint anchorpoint,glm::vec2 size) : 
	      Viewport(owner)
	{
		_anchorpoint = anchorpoint;
		this->relSize = size;
		onResize(owner->getSize());
	}
	virtual ~RelativeAnchoredViewport(){}

	void setAnchorpoint(Anchorpoint point){_anchorpoint = point;onResize(owner->getSize());}

	virtual void onResize(glm::ivec2 newSize){
		size = glm::vec2(newSize) * relSize;
		glm::ivec2 mid = newSize / 2;
		glm::ivec2 half = size / 2;
		
		switch(_anchorpoint){
		case TOPLEFT:
			topLeft = glm::ivec2(0,newSize.y - size.y);
			break;
		case TOP:
			topLeft = glm::ivec2(mid.x - half.x,newSize.y - size.y);
			break;
		case TOPRIGHT:
			topLeft = glm::ivec2(newSize.x - size.x,newSize.y - size.y);
			break;
		case LEFT:
			topLeft = glm::ivec2(0,mid.y - half.y);
			break;
		case CENTER:
			topLeft = glm::ivec2(mid.x - half.x,mid.y - half.y);
			break;
		case RIGHT:
			topLeft = glm::ivec2(newSize.x -size.x,mid.y - half.y);
			break;
		case BOTTOMLEFT:
			topLeft = glm::ivec2(0,0);
			break;
		case BOTTOM:
			topLeft = glm::ivec2(mid.x - half.x,0);
			break;
		case BOTTOMRIGHT:
			topLeft = glm::ivec2(newSize.x-size.x,0);
			break;
		}
	}
};

#endif