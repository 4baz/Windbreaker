#pragma once
#include "../pch.h"
#include "../Types.h"
#include "text_box.h"
#include "abstract_submenu.h"
#include"../gta/vector.h"
#include"imgui.h"

namespace hbase
{
	namespace menu_press_timer
	{
		enum class Button
		{
			None, Up, Down, Left, Right, Back, Accept, num7, num1
		};
		extern menu_press_timer::Button currentButton;
		extern DWORD offsettedTime;

		void Update();
		bool IsButtonHeld(const menu_press_timer::Button& button);
		bool IsButtonTapped(const menu_press_timer::Button& button);
		bool isButtonHeldOrTapped(const menu_press_timer::Button& button);
	}

	class ui_mgr
	{
	public:
		explicit ui_mgr();
		~ui_mgr();

		template <typename SubmenuType, typename ...TArgs>
		void add_submenu(std::size_t Sub, TArgs&&... args)
		{
			auto sub = std::make_unique<SubmenuType>(std::forward<TArgs>(args)...);
			if (m_submenuStack[Sub].stack.empty())
			{
				m_submenuStack[Sub].stack.push(sub.get());
			}

			m_allSubmenus.push_back(std::move(sub));
		}

		void switchToSubmenu(std::uint32_t id)
		{
			for (auto&& sub : m_allSubmenus)
			{
				if (sub->getId() == id)
				{
					m_submenuStack[m_SelectedParentSub].stack.push(sub.get());
					return;
				}
			}
		}

		std::size_t addSub(const char* name)
		{
			std::size_t size = m_submenuStack.size();
			std::stack<AbstractSubmenu*, std::vector<AbstractSubmenu*>> tmp;
			m_submenuStack.push_back(MenuStruct{ name, tmp });
			return size;
		}

		void inputBox(std::function<void()> active)
		{
			m_isInput = true;					//开始输入
			inputString.clear();				//重置字符串
			m_inputAction = active;		//按下确认执行的函数
		}

		std::string getInputText()
		{
			return inputString;				//返回字符串
		}

		void onTick();
	public:
		bool m_opened = false;

		std::size_t m_optionsPerPage = 11;
		std::size_t  m_SelectedParentSub{};

	private:
		float m_pos_x{ 50.f };//default pos for the slighlty wider sizes of stand clone hbase
		float m_pos_y{ 50.f };

		float eth_pos_x{ m_pos_x + 100 };
		//float eth_pos_y{ 50.f };


		//Cursor
		float m_draw_cursor_y{};

		//Menu
		float m_menu_width{ 500.f };

		//Header
		float m_header_height{};
		ImColor m_header_color = ImColor(255, 255, 255, 255);

		ImColor m_submenubar_color = ImColor(0, 0, 0, 255);

		//Option
		float m_option_pos_x{ m_pos_x + 100 };
		float m_option_width{ m_menu_width - 100 };
		float m_option_height{ 30.f };

		ImColor m_description_color = ImColor(0, 0, 0, 200);

		ImColor m_optionTextColor = ImColor(255, 255, 255, 255);
		ImColor m_optionTextColorActive = ImColor(0, 0, 0, 255);

		//Footer
		ImColor m_footerColor = ImColor(0, 0, 0, 255);

		//Func
		void checkInput();
		void drawHeader();
		void drawSidebar();
		void slide(float& oldX, float newX);
		void drawSubMenuBar(AbstractSubmenu* sub);
		void drawFeatures(AbstractSubmenu* sub);
		void drawOption(AbstractOption* opt, bool selected);
		void drawFooter();
		void drawDescription();

		struct MenuStruct
		{
			std::string name;
			std::stack<AbstractSubmenu*, std::vector<AbstractSubmenu*>> stack;
		};

		//Vector
		std::vector<std::unique_ptr<AbstractSubmenu>> m_allSubmenus;
		std::vector<MenuStruct> m_submenuStack;

		//Input Box
		bool m_isInput = false;
		std::string getText();
		void inputBox();
		std::string inputString{};
		std::function<void()> m_inputAction;
	};

	inline ui_mgr* g_ui_mgr;
}
