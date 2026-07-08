#ifndef _Buttons_
#define _Buttons_

#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>

namespace gw{
	void setupButton(gf::TextButtonWidget& button, gf::WidgetContainer& widgets, std::function<void()> callback);
}

#endif