#include "../FluidSimulator/Interpolator.h"

TEST (LinearInterpolatorTests, vec3){
	LinearInterpolator<glm::vec3> l(glm::vec3(0,0,0),glm::vec3(1,1,1));

	EXPECT_EQ(glm::vec3(0.5,0.5,0.5),l.interpolate(0.5f));
}
