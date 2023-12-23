#include "MagnetDirectTask.h"
#include "MagnetMeshInfoLibconfig.h"
#include <iostream>
int main()
{
	libconfig::Config cfg;
	getConfigFromFile(cfg, "DirectTask.cfg");
	cfg.setAutoConvert(true);
	libconfig::Setting& rootSetting = getSettingFromConfig(cfg, "Mesh");

	const size_t GEOM_DIMENSION_COUNT = 2;
	const size_t MAGN_DIMENSION_COUNT = 2;
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
	MagnetTaskSimple<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT> directTask(geometryElements);
	size_t pointCount = 500;
	double step = 2000.0 / pointCount;
	for (size_t i = 0; i < pointCount; i++)
	{
		double x = -1000.0 + i * step;
		std::array<double, GEOM_DIMENSION_COUNT> point;
		point[0] = x;
		for (int j = 1; j < GEOM_DIMENSION_COUNT; j++)
			point[j] = 0;
		std::cout << x << " ";
		std::cout.precision(12);
		std::cout << directTask.getIndoctValue<1>(point, 1000.0) << std::endl;
	}
	return 0;
}