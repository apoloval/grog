/* This file is part of Grog.
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
#include "grog/util/exception.h"

#ifdef __MACOSX__
#include "grog/ui/app-cocoa.h"
#endif

namespace grog { namespace ui {

namespace {

typedef Application::PropertyName PropName;
typedef Application::PropertyName PropValue;

template <typename T>
T ParseProperty(const PropValue&);

template <>
bool ParseProperty(const PropValue& value) {
  PropValue upper_value = boost::to_upper_copy(value);
  if (upper_value == "YES" || upper_value == "TRUE" || upper_value == "1")
    return true;
  if (upper_value == "NO" || upper_value == "FALSE" || upper_value == "0")
    return false;
  throw util::InvalidInputException(
        boost::format("cannot parse %s as boolean value") % value);
}

template <>
unsigned ParseProperty(const PropValue& value) {
  try {
    return unsigned(stoi(value));
  } catch (std::exception&) {
    throw util::InvalidInputException(
          boost::format("cannot parse %s as a unsigned value") % value);
  }
}

} // anonymous namespace

const PropName Application::kPropNameScreenWidth("screen-width");
const PropName Application::kPropNameScreenHeight("screen-height");
const PropName Application::kPropNameScreenDepth("screen-depth");
const PropName Application::kPropNameScreenDoubleBuffer("screen-double-buffer");
const PropName Application::kPropNameScreenBackend("screen-backend");

const PropName Application::kPropValueSDLScreenBackend("sdl");

const Application::Properties Application::kDefaultProperties = {
  { "screen-width", "640" },
  { "screen-height", "480" },
  { "screen-depth", "32" },
  { "screen-double-buffer", "yes" },
  { "screen-backend", "sdl" },
};

Application::Application(const Properties& props) :
    props_(props), context_(new MutableApplicationContext()) {
  InitAppLoop();
  InitScreenBackend();
  InitInternalLoopWorkUnits();
}

Application::~Application() {
}

void Application::Run() {
  context_->loop().Run();
}

void Application::InitScreenBackend() {
  auto prop_value = props_.at(kPropNameScreenBackend);
  if (prop_value == kPropValueSDLScreenBackend) {
    InitSDLCanvas();
  }
}

void Application::InitSDLCanvas() {
  OpenGLContextParams params = {
    {
      ParseProperty<unsigned>(props_.at(kPropNameScreenWidth)),
      ParseProperty<unsigned>(props_.at(kPropNameScreenHeight)),
    },
    ParseProperty<unsigned>(props_.at(kPropNameScreenDepth)),
    ParseProperty<bool>(props_.at(kPropNameScreenDoubleBuffer)),
  };
  auto gl_ctx = new SDLOpenGLContext(params);
  context_->set_canvas(new OpenGLCanvas(gl_ctx));
}

void Application::InitAppLoop() {
#ifdef __MACOSX__
  context_->set_loop(new CocoaApplicationLoop(new SDLApplicationLoop()));
#endif
}

void Application::InitInternalLoopWorkUnits() {
}

bool Application::DrawFrame() {
  context_->canvas().clear();
  if (win_)
    win_->draw();
  return true;
}

}} // namespace grog::ui
