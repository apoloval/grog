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

#ifndef GROG_UI_EUCLIDEAN_H
#define GROG_UI_EUCLIDEAN_H

#define MAX(a, b) (a < b ? b : a)
#define MIN(a, b) (a < b ? a : b)

namespace grog { namespace ui {

template <typename T>
struct Vector2 {
  T x;
  T y;

  inline Vector2(const T& x, const T& y) : x(x), y(y) {}

  inline Vector2 operator + (const Vector2& v) const {
    return Vector2(x + v.x, y + v.y);
  }

  inline Vector2 operator - (const Vector2& v) const {
    return Vector2(x - v.x, y - v.y);
  }
};

template <typename PT, typename ST = PT>
struct Rect2 {
  PT x;
  PT y;
  ST w;
  ST h;

  inline Rect2(const PT& x, const PT& y, const ST& w, const ST& h)
    : x(x), y(y), w(w), h(h) {}

  inline Rect2(const Vector2<PT>& pos, const Vector2<ST>& size)
    : x(pos.x), y(pos.y), w(size.x), h(size.y) {}

  inline Rect2 subrectangle(const Rect2& rect) const {
    return Rect2(x + rect.x, y + rect.y, rect.w, rect.h);
  }

  inline Vector2<PT> position() const { return Vector2<PT>(x, y); }

  inline void set_position(const Vector2<PT>& pos) {
    x = pos.x;
    y = pos.y;
  }

  inline bool Wrap(const Vector2<PT>& p) const {
    return x <= p.x && p.x <= x + w && y <= p.y && p.y <= y + h;
  }
};

}} // namespace grog::ui

#undef MAX
#undef MIN

#endif // GROG_EUCLIDEAN_H
