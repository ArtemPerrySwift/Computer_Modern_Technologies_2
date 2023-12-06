#pragma once
#include "SplitInfo.h"
class SplittingIntervalInfo
{
private:
	size_t _startIntervalPointInd,
		   _endIntervalPointInd;
public:
	SplitInfo splitInfo;

	SplittingIntervalInfo(size_t startIntervalPointInd, size_t endIntervalPointInd);
	SplittingIntervalInfo(size_t startIntervalPointInd, size_t endIntervalPointInd, const SplitInfo& splitInfo);

	size_t getStartIntervalPointInd() const;
	size_t getEndIntervalPointInd() const;

	size_t setStartIntervalPointInd(size_t startIntervalPointInd);
	size_t setEndIntervalPointInd(size_t endIntervalPointInd);
	size_t setIntervalPointsInd(size_t startIntervalPointInd, size_t endIntervalPointInd);
};

