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

#include "grog/ui/draw-gl.h"

#include "grog/util/platform.h" // required for platform-dependent includes

#if GROG_PLATFORM == GROG_PLATFORM_OSX
  #include <OpenGL/gl.h>
#elif GROG_PLATFORM == GROG_PLATFORM_WINDOWS
  #include <Windows.h>
  #include <gl/GL.h>
#else
  #include <gl/GL.h>
#endif

namespace grog { namespace ui {

void OpenGLRectangle::draw() const {
  // TODO: implement this
}

OpenGLCanvas::OpenGLCanvas(const Ptr<OpenGLContext>& ctx)
  : ctx_(ctx), shape_factory_(new OpenGLShapeFactory()) {
}

void OpenGLCanvas::clear() {
  glClearColor(0.0f, 0.0f, 0.0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ctx_->SwapBuffers();
}

}} // namespace grog::ui
