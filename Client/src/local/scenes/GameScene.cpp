#include "../GraphWarEdu.h"

#include "GameScene.h"
#include <iostream>

namespace gw{
	float GameScene::getMapScale(float renderWidth) {
		return renderWidth / m_game->m_map.m_size.width;
	}

	 gf::Vector2f GameScene::getRenderCoordsOnMap(const std::pair<float, float> position, float mapWitdh, float mapHeight, const gf::Vector2f offset) {
		return {(position.first - m_game->m_map.m_limit.minX)*(mapWitdh/m_game->m_map.m_size.width) + offset.x, (position.second - m_game->m_map.m_limit.minY)*(mapHeight/m_game->m_map.m_size.height) + offset.y};
	}

	void GameScene::genarateMapTexture(int imageWitdh) {
		const int imageHeight = std::round(imageWitdh *  m_game->m_map.m_size.height/m_game->m_map.m_size.width);

		// Create white image
		gf::Color4u white = {0xFF, 0xFF, 0xFF, 0xFF};
		gf::Color4u lightGrey = {0xDD, 0xDD, 0xDD, 0xFF};
		gf::Color4u grey = {0xBB, 0xBA, 0xBA, 0xFF};
		gf::Color4u black = {0x00, 0x00, 0x00, 0xFF};
		gf::Image image({imageWitdh, imageHeight}, white);

		// Add rows and columns
		for (int x = std::round(m_game->m_map.m_limit.minX); x < m_game->m_map.m_limit.maxX; x++) {
			const int imageCoordX = (x - m_game->m_map.m_limit.minX)*(imageWitdh/m_game->m_map.m_size.width);
			if (x == 0) {
				for (int y = 0; y < imageHeight; y++) {
					image.setPixel({imageCoordX - 1, y}, black);
					image.setPixel({imageCoordX, y}, black);
					image.setPixel({imageCoordX + 1, y}, black);
				}
			} else if (x % 5 == 0) {
				for (int y = 0; y < imageHeight; y++) {
					image.setPixel({imageCoordX - 1, y}, grey);
					image.setPixel({imageCoordX, y}, grey);
					image.setPixel({imageCoordX + 1, y}, grey);
				}
			} else {
				for (int y = 0; y < imageHeight; y++) {
					image.setPixel({imageCoordX, y}, lightGrey);
				}
			}
		}
		
		for (int y = std::round(m_game->m_map.m_limit.minY); y < m_game->m_map.m_limit.maxY; y++) {
			int imageCoordY = (y - m_game->m_map.m_limit.minY)*(imageHeight/m_game->m_map.m_size.height);
			if (y == 0) {
				for (int x = 0; x < imageWitdh; x++) {
					for (int i = -1; i < 2; i++) {
						image.setPixel({x, imageCoordY + i}, black);
					}
				}
			} else if (y % 5 == 0) {
				for (int x = 0; x < imageWitdh; x++) {
					for (int i = -1; i < 2; i++) {
						// Check override of darker colors
						if (image.getPixel({x, imageCoordY + i}) != black) {
							image.setPixel({x, imageCoordY + i}, grey);
						}
					}
				}
			} else {
				for (int x = 0; x < imageWitdh; x++) {
					// Check override of darker colors
					if (image.getPixel({x, imageCoordY}) == white) {
						image.setPixel({x, imageCoordY}, lightGrey);
					}
				}
			}
		}

		// Add numbers TODO

		// Save image
		m_mapTexture.resize({imageWitdh, imageHeight});
		m_mapTexture.update(image);
	}

	GameScene::GameScene(GraphWarEdu& gameManager):
		gf::Scene(gameManager.getRenderer().getSize()),
		m_backgroundTexture(gameManager.resources.getTexture("background.jpg")),
		m_gameManager(gameManager),
		m_trigerAction("trigerAction"),
		m_home("Go home", gameManager.resources.getFont("RustyHooksRegular.ttf"))
	{
		setClearColor(gf::Color::Black);

		m_trigerAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
		m_trigerAction.addMouseButtonControl(gf::MouseButton::Left);
		addAction(m_trigerAction);

		setupButton(m_home, m_widgets, [&] () {
			m_gameManager.replaceAllScenes(m_gameManager.m_menu);
		});
	}

