#include "SplittedInterval.h"
#include <cmath>

SplittedInterval::SplittedInterval(const std::vector<double>& basePoints, const SplittingIntervalInfo& splittingIntervalInfo)
{
	if (splittingIntervalInfo.getStartIntervalPointInd() >= basePoints.size())
		throw std::exception("Start point number in interval cannot be equal or greater than points count");

	if (splittingIntervalInfo.getEndIntervalPointInd() >= basePoints.size())
		throw std::exception("end point number in interval cannot be equal or greater than points count");

	double startIntervalPointInd, endIntervalPointInd;
	startIntervalPointInd = basePoints[splittingIntervalInfo.getStartIntervalPointInd()];
	endIntervalPointInd = basePoints[splittingIntervalInfo.getEndIntervalPointInd()];

	if (endIntervalPointInd == startIntervalPointInd)
		throw std::exception("Interval fro splitting referes to equal values");

	splitInterval(startIntervalPointInd, endIntervalPointInd, splittingIntervalInfo);
}

const std::vector<double>& SplittedInterval::getIntervalPoints() const
{
	return _points;
}

void SplittedInterval::splitInterval(double startIntervalPointInd, double endIntervalPointInd, const SplittingIntervalInfo& intervalForSplit)
{
	int n = intervalForSplit.splitInfo.intervalsCount;
	double q = intervalForSplit.splitInfo.sparseRatio;

	if (startIntervalPointInd > endIntervalPointInd)
	{
		std::swap(startIntervalPointInd, endIntervalPointInd);
		q = 1 / q;
		//std::cout << "Interval has reverse order of points (from bigger to smaller value) therefore it was reversed.";
	}

	_points.reserve(n + 1);
	_points.emplace_back(startIntervalPointInd);

	double h;
	if (q == 1)
	{
		h = (endIntervalPointInd - startIntervalPointInd) / n;

		for (int i = 1; i < n; i++)
		{
			_points.emplace_back(startIntervalPointInd + i * h);
		}
	}
	else
	{
		h = (endIntervalPointInd - startIntervalPointInd) / (1 - std::pow(q, n)) * (1 - q);
		double previousValue = startIntervalPointInd;
		for (int i = 0; i < n; i++)
		{
			_points.emplace_back(previousValue + h);
			previousValue += h;
			h *= q;
		}
	}
	_points.emplace_back(endIntervalPointInd);

	_subIntervals.reserve(n);
	for (int i = 0; i < n; i++)
		_subIntervals.emplace_back(_points[i], _points[i + 1]);

}

const std::vector<Interval>& SplittedInterval::getSubIntervals() const
{
	return _subIntervals;
}
