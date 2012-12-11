#include "../FluidSimulator/BoundingVolume.h"

TEST (BoundingSphere, Inside){
	BoundingSphere s(glm::vec3(0,0,0),1);

	EXPECT_TRUE(s.inside(glm::vec3(0,0,0)));
	EXPECT_TRUE(s.inside(glm::vec3(0,0,1)));
	EXPECT_TRUE(s.inside(glm::vec3(0,1,0)));
	EXPECT_TRUE(s.inside(glm::vec3(1,0,0)));
	EXPECT_TRUE(s.inside(glm::vec3(0,0,-1)));
	EXPECT_TRUE(s.inside(glm::vec3(0,-1,0)));
	EXPECT_TRUE(s.inside(glm::vec3(-1,0,0)));
}

TEST (BoundingSphere, Outside){
	BoundingSphere s(glm::vec3(0,0,0),1);

	EXPECT_FALSE(s.inside(glm::vec3(0,1,1)));
	EXPECT_FALSE(s.inside(glm::vec3(1,1,0)));
	EXPECT_FALSE(s.inside(glm::vec3(1,1,0)));
	EXPECT_FALSE(s.inside(glm::vec3(0,1,-1)));
	EXPECT_FALSE(s.inside(glm::vec3(1,-1,0)));
	EXPECT_FALSE(s.inside(glm::vec3(-1,0,1)));
}