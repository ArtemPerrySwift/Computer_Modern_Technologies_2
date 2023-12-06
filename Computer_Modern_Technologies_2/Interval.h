#pragma once
struct Interval
{
	const double& leftPoint;
	const double& rightPoint;
	Interval(const double& p1, const double& p2);
	double getLength() const;
	bool isPointBelonsToInterval(double point) const;
};