#include "../GraphWarEdu.h"

#include "MenuScene.h"

namespace gw{
	MenuScene::MenuScene(GraphWarEdu& game) :
		gf::Scene(game.getRenderer().getSize()),
		m_game(game),
		m_backgroundTexture(game.resources.getTexture("background.jpg")),
		m_upAction("UpAction"),
		m_downAction("DownAction"),
		m_triggerAction("TriggerAction"),
		m_quitAction("Quit"),
		m_newGame("New Game", game.resources.getFont("RustyHooksRegular.ttf")),
		m_rules("Rules", game.resources.getFont("RustyHooksRegular.ttf")),
		m_quit("Quit", game.resources.getFont("RustyHooksRegular.ttf"))
	{
		setClearColor(gf::Color::Black);

		SetupActions();

		auto setupButton = [&] (gf::TextButtonWidget& button, auto callback) {
			button.setDefaultTextColor(gf::Color::Black);
			button.setDefaultBackgroundColor(gf::Color::Gray(0.7f));
			button.setSelectedTextColor(gf::Color::Black);
			button.setSelectedBackgroundColor(gf::Color::Blue);
			button.setDisabledTextColor(gf::Color::Black);
			button.setDisabledBackgroundColor(gf::Color::Red);
			button.setAnchor(gf::Anchor::TopLeft);
			button.setAlignment(gf::Alignment::Center);
			button.setCallback(callback);
			m_widgets.addWidget(button);
		};

		setupButton(m_newGame, [&] () {
			m_game.replaceAllScenes(m_game.m_game);
		});

		setupButton(m_rules, [&] () {
			m_game.replaceAllScenes(m_game.m_rules);
		});

		setupButton(m_quit, [&] () {
			m_game.popAllScenes();
		});
	}

	void MenuScene::SetupActions() {
		m_quitAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::B);
		m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
		addAction(m_quitAction);

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
	}

	void MenuScene::doHandleActions([[maybe_unused]] gf::Window& window) {
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

		if (m_quitAction.isActive()) {
			m_game.popAllScenes();
		}
	}

	void MenuScene::doProcessEvent(gf::Event& event) {
		switch (event.type)
		{
		case gf::EventType::MouseMoved:
			m_widgets.pointTo(m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
			break;
		default :
			break;
		}
	}

	void MenuScene::doUpdate(gf::Time time) {

	}

	void MenuScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
		gf::Coordinates coords(target);

		float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
		float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;

		gf::Sprite background(m_backgroundTexture);
		background.setPosition(coords.getCenter());
		background.setAnchor(gf::Anchor::Center);
		background.setScale(backgroundScale);
		target.draw(background, states);

		target.setView(getHudView());

		unsigned titleCharacterSize = coords.getRelativeCharacterSize(0.1f);

		gf::Text title("Graph War Edu", m_game.resources.getFont("RustyHooksRegular.ttf"), titleCharacterSize);
		title.setColor(gf::Color::White);
		title.setPosition(coords.getRelativePoint({ 0.5f, 0.1f }));
		title.setAnchor(gf::Anchor::TopCenter);
		target.draw(title, states);

		constexpr float characterSize = 0.065f;
		constexpr float spaceBetweenButton = 0.05f;
		constexpr gf::Vector2f backgroundSize(0.45f, 0.28f);

		const float paragraphWidth = coords.getRelativeSize(backgroundSize - 0.05f).x;
		const float paddingSize = coords.getRelativeSize({0.01f, 0.f}).x;
		const unsigned resumeCharacterSize = coords.getRelativeCharacterSize(characterSize);

		m_newGame.setCharacterSize(resumeCharacterSize);
		m_newGame.setPosition(coords.getRelativePoint({0.275f, 0.35f}));
		m_newGame.setParagraphWidth(paragraphWidth);
		m_newGame.setPadding(paddingSize);

		m_rules.setCharacterSize(resumeCharacterSize);
		m_rules.setPosition(coords.getRelativePoint({0.275f, 0.35f + (characterSize + spaceBetweenButton) * 2}));
		m_rules.setParagraphWidth(paragraphWidth);
		m_rules.setPadding(paddingSize);

		m_quit.setCharacterSize(resumeCharacterSize);
		m_quit.setPosition(coords.getRelativePoint({0.275f, 0.35f + (characterSize + spaceBetweenButton) * 5}));
		m_quit.setParagraphWidth(paragraphWidth);
		m_quit.setPadding(paddingSize);

		m_widgets.render(target, states);
	}

	void MenuScene::doShow() {
		m_widgets.clear();

		m_newGame.setDefault();
		m_widgets.addWidget(m_newGame);

		m_rules.setDefault();
		m_widgets.addWidget(m_rules);

		m_quit.setDefault();
		m_widgets.addWidget(m_quit);

		m_widgets.selectNextWidget();
	}
}