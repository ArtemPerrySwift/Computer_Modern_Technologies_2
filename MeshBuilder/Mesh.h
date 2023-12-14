#pragma once
#include "MeshInfo.h"
#include "SplittedInterval.h"
#include "Area.h"

template<size_t DIMENSION_COUNT>
class BasicMesh {
private:
	std::array<std::vector<SplittedInterval>, DIMENSION_COUNT> _intervals;

public:
	BasicMesh(BasicMeshInfo<DIMENSION_COUNT>& basicMeshInfo) {
		for (size_t iDimension = 0; iDimension < DIMENSION_COUNT; iDimension++)
		{
			_intervals[iDimension].reserve(basicMeshInfo._intervalPerDim[iDimension].size());
			for (const SplittingIntervalInfo& splittingIntervalInfo : basicMeshInfo._intervalPerDim[iDimension])
				_intervals[iDimension].emplace_back(basicMeshInfo._coordPerDim[iDimension], splittingIntervalInfo);
		}
	}
	const std::array<std::vector<SplittedInterval>, DIMENSION_COUNT>& getIntervals() const
	{
		return _intervals;
	}
};


template<size_t DIMENSION_COUNT>
class Mesh : public BasicMesh<DIMENSION_COUNT> {
private:
	//std::vector<GeometryElement<DIMENSION_COUNT>> _geometryElements;
	std::vector<Area<DIMENSION_COUNT>> areas;

public:
	Mesh(MeshInfo<DIMENSION_COUNT>& meshInfo) : BasicMesh<DIMENSION_COUNT>(meshInfo)
	{
		for (const AreaInfo<DIMENSION_COUNT>& areaInfo : meshInfo.areasInfo)
			areas.emplace_back(BasicMesh<DIMENSION_COUNT>::getIntervals(), areaInfo);
	}

	void getElements(std::vector<GeometryElement<DIMENSION_COUNT>>& geometryElements) const
	{
		int elementsCount = 0;
		for (const Area<DIMENSION_COUNT>& area : areas)
			elementsCount += area.getElementsCount();

		geometryElements.reserve(elementsCount);

		for (const Area<DIMENSION_COUNT>& area : areas)
			area.getElements(geometryElements);
	}
};
