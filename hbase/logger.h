#pragma once
#include "pch.h"
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>

namespace hbase
{
	template <typename TP>
	std::time_t to_time_t(TP tp)
	{
		using namespace std::chrono;
		auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
			+ system_clock::now());
		return system_clock::to_time_t(sctp);
	}

	enum class log_color : std::uint16_t
	{
		red = FOREGROUND_RED,
		green = FOREGROUND_GREEN,
		blue = FOREGROUND_BLUE,
		intensify = FOREGROUND_INTENSITY
	};

	static const int kEventValue = 400;
	static const int kRawValue = 600;
	inline constexpr auto max_padding_length = 26;
	inline constexpr auto level_padding_length = 8;

	const LEVELS INFO_TO_FILE{ INFO.value , {"INFO"} },
		HACKER{ INFO.value, {"HACKER"} },
		RAW_GREEN_TO_CONSOLE{ kRawValue , {"RAW_GREEN_TO_CONSOLE"} },
		RAW_RED{ kRawValue, {"RAW_RED"} };

	inline log_color operator|(log_color a, log_color b)
	{
		return static_cast<log_color>(static_cast<std::underlying_type_t<log_color>>(a) | static_cast<std::underlying_type_t<log_color>>(b));
	}

	class logger;
	inline logger* g_logger{};

	class logger
	{
	public:
		explicit logger() :
			m_file_path(MENU_PATH"\\"),
			m_worker(g3::LogWorker::createLogWorker())
		{
			if ((m_did_console_exist = AttachConsole(GetCurrentProcessId())) == false)
				AllocConsole();

			if ((m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE)) != nullptr)
			{
				SetConsoleTitleA(MENU_NAME);
				SetConsoleOutputCP(CP_UTF8);

				m_console_out.open("CONOUT$", std::ios_base::out | std::ios_base::app);
			}
			try
			{
				m_file_path /= MENU_NAME".log";
				m_file_out.open(m_file_path, std::ios_base::out | std::ios_base::trunc);
				m_worker->addSink(std::make_unique<log_sink>(), &log_sink::callback);
				g3::initializeLogging(m_worker.get());
			}
			catch (std::filesystem::filesystem_error const& error)
			{
				m_console_out << error.what();
			}

			g_logger = this;
		}

		~logger()
		{
			m_worker.reset();
			if (!m_did_console_exist)
				FreeConsole();

			g_logger = nullptr;
		}

		struct log_sink
		{
			std::map<std::string, log_color> log_colors = {
				{INFO.text, log_color::blue | log_color::intensify},
				{WARNING.text, log_color::red},
				{HACKER.text, log_color::green | log_color::intensify},
				{FATAL.text, log_color::red | log_color::intensify},
				{G3LOG_DEBUG.text, log_color::blue},
				{RAW_RED.text, log_color::red},
				{RAW_GREEN_TO_CONSOLE.text, log_color::green | log_color::intensify}
			};

			void callback(g3::LogMessageMover log)
			{
				g3::LogMessage log_message = log.get();
				int level_value = log_message._level.value;

				bool is_raw = level_value == RAW_RED.value || level_value == RAW_GREEN_TO_CONSOLE.value;

				SetConsoleTextAttribute(g_logger->m_console_handle, static_cast<std::uint16_t>(log_colors[log_message._level.text]));
				std::string str = log_message.toString(is_raw ? format_raw : format_console);
				g_logger->m_console_out << str << std::flush;
				g_logger->m_file_out << str << std::flush;
			}

			static std::string format_file(const g3::LogMessage& msg)
			{
				std::string file_name_with_line = "[" + msg.file() + ":" + msg.line() + "]";
				std::stringstream out;
				out << "[" << msg.timestamp("%H:%M:%S") << "] [" << std::left << std::setw(level_padding_length) << msg.level().append("]") << std::setw(max_padding_length) << file_name_with_line;
				return out.str();
			}

			static std::string format_console(const g3::LogMessage& msg)
			{
				std::stringstream out;
				out << "[" << msg.timestamp("%H:%M:%S") << "] ";
				return out.str();
			}

			static std::string format_raw(const g3::LogMessage& msg)
			{
				return "";
			}
		};

		template <typename ...Args>
		inline void log_now(std::string_view format, Args const& ...args)
		{
			auto message = fmt::format(format, args...);
			if (m_file_out)
				m_file_out << message << std::endl << std::flush;
		}

	private:
		bool m_did_console_exist{};
		HANDLE m_console_handle{};
		std::ofstream m_console_out;
		std::filesystem::path m_file_path;
		std::ofstream m_file_out;
		std::unique_ptr<g3::LogWorker> m_worker;
	};

#define LOG_NOW(format, ...) g_logger->log_now(format, __VA_ARGS__)
#define HEX_TO_UPPER(value) "0x" << std::hex << std::uppercase << (DWORD64)value << std::dec << std::nouppercase
}