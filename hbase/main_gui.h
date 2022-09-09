#pragma once

namespace hbase
{
	class main_gui
	{
	public:
		void initialize();
		void tick();
	private:
		bool m_initialized{};

	private:
		std::vector<std::string> m_xmlVehicleList;
		std::vector<std::string> m_iniVehicleList;
	};

	inline main_gui g_main_gui{};
}