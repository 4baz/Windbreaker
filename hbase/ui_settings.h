#pragma once
#include "pch.h"

namespace hbase
{
	class ui_settings
	{
	public:
		nlohmann::json options;

		bool save()
		{
			std::ofstream file(settings_file, std::ios::out | std::ios::trunc);
			file << options.dump(4);
			file.close();
			return true;
		}

		bool load()
		{
			std::ifstream file(settings_file);
			file >> options;
			return true;
		}

		std::string settings_file = MENU_PATH"\\ui\\ui.json";
	};

	inline ui_settings g_ui_settings;
}
