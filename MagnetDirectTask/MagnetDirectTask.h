#pragma once
#include "MagnetIndocFunct.h"
#include "MagnetMesh.h"
#include "GausseIntegr.h"

const double PI = 3.14159265358979323846;

#define GAUSSE_COUNT
template <unsigned char GEOM_DIMENSION_COUNT, unsigned char MAGN_DIMENSION_COUNT>
class MagnetDirectTask {
private:
	const std::vector<MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& _magnetElements;	
	GausseIntegr<GEOM_DIMENSION_COUNT> _gausseIntegr;

public:
	MagnetDirectTask<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>(const std::vector<MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& magnetElements) : _magnetElements{ magnetElements } {
	}
	template<unsigned char MAGN_DIMENSION> requires (MAGN_DIMENSION <= MAGN_DIMENSION_COUNT)
	double getIndoctValue(std::array<double, GEOM_DIMENSION_COUNT>& point, double I) {
		double b = 0;
		std::array<double, GEOM_DIMENSION_COUNT> midPoint;
		MagnetIndocFunctDim<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT, MAGN_DIMENSION> magIndocFunctor(*_magnetElements.begin());
		for (const MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& elem : _magnetElements) {
			magIndocFunctor.setMagnetElement(elem);

#ifdef GAUSSE_COUNT
			double r2 = 0, elemDiag2;
			for (unsigned char i = 0; i < GEOM_DIMENSION_COUNT; i++) {
				midPoint[i] = (elem._intervals[i]->leftPoint + elem._intervals[i]->rightPoint) / 2;
				r2 += pow(point[i] - midPoint[i], 2);
				elemDiag2 = pow(elem._intervals[i]->getLength(), 2);
			}
				

			if (elemDiag2 / r2 > 0.1) {
				std::array<std::pair<double, double>, GEOM_DIMENSION_COUNT> elemSizes;
				for (unsigned char i = 0; i < GEOM_DIMENSION_COUNT; i++) {
					elemSizes[i].first = elem._intervals[i]->leftPoint;
					elemSizes[i].second = elem._intervals[i]->rightPoint;
				}
				b += _gausseIntegr.integrateOnSpace(magIndocFunctor, elemSizes);
			}
			else
				b += elem.getMetrics() * magIndocFunctor(midPoint);
#else
			b += elem.getMetrics() * magIndocFunctor(midPoint);
#endif

		}
		b *= I / (4 * PI);
		return b;
	}
};
