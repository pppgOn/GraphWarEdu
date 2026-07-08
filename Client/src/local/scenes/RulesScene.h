#ifndef _RulesScene_
#define _RulesScene_


#include <gf/ResourceManager.h>
#include <gf/Scene.h>
#include <gf/Rect.h>
#include "../Buttons.h"

namespace gw{
	struct GraphWarEdu;

	class RulesScene : public gf::Scene {
		public:
			RulesScene(GraphWarEdu& game);

			void doHandleActions(gf::Window& window) override;
			void doProcessEvent(gf::Event& event) override;
			void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
			void doShow() override;

		private:
			GraphWarEdu& m_gameManager;

			gf::Texture& m_backgroundTexture;
			gf::TextButtonWidget m_home;

			gf::WidgetContainer m_widgets;

			gf::Action m_upAction;
			gf::Action m_downAction;
			gf::Action m_triggerAction;
	};
}

#endif
