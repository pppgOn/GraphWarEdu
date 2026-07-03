#include "GraphWarEdu.h"

namespace gw{
	GraphWarEdu::GraphWarEdu() :
		gf::GameManager("Graph War Edu", {GAME_DATADIR}),
		m_menu(*this),
		m_rules(*this),
		m_game(*this)
	{
		pushScene(m_menu);

		// Set frame limit
		getWindow().setVerticalSyncEnabled(true);
	}
}