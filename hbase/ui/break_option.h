#pragma once
#include "base_option.h"
#include"ui_manager.h"

namespace hbase
{
	class BreakOption : public BaseOption<BreakOption>
	{
	public:
		explicit BreakOption() = default;
		explicit BreakOption(const char* text)
		{
			SetLeftText(text);
		}

		~BreakOption() noexcept = default;
		BreakOption(BreakOption const&) = default;
		BreakOption& operator=(BreakOption const&) = default;
		BreakOption(BreakOption&&) = default;
		BreakOption& operator=(BreakOption&&) = default;

		bool GetFlag(OptionFlag flag) override
		{
			if (flag == OptionFlag::Break)
			{
				return true;
			}

			return BaseOption::GetFlag(flag);
		}
	};
}
