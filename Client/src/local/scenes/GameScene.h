#ifndef _GameScene_
#define _GameScene_

#include "../../../../Model/Game.h"
#include "../Buttons.h"

namespace gw{
	struct GraphWarEdu;

	class GameScene : public gf::Scene{

		public:
			GameScene(GraphWarEdu& gameManager);
			void loadGame(Scenario scenario);
			void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
			void doProcessEvent(gf::Event& event) override;
			void doHandleActions(gf::Window& window) override;
			void doUpdate(gf::Time time) override;
			void doShow() override;

		private:
			std::pair<double, double> getRenderCoordsOnMap(const double x, const double y, const double mapWitdh, const double mapHeight);
			void genarateMapTexture();

			GraphWarEdu& m_gameManager;

			Game* m_game;

			gf::Texture m_mapTexture;

			gf::Action m_trigerAction;

			gf::Texture& m_backgroundTexture;

			gf::TextButtonWidget m_home;

			gf::WidgetContainer m_widgets;
	};
}

#endif
