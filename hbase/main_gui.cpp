#include"main_gui.h"
#include"script.h"
#include"settings.h"
#include"fiber_pool.h"
#include"gta_util.hpp"
#include"features.h"
#include"mod_vehicle.h"
#include"script_global.h"
#include"data/player.h"
#include"data/vehicles.h"
#include"data/lists.h"
#include"enums.h"
#include"ui/ui_manager.h"
#include"ui/regular_submenu.h"
#include"ui/regular_option.h"
#include"ui/sub_option.h"
#include"ui/bool_option.h"
#include"ui/number_option.h"
#include"ui/choose_option.h"
#include"ui/break_option.h"
#include"ui/vehicle_option.h"
#include"ui/player_submenu.h"

namespace hbase
{
	enum Submenu : std::uint32_t
	{
		main_menu,
		protection_option,
		script_protection,
		net_event_protection,
		other_protection,
		advertise_protection,
		crash_protection,
		onlie_option,
		rid_jion,
		remove_player,
		host_tools,
		info_spoof,
		self_option,
		change_player_model,
		player_list,
		task_option,
		pericoIsland,
		casinorobbery,
		doomsdayheist,
		apartment,
		contracttask,
		one_player_option,
		remove_one_player,
		teleport_player,
		friendly_option,
		attack_options,
		weapon_option,
		create_vehicle,
		create_xml_vehicle,
		create_ini_vehicle,
		vehicle_option,
		teleport_option,
		menu_seetings
	};

