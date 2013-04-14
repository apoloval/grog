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

#ifndef GROG_UI_APP_COCOA_H
#define GROG_UI_APP_COCOA_H

#include "grog/ui/app.h"
#include "grog/ui/app-sdl.h"

#if defined(__cplusplus) && !defined(__OBJC__)
namespace grog { namespace ui {

class NSAutoreleasePool;

}}
#elif defined(__OBJC__)
@class NSAutoreleasePool;
#endif


namespace grog { namespace ui {

class CocoaApplicationLoop : public ApplicationLoop {
public:

  CocoaApplicationLoop(const Ptr<ApplicationLoop>& delegate);

  virtual ~CocoaApplicationLoop();

  inline virtual void AddWorkUnit(const WorkUnit& wu) {
    delegate_->AddWorkUnit(wu);
  }

  inline virtual void RegisterMouseMotionEventHandler(
      const MouseMotionEventHandler& handler) {
    delegate_->RegisterMouseMotionEventHandler(handler);
  }

  inline virtual void RegisterMouseButtonEventHandler(
      const MouseButtonEventHandler& handler) {
    delegate_->RegisterMouseButtonEventHandler(handler);
  }

  virtual void Run();

  virtual void Stop();

private:

  Ptr<ApplicationLoop> delegate_;
  NSAutoreleasePool* pool_;
};

class CocoaSDLApplicationContextFactory : public ApplicationContextFactory {
public:

  inline CocoaSDLApplicationContextFactory() :
      sdl_factory_(new SDLApplicationContextFactory()) {}

  virtual Ptr<Canvas> CreateCanvas(
      const Application::Properties& props);

  virtual Ptr<ApplicationLoop> CreateLoop(
      const Application::Properties& props);

private:

  Ptr<SDLApplicationContextFactory> sdl_factory_;
};

}} // namespace grog::ui

#endif
