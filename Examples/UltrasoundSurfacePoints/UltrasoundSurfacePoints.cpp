#include <fstream>
#include <omp.h>


#include <Engine\DrawableObject.h>

#include <SuperEngine\SuperEngine.h>

#include <Util/StopClock.h>

#include <gl/glfw.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Image\Image.h>
#include <Image\BMPWriter.h>

#include <Geometry\Clustering\Clusterer.h>

#include <string>
#include <iostream>

#include <Volume\UltrasoundSurfacePointExtractor.h>

#include <Math\Plane.h>

#include <Base\XMLObjectHandler.h>

#include <Util\TmpPointer.h>

struct SourceFile{
	char *path;
	int w,h,d;
};

const SourceFile files[] = {
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000013-orig_SC-467x255x124.raw",467,255,124}, //0
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000013-ENH-1_SC-467x255x124.raw",467,255,124}, //1
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000013-ENH-2_SC-467x255x124.raw",467,255,124}, //2
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000013-ENH-3_SC-467x255x124.raw",467,255,124}, //3

	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000016-orig_SC-478x247x79x22.raw",478,247,79}, //4
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000016-ENH-1_SC-478x247x79x22.raw",478,247,79}, //5
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000016-ENH-2_SC-478x247x79x22.raw",478,247,79}, //6
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000016-ENH-3_SC-478x247x79x22.raw",478,247,79}, //7

	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000030_orig_SC-679x307x174.raw",679,307,174}, //8
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000030_ENH-1_SC-679x307x174.raw",679,307,174}, //9
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000030_ENH-2_SC-679x307x174.raw",679,307,174}, //10
	{"C:/Thesis Data/ContextVision/Volumes for rendering/raw/I0000030_ENH-3_SC-679x307x174.raw",679,307,174}, //11

	{"C:/Thesis Data/ContextVision/new volume/I0000027_SC--467x255x101.raw",467,255,101}, //12
	{"C:/Thesis Data/ContextVision/new volume/I0000027_TEMP-1-ENH-3-467x255x101.raw",467,255,101}, //13
	{"C:/Thesis Data/ContextVision/new volume/I0000027_TEMP-1-ENH-3_ENH-1-467x255x101.raw",467,255,101}, //14

	{"C:/Thesis Data/ContextVision/2012-12-10/I0000027-enh3_SC-fLOOD-sc-0-516x244x124.raw",516,244,124}, //15
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000027-enh3_SC-fLOOD-sc-0-n516x244x124.raw",516,244,124}, //16
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000027-enh3_SC-fLOOD-sc-0_2-516x244x124.raw",516,244,124}, //17
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000027-enh3_SC-fLOOD-sc-0_3-516x244x124.raw",516,244,124}, //18
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000027-enh3_SC-fLOOD-sc-0_4-516x244x124.raw",516,244,124}, //19
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000027_SC516x244x124.raw",516,244,124}, //20

	{"C:/Thesis Data/ContextVision/2012-12-10/I0000029-ENH3_SC-fLOOD-sc-1-688x404x124.raw",688,404,124}, //21
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000029-ENH3_SC-fLOOD-sc-1-n688x404x124.raw",688,404,124}, //22
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000029-ENH3_SC-fLOOD-sc-1_2-688x404x124.raw",688,404,124}, //23
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000029-ENH3_SC-fLOOD-sc-1_3-688x404x124.raw",688,404,124}, //24
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000029-ENH3_SC-fLOOD-sc-1_4-688x404x124.raw",688,404,124}, //25
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000029_SC-688x404x124.raw",688,404,124}, //26

	{"C:/Thesis Data/ContextVision/2012-12-10/I0000042-ENH3_SC-fLOOD-sc-0-516x244x124.raw",516,244,124}, //27
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000042-ENH3_SC-fLOOD-sc-0-n516x244x124.raw",516,244,124}, //28
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000042-ENH3_SC-fLOOD-sc-0_2-516x244x124.raw",516,244,124}, //29
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000042-ENH3_SC-fLOOD-sc-0_3-516x244x124.raw",516,244,124}, //30
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000042-ENH3_SC-fLOOD-sc-0_4-516x244x124.raw",516,244,124}, //31
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000042_SC516x244x124.raw",516,244,124}, //32

	{"C:/Thesis Data/ContextVision/2012-12-10/I0000047-ENH3_SC-fLOOD-sc-2-624x327x124.raw" ,624,327,124}, //33
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000047-ENH3_SC-fLOOD-sc-2-n624x327x124.raw",624,327,124}, //34
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000047-ENH3_SC-fLOOD-sc-2_2-624x327x124.raw",624,327,124}, //35
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000047-ENH3_SC-fLOOD-sc-2_3-624x327x124.raw",624,327,124}, //36
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000047_SC624x327x124.raw",624,327,124}, //37

	{"C:/Thesis Data/ContextVision/2012-12-10/I0000048-ENH3_SC-fLOOD-sc-3-n645x432x124.raw",645,432,124}, //38
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000048-ENH3_SC-fLOOD-sc-3_2_645x432x124.raw",645,432,124}, //39
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000048-ENH3_SC-fLOOD-sc-3_3_645x432x124.raw",645,432,124}, //40
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000048-ENH3_SC-fLOOD-sc-3_4_645x432x124.raw",645,432,124}, //41
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000048-ENH3_SC-fLOOD-sc-3_645x432x124.raw",645,432,124}, //42
	{"C:/Thesis Data/ContextVision/2012-12-10/I0000048_SC645x432x124.raw",645,432,124} //43

};
const unsigned int numDatasets = 44;
	
int niceCases[]= {3,7,11,14,19,25,31,42};
const int numNiceCases = 8;
int _case = 3;



class PointRenderer : public DrawableObject{
	std::vector<Vertex> points;
	std::vector<glm::vec3> surfPoints;
public:
	PointRenderer(){
		TmpPointer<ScalarField> tmp = ScalarField::ReadFromRawfile(files[niceCases[_case]].path,files[niceCases[_case]].w,files[niceCases[_case]].h,files[niceCases[_case]].d);
		surfPoints = UltrasoundSurfacePointExtractor::ExtractPoints(tmp.get(),Z_AXIS);
		
		//TmpPointer<ScalarField> vol = tmp->blur();

		/*auto tree = vol->getSurfacePoints(0.4);
		
		glm::vec3 dir = vol->getBoundingAABB().maxPos() - vol->getBoundingAABB().minPos();
		dir /= vol->getDimensions();

		auto clusters = Clusterer::ClusterPoints(tree,glm::length(dir),100,true);

		points = clusters[0].getPoints()->getAsVector();*/

	}


	virtual void draw(){
		glColor3f(1,1,1);
		glPointSize(4);
		glBegin(GL_POINTS);
		for(auto p = surfPoints.begin();p!=surfPoints.end();++p){
			glVertex3fv(glm::value_ptr(*p));
		}
		glEnd();
	}


	virtual std::string toString()const{return "PointRenderer";}
};




void init(int argc,char **argv,Engine *t){
	static_cast<SuperGraphicsEngine*>(SuperEngine::getEngine()->getGraphicEngine())->addDrawableObject(new PointRenderer());
}

ENGINE_MAIN(SuperEngine::CreateEngine(),init)