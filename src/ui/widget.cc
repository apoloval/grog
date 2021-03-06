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

#include "grog/ui/widget.h"

namespace grog { namespace ui {

Widget::Widget(const Ptr<ApplicationContext>& app_ctx)
  : app_ctx_(app_ctx), enabled_(true), locked_(false), visible_(true) {
  if (!app_ctx_)
    app_ctx_ = Application::instance().context();
}

void WrapperWidget::Draw(const Rect2<int>& screen_region) const {
  if (child_)
    child_->Draw(screen_region);
}

}} // namespace grog::ui
