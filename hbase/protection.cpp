#include"gta/player.h"
#include"protection.h"
constexpr auto Block_Time = 60000;
namespace hbase
{
	std::unordered_map <int64_t, protection::user*> protection::users;

	void protection::add_player(CNetGamePlayer* src)
	{
		if (src != nullptr)
		{
			int64_t user_id = src->get_net_data()->m_rockstar_id;
			if (users.find(user_id) == users.end()) users[user_id] = new user(user_id);
		}
	}

	void protection::delete_player(CNetGamePlayer* src)
	{
		int64_t user_id = src->get_net_data()->m_rockstar_id;
		if (users.find(user_id) != users.end()) delete[]  users[user_id];
	}

	void protection::set_block(CNetGamePlayer* src)
	{
		if (src != nullptr)
		{
			add_player(src);
			int64_t user_id = src->get_net_data()->m_rockstar_id;
			users[user_id]->block_time = GetTickCount64() + Block_Time;
			LOG(INFO) << "Set " << src->get_name();
		}
	}

	bool protection::is_block(CNetGamePlayer* src)
	{
		if (src != nullptr)
		{
			add_player(src);
			int64_t user_id = src->get_net_data()->m_rockstar_id;
			return  GetTickCount64() < users[user_id]->block_time;
		}
		return  true;
	}
}