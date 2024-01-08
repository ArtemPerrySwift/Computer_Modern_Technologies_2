#include "MagnetMeshInfoLibconfig.h"
#include "MagnetDirectTask.h"
#include "Point.h"
#include <fstream>
#include <iostream>

const size_t POINTS_PER_DIMENSION = 2;
const size_t GEOM_DIMENSION_COUNT = 2;
const size_t MAGN_DIMENSION_COUNT = 2;
const size_t POINTS_ON_ELEM = POINTS_PER_DIMENSION * GEOM_DIMENSION_COUNT;

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

	double I;
	setVarFromSetting(rootSetting, I, "I");

	std::ifstream in;
	MagnetVectorS earthMagnVect;
	in.open("EarthMagnIndoction");
	for (int i = 0; i < earthMagnVect.size(); i++)
		in >> earthMagnVect[i];
	in.close();

	in.open("Mesh_inverse.txt");
	
	double buf,coeff;
	MagnetVectorS elementMagnVect;
	for (size_t i = 0; i < geometryElements.size(); i++) {
		for (size_t j = 0; j < POINTS_ON_ELEM; j++)
			in >> buf;
		in >> coeff;
		for (size_t j = 0; j < elementMagnVect.size(); j++) {
			elementMagnVect[j] = earthMagnVect[j] * coeff;
		
			geometryElements[i].p = elementMagnVect;
		}
	}
	MagnetTaskSimple<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT> magnetDirectTask(geometryElements);
	in.close();

	double leftPoint, rightPoint;
	size_t reciversCount;
	in.open("ReciversMesh");
	in >> leftPoint >> rightPoint >> reciversCount;
	in.close();
	size_t intervalsCount = reciversCount - 1;
	double step = (rightPoint - leftPoint) / (intervalsCount);

	size_t i;
	PointS point;
	std::ofstream out;
	out.open("B.bin", std::ios::binary | std::ios::out);
	std::array<double, MAGN_DIMENSION_COUNT> B;
	for (i = 0; i < intervalsCount; i++) {
		double x = leftPoint + i * step;
		point[0] = x;
		magnetDirectTask.getIndoctValue(point, I, B);
		std::cout << x << " " << B[0] << std::endl;
		out.write((char*)&x, sizeof(x));
		out.write((char*)&B[0], sizeof(B[0]));
	}

	point[0] = rightPoint;
	magnetDirectTask.getIndoctValue(point, I, B);
	out.write((char*)&rightPoint, sizeof(rightPoint));
	out.write((char*)&B[0], sizeof(B[0]));
	out.close();
	//std::vector<PointS> points;
	//generatePoints(leftPoint, rigthPoint, reciversCount - 1, points);
	

	//MagnetMeshGenerator<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT> meshGen(mesh, points, earthMagnVect);
	

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
