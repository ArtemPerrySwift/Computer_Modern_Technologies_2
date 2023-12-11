#pragma once
#include <vector>
#include <functional>
#include <array>
#include <utility>
#include "GausseIntegrBase.h"

template <unsigned char DIMENSION_COUNT>
class GausseIntegr : GausseIntegrBase
{
private:
	template<unsigned char DIMENSION> //requires is_less<dimNum, DIM_COUNT + 1>
	double iterateOverDimension(std::function<double(std::array<double, DIMENSION_COUNT>& point)>& integrFunct,
							std::array<double, DIMENSION_COUNT>& point) const
	{
		double sum = 0;
		const int dimensionIndex = DIMENSION - 1;
		const std::vector<double>& _gaussePoints = getGaussePoints();
		const std::vector<double>& _gausseWeights = getGausseWights();
		for (int i = 0; i < _gaussePoints.size(); i++)
		{
			point[dimensionIndex] = _gaussePoints[i];			
			sum += _gausseWeights[i] * iterateOverDimension<DIMENSION - 1>(integrFunct, point);
		}
		return sum;
	}

	template<>
	double iterateOverDimension<0>(std::function<double(std::array<double, DIMENSION_COUNT>& point)>& integrFunct,
		std::array<double, DIMENSION_COUNT>& point) const
	{
		return integrFunct(point);
	}

	template<unsigned char DIMENSION> //requires is_less<dimNum, DIM_COUNT + 1>
	double iterateOverDimension(std::function<double(std::array<double, DIMENSION_COUNT>& point)>& integrFunct, 
		std::array<std::pair<double, double>, DIMENSION_COUNT>& intervals,
		std::array<double, DIMENSION_COUNT>& point) const
	{
		double sum = 0;
		const int dimensionIndex = DIMENSION - 1;
		double intervalBeg = intervals[dimensionIndex].first;
		double intervalSize = intervals[dimensionIndex].second - intervals[dimensionIndex].first;

		const std::vector<double>& _gaussePoints = getGaussePoints();
		const std::vector<double>& _gausseWeights = getGausseWights();

		for (int i = 0; i < _gaussePoints.size(); i++)
		{
			point[dimensionIndex] = intervalBeg + _gaussePoints[i] * intervalSize;
			sum += _gausseWeights[i] * iterateOverDimension<DIMENSION - 1>(integrFunct, intervals, point);
		}
		return sum;
	}

	template<>
	double iterateOverDimension<0>(std::function<double(std::array<double, DIMENSION_COUNT>& point)>& integrFunct,
		std::array<std::pair<double, double>, DIMENSION_COUNT>& intervals,
		std::array<double, DIMENSION_COUNT>& point) const
	{
		return integrFunct(point);
	}

public:
	static const unsigned char MAX_ORDER = 5;
	//GausseIntegr();

	//template<unsigned char ORDER> requires (ORDER <= MAX_ORDER)
	//void setIntegrOrder();

	double integrateOnUnitSpace(std::function<double(std::array<double, DIMENSION_COUNT>& point)> integrFunct)
	{
		std::array<double, DIMENSION_COUNT> point;
		return iterateOverDimension<DIMENSION_COUNT>(integrFunct, point);
	}


	double integrateOnSpace(std::function<double(std::array<double, DIMENSION_COUNT>& point)> integrFunct, std::array<std::pair<double, double>, DIMENSION_COUNT>& intervals)
	{
		std::array<double, DIMENSION_COUNT> point;
		double mes = 1;
		for (unsigned char i = 0; i < DIMENSION_COUNT; i++)
			mes *= (intervals[i].second - intervals[i].first);
		return mes * iterateOverDimension<DIMENSION_COUNT>(integrFunct, intervals, point);
	}
};
