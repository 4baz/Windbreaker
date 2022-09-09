#include "../hooking.h"
#include"../enums.h"
#include"../protection.h"

namespace hbase
{
	bool hooks::scripted_game_event(CScriptedGameEvent* scripted_game_event, CNetGamePlayer* player)
	{
		if (protection::is_block(player))
		{
			return true;
		}

		auto args = scripted_game_event->m_args;

		eRemoteEvent hash = (eRemoteEvent)args[0];
		g_settings.lastScriptEventHashReceived = (Hash)hash;
		char type[32] = "";

		switch (hash)
		{
		case eRemoteEvent::Bounty:
			if (g_settings.options["bounty"].get<bool>())
				strcpy(type, "Bounty");

			break;
		case eRemoteEvent::CeoBan:
			if (g_settings.options["ceo_ban"].get<bool>())
				strcpy(type, "Ceo Ban");

			break;
		case eRemoteEvent::CeoKick:
			if (g_settings.options["ceo_kick"].get<bool>())
				strcpy(type, "Ceo Kick");

			break;
		case eRemoteEvent::CeoMoney:
			if (g_settings.options["ceo_money"].get<bool>())
				strcpy(type, "Ceo Money");

			break;
		case eRemoteEvent::ClearWantedLevel:
			if (g_settings.options["clear_wanted_level"].get<bool>())
				strcpy(type, "Clear Wanted Level");

			break;
		case eRemoteEvent::FakeDeposit:
			if (g_settings.options["fake_deposit"].get<bool>())
				strcpy(type, "Deposit");

			break;
		case eRemoteEvent::ForceMission:
			if (g_settings.options["force_mission"].get<bool>())
				strcpy(type, "Force Mission");

			break;
		case eRemoteEvent::GtaBanner:
			if (g_settings.options["gta_banner"].get<bool>())
				strcpy(type, "GTA Banner");

			break;
		case eRemoteEvent::NetworkBail:
			if (g_settings.options["network_bail"].get<bool>())
				strcpy(type, "Network Bail");

			break;
		case eRemoteEvent::PersonalVehicleDestroyed:
			if (g_settings.options["personal_vehicle_destroyed"].get<bool>())
				strcpy(type, "Personal Vehicle Destroyed");

			break;
		case eRemoteEvent::RemoteOffradar:
			if (g_settings.options["remote_off_radar"].get<bool>())
				strcpy(type, "Remote Off Radar");

			break;
		case eRemoteEvent::RotateCam:
			if (g_settings.options["rotate_cam"].get<bool>())
				strcpy(type, "Rotate Cam");

			break;
		case eRemoteEvent::SendToCutscene:
			if (g_settings.options["send_to_cutscene"].get<bool>())
				strcpy(type, "Send To Cutscene");

			break;
		case eRemoteEvent::SendToIsland:
			if (g_settings.options["send_to_island"].get<bool>())
				strcpy(type, "Send To Island");

			break;
		case eRemoteEvent::SoundSpam:
			if (g_settings.options["sound_spam"].get<bool>())
				strcpy(type, "Sound Spam");

			break;
		case eRemoteEvent::Spectate:
			if (g_settings.options["spectate"].get<bool>())
				strcpy(type, "Specate");

			break;
		case eRemoteEvent::Teleport:
			if (g_settings.options["force_teleport"].get<bool>())
				strcpy(type, "Force Teleport");

			break;
		case eRemoteEvent::TransactionError:
			if (g_settings.options["transaction_error"].get<bool>())
				strcpy(type, "Transaction Error");

			break;
		case eRemoteEvent::VehicleKick:
			if (g_settings.options["vehicle_kick"].get<bool>())
				strcpy(type, "Vehicle Kick");

			break;
		}

		if (strlen(type) != 0)
		{
			LOG(INFO) << fmt::format("BLOCKED SCRIPT EVENT\nFrom: {}\nEvent Type: {}", player->get_name(), type);
			return true;
		}

		/*if (g->debug.script_event_logging)
		{
			LOG(INFO) << "== Begin of Script Event ==";
			LOG(INFO) << "Player: " << player->get_name();
			LOG(INFO) << "Hash/Arg #0: " << (int)hash;

			for (int i = 1; i < sizeof(args); i++)
				LOG(INFO) << "Arg #" << i << ": " << args[i];

			LOG(INFO) << "== End of Script Event ==";
		}*/

		return g_hooking->m_scripted_game_event_hook.get_original<functions::scripted_game_event_t>()(scripted_game_event, player);
	}
}