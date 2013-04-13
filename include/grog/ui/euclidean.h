/* This file is part of Grog.
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

namespace grog { namespace ui {

template<typename T>
struct Vector2 {
  T x;
  T y;
};

typedef Vector2<int> Vector2Int;
typedef Vector2<unsigned int> Vector2UInt;
typedef Vector2<float> Vector2Float;
typedef Vector2<double> Vector2Double;

}} // namespace grog::ui

#endif // GROG_EUCLIDEAN_H
