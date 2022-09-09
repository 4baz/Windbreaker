#include "ui_manager.h"
#include "../natives.h"
#include"../settings.h"
#include "../keyboard.h"
#include"../renderer.h"
#include "imgui.h"
#include"../pointers.h"
#include"../ui_settings.h"
#include "../fiber_pool.h"
#include "icnon_font.hpp"//nice spelling retard
namespace hbase
{
	namespace menu_press_timer
	{
		menu_press_timer::Button currentButton = { menu_press_timer::Button::None };
		DWORD offsettedTime = 0;

		void Update()
		{
			if (currentButton == Button::None)
			{
				offsettedTime = GetTickCount() + 630;
			}
			if (g_keyBoard->KeyDown(VK_NUMPAD6))
			{
				currentButton = Button::Right;
			}
			else if (g_keyBoard->KeyDown(VK_NUMPAD4))
			{
				currentButton = Button::Left;
			}
			else if (g_keyBoard->KeyDown(VK_NUMPAD2))
			{
				currentButton = Button::Down;
			}
			else if (g_keyBoard->KeyDown(VK_NUMPAD8))
			{
				currentButton = Button::Up;
			}
			else if (g_keyBoard->KeyDown(VK_NUMPAD7))
			{
				currentButton = Button::num7;
			}
			else if (g_keyBoard->KeyDown(VK_NUMPAD1))
			{
				currentButton = Button::num1;
			}
			else
			{
				currentButton = Button::None;
				offsettedTime = 0;
			}
		}
		bool IsButtonHeld(const menu_press_timer::Button& button)
		{
			static DWORD time = 0;
			DWORD tickCount = GetTickCount();
			if (offsettedTime < tickCount && currentButton == button)
			{
				if (tickCount - time > 50)
				{
					time = tickCount;
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		bool is_key_pressed(std::uint16_t key)
		{
			


			if (GetForegroundWindow() == FindWindowA("grcWindow", "Grand Theft Auto V"))
			{
				if (GetAsyncKeyState(key) & 0x01)
				{
					return true;
				}
			}

			return false;
		}

		bool IsButtonTapped(const menu_press_timer::Button& button)
		{
			bool isTapped = true;
			switch (button)
			{
			case Button::Right: isTapped = g_keyBoard->KeyJustUp(VK_RIGHT); break;
			case Button::Left: isTapped = g_keyBoard->KeyJustUp(VK_LEFT); break;
			case Button::Down: isTapped = g_keyBoard->KeyJustUp(VK_DOWN); break;
			case Button::Up: isTapped = g_keyBoard->KeyJustUp(VK_UP); break;
			case Button::Back: isTapped = g_keyBoard->KeyJustUp(VK_BACK); break;
			case Button::Accept: isTapped = g_keyBoard->KeyJustUp(VK_RETURN); break;
			//case Button::num7: isTapped = is_key_pressed(0xA1); break;
			//case Button::num1: isTapped = is_key_pressed(0xA3); break;
			}//0xA1
			return isTapped;
		}
		bool isButtonHeldOrTapped(const menu_press_timer::Button& button)
		{
			return IsButtonHeld(button) || IsButtonTapped(button);
		}
	}

	void ui_mgr::onTick()
	{
		inputBox();
		menu_press_timer::Update();
		checkInput();
		if (m_opened)
		{
			g_fiber_pool->queue_job([] {
				PAD::DISABLE_CONTROL_ACTION(0, 27, true); // Disable Phone

				});

			if (m_isInput == false)
			{
				/*
				Dont do what i did here, twas too lazy too redo everything so just set there parent submenu stack to always be one specific submenu
				
				*/


				m_draw_cursor_y = m_pos_y;
				drawHeader();
				if (!m_submenuStack[m_SelectedParentSub].stack.empty())
				{
					auto sub = m_submenuStack[m_SelectedParentSub].stack.top();
					sub->Reset();
					sub->Execute();

					drawSubMenuBar(sub);
				//	drawSidebar();
					drawFeatures(sub);
					drawFooter();
					drawDescription();
				}
			}
		}
	}

	typedef struct KeyInfo {
		char* info;
		int key;
	};

	KeyInfo ki[] = {
		// ˝◊÷
		"0",0x30,
		"1",0x31,
		"2",0x32,
		"3",0x33,
		"4",0x34,
		"5",0x35,
		"6",0x36,
		"7",0x37,
		"8",0x38,
		"9",0x39,
		//–°º¸≈Ã
		"0", 0x60,
		"1", 0x61,
		"2", 0x62,
		"3", 0x63,
		"4", 0x64,
		"5", 0x65,
		"6", 0x66,
		"7", 0x67,
		"8", 0x68,
		"9", 0x69,
		//◊÷ƒ∏
		"a",0x41,
		"b",0x42,
		"c",0x43,
		"d",0x44,
		"e",0x45,
		"f",0x46,
		"g",0x47,
		"h",0x48,
		"i",0x49,
		"j",0x4a,
		"k",0x4b,
		"l",0x4c,
		"m",0x4d,
		"n",0x4e,
		"o",0x4f,
		"p",0x50,
		"q",0x51,
		"r",0x52,
		"s",0x53,
		"t",0x54,
		"u",0x55,
		"v",0x56,
		"w",0x57,
		"x",0x58,
		"y",0x59,
		"z",0x5a
	};

	//ªÒ»° ‰»Î◊÷∑˚
	std::string ui_mgr::getText()
	{
		if (g_keyBoard->KeyJustUp(VK_RETURN))
		{
			m_isInput = false;
			if (m_inputAction)
			{
				std::invoke(m_inputAction);
			}
		}
		else if (g_keyBoard->KeyJustUp(VK_BACK))
		{
			inputString.pop_back();
		}
		for (KeyInfo k : ki)
		{
			if (g_keyBoard->KeyJustUp(k.key))
			{
				return k.info;
			}
		}
		return "";
	}

	void ui_mgr::inputBox()
	{
		if (m_isInput)
		{
			//∆¥Ω”–¬ ‰»Îµƒ◊÷∑˚
			std::string c = getText();
			if (c != "")
			{
				inputString += c;
			}

			//‰÷»æ ‰»ÎøÚ
			ImDrawList* list = ImGui::GetForegroundDrawList();
			list->AddRectFilled(ImVec2(m_pos_x, m_pos_y), ImVec2(m_pos_x + m_menu_width, m_pos_y + m_option_height), m_submenubar_color);
			list->AddText(ImVec2(m_pos_x, m_pos_y), m_optionTextColor, "INPUT BOX:");
			list->AddRectFilled(ImVec2(m_pos_x, m_pos_y + m_option_height), ImVec2(m_pos_x + m_menu_width, m_pos_y + m_option_height * 2), m_submenubar_color);
			list->AddText(ImVec2(m_pos_x, m_pos_y + m_option_height), m_optionTextColor, inputString.c_str());
		}
	}

	void ui_mgr::checkInput()
	{
		if (g_keyBoard->KeyJustUp(VK_F8))
		{
			m_opened ^= true;
		}

		if (g_keyBoard->KeyJustUp(VK_END))
		{
			g_running = false;
		}

		if (m_opened && menu_press_timer::IsButtonTapped(menu_press_timer::Button::Back))
		{
			if (m_submenuStack[m_SelectedParentSub].stack.size() <= 1)
			{
				m_opened = false;
			}
			else
			{
				m_submenuStack[m_SelectedParentSub].stack.pop();
			}
		}

		if (m_opened)
		{
			if (menu_press_timer::isButtonHeldOrTapped(menu_press_timer::Button::num7))
			{
				if (m_SelectedParentSub > 0)
				{
					m_SelectedParentSub--;
				}
				else
				{
					m_SelectedParentSub = m_submenuStack.size() - 1;
				}
			}

			if (menu_press_timer::isButtonHeldOrTapped(menu_press_timer::Button::num1))
			{
				if (m_SelectedParentSub < m_submenuStack.size() - 1)
				{
					m_SelectedParentSub++;
				}
				else
				{
					m_SelectedParentSub = 0;
				}
			}

			if (!m_submenuStack[m_SelectedParentSub].stack.empty())
			{
				auto sub = m_submenuStack[m_SelectedParentSub].stack.top();
				if (menu_press_timer::IsButtonTapped(menu_press_timer::Button::Accept) && sub->GetNumOptions() != 0)
				{
					if (auto opt = sub->GetOption(sub->GetSelectedOption()))
					{
						opt->HandleAction(OptionAction::EnterPress);
					}
				}

				if (menu_press_timer::isButtonHeldOrTapped(menu_press_timer::Button::Up) && sub->GetNumOptions() != 0)
				{
					sub->ScrollBackward();
					if (auto opt = sub->GetOption(sub->GetSelectedOption()))
					{
						if (opt->GetFlag(OptionFlag::Break))
						{
							sub->ScrollBackward();
						}
					}
				}

				if (menu_press_timer::isButtonHeldOrTapped(menu_press_timer::Button::Down) && sub->GetNumOptions() != 0)
				{
					sub->ScrollForward();
					if (auto opt = sub->GetOption(sub->GetSelectedOption()))
					{
						if (opt->GetFlag(OptionFlag::Break))
						{
							sub->ScrollForward();
						}
					}
				}

				if (menu_press_timer::isButtonHeldOrTapped(menu_press_timer::Button::Left) && sub->GetNumOptions() != 0)
				{
					if (auto opt = sub->GetOption(sub->GetSelectedOption()))
					{
						opt->HandleAction(OptionAction::LeftPress);
					}
				}

				if (menu_press_timer::isButtonHeldOrTapped(menu_press_timer::Button::Right) && sub->GetNumOptions() != 0)
				{
					if (auto opt = sub->GetOption(sub->GetSelectedOption()))
					{
						opt->HandleAction(OptionAction::RightPress);
					}
				}
			}
		}
	}

	void ui_mgr::drawHeader()
	{
		ImDrawList* list = ImGui::GetForegroundDrawList();
		if (g_ui_settings.options["header"]["count"].get<int>() > 1)
		{
			static int idleFrame = 0;
			static bool timer_reset = true;
			static std::chrono::system_clock::time_point timer_Start;

			if (timer_reset)
			{
				timer_Start = std::chrono::system_clock::now();
				timer_reset = false;
			}

			std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - timer_Start;
			if (elapsed_seconds.count() >= 0.06)
			{
				idleFrame++;
				if (idleFrame == g_ui_settings.options["header"]["count"].get<int>())
					idleFrame = 0;
				timer_reset = true;
			}
			list->AddImage((ImTextureID)g_renderer->m_header_gif_texture.at(idleFrame), ImVec2(eth_pos_x, m_pos_y), ImVec2(m_pos_x + m_menu_width, m_pos_y + m_header_height), ImVec2(0, 0), ImVec2(1, 1), m_header_color);
		}
		else
		{
			list->AddImage((ImTextureID)g_renderer->m_header_texture, ImVec2(eth_pos_x, m_pos_y), ImVec2(m_pos_x + m_menu_width, m_pos_y + m_header_height), ImVec2(0, 0), ImVec2(1, 1), m_header_color);
		}
		m_draw_cursor_y += m_header_height;
	}

	void ui_mgr::drawSubMenuBar(AbstractSubmenu* sub)
	{
		ImDrawList* list = ImGui::GetForegroundDrawList();

		list->AddRectFilled(ImVec2(eth_pos_x, m_draw_cursor_y), ImVec2(m_pos_x + m_menu_width, m_draw_cursor_y + m_option_height), m_submenubar_color);
		char ligfttText[64] = {};
		sprintf_s(ligfttText, "%s", sub->GetName());

		list->AddText(ImVec2(eth_pos_x, m_draw_cursor_y), m_optionTextColor, ligfttText);

		char rightText[32] = {};
		std::snprintf(rightText, sizeof(rightText) - 1, "%zu / %zu", sub->GetSelectedOption() + 1, sub->GetNumOptions());

		float optionPosX = m_pos_x + m_menu_width - ImGui::CalcTextSize(rightText).x;
		list->AddText(ImVec2(optionPosX, m_draw_cursor_y + 7.5), m_optionTextColor, rightText);

		m_draw_cursor_y += m_option_height;
	}

	void ui_mgr::slide(float& oldX, float newX)
	{
		static float step{};
		if (oldX != newX)
		{
			float v = oldX - newX;
			if (v < 0)
			{
				v = -v;
			}
			if (v > 1.5 * m_option_height)
			{
				step = 8.0f;
			}
			else
			{
				step = 4.0f;
			}
			if (oldX > newX)
			{
				if (oldX - step < newX)
				{
					oldX = newX;
				}
				else
				{
					oldX -= step;
				}
			}
			else
			{
				if (oldX + step > newX)
				{
					oldX = newX;
				}
				else
				{
					oldX += step;
				}
			}
		}
	}

	void ui_mgr::drawSidebar()
	{
		ImDrawList* list = ImGui::GetForegroundDrawList();

		static float y{};
		float tmp{ m_draw_cursor_y };

		//‰÷»æµ◊≤„æÿ–Œ
		float boxHeight = m_submenuStack.size() * m_option_height;
		if (g_ui_settings.options["sidebar"]["is_texture_mode"].get<bool>())
		{
			if (g_ui_settings.options["sidebar"]["count"].get<int>() > 1)
			{
				static int idleFrame = 0;
				static bool timer_reset = true;
				static std::chrono::system_clock::time_point timer_Start;

				if (timer_reset)
				{
					timer_Start = std::chrono::system_clock::now();
					timer_reset = false;
				}

				std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - timer_Start;
				if (elapsed_seconds.count() >= 0.06)
				{
					idleFrame++;
					if (idleFrame == g_ui_settings.options["sidebar"]["count"].get<int>())
						idleFrame = 0;
					timer_reset = true;
				}
				list->AddImage((ImTextureID)g_renderer->m_sidebar_gif_texture.at(idleFrame), ImVec2(m_pos_x, m_draw_cursor_y), ImVec2(m_pos_x + 98, m_draw_cursor_y + boxHeight), ImVec2(0, 0), ImVec2(1, 1), m_header_color);
			}
			else
			{
				list->AddImage((ImTextureID)g_renderer->m_sidebar_texture, ImVec2(m_pos_x, m_draw_cursor_y), ImVec2(m_pos_x + 98, m_draw_cursor_y + boxHeight), ImVec2(0, 0), ImVec2(1, 1), m_header_color);
			}
		}
		else
		{
			ImColor tmp{ g_ui_settings.options["sidebar"]["sidebar_rbga"][0].get<int>(),g_ui_settings.options["sidebar"]["sidebar_rbga"][1].get<int>(),g_ui_settings.options["sidebar"]["sidebar_rbga"][2].get<int>(),g_ui_settings.options["sidebar"]["sidebar_rbga"][3].get<int>() };
			list->AddRectFilled(ImVec2(m_pos_x, m_draw_cursor_y), ImVec2(m_pos_x + 98, m_draw_cursor_y + boxHeight), tmp);
		}

		//‰÷»æª¨øÈ
		if (g_ui_settings.options["sidebar"]["slider"]["is_texture_mode"].get<bool>())
		{
			if (g_ui_settings.options["sidebar"]["slider"]["count"].get<int>() > 1)
			{
				static int idleFrame = 0;
				static bool timer_reset = true;
				static std::chrono::system_clock::time_point timer_Start;

				if (timer_reset)
				{
					timer_Start = std::chrono::system_clock::now();
					timer_reset = false;
				}

				std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - timer_Start;
				if (elapsed_seconds.count() >= 0.06)
				{
					idleFrame++;
					if (idleFrame == g_ui_settings.options["sidebar"]["slider"]["count"].get<int>())
						idleFrame = 0;
					timer_reset = true;
				}
				list->AddImage((ImTextureID)g_renderer->m_sidebar_sidebar_gif_texture.at(idleFrame), ImVec2(m_pos_x, y), ImVec2(m_pos_x + 98, y + m_option_height), ImVec2(0, 0), ImVec2(1, 1), m_header_color);
			}
			else
			{
				list->AddImage((ImTextureID)g_renderer->m_sidebar_slider_texture, ImVec2(m_pos_x, y), ImVec2(m_pos_x + 98, y + m_option_height), ImVec2(0, 0), ImVec2(1, 1), m_header_color);
			}
		}
		else
		{
			ImColor tmp{ g_ui_settings.options["sidebar"]["slider"]["slider_rbga"][0].get<int>(),g_ui_settings.options["sidebar"]["slider"]["slider_rbga"][1].get<int>(),g_ui_settings.options["sidebar"]["slider"]["slider_rbga"][2].get<int>(),g_ui_settings.options["sidebar"]["slider"]["slider_rbga"][3].get<int>() };
			list->AddRectFilled(ImVec2(m_pos_x, y), ImVec2(m_pos_x + 98, y + m_option_height), tmp);
		}

		//‰÷»æ…œ≤„
		for (size_t i = 0; i < m_submenuStack.size(); i++)
		{
			//∏¸–¬ª¨øÈƒø±ÍŒª÷√
			bool selected = m_SelectedParentSub == i;
			if (selected)
			{
				static unsigned long long tick{};
				if (GetTickCount64() - tick > 2)
				{
					slide(y, tmp);
					tick = GetTickCount64();
				}
			}
			ImColor text_rgba{ g_ui_settings.options["sidebar"]["text"]["text_rgba"][0].get<int>(),g_ui_settings.options["sidebar"]["text"]["text_rgba"][1].get<int>(),g_ui_settings.options["sidebar"]["text"]["text_rgba"][2].get<int>(),g_ui_settings.options["sidebar"]["text"]["text_rgba"][3].get<int>() };
			ImColor text_rgba_active{ g_ui_settings.options["sidebar"]["text"]["text_rgba_active"][0].get<int>(),g_ui_settings.options["sidebar"]["text"]["text_rgba_active"][1].get<int>(),g_ui_settings.options["sidebar"]["text"]["text_rgba_active"][2].get<int>(),g_ui_settings.options["sidebar"]["text"]["text_rgba_active"][3].get<int>() };

			list->AddText(ImVec2(m_pos_x, tmp), selected ? text_rgba_active : text_rgba, m_submenuStack[i].name.c_str());
			tmp += m_option_height;
		}
	}

	static float optionY{};

	void ui_mgr::drawFeatures(AbstractSubmenu* sub)
	{
		if (sub->GetNumOptions() != 0)
		{
			std::size_t startPoint = 0;
			std::size_t endPoint = sub->GetNumOptions() > m_optionsPerPage ? m_optionsPerPage : sub->GetNumOptions();
			if (sub->GetNumOptions() > m_optionsPerPage && sub->GetSelectedOption() >= m_optionsPerPage)
			{
				startPoint = sub->GetSelectedOption() - m_optionsPerPage + 1;
				endPoint = sub->GetSelectedOption() + 1;
			}

			ImDrawList* list = ImGui::GetForegroundDrawList();

			//‰÷»æµ◊≤„æÿ–Œ
			float boxHeight = (sub->GetNumOptions() > m_optionsPerPage ? m_optionsPerPage : sub->GetNumOptions()) * m_option_height;
			if (g_ui_settings.options["option"]["is_texture_mode"].get<bool>())
			{
				if (g_ui_settings.options["option"]["count"].get<int>() > 1)
				{
					static int idleFrame = 0;
					static bool timer_reset = true;
					static std::chrono::system_clock::time_point timer_Start;

					if (timer_reset)
					{
						timer_Start = std::chrono::system_clock::now();
						timer_reset = false;
					}

					std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - timer_Start;
					if (elapsed_seconds.count() >= 0.06)
					{
						idleFrame++;
						if (idleFrame == g_ui_settings.options["option"]["count"].get<int>())
							idleFrame = 0;
						timer_reset = true;
					}
					list->AddImage((ImTextureID)g_renderer->m_option_gif_texture.at(idleFrame), ImVec2(m_option_pos_x, m_draw_cursor_y), ImVec2(m_option_pos_x + m_option_width, m_draw_cursor_y + boxHeight), ImVec2(0, 0), ImVec2(1, 1), m_header_color);
				}
				else
				{
					list->AddImage((ImTextureID)g_renderer->m_option_texture, ImVec2(m_option_pos_x, m_draw_cursor_y), ImVec2(m_option_pos_x + m_option_width, m_draw_cursor_y + boxHeight), ImVec2(0, 0), ImVec2(1, 1), m_header_color);
				}
			}
			else
			{
				ImColor tmp{ g_ui_settings.options["option"]["option_rbga"][0].get<int>(),g_ui_settings.options["option"]["option_rbga"][1].get<int>(),g_ui_settings.options["option"]["option_rbga"][2].get<int>(),g_ui_settings.options["option"]["option_rbga"][3].get<int>() };
				list->AddRectFilled(ImVec2(m_option_pos_x, m_draw_cursor_y), ImVec2(m_option_pos_x + m_option_width, m_draw_cursor_y + boxHeight), tmp);
			}

			//‰÷»æª¨øÈ
			if (g_ui_settings.options["option"]["slider"]["is_texture_mode"].get<bool>())
			{
				if (g_ui_settings.options["option"]["slider"]["count"].get<int>() > 1)
				{
					static int idleFrame = 0;
					static bool timer_reset = true;
					static std::chrono::system_clock::time_point timer_Start;

					if (timer_reset)
					{
						timer_Start = std::chrono::system_clock::now();
						timer_reset = false;
					}

					std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - timer_Start;
					if (elapsed_seconds.count() >= 0.06)
					{
						idleFrame++;
						if (idleFrame == g_ui_settings.options["option"]["slider"]["count"].get<int>())
							idleFrame = 0;
						timer_reset = true;
					}
					list->AddImage((ImTextureID)g_renderer->m_option_slider_gif_texture.at(idleFrame), ImVec2(m_option_pos_x, optionY), ImVec2(m_option_pos_x + m_option_width, optionY + m_option_height), ImVec2(0, 0), ImVec2(1, 1), m_header_color);
				}
				else
				{
					list->AddImage((ImTextureID)g_renderer->m_option_slider_texture, ImVec2(m_option_pos_x, optionY), ImVec2(m_option_pos_x + m_option_width, optionY + m_option_height), ImVec2(0, 0), ImVec2(1, 1), m_header_color);
				}
			}
			else
			{
				ImColor tmp{ g_ui_settings.options["option"]["slider"]["slider_rbga"][0].get<int>(),g_ui_settings.options["option"]["slider"]["slider_rbga"][1].get<int>(),g_ui_settings.options["option"]["slider"]["slider_rbga"][2].get<int>(),g_ui_settings.options["option"]["slider"]["slider_rbga"][3].get<int>() };
				list->AddRectFilled(ImVec2(m_option_pos_x, optionY), ImVec2(m_option_pos_x + m_option_width, optionY + m_option_height), tmp);
			}

			//‰÷»æ…œ≤„
			for (std::size_t i = startPoint, j = 0; i < endPoint; ++i, ++j)
			{
				drawOption(sub->GetOption(i), i == sub->GetSelectedOption());
			}
			//draw footer here


		}
	}
	const char* bitchesandtitties;
	void ui_mgr::drawOption(AbstractOption* opt, bool selected)
	{
		//∏¸–¬ª¨øÈƒø±ÍŒª÷√
		if (selected)
		{
			static unsigned long long tick{};
			if (GetTickCount64() - tick > 2)
			{
				slide(optionY, m_draw_cursor_y);
				tick = GetTickCount64();
			}
		}

		ImDrawList* list = ImGui::GetForegroundDrawList();
		float optionPosX{ m_option_pos_x };
		char text[64]{};
		ImColor text_rgba{ g_ui_settings.options["sidebar"]["text"]["text_rgba"][0].get<int>(),g_ui_settings.options["sidebar"]["text"]["text_rgba"][1].get<int>(),g_ui_settings.options["sidebar"]["text"]["text_rgba"][2].get<int>(),g_ui_settings.options["sidebar"]["text"]["text_rgba"][3].get<int>() };
		ImColor text_rgba_active{ g_ui_settings.options["sidebar"]["text"]["text_rgba_active"][0].get<int>(),g_ui_settings.options["sidebar"]["text"]["text_rgba_active"][1].get<int>(),g_ui_settings.options["sidebar"]["text"]["text_rgba_active"][2].get<int>(),g_ui_settings.options["sidebar"]["text"]["text_rgba_active"][3].get<int>() };
		if (opt->GetFlag(OptionFlag::Break))
		{
			sprintf_s(text, "[ %s ]", opt->GetLeftText());
			optionPosX = m_option_pos_x + m_option_width / 4 * 2 - ImGui::CalcTextSize(text).x / 2;
		}
		else
		{
			sprintf_s(text, "%s", opt->GetLeftText());
		}

		list->AddText(ImVec2(optionPosX, m_draw_cursor_y + 7.5), selected ? text_rgba_active : text_rgba, text);

		//‰÷»æ”“±ﬂŒƒ◊÷
		const char* rightText = opt->GetRightText();
		optionPosX = m_option_pos_x + m_option_width - ImGui::CalcTextSize(rightText).x;

		if (opt->GetFlag(OptionFlag::BoolON))
		{
			//list->AddRectFilled(ImVec2(m_option_pos_x + m_option_width - 20.f - 5.f, m_draw_cursor_y + 5.f), ImVec2(m_option_pos_x + m_option_width - 5.f, m_draw_cursor_y + 25.f), selected ? text_rgba_active : text_rgba);
			list->AddText(g_renderer->m_IconFont, 15.f, ImVec2(m_option_pos_x + m_option_width - 25.f, m_draw_cursor_y + 7.f), IM_COL32(255, 255, 255, 255), "g");

		}
		else if (opt->GetFlag(OptionFlag::BoolOFF))
		{
			list->AddText(g_renderer->m_IconFont, 15.f, ImVec2(m_option_pos_x + m_option_width - 25.f, m_draw_cursor_y + 7.f), IM_COL32(255, 255, 255, 255), "b");

		//	list->AddRect(ImVec2(m_option_pos_x + m_option_width - 20.f - 5.f, m_draw_cursor_y + 5.f), ImVec2(m_option_pos_x + m_option_width - 5.f, m_draw_cursor_y + 25.f), selected ? text_rgba_active : text_rgba, 0, 0, 2);
		}
		else if (opt->GetFlag(OptionFlag::Enterable))
		{
		//	list->AddRectFilled(ImVec2(m_option_pos_x + m_option_width - 10.f, m_draw_cursor_y), ImVec2(m_option_pos_x + m_option_width, m_draw_cursor_y + m_option_height), ImColor{ 255,0,0,180 });
			list->AddText(g_renderer->m_IconFont, 15.f, ImVec2(m_option_pos_x  + m_option_width - 25.f, m_draw_cursor_y + 7.5), IM_COL32(255, 255, 255, 255), "r");

		}
		else
		{
			list->AddText(ImVec2(optionPosX, m_draw_cursor_y), selected ? text_rgba_active : text_rgba, rightText);
		}

		if (selected)
		{
			const char* description = opt->GetDescription();
			if (description && *description)
			{
				bitchesandtitties = opt->GetDescription();
			//	list->AddRectFilled(ImVec2(m_option_pos_x + m_option_width + 5, optionY), ImVec2(m_option_pos_x + m_option_width + 5 + ImGui::CalcTextSize(description).x, optionY + m_option_height), m_description_color);
			//	list->AddText(ImVec2(m_pos_x + m_menu_width + 5, optionY), m_optionTextColor, description);
				// ^^^ og hbase shit

			
			}
		}
		m_draw_cursor_y += m_option_height;
	}

	void ui_mgr::drawFooter()
	{
		ImDrawList* list = ImGui::GetForegroundDrawList();

		list->AddRectFilled(ImVec2(eth_pos_x, m_draw_cursor_y), ImVec2(m_pos_x + m_menu_width, m_draw_cursor_y + m_option_height), m_footerColor);
		float arrowOffset = (m_menu_width -100) / 2.f - 10.f;
		float centerArrowOffset = (m_draw_cursor_y + m_option_height / 2) - 10.f;
		list->AddText(g_renderer->m_IconFont, 15.f, ImVec2(eth_pos_x + arrowOffset, (m_draw_cursor_y)  + 3.f  ), IM_COL32(255, 255, 255, 255), "u");
		list->AddText(g_renderer->m_IconFont, 15.f, ImVec2(eth_pos_x + arrowOffset, (m_draw_cursor_y) + 12.5f ), IM_COL32(255, 255, 255, 255), "d");

	//	list->AddText(g_renderer->m_IconFont, 15.f, ImVec2(eth_pos_x + (arrowOffset *2), (m_draw_cursor_y)+12.5f), IM_COL32(255, 255, 255, 255), "abcdefghijk ");


		m_draw_cursor_y += m_option_height;
	}

	void ui_mgr::drawDescription()
	{
		float p = 2.f;
		const char* description{};

		if (!m_submenuStack[m_SelectedParentSub].stack.empty())
		{
			auto sub = m_submenuStack[m_SelectedParentSub].stack.top();
			if (sub->GetNumOptions())
			{
				if (auto opt = sub->GetOption(sub->GetSelectedOption()))
				{
					description = opt->GetDescription();
				}
			}
		}

		if (!description || !*description)
			return;

		m_draw_cursor_y += p;
		ImDrawList* list = ImGui::GetForegroundDrawList();
		list->AddRectFilled(ImVec2(eth_pos_x, m_draw_cursor_y + p), ImVec2(m_pos_x + m_menu_width, m_draw_cursor_y + m_option_height + p), m_description_color);

		list->AddText(ImVec2(eth_pos_x +5, m_draw_cursor_y + p), m_optionTextColor, description);
		m_draw_cursor_y += m_option_height;
	}

	ui_mgr::ui_mgr()
	{
		m_header_height = g_ui_settings.options["header"]["header_height"].get<float>();
		g_ui_mgr = this;
	}

	ui_mgr::~ui_mgr()
	{
		g_ui_mgr = nullptr;
	}
}