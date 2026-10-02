#ifndef _GameWindow_
#define _GameWindow_

#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Coordinates.h>
#include <gf/Image.h>
#include <gf/Texture.h>
#include <gf/RenderTarget.h>
#include "Game.h"

namespace gw{
	constexpr gf::Color4u white = {0xFF, 0xFF, 0xFF, 0xFF};
	constexpr gf::Color4u lightGrey = {0xDD, 0xDD, 0xDD, 0xFF};
	constexpr gf::Color4u grey = {0xBB, 0xBA, 0xBA, 0xFF};
	constexpr gf::Color4u black = {0x00, 0x00, 0x00, 0xFF};

	gf::Vector2f getRenderCoordsOnMap(const std::pair<float, float> position, const Game* game, const gf::v1::Vector2i mapImageSize, const gf::v1::Vector2f mapTopLeftCoords);
	void genarateMapTexture(int imageWitdh, const Game* game, gf::Texture &mapTexture);
	float getMapScale(float renderWidth, const Game* game);
	void renderEntityCircle(gf::RenderTarget &target, const gf::RenderStates &states, const Entity entity, const gf::Color4f color, const gf::v1::Vector2i mapImageSize, const Game* game, const gf::v1::Vector2f mapTopLeftCoords);
}

#endif