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

#ifndef GROG_UI_APP_H
#define GROG_UI_APP_H

#include <functional>
#include <list>
#include <map>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include "grog/ui/draw.h"
#include "grog/ui/event.h"
#include "grog/util/exception.h"
#include "grog/util/lang.h"

namespace grog { namespace ui {

/**
 * Application loop. This class provides an interface for an application
 * loop. It aggregates a sequence of work units that are executed on each
 * loop iteration. Some of them are added by the application itself to
 * manage the event processing and the frame drawing, but some others
 * may be added by widgets or any other object close to the application
 * context.
 */
class ApplicationLoop : public EventProducer {
public:

  /**
   * A work unit for the application loop to execute. It is defined as a
   * function that takes no arguments and returns a boolean value, which
   * indicates whether the work unit execution should be queued again and
   * be executed in the next iteration (true) or it must be removed (false).
   */
  typedef std::function<bool(void)> WorkUnit;

  /**
   * Create a new application loop with an empty work unit list.
   */
  inline virtual ~ApplicationLoop() {}
  /**
   * Add a new work unit for the loop to execute.
   * @param wu the working unit to execute
   */
  virtual void AddWorkUnit(const WorkUnit& wu) = 0;

  /**
   * Run until the loop is requested to stop via stop() method.
   */
  virtual void Run() = 0;

  /**
   * Request the loop to stop if it is already running via run().
   */
  virtual void Stop() = 0;  
};

class AbstractApplicationLoop : public ApplicationLoop {
public:

  inline virtual void RegisterMouseMotionEventHandler(
      const MouseMotionEventHandler& handler) {
    mouse_motion_handlers_.push_back(handler);
  }

  inline virtual void RegisterMouseButtonEventHandler(
      const MouseButtonEventHandler& handler) {
    mouse_button_handlers_.push_back(handler);
  }

protected:

  void HandleMouseMotionEvent(const MouseMotionEvent& event) {
    for (auto& handler : mouse_motion_handlers_)
      handler(event);
  }

  void HandleMouseButtonEvent(const MouseButtonEvent& event) {
    for (auto& handler : mouse_button_handlers_)
      handler(event);
  }

private:

  std::list<MouseMotionEventHandler> mouse_motion_handlers_;
  std::list<MouseButtonEventHandler> mouse_button_handlers_;
};

class ApplicationContext {
public:

  inline virtual ~ApplicationContext() {}

  virtual ApplicationLoop& loop() = 0;

  virtual Canvas& canvas() = 0;
};

class MutableApplicationContext : public ApplicationContext {
public:

  inline MutableApplicationContext() {}

  MutableApplicationContext(const MutableApplicationContext&) = delete;

  inline ApplicationLoop& loop() { return *loop_; }

  inline bool has_loop() const { return bool(loop_); }

  inline void set_loop(const Ptr<ApplicationLoop>& loop) { loop_ = loop; }

  inline Canvas& canvas() { return *canvas_; }

  inline bool has_canvas() const { return bool(canvas_); }

  inline void set_canvas(const Ptr<Canvas>& canvas) { canvas_ = canvas; }

private:

  Ptr<ApplicationLoop> loop_;
  Ptr<Canvas> canvas_;
};

class ApplicationContextProvider {
public:

  inline virtual ~ApplicationContextProvider() {}

  virtual Ptr<ApplicationContext> context() = 0;
};

class Window;

class Application : public ApplicationContextProvider {
public:

  /**
   * The type of the name of application properties.
   */
  typedef std::string PropertyName;

  /**
   * The type of the value of application properties.
   */
  typedef std::string PropertyValue;

  /**
   * Application properties in form of a name-value map.
   */
  typedef std::map<PropertyName, PropertyValue> Properties;

  /**
   * The default properties of an application.
   */
  static const Properties kDefaultProperties;

  /**
   * The property name for screen width.
   */
  static const PropertyName kPropNameScreenWidth;

  /**
   * The property name for screen height.
   */
  static const PropertyName kPropNameScreenHeight;

  /**
   * The property name for screen depth.
   */
  static const PropertyName kPropNameScreenDepth;

  /**
   * The property name for screen double buffer.
   */
  static const PropertyName kPropNameScreenDoubleBuffer;

  /**
   * The property name for application engine
   */
  static const PropertyName kPropNameAppEngine;

  /**
   * The property value for SDL application engine
   */
  static const PropertyValue kPropValueSDLAppEngine;

  /**
   * Parse the given property value according to type T.
   */
  template <typename T>
  static T ParseProperty(const PropertyValue&);

  /**
   * Create a new application from given properties. It may throw a
   * InvalidInputException if given properties are incorrect.
   */
  Application(const Properties& props = kDefaultProperties);

  virtual ~Application();

  inline virtual Ptr<ApplicationContext> context() {
    return std::shared_ptr<ApplicationContext>(context_);
  }

  void Run();

private:

  Properties props_;
  Ptr<ApplicationContext> context_;
  Ptr<Window> win_;

  static Ptr<ApplicationContext> InitContext(const Properties& props);

  bool DrawFrame();
};

template <>
inline bool Application::ParseProperty(
      const Application::PropertyValue& value) {
  Application::PropertyValue upper_value = boost::to_upper_copy(value);
  if (upper_value == "YES" || upper_value == "TRUE" || upper_value == "1")
    return true;
  if (upper_value == "NO" || upper_value == "FALSE" || upper_value == "0")
    return false;
  throw util::InvalidInputException(
        boost::format("cannot parse %s as boolean value") % value);
}

template <>
inline unsigned Application::ParseProperty(
      const Application::PropertyValue& value) {
  try {
    return unsigned(stoi(value));
  } catch (std::exception&) {
    throw util::InvalidInputException(
          boost::format("cannot parse %s as a unsigned value") % value);
  }
}

class ApplicationContextFactory {
public:

  inline virtual ~ApplicationContextFactory() {}

  virtual Ptr<ApplicationContext> CreateContext(
      const Application::Properties& props);

  virtual Ptr<Canvas> CreateCanvas(
      const Application::Properties& props) = 0;

  virtual Ptr<ApplicationLoop> CreateLoop(
      const Application::Properties& props) = 0;
};



}} // namespace grog::ui

#endif // GROG_UI_APP_H