#ifndef _GameScene_
#define _GameScene_

#include <imgui.h>
#include <imgui_impl_gf.h>
#include <cstring>
#include <chrono>

#include <gf/Particles.h>
#include "Game.h"
#include "Buttons.h"
#include "GameWindow.h"

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
			void endFunctionResolution();
			GraphWarEdu& m_gameManager;

			Game* m_game;

			std::array<char, 1024> m_functionTyped;

			// Time, in seconds from the start of function rendering
			float m_functionResolutionTime;
			float m_lastFunctionEvaluation;
			float m_lastFunctionUnknownValue;

			gf::VertexArray m_functionRenderPoints;

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
