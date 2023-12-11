#include "GausseIntegrBase.h"

GausseIntegrBase::GausseIntegrBase()
{
	setIntegrOrder_3();
}

const std::vector<double>& GausseIntegrBase::getGaussePoints() const {
	return _gaussePoints;
}
const std::vector<double>& GausseIntegrBase::getGausseWights() const {
	return _gausseWeights;
}

void GausseIntegrBase::setIntegrOrder_1() {
	_gaussePoints = { 0.5 };
	_gausseWeights = { 2.0 };
}

void GausseIntegrBase::setIntegrOrder_2() {
	_gaussePoints = { 0.5 + 1 / (2 * sqrt(3)), 0.5 - 1 / (2 * sqrt(3)) };
	_gausseWeights = { 1.0, 1.0 };
}
void GausseIntegrBase::setIntegrOrder_3() {
	_gaussePoints = { 0.5 , 0.5 + sqrt(3.0 / 5.0) / 2,  0.5 - sqrt(3.0 / 5.0) / 2 };
	_gausseWeights = { 8.0 / 9.0, 5.0 / 9.0 };
}

void GausseIntegrBase::setIntegrOrder_4() {
	double buf[3];
	buf[0] = 2 * sqrt(6.0 / 5.0);
	buf[1] = sqrt((3.0 - buf[0]) / 7.0);
	buf[2] = sqrt((3.0 + buf[0]) / 7.0);
	_gaussePoints = { 0.5 + buf[1] / 2 , 0.5 - buf[1] / 2, 0.5 + buf[2] / 2 , 0.5 - buf[2] / 2 };
	buf[0] = sqrt(30.0);
	buf[1] = (18.0 + buf[0]) / 36.0;
	buf[2] = (18.0 - buf[0]) / 36.0;
	_gausseWeights = { buf[1], buf[1], buf[2], buf[2] };
}

void GausseIntegrBase::setIntegrOrder_5() {
	double buf[3];
	buf[0] = 2 * sqrt(10.0 / 7.0);
	buf[1] = 1.0 / 3.0 * sqrt(5.0 - buf[0]);
	buf[2] = 1.0 / 3.0 * sqrt(5.0 + buf[0]);
	_gaussePoints = { 0.5, 0.5 + buf[1] / 2 , 0.5 - buf[1] / 2, 0.5 + buf[2] / 2 , 0.5 - buf[2] / 2 };
	buf[0] = 13 * sqrt(70.0);
	buf[1] = (322.0 + buf[0]) / 900.0;
	buf[2] = (322.0 - buf[0]) / 900.0;
	_gausseWeights = { 128.0 / 255.0, buf[1], buf[1], buf[2], buf[2] };
}

unsigned char GausseIntegrBase::getIntegrOrder() const {
	return _gaussePoints.size();
}
