#pragma once
#include"../gta/player.h"

namespace hbase
{
	struct player_class {
		char player_name[20];
		int player_id{ 0 };
		int ped_id{ 0 };
		bool is_friend{};
		bool is_host{};
		bool is_script_host{};
		bool is_onlie{};
		bool is_me{};
		CNetGamePlayer* net_player{};
	};

	class players
	{
	public:
		player_class m_players[32];
		int player_id{ 0 };
		void on_tick();
	private:
	};
	namespace player
	{
		void player_script();
	}

	inline players g_players{};
}