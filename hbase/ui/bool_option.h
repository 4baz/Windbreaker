#pragma once
#include"base_option.h"
#include "ui_manager.h"

namespace hbase
{
	template <typename BoolType = bool>
	class BoolOption : public BaseOption<BoolOption<BoolType>>
	{
	public:
		explicit BoolOption(const char* text, const char* description, BoolType* b00l, bool displayInverted = false, std::function<void()> action = [] {}) :
			m_Bool(b00l),
			m_DisplayInverted(displayInverted)
		{
			Base::SetLeftText(text);
			if (description)
				Base::SetDescription(description);
			Base::SetAction(std::move(action));
		}

		~BoolOption() noexcept = default;
		BoolOption(BoolOption const&) = default;
		BoolOption& operator=(BoolOption const&) = default;
		BoolOption(BoolOption&&) = default;
		BoolOption& operator=(BoolOption&&) = default;

		void HandleAction(OptionAction action) override
		{
			if (action == OptionAction::EnterPress)
			{
				*m_Bool = !*m_Bool;
			}

			Base::HandleAction(action);
		}

		bool GetFlag(OptionFlag flag) override
		{
			if (flag == OptionFlag::BoolON && *m_Bool ^ m_DisplayInverted)
			{
				return true;
			}
			else if (flag == OptionFlag::BoolOFF && !(*m_Bool ^ m_DisplayInverted))
			{
				return true;
			}

			return BaseOption::GetFlag(flag);
		}
	private:
		BoolType* m_Bool;
		BoolType m_DisplayInverted = false;

		using Base = BaseOption<BoolOption<BoolType>>;
	};
}
