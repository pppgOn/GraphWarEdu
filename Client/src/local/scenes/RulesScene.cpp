#include "../GraphWarEdu.h"

#include "RulesScene.h"

namespace gw{
	RulesScene::RulesScene(GraphWarEdu& game):
		gf::Scene(game.getRenderer().getSize()),
		m_backgroundTexture(game.resources.getTexture("background.jpg")),
		m_home("Go home", game.resources.getFont("RustyHooksRegular.ttf")),
		m_upAction("UpAction"),
		m_downAction("DownAction"),
		m_triggerAction("TriggerAction"),
		m_gameManager(game)
	{
		setClearColor(gf::Color::White);

		m_upAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Negative);
		m_upAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadUp);
		m_upAction.addScancodeKeyControl(gf::Scancode::Up);
		addAction(m_upAction);

		m_downAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Positive);
		m_downAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadDown);
		m_downAction.addScancodeKeyControl(gf::Scancode::Down);
		addAction(m_downAction);

		m_triggerAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
		m_triggerAction.addScancodeKeyControl(gf::Scancode::Return);
		m_triggerAction.addMouseButtonControl(gf::MouseButton::Left);
		addAction(m_triggerAction);

		setupButton(m_home, m_widgets, [&] () {
			m_gameManager.replaceAllScenes(m_gameManager.m_menu);
		});
	}

	void RulesScene::doHandleActions([[maybe_unused]] gf::Window& window) {
		if (!isActive()) {
			return;
		}

		if (m_upAction.isActive()) {
			m_widgets.selectPreviousWidget();
		}

		if (m_downAction.isActive()) {
			m_widgets.selectNextWidget();
		}

		if (m_triggerAction.isActive()) {
			m_widgets.triggerAction();
		}
	}

	void RulesScene::doProcessEvent(gf::Event& event) {
		switch (event.type){
			case gf::EventType::MouseMoved:
				m_widgets.pointTo(m_gameManager.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
				break;
			default :
				break;
		}
	}


	void RulesScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states){
		gf::Coordinates coords(target);

		float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
		float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;

		gf::Sprite background(m_backgroundTexture);
		background.setPosition(coords.getCenter());
		background.setAnchor(gf::Anchor::Center);
		background.setScale(backgroundScale);
		target.draw(background, states);

		constexpr float characterSize = 0.075f;
		constexpr gf::Vector2f backgroundSize(0.36f, 0.2f);

		const float paragraphWidth = coords.getRelativeSize(backgroundSize - 0.05f).x;
		const float paddingSize = coords.getRelativeSize({0.01f, 0.f}).x;
		const unsigned resumeCharacterSize = coords.getRelativeCharacterSize(characterSize);

		m_home.setCharacterSize(resumeCharacterSize);
		m_home.setPosition(coords.getRelativePoint({0.345f, 0.93f}));
		m_home.setParagraphWidth(paragraphWidth);
		m_home.setPadding(paddingSize);

		m_widgets.render(target, states);

		target.setView(getHudView());
	}

	void RulesScene::doShow(){
		m_home.setDefault();
		m_widgets.addWidget(m_home);

		m_widgets.selectNextWidget();
	}
}
