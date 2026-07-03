#ifndef _MenuScene_
#define _MenuScene_


#include <gf/Scene.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Event.h>
#include <gf/Text.h>
#include <gf/Action.h>
#include <gf/Coordinates.h>
#include <gf/Sprite.h>


namespace gw{
	struct GraphWarEdu;

	class MenuScene : public gf::Scene {
		public:
			MenuScene(GraphWarEdu& game);

		private:
			void doProcessEvent(gf::Event& event) override;
			void doHandleActions(gf::Window& window) override;
			void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
			void doShow() override;
			void doUpdate(gf::Time)override;

		private:
			void SetupActions();

			GraphWarEdu& m_game;

			gf::Action m_quitAction;
			gf::Action m_upAction;
			gf::Action m_downAction;
			gf::Action m_triggerAction;

			gf::Texture& m_backgroundTexture;

			gf::TextButtonWidget m_newGame;
			gf::TextButtonWidget m_quit;
			gf::TextButtonWidget m_rules;

			gf::WidgetContainer m_widgets;
	};
}


#endif