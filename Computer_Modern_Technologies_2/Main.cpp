#include "MagnetDirectTask.h"
#include "Reciver.h"
#include <set>
#include <random>
#include <fstream>

const unsigned char GEOM_DIMENSION_COUNT = 2;
const unsigned char MAGN_DIMENSION_COUNT = 1;
const unsigned char MAGN_DIMENSION1 = 1;

#define MagnetDim GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT

int main()
{
	std::vector<MagnetElement<2, 2>> elems;
	MagnetDirectTask<2,2> directTask(elems);
	Reciver<2, 2> rec;
	directTask.getIndoctValue<1>(rec.point, 5);
	return 0;
}

void generateMagnDataSet(MagnetMesh<MagnetDim>& magnetMesh, std::string_view fileName) {

}

void generateMagnDataSet(MagnetArea<MagnetDim>& magnetArea,
						std::string_view fileName, 
						size_t& iFile, 
						std::vector<MagnetElement<MagnetDim>> elems,
						size_t iBegAreaElems) {

}

class MagnetMeshGenerator {
private:
	std::vector<MagnetElement<MagnetDim>> elems;
	const std::vector<MagnetArea<MagnetDim>>* _areas;
	std::array<size_t, GEOM_DIMENSION_COUNT> elemCountInSpace;
	std::array<std::pair<size_t, size_t>, GEOM_DIMENSION_COUNT> magnIntervals;
	std::array<double, MAGN_DIMENSION_COUNT> _vectMagnEarth;
	std::array<double, MAGN_DIMENSION_COUNT> _vectMagnAnomaly;
	std::vector<std::array<double, GEOM_DIMENSION_COUNT>>* _reciversPoints;
	size_t it;
	double _magnAnomalyCoef;
	size_t iBegAreaElems;
	double _I;

	void calcVectMagnAnomaly() {
		for (int i = 0; i < MAGN_DIMENSION_COUNT; i++)
			_vectMagnAnomaly[i] = _magnAnomalyCoef * _vectMagnEarth[i];
	}

	void writeResults() {
		std::ofstream out;
		out.open(fileName + "_MagnCoef_" + std::to_string(it) + ".bin", std::ios::binary | std::ios::out);
		for (MagnetElement<MagnetDim>& elem : elems)
		{
			double coeff = elem.p[0] == _vectMagnEarth[0] ? 1 : _magnAnomalyCoef;
			out.write((char*)&coeff, sizeof(coeff));
		}
		out.close();
		out.open(fileName + "_B_" + std::to_string(it) + ".bin", std::ios::binary | std::ios::out);

		MagnetDirectTask<MagnetDim> magnetDirectTask(elems);
		std::array<double, MAGN_DIMENSION_COUNT> B;
		for (std::array<double, GEOM_DIMENSION_COUNT>& point : *_reciversPoints) {
			magnetDirectTask.getIndoctValue(point, _I, B);
			for (int i = 0; i < MAGN_DIMENSION_COUNT; i++)
				out.write((char*)&B[i], sizeof(double));
		}
		out.close();
	}

	template<unsigned char GEOM_DIMENSION>
	void interationOverDim(const std::array<const SplittedInterval*, GEOM_DIMENSION_COUNT>& intervals) {
		unsigned char indDim = GEOM_DIMENSION - 1;
		size_t intervalsCount = intervals[indDim]->getSubIntervals().size();

		if (intervalsCount < 2ULL) {
			magnIntervals[indDim].first = 0LL;
			magnIntervals[indDim].second = 0LL;
			interationOverDim<GEOM_DIMENSION - 1>(intervals);
			return;
		}

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<size_t> distrib(1ULL, intervalsCount - 1ULL);

		size_t intervIndCount = distrib(gen);
		std::set<size_t> intervInd;
		while (intervInd.size() != intervIndCount) {
			intervInd.insert(distrib(gen));
		}

		size_t prevIntervInd = 0;

		for (size_t curIntervInd : intervInd) {
			magnIntervals[indDim].first = prevIntervInd;
			magnIntervals[indDim].second = curIntervInd;
			prevIntervInd = curIntervInd + 1;
			interationOverDim<GEOM_DIMENSION - 1>(intervals);
		}

		magnIntervals[indDim].first = prevIntervInd;
		magnIntervals[indDim].second = intervalsCount;
		interationOverDim<GEOM_DIMENSION - 1>(intervals);
	}

