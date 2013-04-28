/* This file is part of Grog.
 *
 * Copyright (c) 2013 Alvaro Polo
 *
 * Grog is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Grog is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>

#include <boost/format.hpp>
#include <grog/ui/app.h>
#include <grog/ui/layout.h>
#include <grog/ui/widget.h>

using namespace grog::ui;
using namespace std::placeholders;

const char* mouse_button_name[] = {
  "left", "right", "middle", "wheel up", "wheel down", "unknown"
};

const char* mouse_button_state_name[] = {
  "pressed", "released", "unknown"
};

class FakeWidget : public Widget, public MouseUnresponder {
public:

  FakeWidget(const Color& col) : rect_(shape_factory().CreateRectangle(col)){
  }

  virtual void Draw(const Rect2<int>& screen_region) const {
    rect_->Draw(screen_region);
  }

  virtual bool Respond(const MouseButtonEvent& ev) { return false; }

  virtual bool Respond(const MouseMotionEvent& ev) { return false; }

private:

  Ptr<Rectangle> rect_;
};

void OnMouseButton(Application* app, const MouseButtonEvent& ev) {
  app->context()->PostRedisplay();
  std::cout << boost::format("Mouse button %s was %s on (%d, %d)") %
          mouse_button_name[ev.button] %
          mouse_button_state_name[ev.state] %
          ev.pos.x % ev.pos.y << std::endl;
}

void GrogMain(const GrogMainArgs& args) throw (grog::util::Error) {
  Application& app = Application::init();
  app.context()->loop().RegisterMouseButtonEventHandler(
        std::bind(OnMouseButton, &app, _1));
  auto win = app.NewWindow();
  Ptr<Widget> fake1 = new FakeWidget(Color::kLightRed);
  Ptr<Widget> fake2 = new FakeWidget(Color::kLightGreen);
  Ptr<Widget> fake3 = new FakeWidget(Color::kLightBlue);

  fake2->set_locked(true); // fake2 cannot be moved

  win->set_child<FixedLayout>(new FixedLayout())
      .AddWidget(fake1, Rect2<int>(100, 100, 50, 50))
      .AddWidget(fake2, Rect2<int>(200, 200, 50, 50))
      .AddWidget(fake3, Rect2<int>(300, 300, 50, 50));
  app.Run();
}
