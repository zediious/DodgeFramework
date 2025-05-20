#pragma once

namespace Hooks
{
	class SprintHandlerHook
	{
	public:
		static void Hook()
		{
			REL::Relocation<uintptr_t> hook1{ RELOCATION_ID(263057, 42409) };
			REL::Relocation<std::uintptr_t> SprintHandlerVtbl{ RE::VTABLE_SprintHandler[0] };    // 166F1D8
			_ProcessButton = SprintHandlerVtbl.write_vfunc(0x4, ProcessButton);


		}

	private:
		static void ProcessButton(RE::SprintHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data);

		static inline REL::Relocation<decltype(ProcessButton)> _ProcessButton;
	};

	void Install();
}
