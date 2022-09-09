#include"util.h"
#include"natives.h"
#include "gta/joaat.h"
#include"script.h"
#include "pointers.h"
#include"gta/enums.h"

namespace hbase
{
	rage::Blip_t* util::get_blip_objective()
	{
		rage::Blip_t* r = nullptr;
		for (int i = 0; i < 0x400; i++)
		{
			if (rage::BlipList* blipList = g_pointers->m_blip_list)
			{
				if (rage::Blip_t* blip = blipList->m_Blips[i].m_pBlip)
				{
					auto colour = blip->m_color;
					auto icon = blip->m_icon;

					if ((colour == ColorYellowMission && icon == SpriteStandard) ||
						(colour == ColorYellowMission2 && icon == SpriteStandard) ||
						(colour == ColorYellow && icon == SpriteStandard) ||
						(colour == ColorWhite && icon == SpriteRaceFinish) ||
						(colour == ColorGreen && icon == SpriteStandard) ||
						(colour == ColorBlue && icon == SpriteStandard) ||
						(icon == SpriteCrateDrop))

					{
						r = blip;
						break;
					}
				}
			}
		}
		return r;
	}

	Vector3 util::get_blip_marker()
	{
		static Vector3 zero;
		Vector3 coords;

		bool blipFound = false;
		int blipIterator = HUD::GET_WAYPOINT_BLIP_ENUM_ID();
		for (Blip i = HUD::GET_FIRST_BLIP_INFO_ID(blipIterator); HUD::DOES_BLIP_EXIST(i) != 0; i = HUD::GET_NEXT_BLIP_INFO_ID(blipIterator))
		{
			if (HUD::GET_BLIP_INFO_ID_TYPE(i) == 4)
			{
				coords = HUD::GET_BLIP_INFO_ID_COORD(i);
				blipFound = true;
				break;
			}
		}
		if (blipFound)
		{
			return coords;
		}

		return zero;
	}

	void util::request_control_of_id(Entity netid)
	{
		int tick = 0;

		while (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(netid) && tick <= 25)
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(netid);
			tick++;
		}
	}

	void util::request_control_of_ent(Entity entity)
	{
		int tick = 0;
		while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity) && tick <= 25)
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
			tick++;
		}
		if (NETWORK::NETWORK_IS_SESSION_STARTED())
		{
			int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(entity);
			request_control_of_id(netID);
			NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netID, 1);
		}
	}

	double util::degree_to_radian(double n) {
		return n * 0.017453292519943295;
	}

	Vector3 util::rotation_to_direction(Vector3 rot) {
		double num = degree_to_radian(rot.z);
		double num2 = degree_to_radian(rot.x);
		double val = cos(num2);
		double num3 = abs(val);
		rot.x = (float)(-(float)sin(num) * num3);
		rot.y = (float)(cos(num) * num3);
		rot.z = (float)sin(num2);
		return rot;
	}

	Vector3 util::add_vector(Vector3 vector, Vector3 vector2) {
		vector.x += vector2.x;
		vector.y += vector2.y;
		vector.z += vector2.z;
		return vector;
	}

	Vector3 util::multiply_vector(Vector3 vector, float inc) {
		vector.x *= inc;
		vector.y *= inc;
		vector.z *= inc;
		return vector;
	}
}