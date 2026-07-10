#include "../GraphWarEdu.h"

#include "GameScene.h"
#include <iostream>

namespace gw{
	std::pair<double, double> GameScene::getRenderCoordsOnMap(const double x, const double y, const double mapWitdh, const double mapHeight) {
		const double lenghtX = m_game->m_map.m_size.maxX - m_game->m_map.m_size.minX;
		const double lenghtY = m_game->m_map.m_size.minY - m_game->m_map.m_size.minY;

		return {(x - m_game->m_map.m_size.minX)/(mapWitdh/lenghtX), (y - m_game->m_map.m_size.minY)/(mapHeight/lenghtX)};
	}

	void GameScene::genarateMapTexture(int imageWitdh) {
		const double lenghtX = m_game->m_map.m_size.maxX - m_game->m_map.m_size.minX;
		const double lenghtY = m_game->m_map.m_size.maxY - m_game->m_map.m_size.minY;
		const int imageHeight = std::round(imageWitdh *  lenghtY/lenghtX);

		// Create white image
		gf::Color4u white = {0xFF, 0xFF, 0xFF, 0xFF};
		gf::Color4u lightGrey = {0xDD, 0xDD, 0xDD, 0xFF};
		gf::Color4u grey = {0xBB, 0xBA, 0xBA, 0xFF};
		gf::Color4u black = {0x00, 0x00, 0x00, 0xFF};
		gf::Image image({imageWitdh, imageHeight}, white);

		// Add rows and columns
		for (int x = std::round(m_game->m_map.m_size.minX); x < m_game->m_map.m_size.maxX; x++) {
			const int imageCoordX = (x - m_game->m_map.m_size.minX)*(imageWitdh/lenghtX);
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
		
		for (int y = std::round(m_game->m_map.m_size.minY); y < m_game->m_map.m_size.maxY; y++) {
			int imageCoordY = (y - m_game->m_map.m_size.minY)*(imageHeight/lenghtY);
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
			default:
				break;
		}
	}

	void GameScene::doUpdate(gf::Time time) {
		
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
			if (scale >= 1.2 || scale <= 0.8) {
				genarateMapTexture(std::round(mapImageSize.width * scale));
			}

			map.scale(scale);
		}

		// Render map
		map.setPosition(coords.getCenter());
		map.setAnchor(gf::Anchor::Center);
		target.draw(map, states);

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
