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

#include "grog/ui/euclidean.h"

namespace grog { namespace ui {

class Drawable {
public:

  inline virtual ~Drawable() {}

  virtual void draw() const = 0;
};

class Shape : public Drawable {
public:

  inline virtual Vector2UInt pos() const { return pos_; }

  inline void set_pos(const Vector2UInt& pos) { pos_ = pos; }

  virtual Vector2UInt size() const = 0;

protected:

  inline Shape(const Vector2UInt& pos) : pos_(pos) {}

private:

  Vector2UInt pos_;
};

class RegularSizeShape : public Shape {
public:

  inline virtual Vector2UInt size() const { return size_; }

  inline void set_size(const Vector2UInt& size) { size_ = size; }

protected:

  inline RegularSizeShape(const Vector2UInt& pos, const Vector2UInt& size) :
    Shape(pos), size_(size) {}

private:

  Vector2UInt size_;
};

class Rectangle : public RegularSizeShape {
public:

  inline Rectangle(const Vector2UInt& pos, const Vector2UInt& size) :
    RegularSizeShape(pos, size) {}

};

class ShapeFactory {
public:

  inline virtual ~ShapeFactory() {}

  virtual Rectangle* createRectangle(const Vector2UInt& pos,
                                     const Vector2UInt& size) = 0;
};

class Canvas {
public:

  virtual void clear() = 0;

  virtual ShapeFactory& shape_factory() = 0;
};

}} // namespace grog::ui

#endif // GROG_DRAW_H
