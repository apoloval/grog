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

#ifndef GROG_UI_LAYOUT_H
#define GROG_UI_LAYOUT_H

#include <list>

#include "grog/ui/app.h"
#include "grog/ui/widget.h"
#include "grog/ui/mouse.h"
#include "grog/util/lang.h"

namespace grog { namespace ui {

class Layout : public Widget, util::NonCopyable {
public:

  struct WidgetPlacement {
    typedef std::function<bool(const WidgetPlacement&)> Predicate;
    typedef std::function<void(const WidgetPlacement&)> Action;

    Ptr<Widget> widget;
    Rect2<int> location;
  };

  static WidgetPlacement::Predicate OnPosition(const Vector2<int>& pos);

  static WidgetPlacement::Predicate IsWidget(const Ptr<Widget>& widget);

  inline Layout(const Ptr<ApplicationContext>& ctx) : Widget(ctx) {}

  inline virtual ~Layout() {}

  virtual Option<WidgetPlacement> Find(
      const WidgetPlacement::Predicate& predicate) = 0;

  void Find(const WidgetPlacement::Predicate& predicate,
            const WidgetPlacement::Action& action);

  Option<Widget> FindWidget(const WidgetPlacement::Predicate& predicate);
};

class FixedLayout : public Layout, public DelegatedMouseResponder {
public:

  FixedLayout(const Ptr<ApplicationContext>& ctx = nullptr);

  virtual void Draw(const Rect2<int>& screen_region) const;

  virtual Option<WidgetPlacement> Find(
      const WidgetPlacement::Predicate& predicate);

  FixedLayout& AddWidget(const Ptr<Widget>& widget,
                         const Rect2<int>& region);

  void MoveWidget(const Ptr<Widget>& widget, const Vector2<int>& to);

  void BringToFront(const Ptr<Widget>& widget);

private:

  typedef std::list<WidgetPlacement> WidgetPlacementList;

  WidgetPlacementList children_;

  void ForWidgetOn(const Vector2<int>& pos,
                   const std::function<void(const WidgetPlacement&)>& action);

  void OnDrag(const Widget& widget,
              const Vector2<int>& pos,
              const Vector2<int>& mov);

  void OnDrop(const Widget& widget,
              const Vector2<int>& pos,
              const Vector2<int>& mov);
};

}} // namespace grog::ui

#endif
