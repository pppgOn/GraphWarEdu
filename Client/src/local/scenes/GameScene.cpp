#include "../GraphWarEdu.h"

#include "GameScene.h"

namespace gw{
	GameScene::GameScene(GraphWarEdu& game):
		gf::Scene(game.getRenderer().getSize()),
		m_backgroundTexture(game.resources.getTexture("background.jpg")),
		m_game(game),
		m_trigerAction("trigerAction"),
		m_home("Go home", game.resources.getFont("RustyHooksRegular.ttf"))
	{
		setClearColor(gf::Color::Black);

		m_trigerAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
		m_trigerAction.addMouseButtonControl(gf::MouseButton::Left);
		addAction(m_trigerAction);
	}


	void GameScene::doHandleActions([[maybe_unused]] gf::Window& window) {
		if (!isActive()) {
			return;
		}

		if(m_trigerAction.isActive()){
			m_widgets.triggerAction();
		}
	}


	void GameScene::doProcessEvent(gf::Event& event) {
		switch (event.type)
		{
			default:
				break;
		}
	}

	void GameScene::doUpdate(gf::Time time){
		
	}

	void GameScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states){
		gf::Coordinates coords(target);

		float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
		float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;

		gf::Sprite background(m_backgroundTexture);
		background.setPosition(coords.getCenter());
		background.setAnchor(gf::Anchor::Center);
		background.setScale(backgroundScale);
		target.draw(background, states);

		target.setView(getHudView());

		//button to go home
		constexpr float characterSize = 0.02f;
		constexpr gf::Vector2f backgroundSize(0.3f, 0.05f);

		const float paragraphWidth = coords.getRelativeSize(backgroundSize - 0.05f).x;
		const float paddingSize = coords.getRelativeSize({0.01f, 0.f}).x;
		const unsigned resumeCharacterSize = coords.getRelativeCharacterSize(characterSize);

		m_home.setCharacterSize(resumeCharacterSize);
		m_home.setPosition(coords.getRelativePoint({0.38f, 0.95f}));
		m_home.setParagraphWidth(paragraphWidth);
		m_home.setPadding(paddingSize);
	}

	void GameScene::doShow() {
		m_widgets.clear();

		m_home.setDefault();
		m_widgets.addWidget(m_home);

		m_widgets.selectNextWidget();
	}
}
