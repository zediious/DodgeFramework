#include "Events.h"
#include "Hooks.h"
#include "Papyrus.h"
#include "Settings.h"

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		Events::SinkEventHandlers();
		Settings::ReadSettings();
		break;
	}
}

namespace
{
	void InitializeLog()
	{
#ifndef NDEBUG
		auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
		auto path = logger::log_directory();
		if (!path) {
			SKSE::stl::report_and_fail("Failed to find standard logging directory"sv);
		}

		*path /= fmt::format("{}.log"sv, Plugin::NAME);
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

#ifndef NDEBUG
		const auto level = spdlog::level::trace;
#else
		const auto level = spdlog::level::info;
#endif

		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
		log->set_level(level);
		log->flush_on(level);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);
	}
}


extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Plugin::NAME.data();
	a_info->version = Plugin::VERSION[0];

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_SSE_1_5_39) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
		return false;
	}

	return true;
}

// Function for AE loading
extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
	SKSE::PluginVersionData v;

	v.PluginVersion(Plugin::VERSION);
	v.PluginName(Plugin::NAME);
	v.AuthorName("Ersh");
	v.UsesAddressLibrary(true);
	v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });
	v.HasNoStructUse(true);

	return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{	
	InitializeLog();
	logger::info("Dodge Framework loaded");

	SKSE::Init(a_skse);

	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		return false;
	}

	Hooks::Install();
	Papyrus::Register();

	return true;
}
