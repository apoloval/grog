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

#ifndef GROG_UI_EVENT_H
#define GROG_UI_EVENT_H

#include <functional>
#include <list>

#include "grog/ui/euclidean.h"

namespace grog { namespace ui {

enum MouseButton {
  kLeftMouseButton,
  kRightMouseButton,
  kMiddleMouseButton,
  kWheelUpMouseButton,
  kWheelDownMouseButton,
  kUnknownMouseButton,
};

enum MouseButtonState {
  kMouseButtonPressed,
  kMouseButtonReleased,
  kUnknownMouseState,
};

struct MouseMotionEvent {
  ScreenCoords abs_pos;
  ScreenCoords rel_pos;

  inline MouseMotionEvent(const ScreenCoords& abs, const ScreenCoords& rel)
    : abs_pos(abs), rel_pos(rel) {}
};

struct MouseButtonEvent {
  MouseButton button;
  MouseButtonState state;
  ScreenCoords pos;

  inline MouseButtonEvent(const MouseButton& button,
                          const MouseButtonState& state,
                          const ScreenCoords& pos)
    : button(button), state(state), pos(pos) {}
};

typedef std::function<void(const MouseMotionEvent)> MouseMotionEventHandler;
typedef std::function<void(const MouseButtonEvent)> MouseButtonEventHandler;
typedef std::function<void(void)> FrameDrawnEventHandler;

/**
 * This class provides an abstraction for an object able to produce input
 * events.
 */
class EventProducer {
public:

  virtual void RegisterMouseMotionEventHandler(
      const MouseMotionEventHandler& handler) = 0;

  virtual void RegisterMouseButtonEventHandler(
      const MouseButtonEventHandler& handler) = 0;
};

class AbstractEventProducer : public EventProducer {
public:

  inline virtual void RegisterMouseMotionEventHandler(
      const MouseMotionEventHandler& handler) {
    mouse_motion_handlers_.push_back(handler);
  }

  inline virtual void RegisterMouseButtonEventHandler(
      const MouseButtonEventHandler& handler) {
    mouse_button_handlers_.push_back(handler);
  }

protected:

  void HandleMouseMotionEvent(const MouseMotionEvent& event) {
    for (auto& handler : mouse_motion_handlers_)
      handler(event);
  }

  void HandleMouseButtonEvent(const MouseButtonEvent& event) {
    for (auto& handler : mouse_button_handlers_)
      handler(event);
  }

private:

  std::list<MouseMotionEventHandler> mouse_motion_handlers_;
  std::list<MouseButtonEventHandler> mouse_button_handlers_;
};

}} // namespace grog::ui

#endif
