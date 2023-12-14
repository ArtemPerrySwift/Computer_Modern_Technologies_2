#pragma once
#include "SplittedInterval.h"
#include "GeometryElement.h"
#include "AreaInfo.h"
#include <string>

template <size_t DIMENSION_COUNT>
class Area
{
private:
	std::array<const SplittedInterval*, DIMENSION_COUNT> _intervals;

	template<size_t DIMENSION_NUMBER> //requires is_less<dimNum, DIM_COUNT + 1>
	void iterateOverDimension(std::array<const std::vector<Interval>*, DIMENSION_COUNT>& splitIntervals, std::vector<GeometryElement<DIMENSION_COUNT>>& geometryElements, std::array<size_t, DIMENSION_COUNT>& ind) const
	{
		const int dimensionIndex = DIMENSION_NUMBER - 1;
		for (ind[dimensionIndex] = 0; ind[dimensionIndex] < splitIntervals[dimensionIndex]->size(); ind[dimensionIndex]++)
			iterateOverDimension<DIMENSION_NUMBER - 1>(splitIntervals, geometryElements, ind);
	}

	template<>
	void iterateOverDimension<0>(std::array<const std::vector<Interval>*, DIMENSION_COUNT>& splitIntervals, std::vector<GeometryElement<DIMENSION_COUNT>>& geometryElements, std::array<size_t, DIMENSION_COUNT>& ind) const
	{
		std::array<const Interval*, DIMENSION_COUNT> elemIntervals;
		for (int i = 0; i < DIMENSION_COUNT; i++)
		{
			const std::vector<Interval>& curIntervDim = *splitIntervals[i];
			elemIntervals[i] = &(curIntervDim[ind[i]]);
		}
		geometryElements.emplace_back(elemIntervals);
	}

public:
	Area(const std::array<std::vector<SplittedInterval>, DIMENSION_COUNT>& intervalsDim, const AreaInfo<DIMENSION_COUNT>& areaInfo) {
		for (int iDimension = 0; iDimension < DIMENSION_COUNT; iDimension++) {
			if (areaInfo.intervalIndexesPerDim[iDimension] >= intervalsDim[iDimension].size()) {
				std::string err{ "Count of intevals for dimension " };
				err += std::to_string(iDimension) + " cannot be less than 0";
				throw std::exception(err.c_str());
			}
			//const SplittedInterval& buf = &((*(intervalsDim[i]))[areaInfo.intervalNumberDim[i]]);
			_intervals[iDimension] = &(intervalsDim[iDimension][areaInfo.intervalIndexesPerDim[iDimension]]);
		}
		//std::array<const SplittedInterval&, DIMENSION_COUNT> intervals;
	}

	const std::array<const SplittedInterval*, DIMENSION_COUNT>& getIntervals() const
	{
		return _intervals;
	}

	//template<size_t dimNum> requires is_less<dimNum, DIMENSION_COUNT>
	//const SplittedInterval& getInterval() const {
	//	return *_intervals[dimNum];
	//}


	void getElements(std::vector<GeometryElement<DIMENSION_COUNT>>& geometryElements) const {
		std::array<size_t, DIMENSION_COUNT> ind;
		std::array<const std::vector<Interval>*, DIMENSION_COUNT> splitIntervals;
		for (int i = 0; i < DIMENSION_COUNT; i++) {
			splitIntervals[i] = &(_intervals[i]->getSubIntervals());
		}
		iterateOverDimension<DIMENSION_COUNT>(splitIntervals, geometryElements, ind);
	}

	int getElementsCount() const {
		int elementsCount = 1;
		for (int i = 0; i < DIMENSION_COUNT; i++)
			elementsCount *= _intervals[i]->getSubIntervals().size();
		return elementsCount;
	}
};


template <size_t DIMENSION_COUNT>
class ElemNumberFunct
{
public:
	ElemNumberFunct(Area<DIMENSION_COUNT>& area) {
		const std::array<const SplittedInterval*, DIMENSION_COUNT>& intervals = area.getIntervals();
		for (int i = 0; i < DIMENSION_COUNT; i++) {
			_dimElemsCount[i] = 1;
			for (int j = i - 1; j > -1; j--)
				_dimElemsCount[i] *= _dimElemsCount[j] * intervals[j]->getSubIntervals().size();
		}
	}

	size_t operator()(const std::array<size_t, DIMENSION_COUNT>& intervalDimInd) const {
		size_t ind = 0;
		for (int i = 0; i < DIMENSION_COUNT; i++)
			ind += intervalDimInd[i] * _dimElemsCount[i];

		return ind;
	}
private:
	std::array<size_t, DIMENSION_COUNT> _dimElemsCount;
};