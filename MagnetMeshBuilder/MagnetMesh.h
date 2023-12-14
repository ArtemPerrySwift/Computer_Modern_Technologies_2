#pragma once
#include "..\MeshBuilder\Mesh.h"
#include "MagnetMeshInfo.h"
#include "MagnetArea.h"
#include "MagnetElement.h"

template <size_t Num1, size_t Num2>
concept is_less = Num1 < Num2;

template <size_t GEOM_DIMENSION_COUNT, size_t MAGN_DIMENSION_COUNT>
class MagnetMesh : public BasicMesh<GEOM_DIMENSION_COUNT> {
private:
	//std::vector<GeometryElement<DIMENSION_COUNT>> _geometryElements;
	std::vector<MagnetArea<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>> _areas;

public:
	MagnetMesh(MeshInfo<GEOM_DIMENSION_COUNT, MagnetAreaInfo<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& meshInfo) : BasicMesh<GEOM_DIMENSION_COUNT>(meshInfo)
	{
		for (const MagnetAreaInfo<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& areaInfo : meshInfo.areasInfo)
			_areas.emplace_back(BasicMesh<GEOM_DIMENSION_COUNT>::getIntervals(), areaInfo);
	}

	const std::vector<MagnetArea<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& getAreas() {
		return _areas;
	}

	void getElements(std::vector<MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>> magnetElements) const
	{
		int elementsCount = 0;
		for (const MagnetArea<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& area : _areas)
			elementsCount += area.getElementsCount();

		magnetElements.reserve(elementsCount);

		for (const MagnetArea<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& area : _areas)
			area.getElements(magnetElements);
	}
};