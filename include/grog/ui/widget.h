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

#ifndef GROG_UI_WIDGET_H
#define GROG_UI_WIDGET_H

#include "grog/ui/app.h"
#include "grog/ui/draw.h"
#include "grog/ui/mouse.h"

namespace grog { namespace ui {

class Widget : public Drawable,
               public AbstractApplicationContextProvider,
               public virtual MouseResponder {
public:

  Widget(const Ptr<ApplicationContext>& app_ctx = nullptr);

  virtual Ptr<ApplicationContext> context() { return app_ctx_; }

  inline bool enabled() const { return enabled_; }

  inline void set_enabled(bool value) { enabled_ = value; }

  inline bool locked() const { return locked_; }

  inline void set_locked(bool value) { locked_ = value; }

  inline bool visible() const { return visible_; }

  inline void set_visible(bool value) { visible_ = value; }

private:

  Ptr<ApplicationContext> app_ctx_;
  bool enabled_;
  bool locked_;
  bool visible_;
};

/**
 * A composite widget which wraps another widget.
 */
class WrapperWidget : public Widget, public DelegatedMouseResponder {
public:

  inline WrapperWidget(
      const Ptr<ApplicationContext>& app_ctx) : Widget(app_ctx) {}

  virtual void Draw(const Rect2<int>& screen_region) const;

  /**
   * Set the wrapped child.
   */
  template <typename L>
  inline L& set_child(const Ptr<L>& child) {
    child_ = child;
    set_delegate(child_);
    return *child;
  }

  inline const Ptr<Widget> child() { return child_; }

private:

  Ptr<Widget> child_;
};

class Window : public WrapperWidget {
public:

  inline Window(ApplicationContextProvider& ctx_prov)
    : WrapperWidget(ctx_prov.context()) {}
};

}} // namespace grog::ui

#endif // GROG_UI_WIDGET_H
