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

#include <boost/range/adaptors.hpp>

#include "grog/ui/layout.h"
#include "grog/ui/widget.h"

using namespace std::placeholders;

namespace grog { namespace ui {

Layout::WidgetPlacement::Predicate Layout::OnPosition(const Vector2<int>& pos) {
  return [pos](const WidgetPlacement& child) -> bool {
    return child.location.Wrap(pos);
  };
}

Layout::WidgetPlacement::Predicate Layout::IsWidget(const Ptr<Widget>& widget) {
  return [widget](const WidgetPlacement& child) -> bool {
    return child.widget == widget;
  };
}

void Layout::Find(const WidgetPlacement::Predicate& predicate,
                  const WidgetPlacement::Action& action) {
  auto child = Find(predicate);
  if (child) {
    action(child.value());
  }
}


Option<Widget> Layout::FindWidget(const WidgetPlacement::Predicate& predicate) {
  auto child = Find(predicate);
  return child ?
        Option<Widget>::Some(child.value().widget) : Option<Widget>::None();
}

FixedLayout::FixedLayout(const Ptr<ApplicationContext>& ctx)
  : Layout(ctx), DelegatedMouseResponder(MouseResponderChain::Builder()
      .With(new DelegatedContextAwareMouseResponder<Layout>(
              [this]() -> Layout& { return *this; },
              new LayoutResponder()))
      .With(new DelegatedContextAwareMouseResponder<Layout>(
              [this]() -> Layout& { return *this; },
              new DragAndDropResponder(
                  std::bind(&FixedLayout::OnDrag, this, _1, _2, _3),
                  std::bind(&FixedLayout::OnDrop, this, _1, _2, _3))))
      .Build()) {}


void FixedLayout::Draw(const Rect2<int>& screen_region) const {
  for (auto& child : boost::adaptors::reverse(children_)) {
    child.widget->Draw(screen_region.subrectangle(child.location));
  }
}

Option<Layout::WidgetPlacement> FixedLayout::Find(
    const WidgetPlacement::Predicate& predicate) {
  for (auto& child : children_) {
    if (predicate(child))
      return Option<WidgetPlacement>::Some(new WidgetPlacement(child));
  }
  return Option<WidgetPlacement>::None();
}

FixedLayout& FixedLayout::AddWidget(const Ptr<Widget>& widget,
                                    const Rect2<int>& region) {
  WidgetPlacement p = { widget, region };
  children_.push_back(p);
  PostRedisplay();
  return *this;
}

void FixedLayout::MoveWidget(const Ptr<Widget>& widget,
                             const Vector2<int>& to) {
  for (auto& child : children_) {
    if (child.widget == widget) {
      child.location.set_position(to);
      PostRedisplay();
    }
  }
}

void FixedLayout::BringToFront(const Ptr<Widget>& widget) {
  for (auto child : children_) {
    if (child.widget == widget) {
      children_.remove_if([&widget](
          const WidgetPlacement& e) { return e.widget == widget; });
      children_.push_front(child);
      PostRedisplay();
    }
  }
}

void FixedLayout::OnDrag(const Widget& widget,
                         const Vector2<int>& pos,
                         const Vector2<int>& mov) {  
  Layout::Find(OnPosition(pos), [this, &mov](const WidgetPlacement& child) {
    if (!child.widget->locked()) {
      MoveWidget(child.widget, child.location.position() + mov);
      BringToFront(child.widget);
    }
  });
}

void FixedLayout::OnDrop(const Widget& widget,
                         const Vector2<int>& from,
                         const Vector2<int>& to) {
  // Move is done on drag, just ignore
}

}} // namespace grog::ui
