#pragma once
//#include "..\MeshBuilder\MeshInfoLibconfig.h"
#include "MeshInfoLibconfig.h"
#include "MagnetMeshInfo.h"
#include "MagnetAreaInfo.h"

namespace MeshLibconfig
{
	template <size_t DIMENSION_COUNT>
	void setClass(libconfig::Setting& setting, MagnetInfo<DIMENSION_COUNT>& magnetInfo, std::array<std::string, DIMENSION_COUNT> pNames)
	{
		for(size_t i = 0; i < DIMENSION_COUNT; i++)
			setVarFromSetting(setting, magnetInfo.p[i], pNames[i]);
	}

	template <size_t GEOM_DIMENSION_COUNT, size_t MAGN_DIMENSION_COUNT>
	void setClass(libconfig::Setting& setting, MagnetAreaInfo<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>& magnetAreaInfo, std::array<std::string, GEOM_DIMENSION_COUNT>& dimNames, std::array<std::string, MAGN_DIMENSION_COUNT>& pNames)
	{
		AreaInfo<GEOM_DIMENSION_COUNT>& areaInfo = magnetAreaInfo;
		MagnetInfo<MAGN_DIMENSION_COUNT>& magnetInfo = magnetAreaInfo;
		setClass(setting, areaInfo, dimNames);
		setClass(setting, magnetInfo, pNames);
	}

	template <size_t GEOM_DIMENSION_COUNT, size_t MAGN_DIMENSION_COUNT>
	void setClassArray(libconfig::Setting& arrSettings, std::vector<MagnetAreaInfo<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& classArr, std::array<std::string, GEOM_DIMENSION_COUNT>& dimNames, std::array<std::string, MAGN_DIMENSION_COUNT>& pNames)
	{
		if (!(arrSettings.isArray() || arrSettings.isList()))
		{
			std::string exeptionMSG;
			exeptionMSG.append("\" Setting has not list or array type in path: ");
			exeptionMSG.append(arrSettings.getPath());
			throw new std::exception(exeptionMSG.c_str());
		}

		classArr.reserve(arrSettings.getLength());

		try
		{
			MagnetAreaInfo<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT> buf;
			for (int i = 0; i < classArr.capacity(); i++)
			{
				setClass(arrSettings[i], buf, dimNames, pNames);
				classArr.emplace_back(buf);
			}

		}
		catch (libconfig::SettingTypeException& ex)
		{
			classArr.clear();
			std::string exeptionMSG = "Array of List \"";
			exeptionMSG.append("\" has invalid element type in path: ");
			exeptionMSG.append(arrSettings.getPath());
			throw new std::exception(exeptionMSG.c_str());
		}
	}

	template<size_t GEOM_DIMENSION_COUNT, size_t MAGN_DIMENSION_COUNT>
	void setClass(libconfig::Setting& setting, 
			MeshInfo<GEOM_DIMENSION_COUNT, MagnetAreaInfo<GEOM_DIMENSION_COUNT, MAGN_DIMENSION_COUNT>>& MeshInfo, 
			std::array<std::string, GEOM_DIMENSION_COUNT>& dimCoordNames, 
			std::array<std::string, GEOM_DIMENSION_COUNT>& dimIntervalNames, 
			std::array<std::string, GEOM_DIMENSION_COUNT>& dimAreaIntervalNames, 
			std::array<std::string, MAGN_DIMENSION_COUNT>& dimMagnNames) {
		BasicMeshInfo<GEOM_DIMENSION_COUNT>& basicMeshInfo = MeshInfo;
		setClass(setting, basicMeshInfo, dimCoordNames, dimIntervalNames);
		setClassArray(getSubSetting(setting, "Areas"), MeshInfo.areasInfo, dimAreaIntervalNames, dimMagnNames);
	}
}