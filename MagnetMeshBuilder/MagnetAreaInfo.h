#pragma once
#include "..\MeshBuilder\AreaInfo.h"
#include "MagnetInfo.h"

template <size_t GEOM_DIMENSION_COUNT, size_t MAGN_DIMENSION_COUNT>
struct MagnetAreaInfo : public AreaInfo<GEOM_DIMENSION_COUNT>, public MagnetInfo<MAGN_DIMENSION_COUNT>
{
};
