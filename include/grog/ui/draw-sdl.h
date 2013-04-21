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

#ifndef GROG_UI_DRAW_SDL_H
#define GROG_UI_DRAW_SDL_H

#include "grog/util/platform.h" // required for platform-dependent includes

#ifdef GROG_PLATFORM_IS_POSIX
  #include <SDL/SDL.h>
#else // Windows
  #include <SDL.h>
#endif

#include "grog/ui/draw-gl.h"
#include "grog/ui/euclidean.h"
#include "grog/util/error.h"

#ifdef __MACOSX__
  #include "grog/ui/cocoa.h"
#endif

namespace grog { namespace ui {

class SDLOpenGLContext final : public OpenGLContext {
public:

  /**
   * A SDL initialization error, including
   *  - SDLFunctionInfo, indicating the SDL function which invocation failed
   *  - SDLErrorMessageInfo, indicating the error message provided by SDL
   */
  DECL_RUNTIME_ERROR(SDLInitError);

  DECL_ERROR_INFO(SDLFunctionInfo, std::string);
  DECL_ERROR_INFO(SDLErrorMessageInfo, std::string);

  SDLOpenGLContext(const OpenGLContextParams& params) throw (SDLInitError);

  virtual Vector2<unsigned> size() const;

  virtual ~SDLOpenGLContext();

  virtual void SwapBuffers();

private:

  SDL_Surface* screen_;
  Vector2<unsigned> size_;

  void InitScreen(const OpenGLContextParams& params) throw (SDLInitError);
};

}} // namespace grog::ui

#endif // GROG_DRAW_SDL_H
