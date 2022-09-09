#pragma once
namespace hbase::protection
{
	class user
	{
	public:
		ULONGLONG block_time;
		Hash player_model;
		user(int64_t user_id) :
			block_time(0),
			player_model(0)
		{
		}
	};
	extern std::unordered_map <int64_t, user*> users;
	void add_player(CNetGamePlayer* src);
	void delete_player(CNetGamePlayer* src);
	void set_block(CNetGamePlayer* src);
	bool is_block(CNetGamePlayer* src);
}