	void main_gui::initialize()
	{
		m_initialized = true;
		std::size_t SELF = g_ui_mgr->addSub("SELF");//this is dumb im just too lazy to change
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "HOME", main_menu, [](RegularSubmenu* sub)
			{
			
				sub->add_option<SubOption>("Self Options", nullptr, self_option);
				sub->add_option<SubOption>("Weapon", nullptr, weapon_option);
				sub->add_option<SubOption>("Protections", nullptr, protection_option);
				sub->add_option<SubOption>("Online", nullptr, onlie_option);
				sub->add_option<SubOption>("Player List", nullptr, player_list);
				sub->add_option<SubOption>("Vehicle", nullptr, vehicle_option);
				sub->add_option<SubOption>("Teleports", nullptr, teleport_option);
				sub->add_option<SubOption>("Heists?", nullptr, task_option);

				sub->add_option<SubOption>("Settings", nullptr, menu_seetings);

				sub->add_option<BreakOption>("\"EtheShit\" Clone ");



			
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "Self Options", self_option, [](RegularSubmenu* sub)
			{//\"Updated\" by 4baz
				static int wanted_level = 0;
				sub->add_option<BoolOption<bool>>("God", "god", g_settings.options["god_mode"].get<bool*>());
				sub->add_option<BoolOption<bool>>("Never wanted", "never wanted", g_settings.options["never_wanted"].get<bool*>());
				sub->add_option<NumberOption<int>>("Wanted lvl", nullptr, &wanted_level, 0, 5, 1, 2, true, [] {features::set_player_wanted_level(wanted_level); });
				sub->add_option<BreakOption>("Player");
				sub->add_option<SubOption>("Change Player model", nullptr, change_player_model);
				sub->add_option<BoolOption<bool>>("Invisible", nullptr, g_settings.options["invisible"].get<bool*>());
				sub->add_option<BreakOption>("Movement");
				sub->add_option<NumberOption<float>>("Run Speed", nullptr, &features::run_speed, 0, 5, 0.1);
				sub->add_option<NumberOption<float>>("Swim Spped", nullptr, &features::swim_speed, 0, 5, 0.1);
				sub->add_option<BoolOption<bool>>("Super Jump", nullptr, g_settings.options["super_jump"].get<bool*>());
				sub->add_option<RegularOption>("Max health", "death", [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), ENTITY::GET_ENTITY_MAX_HEALTH(PLAYER::PLAYER_PED_ID()), 0);
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<RegularOption>("Max Armour", "", [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						PED::ADD_ARMOUR_TO_PED(PLAYER::PLAYER_PED_ID(), 400);
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<RegularOption>("Reset Ped", "", [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						PED::CLEAR_ALL_PED_PROPS(PLAYER::PLAYER_PED_ID());
						PED::CLEAR_PED_BLOOD_DAMAGE(PLAYER::PLAYER_PED_ID());
						PED::CLEAR_PED_WETNESS(PLAYER::PLAYER_PED_ID());
						PED::CLEAR_PED_DECORATIONS(PLAYER::PLAYER_PED_ID());
						PED::CLEAR_PED_DECORATIONS_LEAVE_SCARS(PLAYER::PLAYER_PED_ID());
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<RegularOption>("Sewslide", "", [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 0, 0);
					}
					QUEUE_JOB_END_CLAUSE
					});
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "Change player model", change_player_model, [](RegularSubmenu* sub)
			{
				static std::size_t ped_id{};
				sub->add_option<ChooseOption< const char*, std::size_t>>("Change model", "", &lists::player_model, &ped_id, false, [&] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						features::chage_player_model(MISC::GET_HASH_KEY(lists::player_model[ped_id]));
					}QUEUE_JOB_END_CLAUSE
					});
			});
		
	//	std::size_t PROTECTION = g_ui_mgr->addSub("PROTEX");
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "PROTECTION", protection_option, [](RegularSubmenu* sub)
			{
				sub->add_option<SubOption>("Net events", nullptr, net_event_protection);
				sub->add_option<SubOption>("Script events", nullptr, script_protection);
				sub->add_option<SubOption>("other", nullptr, other_protection);
				sub->add_option<SubOption>("Crash protex", nullptr, crash_protection);
				sub->add_option<SubOption>("Chat protex", nullptr, advertise_protection);
				sub->add_option<BoolOption<bool>>("script_kick_and_crash?", "china translation idk", g_settings.options["script_kick_and_crash"].get<bool*>());
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "Script protex", script_protection, [](RegularSubmenu* sub)
			{
				sub->add_option<BoolOption<bool>>("bounty", nullptr, g_settings.options["bounty"].get<bool*>());
				sub->add_option<BoolOption<bool>>("CEO ban", nullptr, g_settings.options["ceo_ban"].get<bool*>());
				sub->add_option<BoolOption<bool>>("CEO KICK", nullptr, g_settings.options["ceo_kick"].get<bool*>());
				sub->add_option<BoolOption<bool>>("CEO money", nullptr, g_settings.options["ceo_money"].get<bool*>());
				sub->add_option<BoolOption<bool>>("Clear wanted level", nullptr, g_settings.options["clear_wanted_level"].get<bool*>());
				sub->add_option<BoolOption<bool>>("Fake deposit", nullptr, g_settings.options["fake_deposit"].get<bool*>());
				sub->add_option<BoolOption<bool>>("Force mission", nullptr, g_settings.options["force_mission"].get<bool*>());
				sub->add_option<BoolOption<bool>>("Gta banner", nullptr, g_settings.options["gta_banner"].get<bool*>());
				sub->add_option<BoolOption<bool>>("net bail", nullptr, g_settings.options["network_bail"].get<bool*>());
				sub->add_option<BoolOption<bool>>("Personal Vehicle", nullptr, g_settings.options["personal_vehicle_destroyed"].get<bool*>());
				sub->add_option<BoolOption<bool>>("Remote off radar", nullptr, g_settings.options["remote_off_radar"].get<bool*>());
				sub->add_option<BoolOption<bool>>("Rotate cam", nullptr, g_settings.options["rotate_cam"].get<bool*>());
				sub->add_option<BoolOption<bool>>("send to cutscene≠", nullptr, g_settings.options["send_to_cutscene"].get<bool*>());
				sub->add_option<BoolOption<bool>>("sent to cayo perico", nullptr, g_settings.options["send_to_island"].get<bool*>());
				sub->add_option<BoolOption<bool>>("sound spam", nullptr, g_settings.options["sound_spam"].get<bool*>());
				sub->add_option<BoolOption<bool>>("camera joetate", nullptr, g_settings.options["rotate_cam"].get<bool*>());
				sub->add_option<BoolOption<bool>>("Spectate", nullptr, g_settings.options["spectate"].get<bool*>());
				sub->add_option<BoolOption<bool>>("force tp", nullptr, g_settings.options["force_teleport"].get<bool*>());
				sub->add_option<BoolOption<bool>>("transaction error", nullptr, g_settings.options["transaction_error"].get<bool*>());
				sub->add_option<BoolOption<bool>>("Vehicle kick", nullptr, g_settings.options["vehicle_kick"].get<bool*>());
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "Net events", net_event_protection, [](RegularSubmenu* sub)
			{
				sub->add_option<BoolOption<bool>>("freeze", nullptr, g_settings.options["freeze"].get<bool*>());
				sub->add_option<BoolOption<bool>>("control", nullptr, g_settings.options["control"].get<bool*>());
				sub->add_option<BoolOption<bool>>("increment_stat_event®", nullptr, g_settings.options["increment_stat_event"].get<bool*>());
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "other protex", other_protection, [](RegularSubmenu* sub)
			{
				sub->add_option<BoolOption<bool>>("disable_error_screen", nullptr, g_settings.options["disable_error_screen"].get<bool*>());
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "Advertise/chat protex", advertise_protection, [](RegularSubmenu* sub)
			{
				sub->add_option<BoolOption<bool>>("block_ad", nullptr, g_settings.options["block_ad"].get<bool*>());
				sub->add_option<BoolOption<bool>>("crash_ad", nullptr, g_settings.options["crash_ad"].get<bool*>());
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "crash protex", crash_protection, [](RegularSubmenu* sub)
			{
				sub->add_option<BoolOption<bool>>("invalid_sync_type", nullptr, g_settings.options["invalid_sync_type"].get<bool*>());
				sub->add_option<BoolOption<bool>>("sync_model_mismatch", nullptr, g_settings.options["sync_model_mismatch"].get<bool*>());
				sub->add_option<BoolOption<bool>>("model_crash", nullptr, g_settings.options["model_crash"].get<bool*>());
				sub->add_option<BoolOption<bool>>("NetArray", nullptr, g_settings.options["net_array"].get<bool*>());
				sub->add_option<BoolOption<bool>>("PlayerModel", nullptr, g_settings.options["sync_player_model"].get<bool*>());
			});

	//	std::size_t ONLINE = g_ui_mgr->addSub("ONLINE");
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "ONLINE", onlie_option, [](RegularSubmenu* sub)
			{
				sub->add_option<SubOption>("RID JOIN", nullptr, rid_jion);
				sub->add_option<SubOption>("INFO spoofer≠", nullptr, info_spoof);
				sub->add_option<SubOption>("Remove player", nullptr, remove_player);
				sub->add_option<SubOption>("Host tools", nullptr, host_tools);
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "RID", rid_jion, [](RegularSubmenu* sub)
			{
				sub->add_option<RegularOption>(std::move(RegularOption("RID").SetRightText(std::to_string(g_settings.options["join_rid"].get<int>()).c_str())));
				sub->add_option<RegularOption>("Input rid?", "", [] {
					g_ui_mgr->inputBox([] {g_settings.options["join_rid"] = atoi(g_ui_mgr->getInputText().c_str()); });
					});
				sub->add_option<RegularOption>("Join lobby?", "", [] {
					g_fiber_pool->queue_job([]
						{
							if (g_pointers->m_menu_ptr)
							{
								do
								{
									HUD::ACTIVATE_FRONTEND_MENU(0xD528C7E2, 0, 2);
									script::get_current()->yield(1s);

									if (*g_pointers->m_menu_ptr)
									{
										int hash = -632792895;
										int64_t ptr = *g_pointers->m_menu_ptr;
										g_pointers->m_rid_joiner(&ptr, &hash);
									}
								} while (!*g_pointers->m_menu_ptr);
							}
						});
					});
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "ONLINE", remove_player, [](RegularSubmenu* sub)
			{
				sub->add_option<RegularOption>("Tp underground", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						Vector3 pos = { -16257.853, 10793.688, 893.610 };
						Ped myPed = PLAYER::PLAYER_PED_ID();
						Vector3 myPos = ENTITY::GET_ENTITY_COORDS(myPed, false);
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(myPed, pos.x, pos.y, pos.z, false, false, false);
						ENTITY::FREEZE_ENTITY_POSITION(myPed, true);
						script::get_current()->yield(3s);
						ENTITY::FREEZE_ENTITY_POSITION(myPed, false);
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(myPed, myPos.x, myPos.y, myPos.z, false, false, false);
					}
					QUEUE_JOB_END_CLAUSE
					});

				sub->add_option<RegularOption>(std::move(RegularOption("Crash player", nullptr, [] {
					g_settings.buffer_crash.m_buffer_crash = true;
					}).SetRightText(std::to_string(g_settings.buffer_crash.m_buffer_crash_count).c_str())));
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "Host tools", host_tools, [](RegularSubmenu* sub)
			{
				sub->add_option<RegularOption>("force script host?", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						CNetGamePlayer* player = g_pointers->m_get_net_game_player(PLAYER::PLAYER_ID());
						if (player)
						{
							g_pointers->m_forceScriptHost(g_settings.script_handler_net_component_mgr, player,++g_settings.lastToken,1);
						}
					}
					QUEUE_JOB_END_CLAUSE
					});
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "spoofing≠", info_spoof, [](RegularSubmenu* sub)
			{
				sub->add_option<BoolOption<bool>>("spoof_rockstar_id≠", nullptr, g_settings.options["spoof_rockstar_id"].get<bool*>());
				sub->add_option<RegularOption>(std::move(RegularOption("RID").SetRightText(std::to_string(g_settings.options["rockstar_id"].get<int>()).c_str())));
				sub->add_option<RegularOption>("rockstar_id", nullptr, [] {g_ui_mgr->inputBox([] {g_settings.options["rockstar_id"] = atoi(g_ui_mgr->getInputText().c_str()); }); });
				sub->add_option<BreakOption>("spoof username");
				sub->add_option<BoolOption<bool>>("spoof_username≠", nullptr, g_settings.options["spoof_username"].get<bool*>());
				sub->add_option<RegularOption>(std::move(RegularOption("username").SetRightText(g_settings.options["username"].get<std::string>().c_str())));
				sub->add_option<RegularOption>("username input", nullptr, [] {g_ui_mgr->inputBox([] {g_settings.options["username"] = g_ui_mgr->getInputText(); }); });
				sub->add_option<BreakOption>("spoof host token?");
				sub->add_option<BoolOption<bool>>("spoof_host_token≠", nullptr, g_settings.options["spoof_host_token"].get<bool*>());
				sub->add_option<RegularOption>(std::move(RegularOption("host_token?").SetRightText(std::to_string(g_settings.options["host_token"].get<int>()).c_str())));
				sub->add_option<RegularOption>("input host_token", nullptr, [] {g_ui_mgr->inputBox([] {g_settings.options["host_token"] = atoi(g_ui_mgr->getInputText().c_str()); }); });
				sub->add_option<BreakOption>("IP");
				sub->add_option<BoolOption<bool>>("spoof ip≠", nullptr, g_settings.options["spoof_ip"].get<bool*>());
				sub->add_option<RegularOption>(std::move(RegularOption("IP[1]").SetRightText(std::to_string(g_settings.options["ip"][1].get<int>()).c_str())));
				sub->add_option<RegularOption>("spoof IP[1]", nullptr, [] {g_ui_mgr->inputBox([] {g_settings.options["ip"][1] = atoi(g_ui_mgr->getInputText().c_str()); }); });
				sub->add_option<RegularOption>(std::move(RegularOption("IP[2]").SetRightText(std::to_string(g_settings.options["ip"][2].get<int>()).c_str())));
				sub->add_option<RegularOption>("spoof [2]", nullptr, [] {g_ui_mgr->inputBox([] {g_settings.options["ip"][2] = atoi(g_ui_mgr->getInputText().c_str()); }); });
				sub->add_option<RegularOption>(std::move(RegularOption("IP[3]").SetRightText(std::to_string(g_settings.options["ip"][3].get<int>()).c_str())));
				sub->add_option<RegularOption>("spoof [3]", nullptr, [] {g_ui_mgr->inputBox([] {g_settings.options["ip"][3] = atoi(g_ui_mgr->getInputText().c_str()); }); });
			});

		//std::size_t PLAYER_LIST = g_ui_mgr->addSub("PLAYERLIST");
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "PLAYERLIST", onlie_option, [](RegularSubmenu* sub)
			{
				for (size_t i = 0; i < 32; i++)
				{
					player_class& pc = g_players.m_players[i];
					if (pc.is_onlie)
					{
						std::string name{};
						name += pc.player_name;
						if (pc.is_host) {
							name += " [is host]";
						}
						if (pc.is_me)
						{
							name += " [self]";
						}
						if (pc.is_script_host)
						{
							name += " [Script host]";
						}
						sub->add_option<SubOption>(name.c_str(), nullptr, one_player_option, [i] {g_players.player_id = i; });
					}
				}

				g_settings.spectating = false;
			});
		g_ui_mgr->add_submenu<PlayerSubmenu>(SELF, &g_players.player_id, one_player_option, [](PlayerSubmenu* sub)
			{
				sub->add_option<BoolOption<bool>>("spectate", nullptr, &g_settings.spectating);
				sub->add_option<SubOption>("remove player?", nullptr, remove_one_player);
				sub->add_option<SubOption>("teleport player", nullptr, teleport_player);
				sub->add_option<SubOption>("friendly options?", nullptr, friendly_option);
				sub->add_option<SubOption>("griefing?", nullptr, attack_options);
				sub->add_option<RegularOption>("copy outfit", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE() {
						features::copy_outfit(g_players.player_id);
					}QUEUE_JOB_END_CLAUSE});
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "griefing", attack_options, [](RegularSubmenu* sub)
			{
				sub->add_option<RegularOption>("CeoBan", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						int64_t data[] = { -764524031, g_players.player_id, 1 };

						g_pointers->m_trigger_script_event(1, data, 3, 1 << g_players.player_id);
					}
					QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("CeoKick", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						int64_t  data[] = { 248967238, g_players.player_id, 1, 5 };

						g_pointers->m_trigger_script_event(1, data, 4, 1 << g_players.player_id);
					}
					QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("lord who fucking knows", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						int64_t data[] = { -621279188, g_players.player_id, 1, 0, 1 };

						g_pointers->m_trigger_script_event(1, data, 5, 1 << g_players.player_id);
					}
					QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("Clear wanted", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						constexpr size_t arg_count = 3;
						int64_t args[arg_count] = {
							(int)eRemoteEvent::ClearWantedLevel,
							0,
							*script_global(1893548).at(g_players.player_id, 600).at(511).as<int*>()
						};

						g_pointers->m_trigger_script_event(1, args, arg_count, 1 << g_players.player_id);
					}
					QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("Clone", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE() {
						features::clone_player(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id));
					}QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("Cage", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE() {
						features::cage_player(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id));
					}QUEUE_JOB_END_CLAUSE });
				sub->add_option<RegularOption>("Ram", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE() {
						features::ram_player();
					}QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("air strike", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE() {
						features::air_strike_player(g_players.player_id);
					}QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("Clone", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE() {
						features::clone_player(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id));
					}QUEUE_JOB_END_CLAUSE});
				sub->add_option<BoolOption<bool>>("Water loop", nullptr, &g_settings.f_waterloop, false, [] {
					QUEUE_JOB_BEGIN_CLAUSE() {
						features::water_loop(g_settings.f_waterloop, g_players.player_id);
					}QUEUE_JOB_END_CLAUSE });
				sub->add_option<BoolOption<bool>>("Fire loop", nullptr, &g_settings.f_fireloop, false, [] {
					QUEUE_JOB_BEGIN_CLAUSE() {
						features::fire_loop(g_settings.f_fireloop, g_players.player_id);
					}QUEUE_JOB_END_CLAUSE});
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "Friendly", friendly_option, [](RegularSubmenu* sub)
			{
				sub->add_option<RegularOption>("give all weapons (mk1 only?)", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						features::give_all_weapons();
					}
					QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("give mk2 weapons", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE() {
						features::give_all_mk2();
					}QUEUE_JOB_END_CLAUSE });
				sub->add_option<RegularOption>("stun gun", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						features::give_stun_gun();
					}
					QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("Give digi scanner?", "wtf is a digi scanner frfr", [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						features::give_digiscanner();
					}
					QUEUE_JOB_END_CLAUSE});
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "uhhh idk all players?", remove_one_player, [](RegularSubmenu* sub)
			{
				sub->add_option<RegularOption>("kick player", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						NETWORK::NETWORK_SESSION_KICK_PLAYER(g_players.player_id);
					}
					QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("some fucking script event", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						int64_t data[] = { -1386010354, g_players.player_id, 4212424, 442214, 0, 0, 0, 0, 0, 0 };
						g_pointers->m_trigger_script_event(1, data, 10, 1 << g_players.player_id);
					}
					QUEUE_JOB_END_CLAUSE});
				sub->add_option<BoolOption<bool>>("Crash objects?", "maybe crash lobby with object crash?", &g_settings.m_spawn_crash_objects);
				sub->add_option<RegularOption>("kick player?", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						CNetGamePlayer* player = g_pointers->m_get_net_game_player(g_players.player_id);
						if (player)
						{
							rage::CNetworkPlayerMgr* player_mgr = *g_pointers->m_network_player_mgr;
							if (player_mgr)
							{
								g_pointers->m_remove_player(player_mgr, player);
							}
						}
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<RegularOption>(std::move(RegularOption("remove ped crash", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						features::de_remove_ped_crash();
					}
					QUEUE_JOB_END_CLAUSE
					}).SetRightText(g_settings.m_de_remove_crash ? "" : "")));
				sub->add_option<RegularOption>(std::move(RegularOption("vehicle crash", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						features::de_remove_veh_crash();
					}
					QUEUE_JOB_END_CLAUSE
					}).SetRightText(g_settings.m_de_remove_crash ? "" : "")));
				sub->add_option<RegularOption>(std::move(RegularOption("veh crash ? (V2)", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						features::de_remove_veh_crash1();
					}
					QUEUE_JOB_END_CLAUSE
					}).SetRightText(g_settings.m_de_remove_crash ? "wtf is this shit fr" : "")));
				sub->add_option<RegularOption>("spawn crash vehicle???", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id);
						if (ENTITY::DOES_ENTITY_EXIST(ped)) {
							features::spawn_crash_vehicle(ped, "adder", 8, 0);
							features::spawn_crash_vehicle(ped, "blazer", 11, 3);
							features::spawn_crash_vehicle(ped, "dinghy", 1, 13);
							features::spawn_crash_vehicle(ped, "zentorno", 8, 0);
						}
					}
					QUEUE_JOB_END_CLAUSE
					});
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "teleport player", teleport_player, [](RegularSubmenu* sub)
			{
				sub->add_option<RegularOption>("tp player to self", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						Ped myPed = PLAYER::PLAYER_PED_ID();
						Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_players.player_id);
						Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped, false);
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(myPed, pos.x, pos.y, pos.z, false, false, false);
					}
					QUEUE_JOB_END_CLAUSE
					});
			});

	//	std::size_t WEAPON = g_ui_mgr->addSub("WEAPON");
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "Weapon options", weapon_option, [](RegularSubmenu* sub)
			{
				sub->add_option<RegularOption>("give all weapons", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						for (Hash w : lists::all_weapon) {
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), w, 9999, true);
						}
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<RegularOption>("Remove all weapons", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::PLAYER_PED_ID(), false);
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<RegularOption>("give ammo", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						for (Hash w : lists::all_weapon) {
							WEAPON::SET_PED_AMMO(PLAYER::PLAYER_PED_ID(), w, 9999, true);
						}
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<BreakOption>("Ammo modifiers");
				sub->add_option<BoolOption<bool>>("unlimited_ammo", nullptr, g_settings.options["unlimited_ammo"].get<bool*>());
				sub->add_option<BoolOption<bool>>("explosive_ammo", nullptr, g_settings.options["explosive_ammo"].get<bool*>());
				sub->add_option<BoolOption<bool>>("Rapid fire", "", g_settings.options["rapid_fire"].get<bool*>());
			});

