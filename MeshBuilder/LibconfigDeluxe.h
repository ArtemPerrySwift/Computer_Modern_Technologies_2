#pragma once
//#include <libconfig.h++>
#include "..\packages\libconfig_vc120.1.4.9.4\build\native\include\libconfig.h++"
#include <vector>

libconfig::Setting& getSubSetting(libconfig::Setting& setting, std::string_view varNameInConfig);
libconfig::Setting& getSettingFromConfig(libconfig::Config& config, std::string_view settingNameInConfig);
libconfig::Config& getConfigFromFile(libconfig::Config& cfg, std::string_view configFileName);

template <typename T>
void setVarFromSetting(libconfig::Setting& setting, T& var, std::string_view varNameInConfig)
{
	if (!setting.lookupValue(varNameInConfig.data(), var))
	{
		std::string exeptionMSG = "Variable \"";
		exeptionMSG.append(varNameInConfig);
		exeptionMSG.append("\" is not found in configuration or has invalid type in path: ");
		exeptionMSG.append(setting.getPath());
		throw new std::exception(exeptionMSG.c_str());
	}
}

template <typename T>
void setArrayFromConfigArray(libconfig::Setting& arrSett, std::vector<T>& arr)
{
	if (!(arrSett.isArray() || arrSett.isList()))
	{
		std::string exeptionMSG;
		exeptionMSG.append("\" Setting has not list or array type in path: ");
		exeptionMSG.append(arrSett.getPath());
		throw new std::exception(exeptionMSG.c_str());
	}

	arr.resize(arrSett.getLength());

	try
	{	
		for (int i = 0; i < arr.size(); i++)
			arr[i] = arrSett[i];
	}
	catch (libconfig::SettingTypeException& ex)
	{
		arr.clear();
		std::string exeptionMSG = "Array of List \"";
		exeptionMSG.append("\" has invalid element type in path: ");
		exeptionMSG.append(arrSett.getPath());
		throw new std::exception(exeptionMSG.c_str());
	}
}

template <class T> requires std::is_constructible<T, libconfig::Setting&>::value
void setArrayFromConfigList(libconfig::Setting& arrSett, std::vector<T>& arr)
{
	if (!arrSett.isList())
	{
		std::string exeptionMSG;
		exeptionMSG.append("\" Setting has not list type in path: ");
		exeptionMSG.append(arrSett.getPath());
		throw new std::exception(exeptionMSG.c_str());
	}

	arr.reserve(arrSett.getLength());
	try
	{
		for (int i = 0; i < arr.capacity(); i++)
			arr.emplace_back(arrSett[i]);
	}
	catch (std::exception& e)
	{
		arr.clear();
		throw e;
	}
	
	
	
}

template <class T> requires std::is_constructible<T, libconfig::Setting&>::value
void setArrayFromConfigList(libconfig::Setting& arrSetting, std::vector<T>& arr, std::string_view varNameInConfig)
{
	libconfig::Setting& arrSett = getSubSetting(arrSetting, varNameInConfig);
	setArrayFromConfigList(arrSett, arr);

}

template <typename T>
void setArrayFromConfigArray(libconfig::Setting& arrSetting, std::vector<T>& arr, std::string_view varNameInConfig)
{
	libconfig::Setting& arrSett = getSubSetting(arrSetting, varNameInConfig);
	setArrayFromConfigArray(arrSett, arr);
	
}
