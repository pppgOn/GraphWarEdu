#ifndef _GraphWarEdu_
#define _GraphWarEdu_


#include <gf/GameManager.h>
#include "scenes/MenuScene.h"
#include "scenes/RulesScene.h"
#include "scenes/GameScene.h"
#include "../../config.h"

namespace gw{
	struct GraphWarEdu : public gf::GameManager{
		public:
			GraphWarEdu();

			//scenes
			MenuScene m_menu;
			RulesScene m_rules;
			GameScene m_game;
	};
}


#endif