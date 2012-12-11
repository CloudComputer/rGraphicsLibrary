#include "../FluidSimulator/BoundingVolume.h"


TEST (BoundingAABB, Inside){
	BoundingAABB aabb(glm::vec3(0,0,0),glm::vec3(1,1,1));
	EXPECT_TRUE(aabb.inside(glm::vec3(0,0,0)));
	EXPECT_TRUE(aabb.inside(glm::vec3(1,1,1)));
	EXPECT_TRUE(aabb.inside(glm::vec3(1,0,0)));
	EXPECT_TRUE(aabb.inside(glm::vec3(0,1,0)));
	EXPECT_TRUE(aabb.inside(glm::vec3(0,0,1)));
	EXPECT_TRUE(aabb.inside(glm::vec3(1,1,0)));
	EXPECT_TRUE(aabb.inside(glm::vec3(0,1,1)));
	EXPECT_TRUE(aabb.inside(glm::vec3(1,0,1)));

	EXPECT_TRUE(aabb.inside(glm::vec3(0.5,0.5,0.5)));
}

TEST (BoundingAABB, Outside){
	BoundingAABB aabb(glm::vec3(0,0,0),glm::vec3(1,1,1));
	EXPECT_FALSE(aabb.inside(glm::vec3(-1,-1,-1)));
	EXPECT_FALSE(aabb.inside(glm::vec3(-1,0,0)));
	EXPECT_FALSE(aabb.inside(glm::vec3(0,-1,0)));
	EXPECT_FALSE(aabb.inside(glm::vec3(0,0,-1)));
	EXPECT_FALSE(aabb.inside(glm::vec3(-1,-1,0)));
	EXPECT_FALSE(aabb.inside(glm::vec3(0,-1,-1)));
	EXPECT_FALSE(aabb.inside(glm::vec3(-1,0,-1)));


	EXPECT_FALSE(aabb.inside(glm::vec3(-0.0001,-0.0001,-0.0001)));
	EXPECT_FALSE(aabb.inside(glm::vec3(-0.0001,0,0)));
	EXPECT_FALSE(aabb.inside(glm::vec3(0,-0.0001,0)));
	EXPECT_FALSE(aabb.inside(glm::vec3(0,0,-0.0001)));
	EXPECT_FALSE(aabb.inside(glm::vec3(-0.0001,-0.0001,0)));
	EXPECT_FALSE(aabb.inside(glm::vec3(0,-0.0001,-0.0001)));
	EXPECT_FALSE(aabb.inside(glm::vec3(-0.0001,0,-0.0001)));



	EXPECT_FALSE(aabb.inside(glm::vec3(1.00001,1.00001,1.00001)));
	EXPECT_FALSE(aabb.inside(glm::vec3(1.00001,0,0)));
	EXPECT_FALSE(aabb.inside(glm::vec3(0,1.00001,0)));
	EXPECT_FALSE(aabb.inside(glm::vec3(0,0,1.00001)));
	EXPECT_FALSE(aabb.inside(glm::vec3(1.00001,1.00001,0)));
	EXPECT_FALSE(aabb.inside(glm::vec3(0,1.00001,1.00001)));
	EXPECT_FALSE(aabb.inside(glm::vec3(1.00001,0,1.00001)));

	EXPECT_FALSE(aabb.inside(glm::vec3(-0.5,-0.5,-0.5)));
}