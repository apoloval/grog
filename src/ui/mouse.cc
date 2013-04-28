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

#include "grog/ui/layout.h"
#include "grog/ui/mouse.h"
#include "grog/ui/widget.h"

namespace grog { namespace ui {

bool LayoutResponder::Respond(Layout& layout, const MouseButtonEvent &ev) {
  auto child = layout.Find(Layout::OnPosition(ev.pos));
  if (child) {
    auto& c = child.value();
    MouseButtonEvent new_ev(ev.button, ev.state, ev.pos - c.location.position());
    return c.widget->Respond(new_ev);
  }
  return false;
}

bool LayoutResponder::Respond(Layout& layout, const MouseMotionEvent &ev) {
  auto child = layout.Find(Layout::OnPosition(ev.abs_pos));
  if (child) {
    auto& c = child.value();
    MouseMotionEvent new_ev(ev.abs_pos - c.location.position(), ev.rel_pos);
    return c.widget->Respond(new_ev);
  }
  return false;
}

bool SingleClickResponder::Respond(const MouseButtonEvent &ev) {
  if (ev.button == kLeftMouseButton) {
    if (ev.state == kMouseButtonPressed) {
      pressed_ = true;
      on_press_(ev.pos);
      return true;
    } else if (pressed_) {
      pressed_ = false;
      on_click_(ev.pos);
      return true;
    }
  }
  pressed_ = false;
  return false;
}

bool SingleClickResponder::Respond(const MouseMotionEvent &ev) {
  return false;
}

bool DragAndDropResponder::Respond(Layout& layout, const MouseButtonEvent &ev) {
  if (ev.button != kLeftMouseButton)
    return false;
  if (ev.state == kMouseButtonPressed) {
    draging_ = layout.FindWidget(Layout::OnPosition(ev.pos));
    return draging_.valid();
  } else { // button released
    if (draging_.valid()) {
      on_drop_(draging_.value(), draging_from_, ev.pos);
    }
    draging_ = Option<Widget>::None();
    return true;
  }
}

bool DragAndDropResponder::Respond(Layout& layout, const MouseMotionEvent &ev) {
  if (draging_) {
    on_drag_(draging_.value(), ev.abs_pos - ev.rel_pos, ev.rel_pos);
    return true;
  }
  return false;
}

bool MouseResponderChain::Respond(const MouseButtonEvent &ev) {
  for (auto& responder : responders_) {
    if (responder->Respond(ev))
      return true;
  }
  return false;
}

bool MouseResponderChain::Respond(const MouseMotionEvent &ev) {
  for (auto& responder : responders_) {
    if (responder->Respond(ev))
      return true;
  }
  return false;
}

}} // namespace grog::ui