//		std::size_t VEHICLE = g_ui_mgr->addSub("VEHICLE");
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "VEHICLE", vehicle_option, [](RegularSubmenu* sub)
			{
				sub->add_option<SubOption>("Spawn Vehicles", nullptr, create_vehicle);
				sub->add_option<BoolOption<bool>>("vehicle_invincible", nullptr, g_settings.options["vehicle_invincible"].get<bool*>());
				sub->add_option<BoolOption<bool>>("safety_belt", nullptr, g_settings.options["safety_belt"].get<bool*>());
				sub->add_option<BoolOption<bool>>("infinite_nitrogen", "æﬂ”–≈Á…‰π¶ƒ‹µƒ‘ÿæﬂø…“ª÷±≈Á…‰ ø’∏ÒÕ£≈Á", g_settings.options["infinite_nitrogen"].get<bool*>());
				sub->add_option<BoolOption<bool>>("9_key_speed_up (what the fuck)", "I stg if this binds boost too the 9 key thats dumb -baz", g_settings.options["9_key_speed_up"].get<bool*>());
				sub->add_option<BoolOption<bool>>("rainbow_vehicle", nullptr, g_settings.options["rainbow_vehicle"].get<bool*>());
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "veh spawner", create_vehicle, [](RegularSubmenu* sub)
			{
				sub->add_option<BoolOption<bool>>("delete_last_vehicle", "", g_settings.options["delete_last_vehicle"].get<bool*>());
				sub->add_option<BoolOption<bool>>("spwan_vehicle_max", nullptr, g_settings.options["spwan_vehicle_max"].get<bool*>());
				sub->add_option<BoolOption<bool>>("spwan_vehicle_in", nullptr, g_settings.options["spwan_vehicle_in"].get<bool*>());
				sub->add_option<SubOption>("xml vehicle (disabled)", nullptr, create_xml_vehicle);
				sub->add_option<SubOption>("ini vehicle (disabled)", nullptr, create_ini_vehicle);
				sub->add_option<BreakOption>("Break option");
				for (VehicleClass& vc : g_vehicles.classes)
				{
					sub->add_option<VehicleOption>(vc.className.c_str(), nullptr, vc.vehicles, &vc.p, false, [vc] {
						QUEUE_JOB_BEGIN_CLAUSE(vc)
						{
							features::spawn_vehicle(vc.vehicles[vc.p].hash);
						}
						QUEUE_JOB_END_CLAUSE
						});
				}
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "xml", create_xml_vehicle, [&](RegularSubmenu* sub)
			{
				for (std::string& str : m_xmlVehicleList)
				{
					sub->add_option<RegularOption>(str.c_str(), nullptr, [&] {
						std::string file = std::string(MENU_PATH) + "\\vehicle\\xml\\" + str + ".xml";

						QUEUE_JOB_BEGIN_CLAUSE(file)
						{
							modvehicle::create_xml_vehicle(file.c_str());
						}
						QUEUE_JOB_END_CLAUSE
						});
				}
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "ini", create_ini_vehicle, [&](RegularSubmenu* sub)
			{
				for (std::string& str : m_iniVehicleList)
				{
					sub->add_option<RegularOption>(str.c_str(), nullptr, [&] {
						std::string file = std::string(MENU_PATH) + "\\vehicle\\ini\\" + str + ".ini";
						QUEUE_JOB_BEGIN_CLAUSE(file)
						{
							modvehicle::create_ini_vehicle(file.c_str());
						}
						QUEUE_JOB_END_CLAUSE
						});
				}
			});

	//	std::size_t TELEPORT = g_ui_mgr->addSub("TELEPORTS");
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "TELEPORTS", teleport_option, [](RegularSubmenu* sub)
			{
				sub->add_option<RegularOption>("Waypoint", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						features::tp_to_waypoint();
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<RegularOption>("Objective", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						features::teleport_to_objective();
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<BreakOption>("Yeah i cant translate these");
				sub->add_option<RegularOption>("1", nullptr, [] {	QUEUE_JOB_BEGIN_CLAUSE() { features::tp_to_vector3({ 336.158, 4373.934, 64.46 }); }QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("2", nullptr, [] {	QUEUE_JOB_BEGIN_CLAUSE() { features::tp_to_vector3({ 3156.38355,-355.16663, -20.09742 }); }QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("3", nullptr, [] {	QUEUE_JOB_BEGIN_CLAUSE() { features::tp_to_vector3({ 3067.58, 2212.25, 3.00 }); }QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("4", nullptr, [] {	QUEUE_JOB_BEGIN_CLAUSE() { features::tp_to_vector3({ -430.905, 1135.2722, 325.904 }); }QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("5", nullptr, [] {	QUEUE_JOB_BEGIN_CLAUSE() { features::tp_to_vector3({ 719.30,1204.76,325.88 }); }QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("6", nullptr, [] {	QUEUE_JOB_BEGIN_CLAUSE() { features::tp_to_vector3({ -256.43,6569.93, 2.66 }); }QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("7", nullptr, [] {	QUEUE_JOB_BEGIN_CLAUSE() { features::tp_to_vector3({ -604.92, 2113.95, 127.11 }); }QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("8", nullptr, [] {	QUEUE_JOB_BEGIN_CLAUSE() { features::tp_to_vector3({ -74.94243,-818.63446, 326.174347 }); }QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("9", nullptr, [] {	QUEUE_JOB_BEGIN_CLAUSE() { features::tp_to_vector3({ -2012.8470f, 2956.5270f, 32.8101f }); }QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("10", nullptr, [] {	QUEUE_JOB_BEGIN_CLAUSE() { features::tp_to_vector3({ -2356.0940, 3248.645, 101.4505 }); }QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("11", nullptr, [] {	QUEUE_JOB_BEGIN_CLAUSE() { features::tp_to_vector3({ -1338.16, -1278.11, 4.87 }); }QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("LSC", nullptr, [] {	QUEUE_JOB_BEGIN_CLAUSE() { features::tp_to_vector3({ -373.01, -124.91, 38.31 }); }QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("13", nullptr, [] {	QUEUE_JOB_BEGIN_CLAUSE() { features::tp_to_vector3({ 247.3652, -45.8777, 69.9411 }); }QUEUE_JOB_END_CLAUSE});
				sub->add_option<RegularOption>("14", nullptr, [] {	QUEUE_JOB_BEGIN_CLAUSE() { features::tp_to_vector3({ -1102.2910f, -2894.5160f, 13.9467f }); }QUEUE_JOB_END_CLAUSE});
			});

//		std::size_t TASK = g_ui_mgr->addSub("TASK");
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "i think this is heist editor", task_option, [](RegularSubmenu* sub)
			{
				sub->add_option<SubOption>("perico", nullptr, pericoIsland);
				sub->add_option<SubOption>("Casino", nullptr, casinorobbery);
				sub->add_option<SubOption>("Doomsday", nullptr, doomsdayheist);
				sub->add_option<SubOption>("apt (og heists?)", nullptr, apartment);
				sub->add_option<SubOption>("Contract task", nullptr, contracttask);
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "Perico island", pericoIsland, [](RegularSubmenu* sub)
			{
				static int player1 = 100, player2 = 100, player3 = 100, player4 = 100;
				sub->add_option<RegularOption>("uhhh fuck knows, sets stats", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4_MISSIONS"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4_PROGRESS"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4_PLAYTHROUGH_STATUS"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4CNF_APPROACH"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4CNF_BS_ENTR"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4CNF_BS_GEN"), 0, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4_MISSIONS"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4_PROGRESS"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4_PLAYTHROUGH_STATUS"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4CNF_APPROACH"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4CNF_BS_ENTR"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4CNF_BS_GEN"), 0, 0);
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<RegularOption>("set some stats again", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4CNF_BS_GEN"), 131071, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4CNF_BS_ENTR"), 63, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4CNF_BS_ABIL"), 63, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4CNF_APPROACH"), -1, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4_PROGRESS"), 131055, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4CNF_TARGET"), 5, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4_MISSIONS"), 65345, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4CNF_WEAPONS"), 2, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4CNF_TROJAN"), 1, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H4_PLAYTHROUGH_STATUS"), 10, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4CNF_BS_GEN"), 131071, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4CNF_BS_ENTR"), 63, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4CNF_BS_ABIL"), 63, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4CNF_APPROACH"), -1, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4_PROGRESS"), 131055, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4CNF_TARGET"), 5, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4_MISSIONS"), 65345, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4CNF_WEAPONS"), 2, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4CNF_TROJAN"), 1, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H4_PLAYTHROUGH_STATUS"), 10, 0);
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<NumberOption<int>>("goofy ahh global one", nullptr, &player1, 0, 100);
				sub->add_option<RegularOption>("uhhh", nullptr, [] {
					*script_global(1973496).at(823).at(56).at(1).as<int*>() = player1;
					});
				sub->add_option<NumberOption<int>>("goofy ahh global 2", nullptr, &player2, 0, 100);
				sub->add_option<RegularOption>("uhh 2", nullptr, [] {
					*script_global(1973496).at(823).at(56).at(2).as<int*>() = player2;
					});
				sub->add_option<NumberOption<int>>("goofy ahh global 3", nullptr, &player3, 0, 100);
				sub->add_option<RegularOption>("uhh 3", nullptr, [] {
					*script_global(1973496).at(823).at(56).at(3).as<int*>() = player3;
					});
				sub->add_option<NumberOption<int>>("goofy ahh global 3", nullptr, &player4, 0, 100);
				sub->add_option<RegularOption>("uhhh 3", nullptr, [] {
					*script_global(1973496).at(823).at(56).at(4).as<int*>() = player4;
					});
				sub->add_option<RegularOption>("teleport somewhere", nullptr, [] {
					for (int i = 0; i < 4; i++) {
						if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i)) && PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i) != PLAYER::PLAYER_ID()) {
							Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), 1);
							FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 0, 1, 1, 0, 1, 0);
						}
					}
					});
				sub->add_option<RegularOption>("¥teleport somewhere else", nullptr, [] {
					ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 5006.70, -5755.89, 15.48, 1, 0, 0, 0);
					script::get_current()->yield();
					});
				sub->add_option<RegularOption>("teleport somewhere again", nullptr, [] {
					ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 5006.70, -5755.89, 15.48, 1, 0, 0, 0);
					script::get_current()->yield();
					});
				sub->add_option<RegularOption>("teleport to a place", nullptr, [] {
					ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 4580, -6012, 3, 1, 0, 0, 0);
					script::get_current()->yield();
					});
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "Casino", casinorobbery, [](RegularSubmenu* sub)
			{
				static int player1 = 100, player2 = 100, player3 = 100, player4 = 100;
				sub->add_option<RegularOption>("set some stats", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_TARGET"), 3, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_TARGET"), 3, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_APPROACH"), 2, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_APPROACH"), 2, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_ACCESSPOINTS"), -1, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_ACCESSPOINTS"), -1, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_POI"), -1, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_POI"), -1, 0);
						script::get_current()->yield(50ms);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_BITSET1"), -1, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_BITSET1"), -1, 0);
						script::get_current()->yield(2000ms);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_DISRUPTSHIP"), 3, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_DISRUPTSHIP"), 3, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_KEYLEVELS"), 2, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_KEYLEVELS"), 2, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_CREWDRIVER"), 5, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_CREWDRIVER"), 5, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_CREWWEAP"), 4, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_CREWWEAP"), 4, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_CREWHACKER"), 4, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_CREWHACKER"), 4, 0);
						script::get_current()->yield(50ms);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_BITSET0"), -1, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_BITSET0"), -1, 0);
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<RegularOption>("set some other stats", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_BITSET1"), 0, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_BITSET1"), 0, 0);
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<RegularOption>("set some more", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_BITSET0"), 0, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_BITSET0"), 0, 0);
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<NumberOption<int>>("global uno", nullptr, &player1, 0, 100);
				sub->add_option<RegularOption>("ddddd", nullptr, [] {
					*script_global(1966718).at(1497).at(736).at(92).at(1).as<int*>() = player1;
					});
				sub->add_option<NumberOption<int>>("global dos", nullptr, &player2, 0, 100);
				sub->add_option<RegularOption>("aaaaa", nullptr, [] {
					*script_global(1966718).at(1497).at(736).at(92).at(2).as<int*>() = player2;
					});
				sub->add_option<NumberOption<int>>("global 3 ", nullptr, &player3, 0, 100);
				sub->add_option<RegularOption>("ffff", nullptr, [] {
					*script_global(1966718).at(1497).at(736).at(92).at(3).as<int*>() = player3;
					});
				sub->add_option<NumberOption<int>>("global 4", nullptr, &player4, 0, 100);
				sub->add_option<RegularOption>("idk spanish tbh", nullptr, [] {
					*script_global(1966718).at(1497).at(736).at(92).at(4).as<int*>() = player4;
					});
				sub->add_option<RegularOption>("set some stats again (i think this is payouts)", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_CREWWEAP"), 6, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_CREWDRIVER"), 6, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_CREWHACKER"), 6, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_CREWWEAP"), 6, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_CREWDRIVER"), 6, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_CREWHACKER"), 6, 0);
						*script_global(262145).at(28439).as<float*>() = 0;
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<RegularOption>("some other shit (stats)", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_BITSET1"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_BITSET0"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_POI"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_H3OPT_ACCESSPOINTS"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CAS_HEIST_FLOW"), 0, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_BITSET1"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_BITSET0"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_POI"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_H3OPT_ACCESSPOINTS"), 0, 0);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CAS_HEIST_FLOW"), 0, 0);
					}
					QUEUE_JOB_END_CLAUSE
					});
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "Doomsday", doomsdayheist, [](RegularSubmenu* sub)
			{
				static int player1 = 100, player2 = 100, player3 = 100, player4 = 100;
				static int endDay_combo_pos = 0;
				static std::vector<char*> endDay_combo = {
					"gibberish 1",
					"gibberish 2",
					"gibberish 3"
				};
				sub->add_option<ChooseOption<char*, int>>("endDay_combo", nullptr, &endDay_combo, &endDay_combo_pos);
				sub->add_option<RegularOption>("MP0_GANGOPS_FLOW_MISSION_PROG", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						switch (endDay_combo_pos)
						{
						case 0:
						{
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_GANGOPS_FLOW_MISSION_PROG"), 503, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_GANGOPS_HEIST_STATUS"), 229383, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_GANGOPS_FLOW_NOTIFICATIONS"), 1557, 0);
							//
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_GANGOPS_FLOW_MISSION_PROG"), 503, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_GANGOPS_HEIST_STATUS"), 229383, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_GANGOPS_FLOW_NOTIFICATIONS"), 1557, 0);
							//	features::notifyfmt("“—Ã¯π˝«∞÷√:~b~ ˝æ›–π¬∂");
							break;
						}
						case 1:
						{
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_GANGOPS_FLOW_MISSION_PROG"), 240, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_GANGOPS_HEIST_STATUS"), 229378, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_GANGOPS_FLOW_NOTIFICATIONS"), 1557, 0);
							//
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_GANGOPS_FLOW_MISSION_PROG"), 240, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_GANGOPS_HEIST_STATUS"), 229378, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_GANGOPS_FLOW_NOTIFICATIONS"), 1557, 0);
							//	features::notifyfmt("“—Ã¯π˝«∞÷√:~b~≤®∏Òµ§Œ£ª˙");
							break;
						}
						case 2:
						{
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_GANGOPS_FLOW_MISSION_PROG"), 16368, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_GANGOPS_HEIST_STATUS"), 229380, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_GANGOPS_FLOW_NOTIFICATIONS"), 1557, 0);
							//
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_GANGOPS_FLOW_MISSION_PROG"), 16368, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_GANGOPS_HEIST_STATUS"), 229380, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_GANGOPS_FLOW_NOTIFICATIONS"), 1557, 0);
							//	features::notifyfmt("“—Ã¯π˝«∞÷√:~b~ƒ©»’Ω´÷¡");
							break;
						}
						}
					}
					QUEUE_JOB_END_CLAUSE
					});

				sub->add_option<NumberOption<int>>("bellow are globals", nullptr, &player1, 0, 100);
				sub->add_option<RegularOption>("fff", nullptr, [] {
					*script_global(1962755).at(812).at(50).at(1).as<int*>() = player1;
					});
				sub->add_option<NumberOption<int>>("gg", nullptr, &player2, 0, 100);
				sub->add_option<RegularOption>("ggg", nullptr, [] {
					*script_global(1962755).at(812).at(50).at(2).as<int*>() = player2;
					});
				sub->add_option<NumberOption<int>>("fff", nullptr, &player3, 0, 100);
				sub->add_option<RegularOption>("ffff", nullptr, [] {
					*script_global(1962755).at(812).at(50).at(3).as<int*>() = player3;
					});
				sub->add_option<NumberOption<int>>("fff", nullptr, &player4, 0, 100);
				sub->add_option<RegularOption>("ffff", nullptr, [] {
					*script_global(1962755).at(812).at(50).at(4).as<int*>() = player4;
					});
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "apartment", apartment, [](RegularSubmenu* sub)
			{
				static int player1 = 100, player2 = 100, player3 = 100, player4 = 100;
				sub->add_option<RegularOption>("stats", "", [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_HEIST_PLANNING_STAGE"), -1, 0);
						//
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_HEIST_PLANNING_STAGE"), -1, 0);
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<NumberOption<int>>("globals again", nullptr, &player1, 0, 100);
				sub->add_option<RegularOption>("aaaa", nullptr, [] {
					*script_global(1934631).at(3008).at(1).as<int*>() = player1;
					});
				sub->add_option<NumberOption<int>>("aaa", nullptr, &player2, 0, 100);
				sub->add_option<RegularOption>("aaaa", nullptr, [] {
					*script_global(1934631).at(3008).at(2).as<int*>() = player2;
					});
				sub->add_option<NumberOption<int>>("aaa", nullptr, &player3, 0, 100);
				sub->add_option<RegularOption>("aaaa", nullptr, [] {
					*script_global(1934631).at(3008).at(3).as<int*>() = player3;
					});
				sub->add_option<NumberOption<int>>("aaaa", nullptr, &player4, 0, 100);
				sub->add_option<RegularOption>("dssdsdsd", nullptr, [] {
					*script_global(1934631).at(3008).at(4).as<int*>() = player4;
					});
			});
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "Contract", contracttask, [](RegularSubmenu* sub)
			{
				static std::vector<const char*> Contract_combo = {
					"fortnite",
					"ninja",
					"tyler blevins",
					"balls)",
					"gaymer is sick"
				};
				static int Contract_combo_pos = 0;
				static int finalMoney = 1000000;
				sub->add_option<ChooseOption<const char*, int>>("blintega", nullptr, &Contract_combo, &Contract_combo_pos);
				sub->add_option<RegularOption>("FUCK THIS IS BORING ", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						switch (Contract_combo_pos)
						{
						case 0:
						{
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_GENERAL_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_COMPLETED_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_BS"), 28, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_STRAND"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_COOLDOWN"), -1, 0);
							//
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_GENERAL_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_COMPLETED_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_BS"), 28, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_STRAND"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_COOLDOWN"), -1, 0);
							//	features::notifyfmt("“—Ã¯π˝»ŒŒÒ:~p~“π…˙ªÓ–π√‹");
							break;
						}
						case 1:
						{
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_GENERAL_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_COMPLETED_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_BS"), 124, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_STRAND"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_COOLDOWN"), -1, 0);
							//
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_GENERAL_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_COMPLETED_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_BS"), 124, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_STRAND"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_COOLDOWN"), -1, 0);
							//	features::notifyfmt("“—Ã¯π˝»ŒŒÒ:~p~…œ¡˜…Áª·–π√‹");
							break;
						}
						case 2:
						{
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_GENERAL_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_COMPLETED_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_BS"), 2044, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_STRAND"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_COOLDOWN"), -1, 0);
							//
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_GENERAL_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_COMPLETED_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_BS"), 2044, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_STRAND"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_COOLDOWN"), -1, 0);
							//features::notifyfmt("“—Ã¯π˝»ŒŒÒ:~p~ƒœ÷––ƒ–π√‹");
							break;
						}
						case 3:
						{
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_GENERAL_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_COMPLETED_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_BS"), 4092, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_STRAND"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_COOLDOWN"), -1, 0);
							//
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_GENERAL_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_COMPLETED_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_BS"), 4092, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_STRAND"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_COOLDOWN"), -1, 0);
							//	features::notifyfmt("“—Ã¯π˝»ŒŒÒ:~p~∑÷∫Ïπÿ(µµ∞∏»ŒŒÒ)(xx≤© ø)");
							break;
						}
						case 4:
						{
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_GENERAL_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_COMPLETED_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_STRAND"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIXER_STORY_COOLDOWN"), -1, 0);
							//
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_GENERAL_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_COMPLETED_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_BS"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_STRAND"), -1, 0);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIXER_STORY_COOLDOWN"), -1, 0);
							//	features::notifyfmt("“—“ªº¸Õ®πÿ");
							break;
						}
						}
					}
					QUEUE_JOB_END_CLAUSE
					});
				sub->add_option<NumberOption<int>>("finalMoney ", "", &finalMoney, 0, 1000000);
				sub->add_option<RegularOption>("a global", nullptr, [] {
					*script_global(262145).at(31373).as<int*>() = finalMoney;
					});
				sub->add_option<RegularOption>("teleport option", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 510.23, -667.92, 24.86, 1, 0, 0, 0);
						script::get_current()->yield();
					}
					QUEUE_JOB_END_CLAUSE					});
				sub->add_option<RegularOption>("another tp option", nullptr, [] {
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), -935, -3018, 19, 1, 0, 0, 0);
						script::get_current()->yield();
					}
					QUEUE_JOB_END_CLAUSE
					});
			});

	//	std::size_t SELF = g_ui_mgr->addSub("SETTINGS");
		g_ui_mgr->add_submenu<RegularSubmenu>(SELF, "settings", menu_seetings, [](RegularSubmenu* sub)
			{
				sub->add_option<RegularOption>("Unload", nullptr, [] {
					g_running = false;
					});
				sub->add_option<BreakOption>("");

				sub->add_option<RegularOption>("Quit", nullptr, [] {
					exit(0);
					});

			});
	}

	void main_gui::tick()
	{
		if (m_initialized)
		{
			g_ui_mgr->onTick();
		}
		else
		{
			initialize();
			/*
						std::string path = MENU_PATH"\\vehicle\\ini\\";
						for (const auto& entry : std::filesystem::directory_iterator(path))
						{
							std::string str = entry.path().generic_u8string();
							if (str.find(".ini"))
							{
								std::size_t found = str.find_last_of(".ini");
								str = str.substr(0, found - 3);

								found = str.find_last_of("/\\");
								str = str.substr(found + 1);
								m_iniVehicleList.push_back(str);
							}
						}

						path = MENU_PATH"\\vehicle\\xml\\";
						for (const auto& entry : std::filesystem::directory_iterator(path))
						{
							std::string str = entry.path().generic_u8string();
							if (str.find(".xml"))
							{
								std::size_t found = str.find_last_of(".xml");
								str = str.substr(0, found - 3);

								found = str.find_last_of("/\\");
								str = str.substr(found + 1);
								m_xmlVehicleList.push_back(str);
							}
						}*/
		}
	}
}