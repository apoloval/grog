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

#ifndef GROG_UI_COLOR_H
#define GROG_UI_COLOR_H

namespace grog { namespace ui {

struct Color {
  float r;
  float g;
  float b;
  float a;

  static Color kBlue;
  static Color kGreen;
  static Color kLightBlue;
  static Color kLightGreen;
  static Color kLightRed;
  static Color kRed;
};

}} // namespace grog::ui

#endif
