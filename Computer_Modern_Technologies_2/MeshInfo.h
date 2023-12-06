#pragma once
#include "SplittingIntervalInfo.h"
#include "AreaInfo.h"
#include <vector>

template <size_t DIMENSION_COUNT>
struct MeshInfo
{
public:
	std::array<std::vector<double>, DIMENSION_COUNT> _coordPerDim;
	std::array<std::vector<SplittingIntervalInfo>, DIMENSION_COUNT> _intervalPerDim;
	std::vector<AreaInfo<DIMENSION_COUNT>> areasInfo;
};