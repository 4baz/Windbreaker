#pragma once
#include"gta/natives.h"
#include"gta/blip.h"

namespace hbase
{
	namespace util
	{
		rage::Blip_t* get_blip_objective();
		Vector3 get_blip_marker();
		void request_control_of_id(Entity netid);
		void request_control_of_ent(Entity entity);
		double degree_to_radian(double n);
		Vector3 rotation_to_direction(Vector3 rot);
		Vector3 add_vector(Vector3 vector, Vector3 vector2);
		Vector3 multiply_vector(Vector3 vector, float inc);
	}
}