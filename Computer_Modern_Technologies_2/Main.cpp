#include "MagnetMeshGenerator.h"
#include "MagnetMeshInfoLibconfig.h"
#include "Point.h"
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
	int a = 0;
	//MagnetTaskSimple<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT> directTask(geometryElements);

	std::vector<PointS> points;
	
	MagnetVectorS earthMagnVect;
	for (double& elem : earthMagnVect) elem = 1;

	generatePoints(-100, 100, 500, points);
	MagnetMeshGenerator<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT> meshGen(mesh, points, earthMagnVect);
	meshGen.generateMagnDataSet();
	//std::vector<MagnetElement<2, 2>> elems;
	//MagnetTaskSimple<2,2> directTask(elems);
	////Reciver<2, 2> rec;
	//directTask.getIndoctValue<1>(rec.point, 5);
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
