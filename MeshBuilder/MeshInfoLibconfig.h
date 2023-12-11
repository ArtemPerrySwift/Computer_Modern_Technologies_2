#pragma once
#include "MeshInfo.h"
#include "LibconfigDeluxe.h"

namespace MeshLibconfig
{
	template <size_t DIMENSION_COUNT>
	void readCoord(libconfig::Setting& arrSettings, std::array<std::vector<double>, DIMENSION_COUNT>& _coordPerDim, std::array<std::string, DIMENSION_COUNT>& dimCoordNames)
	{
		for (int i = 0; i < DIMENSION_COUNT; i++)
			setArrayFromConfigArray(arrSettings, _coordPerDim[i], dimCoordNames[i]);
	}

	void setClass(libconfig::Setting& setting, SplitInfo& splitInfo)
	{
		setVarFromSetting(setting, splitInfo.intervalsCount, "IntervalsCount");
		setVarFromSetting(setting, splitInfo.sparseRatio, "SparseRatio");
	}

	void setClass(libconfig::Setting& setting, SplittingIntervalInfo& splittingIntervalInfo)
	{
		int p1, p2;
		setVarFromSetting(setting, p1, "PointNum1");
		setVarFromSetting(setting, p2, "PointNum2");
		splittingIntervalInfo.setIntervalPointsInd(p1, p2);
		setClass(getSubSetting(setting, "SplitOptions"), splittingIntervalInfo.splitInfo);
	}

	template<size_t DIMENSION_COUNT>
	void setClass(libconfig::Setting& setting, AreaInfo<DIMENSION_COUNT>& areaInfo, std::array<std::string, DIMENSION_COUNT>& dimIntervalNames) {
		for (int i = 0; i < DIMENSION_COUNT; i++)
			setVarFromSetting(setting, areaInfo.intervalIndexesPerDim[i], dimIntervalNames[i]);
	}

	template <class T> requires std::is_default_constructible<T>::value
	void setClassArray(libconfig::Setting& arrSettings, std::vector<T>& classArr)
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
			T buf;
			for (int i = 0; i < classArr.capacity(); i++)
			{
				setClass(arrSettings[i], buf);
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
	template <size_t DIMENSION_COUNT>
	void readIntervalsInfo(libconfig::Setting& settings, std::array<std::vector<SplittingIntervalInfo>, DIMENSION_COUNT>& intervalsInfo, std::array<std::string, DIMENSION_COUNT>& dimIntervalsNames)
	{
		for (size_t i = 0; i < DIMENSION_COUNT; i++)
			setClassArray(getSubSetting(settings, dimIntervalsNames[i]), intervalsInfo[i]);
	}

	template <size_t DIMENSION_COUNT>
	void setClassArray(libconfig::Setting& arrSettings, std::vector<AreaInfo<DIMENSION_COUNT>>& classArr, std::array<std::string, DIMENSION_COUNT>& dimIntervalNames)
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
			AreaInfo<DIMENSION_COUNT> buf;
			for (int i = 0; i < classArr.capacity(); i++)
			{
				setClass(arrSettings[i], buf, dimIntervalNames);
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

	template<size_t DIMENSION_COUNT>
	void setClass(libconfig::Setting& setting, BasicMeshInfo<DIMENSION_COUNT>& basicMeshInfo, std::array<std::string, DIMENSION_COUNT>& dimCoordNames, std::array<std::string, DIMENSION_COUNT>& dimIntervalNames) {
		readCoord(getSubSetting(setting, "BasePoints"), basicMeshInfo._coordPerDim, dimCoordNames);
		readIntervalsInfo(getSubSetting(setting, "Intervals"), basicMeshInfo._intervalPerDim, dimIntervalNames);
	}

	template<size_t DIMENSION_COUNT>
	void setClass(libconfig::Setting& setting, MeshInfo<DIMENSION_COUNT>& MeshInfo, std::array<std::string, DIMENSION_COUNT>& dimCoordNames, std::array<std::string, DIMENSION_COUNT>& dimIntervalNames, std::array<std::string, DIMENSION_COUNT>& dimAreaIntervalNames) {
		BasicMeshInfo<DIMENSION_COUNT>& basicMeshInfo = MeshInfo;
		setClass(setting, basicMeshInfo, dimCoordNames, dimIntervalNames);
		setClassArray(getSubSetting(setting, "Areas"), MeshInfo.areasInfo, dimAreaIntervalNames);
	}
}
