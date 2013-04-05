#pragma once

#include <glm/glm.hpp>

#include <Base\Object.h>

#define within(x,a,b) (x >= a && x <= b)

class BoundingVolume : public Object
{
public:
	BoundingVolume(void);
	virtual ~BoundingVolume(void);

	virtual bool inside(glm::vec3 pos)const = 0;
	virtual void extend(glm::vec3 pos) = 0;
};

class BoundingSphere : public BoundingVolume{
	float _radius;
	glm::vec3 _center;

public:
	BoundingSphere(glm::vec3 center,float radius){
		_center = center;
		_radius = radius;
	}

	virtual void extend(glm::vec3 pos){
		if(!inside(pos)){
			_radius = glm::distance(pos,_center);
		}
	}

	virtual bool inside(glm::vec3 pos)const{
		return glm::distance(pos,_center) <= _radius;
	}

	virtual std::string toString()const{
		std::stringstream ss;
		ss << "Bounding sphere at [" << _center.x  << " "<< _center.y << " "  << _center.z << "] with radius: " << _radius;
		return ss.str();
	}
};


class BoundingAABB : public BoundingVolume{
	glm::vec3 _minPos;
	glm::vec3 _maxPos;

public:
	BoundingAABB(glm::vec3 minPos,glm::vec3 maxPos){
		_minPos = minPos;
		_maxPos = maxPos;
	}

	BoundingAABB(const BoundingAABB &b){
		_minPos = b._minPos;
		_maxPos = b._maxPos;
	}

	virtual void extend(glm::vec3 pos){
		_minPos = glm::min(_minPos,pos);
		_maxPos = glm::max(_maxPos,pos);
	}

	virtual bool inside(glm::vec3 pos)const{
		return within(pos.x,_minPos.x,_maxPos.x) && within(pos.y,_minPos.y,_maxPos.y) && within(pos.z,_minPos.z,_maxPos.z);
	}
	
	glm::vec3 &minPos(){return _minPos;}
	glm::vec3 &maxPos(){return _maxPos;}

	glm::vec3 getInternalPosition(glm::vec3 worldPos)const;
	glm::vec3 getPosition( glm::vec3 t )const;
	glm::vec3 getDiscretePosition( glm::vec3 pos, glm::ivec3 dim )const;

	virtual std::string toString()const{
		std::stringstream ss;
		ss << "Bounding AABB between [" << _minPos.x  << " "<< _minPos.y << " "  << _minPos.z << "] and [ "  << _maxPos.x  << " "<< _maxPos.y << " "  << _maxPos.z << "] ";
		return ss.str();
	}

#ifdef GL_VERSION_1_1
	void draw()const{
		glPushAttrib(GL_LIGHTING_BIT);
		glPushAttrib(GL_CURRENT_BIT);
		glColor3f(1,1,1);
		glDisable(GL_LIGHTING);
		float _vertices[8][3] = {{ 
			_minPos.x,_minPos.y,_minPos.z},
			{_maxPos.x,_minPos.y,_minPos.z},
			{_maxPos.x,_minPos.y,_maxPos.z},
			{_minPos.x,_minPos.y,_maxPos.z},
			{_minPos.x,_maxPos.y,_minPos.z},
			{_maxPos.x,_maxPos.y,_minPos.z},
			{_maxPos.x,_maxPos.y,_maxPos.z},
			{_minPos.x,_maxPos.y,_maxPos.z}};

		glBegin(GL_LINE_LOOP);
			glVertex3fv(_vertices[0]);
			glVertex3fv(_vertices[1]);
			glVertex3fv(_vertices[2]);
			glVertex3fv(_vertices[3]);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex3fv(_vertices[4]);
			glVertex3fv(_vertices[5]);
			glVertex3fv(_vertices[6]);
			glVertex3fv(_vertices[7]);
		glEnd();

		glBegin(GL_LINES);
			glVertex3fv(_vertices[0]);
			glVertex3fv(_vertices[4]);

			glVertex3fv(_vertices[1]);
			glVertex3fv(_vertices[5]);

			glVertex3fv(_vertices[2]);
			glVertex3fv(_vertices[6]);

			glVertex3fv(_vertices[3]);
			glVertex3fv(_vertices[7]);
		glEnd();


		glPopAttrib();
		glPopAttrib();
	}
#endif

};
