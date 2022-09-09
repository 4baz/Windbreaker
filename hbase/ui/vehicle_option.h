#pragma once
#include "base_option.h"
#include"data/vehicles.h"
#include "ui/util.h"

namespace hbase
{
	class VehicleOption : public BaseOption<VehicleOption>
	{
	public:
		explicit VehicleOption() = default;

		explicit VehicleOption(const char* text, const char* description, const std::vector<VehiclePack>& v, int* position, bool actionOnHorizontal = false, std::function<void()> action = [] {}) :
			m_Data(v),
			m_DataSize(v.size()),
			m_Position(position),
			m_ActionOnHorizontal(actionOnHorizontal)
		{
			Base::SetLeftText(text);
			if (description)
				Base::SetDescription(description);
			Base::SetAction(std::move(action));
		}

		const char* GetRightText() override
		{
			MemoryStringStream stream(Base::m_RightText);

			if (!m_Data.empty())
			{
				stream << m_Data[*m_Position].labelText;
				stream << "  [" << *m_Position + 1 << " / " << m_DataSize << "]";
			}
			else
			{
				stream << "Unknown";
			}

			return Base::GetRightText();
		}

		void HandleAction(OptionAction action) override
		{
			if (action == OptionAction::LeftPress)
			{
				if (!m_Data.empty())
				{
					if (*m_Position > 0)
						--(*m_Position);
					else
						*m_Position = static_cast<int>(m_DataSize - 1);

					if (m_ActionOnHorizontal && Base::m_Action)
						std::invoke(Base::m_Action);
				}
			}
			else if (action == OptionAction::RightPress)
			{
				if (!m_Data.empty())
				{
					if (*m_Position < m_DataSize - 1)
						++(*m_Position);
					else
						*m_Position = 0;

					if (m_ActionOnHorizontal && Base::m_Action)
						std::invoke(Base::m_Action);
				}
			}

			if (!m_Data.empty())
				Base::HandleAction(action);
		}

		~VehicleOption() noexcept = default;
		VehicleOption(VehicleOption const&) = default;
		VehicleOption& operator=(VehicleOption const&) = default;
		VehicleOption(VehicleOption&&) = default;
		VehicleOption& operator=(VehicleOption&&) = default;
	private:
		const std::vector<VehiclePack>& m_Data{};
		std::size_t m_DataSize{};
		int* m_Position{};
		bool m_ActionOnHorizontal{};

		using Base = BaseOption<VehicleOption>;
	};
}
