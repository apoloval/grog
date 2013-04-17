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

#include "grog/ui/draw.h"
#include "grog/util/lang.h"

namespace grog { namespace ui {

class OpenGLRectangle : public Rectangle {
public:

  inline OpenGLRectangle(const Vector2UInt& pos, const Vector2UInt& size) :
    Rectangle(pos, size) {}

  virtual void draw() const;
};

struct OpenGLContextParams {
  Vector2UInt screen_size;
  unsigned int screen_depth;
  bool double_buffer;
};

class OpenGLContext {
public:

  inline virtual ~OpenGLContext() {}

  virtual void SwapBuffers() = 0;
};

class OpenGLShapeFactory : public ShapeFactory {
public:

  inline virtual OpenGLRectangle* createRectangle(
      const Vector2UInt& pos, const Vector2UInt& size) {
    return new OpenGLRectangle(pos, size);
  }
};

class OpenGLCanvas : public Canvas {
public:

  OpenGLCanvas(const Ptr<OpenGLContext>& ctx);

  virtual void clear();

  virtual OpenGLShapeFactory& shape_factory() {
    return *shape_factory_;
  }

private:

  Ptr<OpenGLContext> ctx_;
  Ptr<OpenGLShapeFactory> shape_factory_;
};

}} // namespace grog::ui

#endif // GROG_DRAW_GL_H
