#pragma once
#include"../gta/natives.h"

namespace hbase
{
	struct VehiclePack {
		std::string name{};
		std::string labelText{};
		Hash hash{};
	};

	struct VehicleClass {
		std::string className{};
		std::vector<VehiclePack> vehicles;
		int p{ 0 };
	};

	class Vehicles {
	public:
		void initialize();
	public:
		std::vector<VehicleClass> classes;
	};
	inline Vehicles g_vehicles{};
}