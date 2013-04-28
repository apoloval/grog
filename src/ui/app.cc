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

#include "grog/ui/app.h"

#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include "grog/ui/app-sdl.h"
#include "grog/ui/draw-gl.h"
#include "grog/ui/draw-sdl.h"
#include "grog/ui/event-sdl.h"
#include "grog/ui/widget.h"

#ifdef __MACOSX__
#include "grog/ui/app-cocoa.h"
#endif

namespace grog { namespace ui {

namespace {

typedef Application::PropertyName PropName;
typedef Application::PropertyName PropValue;

Application::Properties InitDefaultProperties() {
  Application::Properties props;
  props["screen-width"] = "640";
  props["screen-height"] = "480";
  props["screen-depth"] = "32";
  props["screen-double-buffer"] = "yes";
  props["app-engine"] = "sdl";
  return props;
}

} // anonymous namespace

DefaultApplicationContext::DefaultApplicationContext(
    const Ptr<ApplicationLoop>& loop, const Ptr<Screen>& screen)
  : loop_(loop), screen_(screen), post_redisplay_requested_(false) {
  loop_->RegisterMouseMotionEventHandler([this](const MouseMotionEvent& ev) {
    auto win = window();
    if (win)
      win->Respond(ev);
  });
  loop_->RegisterMouseButtonEventHandler([this](const MouseButtonEvent& ev) {
    auto win = window();
    if (win)
      win->Respond(ev);
  });
}


void DefaultApplicationContext::PostRedisplay()
{
  if (!post_redisplay_requested_) {
    post_redisplay_requested_ = true;
    loop().AddWorkUnit([this]() {
      auto& scr = screen();
      scr.Clear();
      auto win = window();
      if (win)
        win->Draw(Rect2<int>(Vector2<int>(0, 0), scr.size()));
      scr.Flush();

      post_redisplay_requested_ = false;

      // Remove work unit, will be inserted when post redisplay is requested
      return false;
    });
  }
}

AbstractApplicationContextProvider::AbstractApplicationContextProvider(
    const Ptr<ApplicationContext>& context)
  : context_(context){
  if (!context_)
    context_ = Application::instance().context();
}

const PropName Application::kPropNameScreenWidth("screen-width");
const PropName Application::kPropNameScreenHeight("screen-height");
const PropName Application::kPropNameScreenDepth("screen-depth");
const PropName Application::kPropNameScreenDoubleBuffer("screen-double-buffer");
const PropName Application::kPropNameAppEngine("app-engine");

const PropName Application::kPropValueSDLAppEngine("sdl");

const Application::Properties Application::kDefaultProperties =
    InitDefaultProperties();

Application& Application::init(const Properties& props) throw (InitError) {
  if (singleton_)
    GROG_THROW_ERROR(AlreadyInitializedError());
  singleton_ = new Application(props);
  return *singleton_;
}

Application& Application::instance() throw (UninitializedError) {
  if (!singleton_)
    GROG_THROW_ERROR(UninitializedError());
  return *singleton_;
}

Application::~Application() {
}

void Application::Run() {
  context_->loop().Run();
}

Ptr<Window> Application::NewWindow() {
  Ptr<Window> win = new Window(*this);
  context_->set_window(win);
  return win;
}

Ptr<Application> Application::singleton_(nullptr);

Application::Application(const Properties& props) throw (InitError)
  : props_(props), context_(InitContext(props)) {
}

Ptr<ApplicationContext> Application::InitContext(
    const Properties& props) throw (InitError) {
  auto prop_value = props.at(kPropNameAppEngine);
  try {
    if (prop_value == kPropValueSDLAppEngine) {
  #ifdef __MACOSX__
      return CocoaSDLApplicationContextFactory().CreateContext(props);
  #else
      return SDLApplicationContextFactory().CreateContext(props);
  #endif
    }
  } catch (util::Error& e) {
    GROG_THROW_ERROR(InitError() << util::NestedErrorInfo(e));
  }

  // Unknown app engine
  GROG_THROW_ERROR(InvalidConfigError() <<
      ActualPropertyValueInfo(prop_value) <<
      ExpectedPropertyValueInfo("[sdl]"));
}

Ptr<ApplicationContext> ApplicationContextFactory::CreateContext(
    const Application::Properties &props) {
  auto loop = CreateLoop(props);
  auto screen = CreateScreen(props);
  return new DefaultApplicationContext(loop, screen);
}

}} // namespace grog::ui
