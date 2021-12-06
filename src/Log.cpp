// Copyright 2021 <github.com/razaqq>

#include "Log.hpp"

#pragma warning(push, 0)
#include <spdlog/pattern_formatter.h>
// #include <spdlog/pattern_formatter-inl.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)

#include <string>
#include <memory>
#include <vector>


using Raytracer::Log;

std::shared_ptr<spdlog::logger> Log::s_logger;

template<typename ScopedPadder>
class SourceLocationFlag : public spdlog::custom_flag_formatter
{
public:
	void format(const spdlog::details::log_msg& msg, const std::tm&, spdlog::memory_buf_t& dest) override
	{
		if (msg.source.empty())
		{
			return;
		}

		std::string fileName;
		std::string_view before = "..\\";
		std::string_view old(msg.source.filename);
		size_t i = 0, j = 0;

		while ((i = old.find(before, j) != std::string_view::npos))
		{
			fileName.append(msg.source.filename, j, i - j);
			j = i + before.size();
		}
		fileName.append(old.substr(j));

		size_t text_size;
		if (padinfo_.enabled())
		{
			text_size = fileName.size() + ScopedPadder::count_digits(msg.source.line) + 1;
		}
		else
		{
			text_size = 0;
		}

		ScopedPadder p(text_size, padinfo_, dest);

		spdlog::details::fmt_helper::append_string_view(fileName, dest);
		dest.push_back(':');
		spdlog::details::fmt_helper::append_int(msg.source.line, dest);
	}

	[[nodiscard]] std::unique_ptr<custom_flag_formatter> clone() const override
	{
		return spdlog::details::make_unique<SourceLocationFlag>();
	}
};

void Log::Init()
{
	std::string filePath = "PotatoAlert.log";

	spdlog::set_error_handler([](const std::string& msg)
							  { spdlog::get("console")->error("*** LOGGER ERROR ***: {}", msg); });

	auto stdoutSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	stdoutSink->set_pattern("%^[%T] %n: %v%$");

#ifndef NDEBUG
	stdoutSink->set_level(spdlog::level::trace);
#else
	stdoutSink->set_level(spdlog::level::info);
#endif

	auto formatter = std::make_unique<spdlog::pattern_formatter>();
	// formatter->add_flag<SourceLocationFlag<spdlog::details::scoped_padder>>('S');
	formatter->set_pattern("[%d-%m-%Y %T] [%=7l] %n [thread %-5t] (%-30!S): %v");

	auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filePath);
	fileSink->set_formatter(std::move(formatter));
	fileSink->set_level(spdlog::level::info);

	std::vector<spdlog::sink_ptr> sinks{ stdoutSink, fileSink };
	s_logger = std::make_shared<spdlog::logger>("Raytracer", sinks.begin(), sinks.end());

	spdlog::register_logger(s_logger);
	s_logger->set_level(spdlog::level::trace);
	s_logger->flush_on(spdlog::level::trace);
}
