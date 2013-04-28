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

#ifndef GROG_UI_MOUSE_H
#define GROG_UI_MOUSE_H

#include <list>

#include "grog/ui/event.h"
#include "grog/util/lang.h"

namespace grog { namespace ui {

class Layout;
class Widget;

class MouseResponder {
public:

  inline virtual ~MouseResponder() {}

  virtual bool Respond(const MouseButtonEvent& ev) = 0;

  virtual bool Respond(const MouseMotionEvent& ev) = 0;
};

class DelegatedMouseResponder : public virtual MouseResponder {
public:

  inline DelegatedMouseResponder(const Ptr<MouseResponder>& delegate = nullptr)
    : delegate_(delegate) {}

  inline virtual bool Respond(const MouseButtonEvent& ev) {
    return delegate_ ? delegate_->Respond(ev) : false;
  }

  inline virtual bool Respond(const MouseMotionEvent& ev) {
    return delegate_ ? delegate_->Respond(ev) : false;
  }

  inline void set_delegate(const Ptr<MouseResponder>& delegate) {
    delegate_ = delegate;
  }

private:

  Ptr<MouseResponder> delegate_;
};

template <typename C>
class ContextAwareMouseResponder {
public:

  inline virtual ~ContextAwareMouseResponder() {}

  virtual bool Respond(C& context, const MouseButtonEvent& ev) = 0;

  virtual bool Respond(C& context, const MouseMotionEvent& ev) = 0;
};

template <typename C>
class DelegatedContextAwareMouseResponder : public virtual MouseResponder {
public:

  typedef std::function<C&(void)> ContextProvider;

  inline DelegatedContextAwareMouseResponder(
      const ContextProvider& context_provider,
      const Ptr<ContextAwareMouseResponder<C>>& delegate = nullptr)
    : context_provider_(context_provider), delegate_(delegate) {}

  inline virtual ~DelegatedContextAwareMouseResponder() {}

  inline virtual bool Respond(const MouseButtonEvent& ev) {
    auto& ctx = context_provider_();
    return delegate_ ? delegate_->Respond(ctx, ev) : false;
  }

  inline virtual bool Respond(const MouseMotionEvent& ev) {
    auto& ctx = context_provider_();
    return delegate_ ? delegate_->Respond(ctx, ev) : false;
  }

private:

  ContextProvider context_provider_;
  Ptr<ContextAwareMouseResponder<C>> delegate_;
};

struct MouseSingleClickSignal {
  Vector2<int> pos;
};

struct MousePressedSignal {
  Vector2<int> pos;
};

struct DragAndDropSignal {
  Vector2<int> from;
  Vector2<int> to;
};

/**
 * A mouse responder which doesn't respond to any event.
 */
class MouseUnresponder : public MouseResponder {
public:

  inline virtual bool Respond(const MouseButtonEvent& ev) { return false; }
  inline virtual bool Respond(const MouseMotionEvent& ev) { return false; }
};

class LayoutResponder : public ContextAwareMouseResponder<Layout> {
public:

  virtual bool Respond(Layout& layout, const MouseButtonEvent& ev);

  virtual bool Respond(Layout& layout, const MouseMotionEvent& ev);
};

class SingleClickResponder : public MouseResponder {
public:

  typedef std::function<void(const Vector2<int>&)> OnPressHandler;
  typedef std::function<void(const Vector2<int>&)> OnClickHandler;

  inline SingleClickResponder(const OnPressHandler& on_press,
                              const OnClickHandler& on_click)
    : pressed_(false), on_press_(on_press), on_click_(on_click) {}

  virtual bool Respond(const MouseButtonEvent& ev);

  virtual bool Respond(const MouseMotionEvent& ev);

private:

  bool pressed_;
  OnPressHandler on_press_;
  OnClickHandler on_click_;
};

class DragAndDropResponder : public ContextAwareMouseResponder<Layout> {
public:

  typedef std::function<void(const Widget& widget,
                             const Vector2<int>& pos,
                             const Vector2<int>& mov)> OnDragHandler;

  typedef std::function<void(const Widget& widget,
                             const Vector2<int>& from,
                             const Vector2<int>& to)> OnDropHandler;

  inline DragAndDropResponder(
      const OnDragHandler& on_drag,
      const OnDropHandler& on_drop)
    : on_drag_(on_drag), on_drop_(on_drop), draging_(Option<Widget>::None()),
      draging_from_(0, 0) {
  }

  virtual bool Respond(Layout& layout, const MouseButtonEvent& ev);

  virtual bool Respond(Layout& layout, const MouseMotionEvent& ev);

private:

  OnDragHandler on_drag_;
  OnDropHandler on_drop_;
  Option<Widget> draging_;
  Vector2<int> draging_from_;
};

class MouseResponderChain : public MouseResponder {
public:

  GROG_BEGIN_BUILDER(MouseResponderChain)
  GROG_BUILDER_WITH(With, Ptr<MouseResponder>, AddResponder)
  GROG_END_BUILDER()

  inline void AddResponder(const Ptr<MouseResponder>& responder) {
    responders_.push_back(responder);
  }

  virtual bool Respond(const MouseButtonEvent& ev);

  virtual bool Respond(const MouseMotionEvent& ev);

private:

  std::list<Ptr<MouseResponder> > responders_;
};

}} // namespace grog::ui

#endif
