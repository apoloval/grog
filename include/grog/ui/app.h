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
#include "grog/util/error.h"
#include "grog/util/lang.h"

/**
 * The type of command line arguments for a Grog application.
 */
typedef std::vector<std::wstring> GrogMainArgs;

/**
 * The entry point for a Grog application. This must be implemented by
 * client code, and will be invoked by Grog to initialize the app.
 */
void GrogMain(const GrogMainArgs& args) throw (::grog::util::Error);

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

class Window;

class ApplicationContext {
public:

  inline virtual ~ApplicationContext() {}

  virtual ApplicationLoop& loop() = 0;

  virtual Screen& screen() = 0;

  virtual const Ptr<Window>& window() = 0;

  virtual void set_window(const Ptr<Window>& window) = 0;

  virtual void PostRedisplay() = 0;
};

class DefaultApplicationContext : public ApplicationContext {
public:

  DefaultApplicationContext(const Ptr<ApplicationLoop>& loop,
                            const Ptr<Screen>& screen);

  inline ApplicationLoop& loop() { return *loop_; }

  inline Screen& screen() { return *screen_; }

  inline virtual const Ptr<Window>& window() { return window_; }

  inline virtual void set_window(const Ptr<Window>& window) {
    window_ = window;
    PostRedisplay();
  }

  virtual void PostRedisplay();

private:

  Ptr<ApplicationLoop> loop_;
  Ptr<Screen> screen_;
  Ptr<Window> window_;
  bool post_redisplay_requested_;

  inline DefaultApplicationContext(const DefaultApplicationContext&) {}

};

class ApplicationContextProvider {
public:

  inline virtual ~ApplicationContextProvider() {}

  virtual Ptr<ApplicationContext> context() = 0;
};

class AbstractApplicationContextProvider : public ApplicationContextProvider {
public:

  /**
   * Initialize with given application context. If null, it will
   * attempt to obtain the context from the application singleton (which
   * requires the app to be initialized).
   */
  AbstractApplicationContextProvider(
      const Ptr<ApplicationContext>& context = nullptr);

  inline virtual Ptr<ApplicationContext> context() { return context_; }

  /**
   * Convenience function to obtain the shape factory from application context.
   */
  ShapeFactory& shape_factory() { return context()->screen().shape_factory(); }

  /**
   * Convenience function to post redisplay using the application context.
   */
  inline void PostRedisplay() { context()->PostRedisplay(); }

private:

  Ptr<ApplicationContext> context_;
};

class Application : public ApplicationContextProvider, util::NonCopyable {
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

  GROG_DECL_ERROR(InitError, util::InvalidInputError);

  /**
   * An error while parsing a property, including:
   *  - ActualPropertyValueInfo, indicating the value of the property
   *  - ExpectedPropertyTypeInfo, indicating the expected type of the property
   */
  GROG_DECL_ERROR(PropertyParseError, InitError);

  /**
   * An error caused by an invalid configuration, including:
   *  - ActualPropertyValueInfo, indicating the actual value of the property
   *  - ExpectedPropertyValueInfo, indicating the expected value of the property
   */
  GROG_DECL_ERROR(InvalidConfigError, InitError);

  /**
   * An error caused by an attempt to initialize an already
   * initialized application.
   */
  GROG_DECL_ERROR(AlreadyInitializedError, InitError);

  /**
   * An error caused by an attempt to obtain application object before its
   * initialization.
   */
  GROG_DECL_ERROR(UninitializedError, util::IllegalStateError);

  GROG_DECL_ERROR_INFO(ActualPropertyValueInfo, PropertyValue);
  GROG_DECL_ERROR_INFO(ExpectedPropertyValueInfo, std::string);
  GROG_DECL_ERROR_INFO(ExpectedPropertyTypeInfo, std::string);

  /**
   * Parse the given property value according to type T.
   */
  template <typename T>
  static T ParseProperty(const PropertyValue&) throw (PropertyParseError);

  /**
   * Initialize the application from given properties.
   */
  static Application& init(
      const Properties& props = kDefaultProperties) throw (InitError);

  /**
   * Obtain the application object, or throw UninitializedError if it wasn't
   * actually initialized.
   */
  static Application& instance() throw (UninitializedError);

  virtual ~Application();

  inline virtual Ptr<ApplicationContext> context() {
    return std::shared_ptr<ApplicationContext>(context_);
  }

  void Run();

  Ptr<Window> NewWindow();

private:

  static Ptr<Application> singleton_;

  Properties props_;
  Ptr<ApplicationContext> context_;

  Application(const Properties& props) throw (InitError);

  static Ptr<ApplicationContext> InitContext(
      const Properties& props) throw (InitError);
};

template <>
inline bool Application::ParseProperty(
      const Application::PropertyValue& value) throw (PropertyParseError) {
  Application::PropertyValue upper_value = boost::to_upper_copy(value);
  if (upper_value == "YES" || upper_value == "TRUE" || upper_value == "1")
    return true;
  if (upper_value == "NO" || upper_value == "FALSE" || upper_value == "0")
    return false;
  GROG_THROW_ERROR(PropertyParseError() <<
      ActualPropertyValueInfo(value) << ExpectedPropertyTypeInfo("bool"));
}

template <>
inline unsigned Application::ParseProperty(
      const Application::PropertyValue& value) throw (PropertyParseError) {
  try {
    return unsigned(stoi(value));
  } catch (std::exception&) {
    GROG_THROW_ERROR(PropertyParseError() <<
        ActualPropertyValueInfo(value) << ExpectedPropertyTypeInfo("uint"));
  }
}

class ApplicationContextFactory {
public:

  inline virtual ~ApplicationContextFactory() {}

  virtual Ptr<ApplicationContext> CreateContext(
      const Application::Properties& props);

  virtual Ptr<Screen> CreateScreen(
      const Application::Properties& props) = 0;

  virtual Ptr<ApplicationLoop> CreateLoop(
      const Application::Properties& props) = 0;
};

}} // namespace grog::ui

#endif // GROG_UI_APP_H