	template<>
	void interationOverDim<0>(const std::array<const SplittedInterval*, GEOM_DIMENSION_COUNT>& intervals) {
		
		interateOverDimMagSet<GEOM_DIMENSION_COUNT>(iBegAreaElems);
		std::array<double, MAGN_DIMENSION_COUNT> buf = _vectMagnAnomaly;
		_vectMagnAnomaly = _vectMagnEarth;
		MagnetDirectTask<MagnetDim> magnetDirectTask(elems);


		///Дописать!
		interateOverDimMagSet<GEOM_DIMENSION_COUNT>(iBegAreaElems);
		_vectMagnAnomaly = buf;
	}

	template<unsigned char GEOM_DIMENSION>
	void interateOverDimMagSet(size_t iElem) {
		unsigned char dimInd = GEOM_DIMENSION - 1;
		size_t iBeg = magnIntervals[dimInd].first, iEnd = magnIntervals[dimInd].second;
		iElem += iBeg * elemCountInSpace[dimInd];
		for (size_t i = iBeg; i < iEnd; i++) {
			interateOverDimMagSet<GEOM_DIMENSION - 1>(iElem);
			iElem += elemCountInSpace[dimInd];
		}
	}

	template<>
	void interateOverDimMagSet<1>(size_t iElem) {
		unsigned char dimInd = 0;
		size_t iBeg = magnIntervals[dimInd].first, iEnd = magnIntervals[dimInd].second;
		iElem += iBeg;
		for (size_t i = iBeg; i < iEnd; i++, iElem++) {
			elems[iElem].p = _vectMagnAnomaly;
		}
	}

public:
	std::string fileName;
	
	MagnetMeshGenerator(MagnetMesh<MagnetDim>& magnetMesh,
						std::vector<std::array<double, GEOM_DIMENSION_COUNT>>& reciversPoints,
						std::array<double, MAGN_DIMENSION_COUNT>& vectMagnEarth, 
						double I = 100, double magnAnomalyCoef = 10, 
						std::string fileName = "MagnetMesh") : _vectMagnEarth{ vectMagnEarth }, fileName{ fileName }, _magnAnomalyCoef{magnAnomalyCoef} {
		setMesh(magnetMesh);
		setI(I);
		_reciversPoints = &reciversPoints;
		it = 0;
	}

	void setMesh(MagnetMesh<MagnetDim>& magnetMesh) {
		_areas = &magnetMesh.getAreas();
		magnetMesh.getElements(elems);
	}

	void generateMagnDataSet() {
		
		for (const MagnetArea<MagnetDim>& magnetArea : *_areas)
		{
			const std::array<const SplittedInterval*, GEOM_DIMENSION_COUNT>& intervals = magnetArea.getIntervals();
			elemCountInSpace[0] = 1;
			for (int i = 1; i < GEOM_DIMENSION_COUNT; i++) {
				elemCountInSpace[i] = elemCountInSpace[i - 1] * intervals[i - 1]->getSubIntervals().size();
			}
			interationOverDim<GEOM_DIMENSION_COUNT>(intervals);
		}
		it = 0;
	}

	void setMagnitAnomalyCoef(double magnitAnomalyCoef) {
		_magnAnomalyCoef;
		calcVectMagnAnomaly();
	}

	void setVectMagnEarth(std::array<double, MAGN_DIMENSION_COUNT>& vectMagnEarth) {
		_vectMagnEarth = vectMagnEarth;
		calcVectMagnAnomaly();
	}

	void setI(double I) {
		if (I < 0)
			throw std::exception("I cannot be less than 0 for dataset generation");
		_I = I;
	}
};