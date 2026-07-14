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
			float getMapScale(float renderWidth);
			gf::Vector2f getRenderCoordsOnMap(const std::pair<float, float> position, const float mapWitdh, const float mapHeight, const gf::Vector2f offset);
			void genarateMapTexture(int width);
			void renderEntityCircle(gf::RenderTarget &target, const gf::RenderStates &states, const Entity entity, const gf::v1::Vector2i mapImageSize, const gf::Vector2f mapTopLeftCoords, const gf::Color4f color);

			GraphWarEdu& m_gameManager;

			Game* m_game;

			gf::Texture m_mapTexture = gf::vec(1000, 1000);

			gf::Action m_trigerAction;

			gf::Texture& m_backgroundTexture;

			gf::TextButtonWidget m_home;

			gf::WidgetContainer m_widgets;
	};
}

#endif
