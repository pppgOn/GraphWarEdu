#ifndef _GameScene_
#define _GameScene_

namespace gw{
	struct GraphWarEdu;

	class GameScene : public gf::Scene{

		public:
			GameScene(GraphWarEdu& game);
			void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
			void doProcessEvent(gf::Event& event) override;
			void doHandleActions(gf::Window& window) override;
			void doUpdate(gf::Time time) override;
			void doShow() override;

		private:
			GraphWarEdu& m_game;

			gf::Action m_trigerAction;

			gf::Texture& m_backgroundTexture;

			gf::TextButtonWidget m_home;

			gf::WidgetContainer m_widgets;
	};
}

#endif
