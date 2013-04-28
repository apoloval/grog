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

#include "grog/ui/color.h"

namespace grog { namespace ui {

Color Color::kBlue= { 0.0f, 0.0f, 1.0f, 1.0f };
Color Color::kGreen = { 0.0f, 1.0f, 0.0f, 1.0f };
Color Color::kLightBlue= { 0.5f, 0.5f, 1.0f, 1.0f };
Color Color::kLightGreen = { 0.5f, 1.0f, 0.5f, 1.0f };
Color Color::kLightRed = { 1.0f, 0.5f, 0.5f, 1.0f };
Color Color::kRed = { 1.0f, 0.0f, 0.0f, 1.0f };

}} // namespace grog::ui
