#include "SplitInfo.h"

SplitInfo::SplitInfo() : sparseRatio{ 1.0 }, intervalsCount{ 1 }
{

}

SplitInfo::SplitInfo(double sparseRatio, size_t intervalsCount) : sparseRatio{ sparseRatio }, intervalsCount{ intervalsCount }
{

}