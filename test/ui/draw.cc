/* This file is part of Grog.
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

#include <grog/ui/app.h>
#include <boost/format.hpp>

using namespace grog::ui;

const char* mouse_button_name[] = {
  "left", "right", "middle", "wheel up", "wheel down", "unknown"
};

const char* mouse_button_state_name[] = {
  "pressed", "released", "unknown"
};

void OnMouseButton(const MouseButtonEvent& ev) {
  std::cout << boost::format("Mouse button %s was %s on (%d, %d)") %
          mouse_button_name[ev.button] %
          mouse_button_state_name[ev.state] %
          ev.pos.x % ev.pos.y << std::endl;
}

void GrogMain(const GrogMainArgs& args) throw (grog::util::Error) {
  Application app;
  app.context()->loop().RegisterMouseButtonEventHandler(
        &OnMouseButton);
  app.Run();
}
