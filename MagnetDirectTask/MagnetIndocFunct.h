#pragma once
#include <array>
#include "MagnetInfo.h"
template <unsigned char GEOM_DIMENSION_COUNT, unsigned char MAGN_DIMENSION_COUNT>
class MagnetIndocFunct
{
	const MagnetInfo<MAGN_DIMENSION_COUNT>* _magnetInfo;
protected:
	virtual double calcCoefs(std::array<double, GEOM_DIMENSION_COUNT>& elemPoint, std::array<double, MAGN_DIMENSION_COUNT>& magnCoeff) const = 0;
public:
	std::array<double, GEOM_DIMENSION_COUNT> targetPoint;

	double calcMagneticIndoctionFromElemPoint(std::array<double, GEOM_DIMENSION_COUNT>& elemPoint) const {
		std::array<double, MAGN_DIMENSION_COUNT> magnCoeff;
		double r2 = calcCoefs(elemPoint, magnCoeff);
		double r3 = pow(sqrt(r2), 3);
		double buf = 0;
		for (int i = 0; i < MAGN_DIMENSION_COUNT; i++)
			buf += _magnetInfo->p[i] * magnCoeff[i];
		return  buf / r3;
	}

	double operator()(std::array<double, GEOM_DIMENSION_COUNT>& elemPoint) const {
		return calcMagneticIndoctionFromElemPoint(elemPoint);
	}

	void setMagnetElement(const MagnetInfo<MAGN_DIMENSION_COUNT>& magnetInfo) {
		_magnetInfo = &magnetInfo;
	}

	MagnetIndocFunct(std::array<double, GEOM_DIMENSION_COUNT>& targetPoint, const MagnetInfo<MAGN_DIMENSION_COUNT>& magnetInfo) : targetPoint{ targetPoint } {
		_magnetInfo = &magnetInfo;
	}

	MagnetIndocFunct(const MagnetInfo<MAGN_DIMENSION_COUNT>& magnetInfo) {
		_magnetInfo = &magnetInfo;
	}
};

template <unsigned char GEOM_DIMENSION_COUNT, unsigned char MAGN_DIMENSION_COUNT, unsigned char MAGN_DIMENSION> requires (MAGN_DIMENSION <= MAGN_DIMENSION_COUNT)
class MagnetIndocFunctDim : public MagnetIndocFunct<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT> {
protected:
	double calcCoefs(std::array<double, GEOM_DIMENSION_COUNT>& elemPoint, std::array<double, MAGN_DIMENSION_COUNT>& magnCoeff) const override {
		int i;
		int magDim = MAGN_DIMENSION - 1;
		std::array<double, GEOM_DIMENSION_COUNT> localPoint;
		double r2 = 0;
		for (i = 0; i < GEOM_DIMENSION_COUNT; i++) {
			localPoint[i] = elemPoint[i] - MagnetIndocFunct<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>::targetPoint[i];
			r2 += localPoint[i] * localPoint[i];
		}

		for (i = 0; i < magDim; i++) {
			magnCoeff[i] = 3 * localPoint[i] * localPoint[magDim] / r2;
		}

		magnCoeff[i] = 3 * (localPoint[i] * localPoint[i] / r2 - 1);

		for(i++; i < MAGN_DIMENSION_COUNT; i++) {
			magnCoeff[i] = 3 * localPoint[i] * localPoint[magDim] / r2;
		}
		return r2;
	}
public:
	MagnetIndocFunctDim<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT, MAGN_DIMENSION>(const MagnetInfo<MAGN_DIMENSION_COUNT>& magnetInfo) : MagnetIndocFunct<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>(magnetInfo){

	}

};

