#include "../GraphWarEdu.h"

#include "GameScene.h"
#include <iostream>

namespace gw{
	float GameScene::getMapScale(float renderWidth) {
		return renderWidth / m_game->m_map.m_size.width;
	}

	 gf::Vector2f GameScene::getRenderCoordsOnMap(const std::pair<float, float> position) {
		return {(position.first - m_game->m_map.m_limit.minX)*(m_mapImageSize.width/m_game->m_map.m_size.width) + m_mapTopLeftCoords.x, (-position.second - m_game->m_map.m_limit.minY)*(m_mapImageSize.height/m_game->m_map.m_size.height) + m_mapTopLeftCoords.y};
	}

	void GameScene::genarateMapTexture(int imageWitdh) {
		const int imageHeight = std::round(imageWitdh * m_game->m_map.m_size.height/m_game->m_map.m_size.width);

		// Create white image
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
		m_home("Leave", gameManager.resources.getFont("RustyHooksRegular.ttf")),
		m_mapImageSize(1000, 1000),
		m_mapTopLeftCoords(0, 0)
	{
		setClearColor(gf::Color::Black);
		std::strncpy(m_functionTyped.data(),"", 1023);

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
		ImGui_ImplGF_ProcessEvent(event);
	}

	void GameScene::doUpdate(gf::Time time) {
		ImGui_ImplGF_Update(time);
		if (m_game->m_state == FunctionResolution) {
			const Character currentCharacter = *m_game->getCurrentCharacter();
			const float x = m_functionResolutionTime * FUNCTION_RESOLUTION_TIME_FACTOR + currentCharacter.m_position.first;
			const float y = currentCharacter.evaluateFonction(x) - currentCharacter.m_position.second;

			if (x > m_game->m_map.m_limit.maxX || y > m_game->m_map.m_limit.maxY || y < m_game->m_map.m_limit.minY) {
				// Map limit reached, stop the function resolution
				endFunctionResolution();
			} else {
				// Check if a player or an obstacle has been hit
				std::pair<float,float> characterPositionHit;
				if (m_game->m_map.CheckObstaclesHit({x, y})) {
					// TODO : obstacle explosion animation on the position
					endFunctionResolution();
				} else {
					if (m_game->m_map.CheckPlayerHit({x, y}, characterPositionHit, currentCharacter)) {
						// TODO : character explosion animation on the characterPositionHit
					}

					m_functionRenderPoints.addPoint(getRenderCoordsOnMap({x, y}), black);
					m_functionResolutionTime += time.asSeconds();
				}
			}
		}
	}

	void GameScene::endFunctionResolution() {
		m_game->m_state = PlayerTurn;
		// Setup function for the new charater playing
		std::strncpy(m_functionTyped.data(), m_game->getCurrentCharacter()->getCurrentFunction().data(), 1023);
		// Clear rendered points
		// m_functionRenderPoints = gf::PointParticles(); // TODO: UNCOMMENT
	}

	void GameScene::renderEntityCircle(gf::RenderTarget &target, const gf::RenderStates &states, const Entity entity, const gf::Color4f color) {
		gf::CircleShape circle;
		circle.setRadius(entity.m_radius * getMapScale(m_mapImageSize.width));
		circle.setColor(color);
		circle.setPointCount(std::round(circle.getRadius()) + 30);
		circle.setPosition(getRenderCoordsOnMap(entity.m_position));
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
		m_mapImageSize = m_mapTexture.getSize();
		float scale = 1;
		if (m_mapImageSize.width / m_mapImageSize.height > windowSize.width / windowSize.height) {
			scale = (windowSize.width * 0.8) / m_mapImageSize.width;
		} else {
			scale = (windowSize.height * 0.8) / m_mapImageSize.height;
		}
		
		if (abs(map.getScale().width - scale) > 0.001) {
			genarateMapTexture(std::round(m_mapImageSize.width * scale));
		}

		// Render map
		map.setPosition(coords.getRelativePoint({0.07f, 0.5f}));
		map.setAnchor(gf::Anchor::CenterLeft);
		target.draw(map, states);

		m_mapTopLeftCoords = map.getPosition() - map.getOrigin();

		// Render function
		// if (m_game->m_state == FunctionResolution) { // TODO : UNCOMMENT
			target.draw(m_functionRenderPoints, states);
		// }

		// Render obstacles
		for (const Entity obstacle : m_game->m_map.m_obstacles) {
			renderEntityCircle(target, states, obstacle, gf::Color::Black);
		}

		// Render player
		for (const Entity charachter : m_game->m_map.m_playerOne.m_charachters) {
			renderEntityCircle(target, states, charachter, gf::Color::Blue);
		}

		// Render enemies
		for (const Entity enemy : m_game->m_map.m_playerTwo.m_charachters) {
			renderEntityCircle(target, states, enemy, gf::Color::Red);
		}

		const gf::Vector2f mapTopRightCoords = m_mapTopLeftCoords + gf::Vector2f((float)m_mapImageSize.width, 0);
		const gf::Vector2f mapBottomRightCoords = m_mapTopLeftCoords + gf::Vector2f(0, (float)m_mapImageSize.height);

		//button to go home
		constexpr float characterSize = 0.02f;
		const float remainingPanelXSpace = windowSize.x - mapTopRightCoords.x;
		const float centerXOfRightPanel = (remainingPanelXSpace / 2) + mapTopRightCoords.x;

		const float paragraphWidth = coords.getRelativeSize({0.08f, 0.f}).x;
		const float paddingSize = coords.getRelativeSize({0.01f, 0.f}).x;
		const unsigned resumeCharacterSize = coords.getRelativeCharacterSize(characterSize);

		m_home.setCharacterSize(resumeCharacterSize);
		m_home.setPosition({centerXOfRightPanel + paddingSize, m_mapTopLeftCoords.y + paddingSize});
		m_home.setAnchor(gf::Anchor::TopCenter);
		m_home.setParagraphWidth(paragraphWidth);
		m_home.setPadding(paddingSize);

		m_widgets.render(target, states);
		// TODO: check player turn to in multi
		const bool isPlayerTurn = m_game->m_state == PlayerTurn;

		// Function input
		constexpr ImGuiWindowFlags defaultWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
		const ImGuiWindowFlags windowFlags = defaultWindowFlags | (isPlayerTurn ? ImGuiWindowFlags_None : ImGuiWindowFlags_NoInputs);
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(windowSize.width, windowSize.height);

		ImGui::NewFrame();
		const float frameHeight = coords.getRelativeSize({0.f, 0.2f}).y;
		ImGui::SetNextWindowPos(ImVec2(centerXOfRightPanel, mapBottomRightCoords.y - frameHeight), 0, ImVec2(0.5f, 0.f));
		ImGui::SetNextWindowSize(ImVec2(remainingPanelXSpace * 0.9, frameHeight), 0);
		if (ImGui::Begin(isPlayerTurn ? "Your turn" : "Waiting for your turn", nullptr, windowFlags)) {
			ImGui::SetWindowFontScale(1);
			ImGui::Text("Function");
			ImGui::SameLine();
			ImGui::InputText("###identifiant", m_functionTyped.data(), m_functionTyped.size());
			ImGui::SetItemDefaultFocus();

			ImGui::Indent();

			if (ImGui::Button("Send", ImVec2(remainingPanelXSpace * 0.8, coords.getRelativeSize({0.f, 0.03f}).y))) {
				m_game->getCurrentCharacter()->setCurrentFunction(m_functionTyped.data()); // TODO: trim
				m_game->m_state = FunctionResolution;
				m_functionResolutionTime = 0;
				m_functionRenderPoints = gf::PointParticles(); // TODO: REMOVE
			}
		}
		ImGui::End();

		target.setView(getHudView());

		ImGui::Render();
		ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
	}

	void GameScene::doShow() {
		m_widgets.clear();

		m_home.setDefault();
		m_widgets.addWidget(m_home);

		m_widgets.selectNextWidget();

		loadGame(linear);
	}
}
