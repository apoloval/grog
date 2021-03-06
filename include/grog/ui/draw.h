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

#ifndef GROG_UI_DRAW_H
#define GROG_UI_DRAW_H

#include "grog/ui/color.h"
#include "grog/ui/euclidean.h"
#include "grog/util/lang.h"

namespace grog { namespace ui {

class Drawable {
public:

  inline virtual ~Drawable() {}

  virtual void Draw(const Rect2<int>& screen_region) const = 0;
};

class Rectangle : public Drawable {};

class ShapeFactory : util::NonCopyable {
public:

  inline virtual ~ShapeFactory() {}

  virtual Ptr<Rectangle> CreateRectangle(const Color& color) = 0;
};

class Screen {
public:

  inline virtual ~Screen() {}

  virtual Vector2<int> size() const = 0;

  virtual void Clear() = 0;

  virtual void Flush() = 0;

  virtual ShapeFactory& shape_factory() = 0;
};

}} // namespace grog::ui

#endif // GROG_DRAW_H
