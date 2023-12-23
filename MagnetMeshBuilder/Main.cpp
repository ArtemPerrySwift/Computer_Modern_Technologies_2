#include "MagnetMeshInfoLibconfig.h"
#include "MagnetMesh.h"

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
	return 0;
}