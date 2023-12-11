#pragma once

struct SplitInfo
{
	double sparseRatio;
	size_t intervalsCount;
	SplitInfo();
	SplitInfo(double sparseRatio, size_t intervalsCount);
};

