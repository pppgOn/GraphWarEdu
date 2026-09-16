#include "GraphWarEdu.h"

namespace gw{
	GraphWarEdu::GraphWarEdu() :
		gf::GameManager("Graph War Edu", {GAME_DATADIR}),
		m_menu(*this),
		m_rules(*this),
		m_game(*this)
	{		
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;
		ImGui_ImplGF_Init(getWindow(), getRenderer());

		// Set frame limit
		getWindow().setVerticalSyncEnabled(true);

		pushScene(m_menu);
	}
}