#pragma once
#include "MagnetIndocFunct.h"
#include "MagnetMesh.h"
#include "GausseIntegr.h"

const double PI = 3.14159265358979323846;


template <unsigned char GEOM_DIMENSION_COUNT, unsigned char MAGN_DIMENSION_COUNT>
class MagnetDirectTask {
private:
	//const std::vector<MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& _magnetElements;	
	//GausseIntegr<GEOM_DIMENSION_COUNT> _gausseIntegr;

	template<unsigned char MAGN_DIMENSION> requires (MAGN_DIMENSION <= MAGN_DIMENSION_COUNT)
	void getIndoctValue(std::array<double, GEOM_DIMENSION_COUNT>& point, double I, std::array<double, MAGN_DIMENSION_COUNT>& magnIndoc) {
		magnIndoc[MAGN_DIMENSION - 1] = getIndoctValue<MAGN_DIMENSION>(point, I);
		getIndoctValue<MAGN_DIMENSION - 1>(point, I, magnIndoc);
	}

	template<>
	void getIndoctValue<1>(std::array<double, GEOM_DIMENSION_COUNT>& point, double I, std::array<double, MAGN_DIMENSION_COUNT>& magnIndoc) {
		magnIndoc[0] = getIndoctValue<1>(point, I);
	}
protected:
	const std::vector<MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& _magnetElements;

	//template<unsigned char MAGN_DIMENSION> requires (MAGN_DIMENSION <= MAGN_DIMENSION_COUNT)
	virtual double getIndoctValueFromElem(const MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& elem, 
								MagnetIndocFunct<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& magIndocFunctor) const = 0;
public:
	MagnetDirectTask<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>(const std::vector<MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& magnetElements) : _magnetElements{ magnetElements } {
	}
	template<unsigned char MAGN_DIMENSION> requires (MAGN_DIMENSION <= MAGN_DIMENSION_COUNT)
	double getIndoctValue(std::array<double, GEOM_DIMENSION_COUNT>& point, double I) {
		double b = 0;
		std::array<double, GEOM_DIMENSION_COUNT> midPoint;
		double buf;
		for (const MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& elem : _magnetElements) {
			MagnetIndocFunctDim<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT, MAGN_DIMENSION> magIndocFunctor(elem);
			magIndocFunctor.targetPoint = point;
			buf = getIndoctValueFromElem(elem, magIndocFunctor);
			b += buf;
//#ifdef GAUSSE_COUNT
//			double r2 = 0, elemDiag2;
//			for (unsigned char i = 0; i < GEOM_DIMENSION_COUNT; i++) {
//				midPoint[i] = (elem._intervals[i]->leftPoint + elem._intervals[i]->rightPoint) / 2;
//				r2 += pow(point[i] - midPoint[i], 2);
//				elemDiag2 = pow(elem._intervals[i]->getLength(), 2);
//			}
//				
//
//			if (elemDiag2 / r2 > 0.1) {
//				std::array<std::pair<double, double>, GEOM_DIMENSION_COUNT> elemSizes;
//				for (unsigned char i = 0; i < GEOM_DIMENSION_COUNT; i++) {
//					elemSizes[i].first = elem._intervals[i]->leftPoint;
//					elemSizes[i].second = elem._intervals[i]->rightPoint;
//				}
//				b += _gausseIntegr.integrateOnSpace(magIndocFunctor, elemSizes);
//			}
//			else
//				b += elem.getMetrics() * magIndocFunctor(midPoint);
//#else
//			b += elem.getMetrics() * magIndocFunctor(midPoint);
//#endif

		}
		b *= I / (4 * PI);
		return b;
	}

	void getIndoctValue(std::array<double, GEOM_DIMENSION_COUNT>& point, double I, std::array<double, MAGN_DIMENSION_COUNT>& magnIndoc) {
		getIndoctValue<MAGN_DIMENSION_COUNT>(point, I, magnIndoc);
	}
};

template <unsigned char GEOM_DIMENSION_COUNT, unsigned char MAGN_DIMENSION_COUNT>
class MagnetTaskSimple : virtual public MagnetDirectTask<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>{
public:
	MagnetTaskSimple<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>(const std::vector<MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& magnetElements) : MagnetDirectTask<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>(magnetElements) {}
	
	double getIndoctValueFromElem(const MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& elem,
								MagnetIndocFunct<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& magIndocFunctor) const override {
		std::array<double, GEOM_DIMENSION_COUNT> midPoint;
		for (unsigned char i = 0; i < GEOM_DIMENSION_COUNT; i++) {
			midPoint[i] = (elem._intervals[i]->leftPoint + elem._intervals[i]->rightPoint) / 2;
		}
		return elem.getMetrics() * magIndocFunctor(midPoint);
	}
};

template <unsigned char GEOM_DIMENSION_COUNT, unsigned char MAGN_DIMENSION_COUNT>
class MagnetTaskGausse : virtual public MagnetDirectTask<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT> {
private:
	GausseIntegr<GEOM_DIMENSION_COUNT> _gausseIntegr;
public:
	MagnetTaskGausse<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>(const std::vector<MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& magnetElements) : 
		MagnetDirectTask<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>(magnetElements) {}

	double getIndoctValueFromElem(const MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& elem,
								MagnetIndocFunct<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& magIndocFunctor) const override {
		std::array<std::pair<double, double>, GEOM_DIMENSION_COUNT> elemSizes;
		for (unsigned char i = 0; i < GEOM_DIMENSION_COUNT; i++) {
			elemSizes[i].first = elem._intervals[i]->leftPoint;
			elemSizes[i].second = elem._intervals[i]->rightPoint;
		}
		return _gausseIntegr.integrateOnSpace(magIndocFunctor, elemSizes);
	}
};

template <unsigned char GEOM_DIMENSION_COUNT, unsigned char MAGN_DIMENSION_COUNT>
class MagnetTaskSimpleGausseCompine : public MagnetTaskSimple<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>, 
										public MagnetTaskGausse<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT> {
private:
	double _simpleGausseBorder;
public:

	MagnetTaskSimpleGausseCompine<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>(const std::vector<MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& magnetElements) : 
		MagnetDirectTask<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>(magnetElements),
		MagnetTaskSimple<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>(magnetElements), 
		MagnetTaskGausse<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>(magnetElements)
	{
		_simpleGausseBorder = 0.1;
	}
	bool setSimpleGausseBorder(double simpleGausseBorder) {
		if (simpleGausseBorder <= 0)
			return false;
		_simpleGausseBorder = simpleGausseBorder;
		return true;
	}

	double getSimpleGausseBorder() const {
		return _simpleGausseBorder;
	}

	double getIndoctValueFromElem(const MagnetElement<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& elem,
		MagnetIndocFunct<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& magIndocFunctor) const override {
		double rFromPointToElem = 0;
		double rElem = 0;
		for (unsigned char i = 0; i < GEOM_DIMENSION_COUNT; i++) {
			double buf = magIndocFunctor.targetPoint[i] - (elem._intervals[i]->leftPoint + elem._intervals[i]->rightPoint) / 2;
			rFromPointToElem += buf*buf;
			buf = elem._intervals[i]->getLength();
			rElem += buf*buf;
		}
		return rElem/ rFromPointToElem > _simpleGausseBorder ? 
			MagnetTaskSimple<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>::getIndoctValueFromElem(elem, magIndocFunctor) :
			MagnetTaskGausse<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>::getIndoctValueFromElem(elem, magIndocFunctor);
	}
};