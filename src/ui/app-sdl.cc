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

#include <iostream>

#include "grog/util/platform.h" // required for platform-dependent includes

#ifdef GROG_PLATFORM_IS_POSIX
  #include <SDL/SDL.h>
#else
  #include <SDL.h>
#endif

#include "grog/ui/app-sdl.h"
#include "grog/ui/draw-gl.h"
#include "grog/ui/draw-sdl.h"

namespace grog { namespace ui {

namespace {

enum SDLCustomEvent {
  kRequestRedisplayEvent,
  kWorkUnitPending,
};

MouseMotionEvent ToEvent(const SDL_MouseMotionEvent& ev) {
  return MouseMotionEvent(
        Vector2<int>(ev.x, ev.y),
        Vector2<int>(ev.xrel, ev.yrel));
}

MouseButton ToMouseButton(Uint8 btn) {
  switch (btn) {
    case SDL_BUTTON_LEFT: return kLeftMouseButton;
    case SDL_BUTTON_RIGHT: return kRightMouseButton;
    case SDL_BUTTON_MIDDLE: return kMiddleMouseButton;
    case SDL_BUTTON_WHEELUP: return kWheelUpMouseButton;
    case SDL_BUTTON_WHEELDOWN: return kWheelDownMouseButton;
    default: return kUnknownMouseButton;
  }
}

MouseButtonState ToMouseButtonState(Uint8 state) {
  switch (state) {
    case SDL_PRESSED: return kMouseButtonPressed;
    case SDL_RELEASED: return kMouseButtonReleased;
    default: return kUnknownMouseState;
  }
}

MouseButtonEvent ToEvent(const SDL_MouseButtonEvent& ev) {
  return MouseButtonEvent(
        ToMouseButton(ev.button),
        ToMouseButtonState(ev.state),
        Vector2<int>(ev.x, ev.y));
}

} // anonymous namespace

SDLApplicationLoop::~SDLApplicationLoop()
{
  if (consolidation_pool_)
    delete consolidation_pool_;
}

Ptr<ApplicationLoop> SDLApplicationLoop::instance() {
  static Ptr<ApplicationLoop> instance(new SDLApplicationLoop());
  return instance;
}

void SDLApplicationLoop::AddWorkUnit(const WorkUnit& wu) {
  work_units_.push(wu);
  SDL_Event event;
  event.type = SDL_USEREVENT;
  event.user.code = kWorkUnitPending;
  SDL_PushEvent(&event);
}

void SDLApplicationLoop::Run() {
  running_ = true;
  while (running_) {
    SDL_Event event;
    if (SDL_WaitEvent(&event)) {
      switch (event.type) {
        case SDL_MOUSEMOTION:
          ConsolidateMouseMotion(event.motion);
          HandleMouseMotionEvent(ToEvent(event.motion));
          break;
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN:
          HandleMouseButtonEvent(ToEvent(event.button));
          break;
        case SDL_QUIT:
          Stop();
          break;
        case SDL_USEREVENT:
          OnUserEvent(event.user);
          break;
      }
    }
  }
}

void SDLApplicationLoop::Stop() {
  running_ = false;
}

SDLApplicationLoop::SDLApplicationLoop()
  : running_(false), consolidation_pool_len_(1024) {
  if (!SDL_WasInit(SDL_INIT_VIDEO)) {
    SDL_Init(SDL_INIT_VIDEO);
  }
  consolidation_pool_ = new SDL_Event[1024];
}

void SDLApplicationLoop::ConsolidateMouseMotion(SDL_MouseMotionEvent &ev) {
  auto nevents = SDL_PeepEvents(
        consolidation_pool_,
        consolidation_pool_len_,
        SDL_GETEVENT,
        SDL_EVENTMASK(SDL_MOUSEMOTION));
  for (unsigned int i = 0; i < nevents; i++) {
    SDL_MouseMotionEvent& con_ev = consolidation_pool_[i].motion;
    ev.state = con_ev.state;
    ev.x = con_ev.x;
    ev.y = con_ev.y;
    ev.xrel += con_ev.xrel;
    ev.yrel += con_ev.yrel;
  }
}

void SDLApplicationLoop::OnUserEvent(SDL_UserEvent &ev) {
  switch (ev.code) {
    case kWorkUnitPending:
      ProcessWorkUnit();
  }
}

void SDLApplicationLoop::ProcessWorkUnit() {
  auto wu = work_units_.front();
  if (wu())
    AddWorkUnit(wu);
  work_units_.pop();
}

Ptr<Screen> SDLApplicationContextFactory::CreateScreen(
    const Application::Properties& props) {
  OpenGLContextParams params = {
    Vector2<int>(
      Application::ParseProperty<unsigned>(
          props.at(Application::kPropNameScreenWidth)),
      Application::ParseProperty<unsigned>(
          props.at(Application::kPropNameScreenHeight))),
    Application::ParseProperty<unsigned>(
        props.at(Application::kPropNameScreenDepth)),
    Application::ParseProperty<bool>(
        props.at(Application::kPropNameScreenDoubleBuffer)),
  };
  auto gl_ctx = new SDLOpenGLContext(params);
  return new OpenGLScreen(gl_ctx);
}

Ptr<ApplicationLoop> SDLApplicationContextFactory::CreateLoop(
    const Application::Properties &props) {
  return SDLApplicationLoop::instance();
}

}} // namespace grog::ui

