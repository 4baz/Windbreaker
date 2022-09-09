#pragma once

namespace hbase
{
	enum class OptionAction
	{
		LeftPress,
		RightPress,
		EnterPress
	};

	enum class OptionFlag
	{
		Horizontal,
		Enterable,
		Break,
		BoolON,
		BoolOFF
	};

	class AbstractOption
	{
	public:
		virtual ~AbstractOption() noexcept = default;

		virtual const char* GetLeftText() = 0;
		virtual const char* GetRightText() = 0;
		virtual const char* GetDescription() = 0;
		virtual void HandleAction(OptionAction action) = 0;
		virtual bool GetFlag(OptionFlag flag) = 0;
	protected:
		explicit AbstractOption() = default;
		AbstractOption(AbstractOption const&) = default;
		AbstractOption& operator=(AbstractOption const&) = default;
		AbstractOption(AbstractOption&&) = default;
		AbstractOption& operator=(AbstractOption&&) = default;
	};
}
