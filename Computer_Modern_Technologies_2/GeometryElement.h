#pragma once
#include "Interval.h"
#include <array>

template<size_t DIMENSION_COUNT>
struct GeometryElement {
	const std::array<const Interval*, DIMENSION_COUNT> _intervals;

	GeometryElement(const std::array<const Interval*, DIMENSION_COUNT>& intervals) : _intervals(intervals)
	{
	}

	double getMetrics() const
	{
		double metrics = 1;
		for (int i = 0; i < DIMENSION_COUNT; i++)
			metrics *= _intervals[i]->getLength();
		return metrics;
	}
};
