#ifndef _GameScene_
#define _GameScene_

#include <imgui.h>
#include <imgui_impl_gf.h>
#include <cstring>

#include <gf/Particles.h>
#include "../../../../Model/Game.h"
#include "../Buttons.h"

constexpr gf::Color4u white = {0xFF, 0xFF, 0xFF, 0xFF};
constexpr gf::Color4u lightGrey = {0xDD, 0xDD, 0xDD, 0xFF};
constexpr gf::Color4u grey = {0xBB, 0xBA, 0xBA, 0xFF};
constexpr gf::Color4u black = {0x00, 0x00, 0x00, 0xFF};

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
			gf::Vector2f getRenderCoordsOnMap(const std::pair<float, float> position);
			void genarateMapTexture(int width);
			void renderEntityCircle(gf::RenderTarget &target, const gf::RenderStates &states, const Entity entity, const gf::Color4f color);
			void endFunctionResolution();
			GraphWarEdu& m_gameManager;

			Game* m_game;

			std::array<char, 1024> m_functionTyped;

			// Time, in seconds from the start of function rendering
			float m_functionResolutionTime;

			gf::PointParticles m_functionRenderPoints;

			gf::Texture m_mapTexture = gf::vec(1000, 1000);
			gf::v1::Vector2i m_mapImageSize;
			gf::v1::Vector2f m_mapTopLeftCoords;

			gf::Action m_trigerAction;

			gf::Texture& m_backgroundTexture;

			gf::TextButtonWidget m_home;

			gf::WidgetContainer m_widgets;
	};
}

#endif
