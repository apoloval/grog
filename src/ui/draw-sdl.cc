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

#include "grog/ui/draw-sdl.h"

namespace grog { namespace ui {

SDLOpenGLContext::SDLOpenGLContext(
    const OpenGLContextParams& params) throw (SDLInitError)
  : size_(params.screen_size) {
  InitScreen(params);
}

Vector2<unsigned> SDLOpenGLContext::size() const {
  return size_;
}

SDLOpenGLContext::~SDLOpenGLContext() {
  SDL_FreeSurface(screen_);
}

void SDLOpenGLContext::SwapBuffers() {
  SDL_GL_SwapBuffers();
}

void SDLOpenGLContext::InitScreen(
    const OpenGLContextParams& params) throw (SDLInitError) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    GROG_THROW_ERROR(SDLInitError() <<
        SDLFunctionInfo("SDL_Init") << SDLErrorMessageInfo(SDL_GetError()));
  }

  Uint32 flags = SDL_OPENGL;
  if (params.double_buffer)
    flags |= SDL_DOUBLEBUF;
  screen_ = SDL_SetVideoMode(
        params.screen_size.x,
        params.screen_size.y,
        params.screen_depth,
        flags);
  if (!screen_)
    GROG_THROW_ERROR(SDLInitError() <<
        SDLFunctionInfo("SDL_SetVideoMode") <<
        SDLErrorMessageInfo(SDL_GetError()));
}

}} // namespace grog::ui

