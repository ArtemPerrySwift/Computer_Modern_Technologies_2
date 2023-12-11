#include "SplittingIntervalInfo.h"
#include <exception>

SplittingIntervalInfo::SplittingIntervalInfo()
{
	_startIntervalPointInd = 0ULL;
	_endIntervalPointInd = 1ULL;
}
SplittingIntervalInfo::SplittingIntervalInfo(size_t startIntervalPointInd, size_t endIntervalPointInd)
{
	setIntervalPointsInd(startIntervalPointInd, endIntervalPointInd);
}
SplittingIntervalInfo::SplittingIntervalInfo(size_t startIntervalPointInd, size_t endIntervalPointInd, const SplitInfo& splitInfo): splitInfo{ splitInfo }
{
	setIntervalPointsInd(startIntervalPointInd, endIntervalPointInd);
}

size_t SplittingIntervalInfo::getStartIntervalPointInd() const
{
	return _startIntervalPointInd;
}
size_t SplittingIntervalInfo::getEndIntervalPointInd() const
{
	return _endIntervalPointInd;
}

size_t SplittingIntervalInfo::setStartIntervalPointInd(size_t startIntervalPointInd)
{
	if (startIntervalPointInd == _endIntervalPointInd)
		throw std::exception("Attemp to assign index of end point to start point");
	_startIntervalPointInd = startIntervalPointInd;
}
size_t SplittingIntervalInfo::setEndIntervalPointInd(size_t endIntervalPointInd)
{
	if (endIntervalPointInd == _startIntervalPointInd)
		throw std::exception("Attemp to assign index of start point to end point");
	_endIntervalPointInd = endIntervalPointInd;
}

size_t SplittingIntervalInfo::setIntervalPointsInd(size_t startIntervalPointInd, size_t endIntervalPointInd)
{
	if(startIntervalPointInd == endIntervalPointInd)
		throw std::exception("Attemp to assign index of start point and end point same value");

	_startIntervalPointInd = startIntervalPointInd;
	_endIntervalPointInd = endIntervalPointInd;
}
