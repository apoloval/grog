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

#include <iostream>

#include "grog/ui/app-cocoa.h"

#define Ptr CocoaPtr
#import <Cocoa/Cocoa.h>
#undef Ptr

@interface CocoaAppDelegate : NSObject {
}
@end

@implementation CocoaAppDelegate

- (id) init {
  self = [super init];
  return self;
}

- (void) applicationDidFinishLaunching: (NSNotification*) note {
  /*
   * When Cocoa app finishes launching, we just stop its main loop. That
   * returns the control to the caller (CocoaApplicationLoop constructor)
   * with the Cocoa App initialized, so a custom event loop (e.g., SDL-based)
   * is fully ready to work.
   */
  [NSApp stop: self];
}

@end

namespace grog { namespace ui {

CocoaApplicationLoop::CocoaApplicationLoop(const Ptr<ApplicationLoop>& delegate)
    : delegate_(delegate), pool_([[NSAutoreleasePool alloc] init]) {
  pool_ = [[NSAutoreleasePool alloc] init];

  [NSApplication sharedApplication];

  CocoaAppDelegate *cocoa_delegate= [[CocoaAppDelegate alloc] init];
  [NSApp setDelegate:cocoa_delegate];
  [NSApp run];
}

CocoaApplicationLoop::~CocoaApplicationLoop()
{
  [pool_ release];
}

void CocoaApplicationLoop::Run() {
  delegate_->Run();
}

void CocoaApplicationLoop::Stop() {
  delegate_->Stop();
}

Ptr<Screen> CocoaSDLApplicationContextFactory::CreateScreen(
    const Application::Properties &props) {
  return sdl_factory_->CreateScreen(props);
}

Ptr<ApplicationLoop> CocoaSDLApplicationContextFactory::CreateLoop(
    const Application::Properties &props) {
  return new CocoaApplicationLoop(sdl_factory_->CreateLoop(props));
}

}} // namespace grog::ui
