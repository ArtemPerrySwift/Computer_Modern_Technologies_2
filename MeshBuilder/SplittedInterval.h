#pragma once
#include "Interval.h"
#include "SplittingIntervalInfo.h"
#include <vector>

class SplittedInterval
{
private:
	std::vector<double> _points;
	std::vector<Interval> _subIntervals;
	void splitInterval(double p1, double p2, const SplittingIntervalInfo& SplittingIntervalInfo);
public:
	SplittedInterval(const std::vector<double>& basePoints, const SplittingIntervalInfo& SplittingIntervalInfo);
	const std::vector<double>& getIntervalPoints() const;
	const std::vector<Interval>& getSubIntervals() const;
};

