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

#ifndef GROG_UI_DRAW_GL_H
#define GROG_UI_DRAW_GL_H

#include <functional>

#include "grog/ui/color.h"
#include "grog/ui/draw.h"
#include "grog/util/lang.h"

namespace grog { namespace ui {

struct OpenGLContextParams {
  Vector2<int> screen_size;
  unsigned int screen_depth;
  bool double_buffer;
};

class OpenGLContext {
public:

  inline virtual ~OpenGLContext() {}

  virtual Vector2<int> size() const = 0;

  virtual void SwapBuffers() = 0;
};

class OpenGLRectangle : public Rectangle {
public:

  inline OpenGLRectangle(const Color& color) : color_(color) {}

  virtual void Draw(const Rect2<int>& screen_region) const;

private:

  Color color_;
};

class OpenGLShapeFactory : public ShapeFactory {
public:

  inline virtual Ptr<Rectangle> CreateRectangle(const Color& color) {
    return new OpenGLRectangle(color);
  }
};

class OpenGLScreen : public Screen {
public:

  OpenGLScreen(const Ptr<OpenGLContext>& ctx);

  virtual Vector2<int> size() const;

  virtual void Clear();

  virtual void Flush();

  virtual OpenGLShapeFactory& shape_factory() {
    return *shape_factory_;
  }

private:

  Ptr<OpenGLContext> ctx_;
  Ptr<OpenGLShapeFactory> shape_factory_;

  void Init2DState();
};

}} // namespace grog::ui

#endif // GROG_DRAW_GL_H
