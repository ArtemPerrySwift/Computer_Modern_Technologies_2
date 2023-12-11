#pragma once
#include <array>
template <size_t GEOM_DIMENSION_COUNT, size_t MAGN_DIMENSION_COUNT>
struct Reciver {
	std::array<double, GEOM_DIMENSION_COUNT> point;
	std::array<double, MAGN_DIMENSION_COUNT> B;
};