#include "LibconfigDeluxe.h"

libconfig::Setting& getSubSetting(libconfig::Setting& setting, std::string_view varNameInConfig)
{
	try
	{
		libconfig::Setting& subSetting = setting[varNameInConfig.data()];
		return subSetting;
	}
	catch (libconfig::SettingNotFoundException& ex)
	{
		std::string exeptionMSG = "Setting \"";
		exeptionMSG.append(varNameInConfig);
		exeptionMSG.append("\" is not found in path: ");
		exeptionMSG.append(setting.getPath());
		throw new std::exception(exeptionMSG.c_str());
	}
}

libconfig::Setting& getSettingFromConfig(libconfig::Config& config, std::string_view settingNameInConfig)
{
	try
	{
		libconfig::Setting& rootSet = config.lookup(settingNameInConfig.data());
		return rootSet;
	}
	catch (libconfig::SettingNotFoundException& e)
	{
		std::string exeptionMSG = "\"";
		exeptionMSG.append(settingNameInConfig.data());
		exeptionMSG.append("\" setting is not found in configuration");
		throw new std::exception(exeptionMSG.c_str());
	}
	
}

libconfig::Config& getConfigFromFile(libconfig::Config& cfg, std::string_view configFileName) // опсаное место
{
	try
	{
		cfg.readFile(configFileName.data());
		return cfg;
	}
	catch (libconfig::FileIOException& fioex)
	{
		std::string exeptionMSG = "I / O error while reading file: ";
		exeptionMSG.append(configFileName);
		throw new std::exception(exeptionMSG.c_str());
	}
	catch (libconfig::ParseException& pex)
	{
		std::string exeptionMSG = "I / O error while reading file: ";
		exeptionMSG.append(pex.getFile());
		exeptionMSG.append(" in line :");
		exeptionMSG.append(std::to_string(pex.getLine()));
		throw new std::exception(exeptionMSG.c_str());
	}
}