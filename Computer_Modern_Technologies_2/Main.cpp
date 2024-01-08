#include "MagnetMeshGenerator.h"
#include "MagnetMeshInfoLibconfig.h"
#include "Point.h"
#include <fstream>
const size_t GEOM_DIMENSION_COUNT = 2;
const size_t MAGN_DIMENSION_COUNT = 2;

using PointS = Point<GEOM_DIMENSION_COUNT>;
using MagnetVectorS = std::array<double, MAGN_DIMENSION_COUNT>;

void generatePoints(double leftPoint, double rightPoint, size_t pointsCount, std::vector<PointS>& points);

int main() {
	libconfig::Config cfg;
	getConfigFromFile(cfg, "DirectTask.cfg");
	cfg.setAutoConvert(true);
	libconfig::Setting& rootSetting = getSettingFromConfig(cfg, "Mesh");

	
	std::array<std::string, GEOM_DIMENSION_COUNT> dimCoordNames = { "X", "Z" };
	std::array<std::string, GEOM_DIMENSION_COUNT> dimIntervalNames = { "X", "Z" };
	std::array<std::string, GEOM_DIMENSION_COUNT> dimAreaIntervalNames = { "IntervalXNum", "IntervalZNum" };
	std::array<std::string, MAGN_DIMENSION_COUNT> dimMagnNames = { "pX", "pZ" };
	MeshInfo<GEOM_DIMENSION_COUNT, MagnetAreaInfo<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>> meshInfo;
	MeshLibconfig::setClass(rootSetting, meshInfo, dimCoordNames, dimIntervalNames, dimAreaIntervalNames, dimMagnNames);
	MagnetMesh<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT> mesh(meshInfo);
	std::vector<MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>> geometryElements;
	mesh.getElements(geometryElements);

	std::ifstream in;
	MagnetVectorS earthMagnVect;
	in.open("EarthMagnIndoction");
	for (int i = 0; i < earthMagnVect.size(); i++)
		in >> earthMagnVect[i];
	in.close();

	
	double leftPoint, rigthPoint;
	size_t reciversCount;
	in.open("ReciversMesh");
	in >> leftPoint >> rigthPoint >> reciversCount;
	std::vector<PointS> points;
	generatePoints(leftPoint, rigthPoint, reciversCount - 1, points);
	in.close();

	size_t dataSetSize;
	in.open("DataSetSize");
	in >> dataSetSize;
	in.close();

	MagnetMeshGenerator<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT> meshGen(mesh, points, earthMagnVect);
	double I;
	setVarFromSetting(rootSetting, I, "I");
	meshGen.setI(I);
	while(meshGen.it < dataSetSize)
		meshGen.generateMagnDataSet();

	return 0;
}

void generatePoints(double leftPoint, double rightPoint, size_t intervalsCount, std::vector<PointS>& points) {	
	double step = (rightPoint - leftPoint) / (intervalsCount);
	points.resize(intervalsCount + 1);

	size_t i;
	for (i = 0; i < intervalsCount; i++) {	
		double x = leftPoint + i * step;
		points[i][0] = x;
	}
	points[i][0] = rightPoint;
}
