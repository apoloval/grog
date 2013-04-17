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

namespace grog { namespace ui {

class Widget : public Drawable, public ApplicationContextProvider {
public:

  inline Widget(const Ptr<ApplicationContext>& app_ctx) : app_ctx_(app_ctx) {}

  virtual Ptr<ApplicationContext> app_ctx() { return app_ctx_; }

private:

  Ptr<ApplicationContext> app_ctx_;
};

class Window : public Widget {
public:

  inline Window(ApplicationContextProvider& app_ctx_prov) :
    Widget(app_ctx_prov.context()) {}

  virtual void draw();
};

}} // namespace grog::ui

#endif // GROG_UI_WIDGET_H
