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

#include <SDL/SDL.h>

#include "grog/ui/event-sdl.h"

namespace grog { namespace ui {

namespace {

MouseMotionEvent ToEvent(const SDL_MouseMotionEvent& ev) {
  MouseMotionEvent result = {
    { ev.x, ev.y },
    { ev.xrel, ev.yrel }
  };
  return result;
}

MouseButton ToMouseButton(Uint8 btn) {
  switch (btn) {
    case SDL_BUTTON_LEFT: return kLeftMouseButton;
    case SDL_BUTTON_RIGHT: return kRightMouseButton;
    case SDL_BUTTON_MIDDLE: return kMiddleMouseButton;
    case SDL_BUTTON_WHEELUP: return kWheelUpMouseButton;
    case SDL_BUTTON_WHEELDOWN: return kWheelDownMouseButton;
    default: return kUnknownMouseButton;
  }
}

MouseButtonState ToMouseButtonState(Uint8 state) {
  switch (state) {
    case SDL_PRESSED: return kMouseButtonPressed;
    case SDL_RELEASED: return kMouseButtonReleased;
    default: return kUnknownMouseState;
  }
}

MouseButtonEvent ToEvent(const SDL_MouseButtonEvent& ev) {
  MouseButtonEvent result = {
    ToMouseButton(ev.button),
    ToMouseButtonState(ev.state),
    { ev.x, ev.y }
  };
  return result;
}

} // anonymous namespace

void SDLEventProducer::PollPendingEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_MOUSEMOTION:
        HandleMouseMotionEvent(ToEvent(event.motion));
        break;
      case SDL_MOUSEBUTTONUP:
      case SDL_MOUSEBUTTONDOWN:
        HandleMouseButtonEvent(ToEvent(event.button));
        break;
    }
  }
}

}} // namespace grog::ui

