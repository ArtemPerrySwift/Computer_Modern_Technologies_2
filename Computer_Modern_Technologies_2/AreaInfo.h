#pragma once
#include <array>

template <size_t DIMENSION_COUNT>
struct AreaInfo
{
	std::array<size_t, DIMENSION_COUNT> intervalIndexesPerDim;
};