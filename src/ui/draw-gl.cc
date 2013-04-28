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
  #include <GL/gl.h>
#else
  #include <GL/gl.h>
#endif

namespace grog { namespace ui {

void OpenGLRectangle::Draw(const Rect2<int> &screen_region) const {
  glColor4f(color_.r, color_.g, color_.b, color_.a);
  glBegin(GL_QUADS);
    glVertex2f(screen_region.x,
               screen_region.y);
    glVertex2f(screen_region.x + screen_region.w,
               screen_region.y);
    glVertex2f(screen_region.x + screen_region.w,
               screen_region.y + screen_region.h);
    glVertex2f(screen_region.x,
               screen_region.y + screen_region.h);
  glEnd();
}

OpenGLScreen::OpenGLScreen(const Ptr<OpenGLContext>& ctx)
  : ctx_(ctx), shape_factory_(new OpenGLShapeFactory()) {
  Init2DState();
}

Vector2<int> OpenGLScreen::size() const {
  return ctx_->size();
}

void OpenGLScreen::Clear() {
  glClearColor(0.0f, 0.0f, 0.0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLScreen::Flush() {
  glFlush();
  ctx_->SwapBuffers();
}

void OpenGLScreen::Init2DState() {
  // Set blending for considering alpha channel
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthFunc(GL_LEQUAL);

  // Set projection properties
  auto screen_size = ctx_->size();
  glViewport(0, 0, screen_size.x, screen_size.y);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0,
         GLdouble(screen_size.x),
         GLdouble(screen_size.y),
         0.0,
         0.0,
         1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

}} // namespace grog::ui
