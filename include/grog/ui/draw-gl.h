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

struct OpenGLContextParams {
  Vector2<unsigned> screen_size;
  unsigned int screen_depth;
  bool double_buffer;
};

class OpenGLContext {
public:

  inline virtual ~OpenGLContext() {}

  virtual Vector2<unsigned> size() const = 0;

  virtual void SwapBuffers() = 0;
};

class OpenGLShapeFactory : public ShapeFactory {
public:
};

class OpenGLScreen : public Screen {
public:

  OpenGLScreen(const Ptr<OpenGLContext>& ctx);

  virtual Vector2<unsigned> size() const;

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
