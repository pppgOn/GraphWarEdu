#include "GameWindow.h"

namespace gw{
	 gf::Vector2f getRenderCoordsOnMap(const std::pair<float, float> position, const Game* game, const gf::v1::Vector2i mapImageSize, const gf::v1::Vector2f mapTopLeftCoords) {
		return {(position.first - game->m_map.m_limit.minX)*(mapImageSize.width/game->m_map.m_size.width) + mapTopLeftCoords.x, (-position.second - game->m_map.m_limit.minY)*(mapImageSize.height/game->m_map.m_size.height) + mapTopLeftCoords.y};
	}

	void genarateMapTexture(int imageWitdh, const Game* game, gf::Texture &mapTexture) {
		const int imageHeight = std::round(imageWitdh * game->m_map.m_size.height/game->m_map.m_size.width);

		// Create white image
		gf::Image image({imageWitdh, imageHeight}, white);

		// Add rows and columns
		for (int x = std::round(game->m_map.m_limit.minX); x < game->m_map.m_limit.maxX; x++) {
			const int imageCoordX = (x - game->m_map.m_limit.minX)*(imageWitdh/game->m_map.m_size.width);
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
		
		for (int y = std::round(game->m_map.m_limit.minY); y < game->m_map.m_limit.maxY; y++) {
			int imageCoordY = (y - game->m_map.m_limit.minY)*(imageHeight/game->m_map.m_size.height);
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
		mapTexture.resize({imageWitdh, imageHeight});
		mapTexture.update(image);
	}

	float getMapScale(float renderWidth, const Game* game) {
		return renderWidth / game->m_map.m_size.width;
	}

	void renderEntityCircle(gf::RenderTarget &target, const gf::RenderStates &states, const Entity entity, const gf::Color4f color, const gf::v1::Vector2i mapImageSize, const Game* game, const gf::v1::Vector2f mapTopLeftCoords) {
		gf::CircleShape circle;
		circle.setRadius(entity.m_radius * getMapScale(mapImageSize.width, game));
		circle.setColor(color);
		circle.setPointCount(std::round(circle.getRadius()) + 30);
		circle.setPosition(getRenderCoordsOnMap(entity.m_position, game, mapImageSize, mapTopLeftCoords));
		circle.setAnchor(gf::Anchor::Center);
		target.draw(circle, states);
	}
}