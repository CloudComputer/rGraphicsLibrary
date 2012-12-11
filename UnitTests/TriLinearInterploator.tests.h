#include "../FluidSimulator/Interpolator.h"

TEST (TriLinearInterpolatorTests, vec3){
	TriLinearInterpolator<glm::vec3> l(glm::vec3(0,0,0),glm::vec3(0,1,0),glm::vec3(0,0,1),glm::vec3(0,1,1),glm::vec3(1,0,0),glm::vec3(1,1,0),glm::vec3(1,0,1),glm::vec3(1,1,1));

	EXPECT_EQ(glm::vec3(0.5,0.5,0.5),l.interpolate(glm::vec3(0.5,0.5,0.5)));
}
