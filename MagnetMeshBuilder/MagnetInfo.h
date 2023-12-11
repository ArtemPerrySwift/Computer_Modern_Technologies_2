#pragma once
#include <array>

template <size_t DIMENSION_COUNT>
struct MagnetInfo {
public:
	std::array<double, DIMENSION_COUNT> p;
};