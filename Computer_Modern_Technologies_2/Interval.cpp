#include "Interval.h"
#include <algorithm>

Interval::Interval(const double& p1, const double& p2) : leftPoint{ std::min(p1, p2) }, rightPoint{ std::max(p1, p2) } {

}

double Interval::getLength() const
{
	return rightPoint - leftPoint;
}

bool Interval::isPointBelonsToInterval(double point) const
{
	return leftPoint <= point || point <= rightPoint;
}