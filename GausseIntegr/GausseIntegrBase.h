#pragma once
#include <vector>
class GausseIntegrBase {
private:
	std::vector<double> _gaussePoints;
	std::vector<double> _gausseWeights;

public:
	GausseIntegrBase();

	const std::vector<double>& getGaussePoints() const;
	const std::vector<double>& getGausseWights() const;

	void setIntegrOrder_1();
	void setIntegrOrder_2();
	void setIntegrOrder_3();
	void setIntegrOrder_4();
	void setIntegrOrder_5();

	unsigned char getIntegrOrder() const;
};

