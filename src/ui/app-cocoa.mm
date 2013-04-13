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
  grog::ui::ApplicationLoop* loop_;
}
@end

@implementation CocoaAppDelegate

- (id) initWithDelegateLoop: (grog::ui::ApplicationLoop*) loop {
  if (self = [super init])
    loop_ = loop;
  return self;
}

- (void) applicationDidFinishLaunching: (NSNotification*) note {
  loop_->Run();
  [[NSApplication sharedApplication] stop: self];
}

- (void) terminate {
  loop_->Stop();
}

@end

namespace grog { namespace ui {

CocoaApplicationLoop::CocoaApplicationLoop(const Ptr<ApplicationLoop>& delegate)
    : delegate_(delegate), pool_([[NSAutoreleasePool alloc] init]) {
  pool_ = [[NSAutoreleasePool alloc] init];

  [NSApplication sharedApplication];

  CocoaAppDelegate *cocoa_delegate=
      [[CocoaAppDelegate alloc] initWithDelegateLoop: delegate_.get()];
  [NSApp setDelegate:cocoa_delegate];
}

CocoaApplicationLoop::~CocoaApplicationLoop()
{
  [pool_ release];
}

void CocoaApplicationLoop::Run() {
  [NSApp run];
}

void CocoaApplicationLoop::Stop() {
  delegate_->Stop();
}

}} // namespace grog::ui
