#pragma once
#include <cstdint>
#include "fwddec.h"
#include "extensible.h"
#include "vector.h"
#include "natives.h"
#include "enums.h"

#pragma pack(push, 1)
namespace rage
{
	class netAddress
	{
	public:
		uint8_t m_field4; //0x0000
		uint8_t m_field3; //0x0001
		uint8_t m_field2; //0x0002
		uint8_t m_field1; //0x0003
	}; //Size: 0x0004
	static_assert(sizeof(netAddress) == 0x4);

	class netPlayerData
	{
	public:
		char pad_0000[8]; //0x0000
		uint64_t m_rockstar_id; //0x0008
		char pad_0010[52]; //0x0010
		class netAddress m_relay_ip; //0x0044
		uint16_t m_relay_port; //0x0048
		char pad_004A[2]; //0x004A
		class netAddress m_external_ip; //0x004C
		uint16_t m_external_port; //0x0050
		char pad_0052[2]; //0x0052
		class netAddress m_internal_ip; //0x0054
		uint16_t m_internal_port; //0x0058
		char pad_005A[6]; //0x005A
		uint64_t m_host_token; //0x0060
		uint64_t m_peer_id; //0x0068
		uint64_t m_rockstar_id2; //0x0070
		char pad_0078[12]; //0x0078
		char m_name[20]; //0x0084
	}; //Size: 0x0098
	static_assert(sizeof(netPlayerData) == 0x98);

	class nonPhysicalPlayerDataBase
	{
	public:
		virtual ~nonPhysicalPlayerDataBase() = default;    // 0 (0x00)
		virtual void unk_0x08() = 0;                       // 1 (0x08)
		virtual void unk_0x10() = 0;                       // 2 (0x10)
		virtual void unk_0x18() = 0;                       // 3 (0x18)
		virtual void log(netLoggingInterface* logger) = 0; // 4 (0x20)
	};

	class netPlayer
	{
	public:
		virtual ~netPlayer() = default;            // 0 (0x00)
		virtual void reset() = 0;                  // 1 (0x08)
		virtual bool is_valid() const = 0;         // 2 (0x10)
		virtual const char* get_name() const = 0;  // 3 (0x18)
		virtual void _0x20() = 0;                  // 4 (0x20)
		virtual bool is_host() = 0;                // 5 (0x28)
		virtual netPlayerData* get_net_data() = 0; // 6 (0x30)
		virtual void _0x38() = 0;                  // 7 (0x38)
	};

	class netPlayerMgrBase
	{
	public:
		virtual ~netPlayerMgrBase() = default; // 0 (0x00)
	};

	class CNetworkPlayerMgr : netPlayerMgrBase
	{
	};
}

namespace gta
{
	inline constexpr auto num_players = 32;
}

class CNonPhysicalPlayerData : public rage::nonPhysicalPlayerDataBase
{
public:
	std::int32_t  m_bubble_id; // 0x08
	std::int32_t  m_player_id; // 0x0C
	rage::vector3 m_position;  // 0x10
};

class CNetGamePlayer : public rage::netPlayer
{
public:
	char pad_0008[8]; //0x0008
	class CNonPhysicalPlayerData* m_non_physical_player; //0x0010
	uint32_t m_msg_id; //0x0018
	char pad_001C[4]; //0x001C
	uint8_t m_active_id; //0x0020
	uint8_t m_player_id; //0x0021
	char pad_0022[126]; //0x0022
	class CPlayerInfo* m_player_info; //0x00A0
}; //Size: 0x00A8
static_assert(sizeof(CNetGamePlayer) == 0xA8);

class CPlayerInfo : public rage::fwExtensibleBase
{
public:
	class rage::netPlayerData m_net_player_data; //0x0020
	char pad_00B8[184]; //0x00B8
	float m_swim_speed; //0x0170
	char pad_0174[20]; //0x0174
	uint32_t m_water_proof; //0x0188
	char pad_018C[76]; //0x018C
	eGameState m_game_state; //0x01D8
	char pad_01DC[12]; //0x01DC
	class CPed* m_ped; //0x01E8
	char pad_01F0[40]; //0x01F0
	uint32_t m_frame_flags; //0x0218
	char pad_021C[52]; //0x021C
	uint32_t m_player_controls; //0x0250
	char pad_0254[1256]; //0x0254
	float m_wanted_can_change; //0x073C
	char pad_0740[304]; //0x0740
	uint32_t m_npc_ignore; //0x0870
	char pad_0874[12]; //0x0874
	bool m_is_wanted; //0x0880
	char pad_0881[7]; //0x0881
	uint32_t m_wanted_level; //0x0888
	uint32_t m_wanted_level_display; //0x088C
	char pad_0890[1120]; //0x0890
	float m_run_speed; //0x0CF0
	float m_stamina; //0x0CF4
	float m_stamina_regen; //0x0CF8
	char pad_0CFC[16]; //0x0CFC
	float m_weapon_damage_mult; //0x0D0C
	float m_weapon_defence_mult; //0x0D10
	char pad_0D14[4]; //0x0D14
	float m_melee_weapon_damage_mult; //0x0D18
	float m_melee_damage_mult; //0x0D1C
	float m_melee_defence_mult; //0x0D20
	char pad_0D24[8]; //0x0D24
	float m_melee_weapon_defence_mult; //0x0D2C
}; //Size: 0x0D30
static_assert(sizeof(CPlayerInfo) == 0x0D30);

static_assert(sizeof(CNonPhysicalPlayerData) == 0x1C);
#pragma pack(pop)
