#include "MeshInfoLibconfig.h"
#include "Mesh.h"
//#include <iostream>
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
	MeshInfo<GEOM_DIMENSION_COUNT> meshInfo;
	MeshLibconfig::setClass(rootSetting, meshInfo, dimCoordNames, dimIntervalNames, dimAreaIntervalNames);
	Mesh<GEOM_DIMENSION_COUNT> mesh(meshInfo);
	std::vector<GeometryElement<GEOM_DIMENSION_COUNT>> geometryElements;
	mesh.getElements(geometryElements);
	/*for (auto& element : geometryElements) {
		std::cout << "x " << element._intervals[0]->leftPoint << " " << element._intervals[0]->rightPoint << " y " << element._intervals[1]->leftPoint << " " << element._intervals[1]->rightPoint << std::endl;
	}
	int a = 0;*/
	return 0;
}