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

const PropName Application::kPropNameScreenWidth("screen-width");
const PropName Application::kPropNameScreenHeight("screen-height");
const PropName Application::kPropNameScreenDepth("screen-depth");
const PropName Application::kPropNameScreenDoubleBuffer("screen-double-buffer");
const PropName Application::kPropNameAppEngine("app-engine");

const PropName Application::kPropValueSDLAppEngine("sdl");

const Application::Properties Application::kDefaultProperties =
    InitDefaultProperties();

Application::Application(const Properties& props) :
    props_(props), context_(InitContext(props)) {}

Application::~Application() {
}

void Application::Run() {
  context_->loop().Run();
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
    THROW_ERROR(InitError() << util::NestedErrorInfo(e));
  }

  // Unknown app engine
  THROW_ERROR(InvalidConfigError() <<
      ActualPropertyValueInfo(prop_value) <<
      ExpectedPropertyValueInfo("[sdl]"));
}

bool Application::DrawFrame() {
  context_->canvas().clear();
  if (win_)
    win_->draw();
  return true;
}

Ptr<ApplicationContext> ApplicationContextFactory::CreateContext(
    const Application::Properties &props) {
  MutableApplicationContext* ctx = new MutableApplicationContext();
  ctx->set_loop(CreateLoop(props));
  ctx->set_canvas(CreateCanvas(props));
  return ctx;
}

}} // namespace grog::ui
