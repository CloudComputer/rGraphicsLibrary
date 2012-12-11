#include "../FluidSimulator/Mesh.h"


TEST (Mesh, LoadFromWavefront){
	Mesh *m = Mesh::LoadWavefront("C:/Users/rickard/Dropbox/LIU/år 1/2011 VY P2 - Animation and Modeling/Objs/cow.obj");
}

