#pragma once
#include "..\MeshBuilder\Area.h"
#include "MagnetAreaInfo.h"
#include "MagnetInfo.h"
#include "MagnetElement.h"

template <size_t GEOM_DIMENSION_COUNT, size_t MAGN_DIMENSION_COUNT>
class MagnetArea : public Area<GEOM_DIMENSION_COUNT>, public MagnetInfo<MAGN_DIMENSION_COUNT> {
private:
	template<size_t DIMENSION_NUMBER>
	void iterateOverDimension(std::array<const std::vector<Interval>*, GEOM_DIMENSION_COUNT>& splitIntervals, std::vector<MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& magnetElements, std::array<size_t, GEOM_DIMENSION_COUNT>& ind) const
	{
		const int dimensionIndex = DIMENSION_NUMBER - 1;
		for (ind[dimensionIndex] = 0; ind[dimensionIndex] < splitIntervals[dimensionIndex]->size(); ind[dimensionIndex]++)
			iterateOverDimension<DIMENSION_NUMBER - 1>(splitIntervals, magnetElements, ind);
	}

	template<>
	void iterateOverDimension<0LL>(std::array<const std::vector<Interval>*, GEOM_DIMENSION_COUNT>& splitIntervals, std::vector<MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& magnetElements, std::array<size_t, GEOM_DIMENSION_COUNT>& ind) const
	{
		std::array<const Interval*, GEOM_DIMENSION_COUNT> elemIntervals;
		for (int i = 0; i < GEOM_DIMENSION_COUNT; i++)
		{
			const std::vector<Interval>& curIntervDim = *splitIntervals[i];
			elemIntervals[i] = &(curIntervDim[ind[i]]);
		}
		GeometryElement geometryElement(elemIntervals);
		magnetElements.emplace_back(geometryElement, (MagnetInfo<MAGN_DIMENSION_COUNT>&)(*this));
	}

public:
	MagnetArea(const std::array<std::vector<SplittedInterval>, GEOM_DIMENSION_COUNT>& intervalsDim, const MagnetAreaInfo<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& magnetAreaInfo) : Area<GEOM_DIMENSION_COUNT>(intervalsDim, /*(const AreaInfo<GEOM_DIMENSION_COUNT>&)*/magnetAreaInfo), MagnetInfo<MAGN_DIMENSION_COUNT>(magnetAreaInfo)
	{

	}
	/*MagnetArea(const std::array<std::vector<SplittedInterval>, GEOM_DIMENSION_COUNT>& intervalsDim, const AreaInfo<GEOM_DIMENSION_COUNT>& areaInfo, const MagnetInfo<MAGN_DIMENSION_COUNT>& magnetInfo) :
		Area<GEOM_DIMENSION_COUNT>(intervalsDim, areaInfo),
		MagnetInfo<MAGN_DIMENSION_COUNT>(magnetInfo)
	{

	}*/
	//MagnetArea() {};
	void getElements(std::vector<MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& geometryElements) const {
		std::array<size_t, GEOM_DIMENSION_COUNT> ind;
		std::array<const std::vector<Interval>*, GEOM_DIMENSION_COUNT> splitIntervals;
		const std::array<const SplittedInterval*, GEOM_DIMENSION_COUNT>& intervals = Area<GEOM_DIMENSION_COUNT>::getIntervals();
		for (int i = 0; i < GEOM_DIMENSION_COUNT; i++) {
			splitIntervals[i] = &(intervals[i]->getSubIntervals());
		}
		iterateOverDimension<GEOM_DIMENSION_COUNT>(splitIntervals, geometryElements, ind);
	}
};