	void GameScene::loadGame(Scenario screnarioName) {
		m_game = new Game(screnarioName),
		genarateMapTexture(1000);
	}

	void GameScene::doHandleActions([[maybe_unused]] gf::Window& window) {
		if (!isActive()) {
			return;
		}

		if (m_trigerAction.isActive()) {
			m_widgets.triggerAction();
		}
	}

	void GameScene::doProcessEvent(gf::Event& event) {
		switch (event.type)
		{
			case gf::EventType::MouseMoved:
				m_widgets.pointTo(m_gameManager.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
				break;
			default:
				break;
		}
	}

	void GameScene::doUpdate(gf::Time time) {
		
	}

	void GameScene::renderEntityCircle(gf::RenderTarget &target, const gf::RenderStates &states, const Entity entity, const gf::v1::Vector2i mapImageSize, const gf::Vector2f mapTopLeftCoords, const gf::Color4f color) {
		gf::CircleShape circle;
		circle.setRadius(entity.m_radius * getMapScale(mapImageSize.width));
		circle.setColor(color);
		circle.setPointCount(std::round(circle.getRadius()) + 30);
		circle.setPosition(getRenderCoordsOnMap(entity.m_position, mapImageSize.width, mapImageSize.height, mapTopLeftCoords));
		circle.setAnchor(gf::Anchor::Center);
		target.draw(circle, states);
	}

	void GameScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states)
	{
		gf::Coordinates coords(target);

		float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
		float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;

		gf::Sprite background(m_backgroundTexture);
		background.setPosition(coords.getCenter());
		background.setAnchor(gf::Anchor::Center);
		background.setScale(backgroundScale);
		target.draw(background, states);

		target.setView(getHudView());

		// Load map texture
		gf::Sprite map(m_mapTexture);

		// Resize sprite to fit window
		const gf::v1::Vector2i windowSize = coords.getWindowSize();
		const gf::v1::Vector2i mapImageSize = m_mapTexture.getSize();
		float scale = 1;
		if (mapImageSize.width / mapImageSize.height > windowSize.width / windowSize.height) {
			scale = (windowSize.width * 0.8) / mapImageSize.width;
		} else {
			scale = (windowSize.height * 0.8) / mapImageSize.height;
		}
		
		if (map.getScale().width != scale) {
			genarateMapTexture(std::round(mapImageSize.width * scale));
		}

		// Render map
		map.setPosition(coords.getCenter());
		map.setAnchor(gf::Anchor::Center);
		target.draw(map, states);

		const gf::Vector2f mapTopLeftCoords = map.getPosition() - map.getOrigin();

		// Render obstacles
		for (const Entity obstacle : m_game->m_map.m_obstacles) {
			renderEntityCircle(target, states, obstacle, mapImageSize, mapTopLeftCoords, gf::Color::Black);
		}

		// Render player
		for (const Entity charachter : m_game->m_map.m_playerOne.m_charachters) {
			renderEntityCircle(target, states, charachter, mapImageSize, mapTopLeftCoords, gf::Color::Blue);

		}

		// Render enemies
		for (const Entity enemy : m_game->m_map.m_playerTwo.m_charachters) {
			renderEntityCircle(target, states, enemy, mapImageSize, mapTopLeftCoords, gf::Color::Red);
		}

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

		m_widgets.render(target, states);

		target.setView(getHudView());
	}

	void GameScene::doShow() {
		m_widgets.clear();

		m_home.setDefault();
		m_widgets.addWidget(m_home);

		m_widgets.selectNextWidget();

		loadGame(linear);
	}
}
