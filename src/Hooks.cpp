#include "Hooks.h"

#include "Events.h"
#include "Settings.h"

namespace Hooks
{
	void Install()
	{
		logger::info("Hooking...");

		SprintHandlerHook::Hook();

		logger::info("...success");
	}

	static bool bStoppingSprint = false;
	//static float heldDownTimeOffset = 0.f;

	void SprintHandlerHook::ProcessButton(RE::SprintHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data)
	{
		using PlayerFlags = RE::PlayerCharacter::PlayerFlags;

		logger::debug("Sprint handler hook called");

		if (Settings::bUseSprintButton) {
			auto playerCharacter = RE::PlayerCharacter::GetSingleton();
			auto userEvent = a_event->QUserEvent();
			auto userEvents = RE::UserEvents::GetSingleton();

			logger::debug("Is using sprint button");
			if (userEvent == userEvents->sprint) {
				logger::debug("user event is sprint");
				if (a_event->IsDown() && playerCharacter->GetPlayerRuntimeData().playerFlags.isSprinting == 1) { // stopping sprint
					logger::debug("stopping sprint");
					bStoppingSprint = true;
				} else if (a_event->HeldDuration() < Settings::fSprintHoldDuration) {
					logger::debug("held duration less than setting for hold duration");
					if (a_event->IsUp())
					{	
						logger::debug("event is up, dodging");
						Events::Dodge();
						bStoppingSprint = false;
					}
					return;
				} else if (playerCharacter && (playerCharacter->GetPlayerRuntimeData().playerFlags.isSprinting == 0) && !bStoppingSprint) {
					logger::debug("not sprinting, reset held seconds");
					a_event->heldDownSecs = 0.f;
				} else if (a_event->IsUp()) {
					logger::debug("key is back up, not stopping sprint");
					bStoppingSprint = false;
				}
			}
		}

		/*if (Settings::bUseSprintButton) {
			auto playerCharacter = RE::PlayerCharacter::GetSingleton();
			auto userEvent = a_event->QUserEvent();
			auto userEvents = RE::UserEvents::GetSingleton();

			if (userEvent == userEvents->sprint) {
				if (a_event->HeldDuration() < Settings::fSprintHoldDuration) {
					if (a_event->IsUp()) {
						Events::Dodge();
						heldDownTimeOffset = 0.f;
					}
					return;
				} else if (a_event->IsUp()) {
					heldDownTimeOffset = 0.f;
				} else {
					if (heldDownTimeOffset == 0.f) {
						heldDownTimeOffset = a_event->HeldDuration();
					}
					a_event->heldDownSecs -= heldDownTimeOffset;
				}
			}
		}*/
				
		_ProcessButton(a_this, a_event, a_data);
	}
}
