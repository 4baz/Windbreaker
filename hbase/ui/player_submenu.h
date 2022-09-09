#pragma once
#include"base_submenu.h"
#include"data/player.h"

namespace hbase
{
	class PlayerSubmenu : public BaseSubmenu<PlayerSubmenu>
	{
	public:
		explicit PlayerSubmenu() = default;
		~PlayerSubmenu() noexcept = default;
		explicit PlayerSubmenu(int* selectedPlayer, std::uint32_t id, std::function<void(PlayerSubmenu*)> action) :
			m_SelectedPlayer(selectedPlayer)
		{
			SetId(id);
			SetAction(std::move(action));
		}

		PlayerSubmenu(PlayerSubmenu const&) = default;
		PlayerSubmenu& operator=(PlayerSubmenu const&) = default;
		PlayerSubmenu(PlayerSubmenu&&) = default;
		PlayerSubmenu& operator=(PlayerSubmenu&&) = default;

		const char* GetName() override
		{
			player_class& player = g_players.m_players[*m_SelectedPlayer];
			return player.player_name;
		}
	private:
		int* m_SelectedPlayer;
	};
}
