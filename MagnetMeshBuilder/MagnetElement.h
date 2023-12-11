#pragma once
#include "..\MeshBuilder\GeometryElement.h"
#include "MagnetInfo.h"

template <size_t GEOM_DIMENSION_COUNT, size_t MAGN_DIMENSION_COUNT>
struct MagnetElement : public GeometryElement<GEOM_DIMENSION_COUNT>, public MagnetInfo<MAGN_DIMENSION_COUNT> {
public:
	//const GeometryElement<GEOM_DIMENSION_COUNT>& geometryElement;
	MagnetElement(const GeometryElement<GEOM_DIMENSION_COUNT>& geometryElement) : GeometryElement<GEOM_DIMENSION_COUNT>(geometryElement) {
	}
	MagnetElement(const GeometryElement<GEOM_DIMENSION_COUNT>& geometryElement, MagnetInfo<MAGN_DIMENSION_COUNT>& magnetInfo) : GeometryElement<GEOM_DIMENSION_COUNT>(geometryElement), MagnetInfo<MAGN_DIMENSION_COUNT>(magnetInfo) {
	}
	MagnetElement(const GeometryElement<GEOM_DIMENSION_COUNT>& geometryElement, std::array<double, MAGN_DIMENSION_COUNT>& p) : GeometryElement<GEOM_DIMENSION_COUNT>(geometryElement) {
		this->p = p;
	}
};