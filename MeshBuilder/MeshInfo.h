#pragma once
#include "SplittingIntervalInfo.h"
#include "AreaInfo.h"
#include <vector>

template <size_t DIMENSION_COUNT>
struct BasicMeshInfo {
public:
	std::array<std::vector<double>, DIMENSION_COUNT> _coordPerDim;
	std::array<std::vector<SplittingIntervalInfo>, DIMENSION_COUNT> _intervalPerDim;
};

template <size_t DIMENSION_COUNT, class AreaType = AreaInfo<DIMENSION_COUNT>> requires std::is_base_of<AreaInfo<DIMENSION_COUNT>, AreaType>::value
struct MeshInfo : public BasicMeshInfo<DIMENSION_COUNT>
{
public:
	std::vector<AreaType> areasInfo;
};