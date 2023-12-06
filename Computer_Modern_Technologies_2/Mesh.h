#pragma once
#include "MeshInfo.h"
#include "SplittedInterval.h"
#include "Area.h"
template<size_t DIMENSION_COUNT = 1>
class Mesh {
private:
	std::array<std::vector<SplittedInterval>, DIMENSION_COUNT> _intervals;
	std::vector<GeometryElement<DIMENSION_COUNT>> _geometryElements;
	void setMesh(MeshInfo<DIMENSION_COUNT>& meshInfo, std::vector<Area<DIMENSION_COUNT>>& areas)
	{
		for (size_t iDimension = 0; iDimension < DIMENSION_COUNT; iDimension++)
		{
			_intervals[iDimension].reserve(meshInfo._intervalPerDim[iDimension].size());
			for (const SplittingIntervalInfo& splittingIntervalInfo : meshInfo._intervalPerDim[iDimension])
				_intervals[iDimension].emplace_back(meshInfo._coordPerDim[iDimension], splittingIntervalInfo);
		}

		/*std::array<const std::vector<SplittedInterval>*, DIMENSION_COUNT> intervalsDim;
		for (size_t iDimension = 0; iDimension < DIMENSION_COUNT; iDimension++)
		//{
			//intervalsDim[iDimension] = &_intervals[iDimension];
		//}*/

		//for (const AreaInfo<DIMENSION_COUNT>& areaInfo : meshInfo.areasInfo)
		//	areas.emplace_back(_intervals, areaInfo);

		for (const AreaInfo<DIMENSION_COUNT>& areaInfo : meshInfo.areasInfo)
			areas.emplace_back(_intervals, areaInfo);

		int elementsCount = 0;
		for (const Area<DIMENSION_COUNT>& area : areas)
			elementsCount += area.getElementsCount();

		//_magneticElements.clear();
		_geometryElements.reserve(elementsCount);

		for (const Area<DIMENSION_COUNT>& area : areas)
			area.getElements(_geometryElements);
	}

public:
	Mesh(MeshInfo<DIMENSION_COUNT>& meshInfo)
	{
		std::vector<Area<DIMENSION_COUNT>> areas;
		setMesh(meshInfo, areas);
	}
	Mesh(MeshInfo<DIMENSION_COUNT>& meshInfo, std::vector<Area<DIMENSION_COUNT>>& areas)
	{
		setMesh(meshInfo, areas);
	}
	const std::array<std::vector<SplittedInterval>, DIMENSION_COUNT>& getIntervals() const
	{
		return _intervals;
	}
	const std::vector<GeometryElement<DIMENSION_COUNT>>& getGeometryElements() const
	{
		return _geometryElements;
	}
};
