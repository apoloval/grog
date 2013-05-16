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

#ifndef GROG_UI_APP_SDL_H
#define GROG_UI_APP_SDL_H

#include <queue>

#include "grog/ui/app.h"

extern "C" {
  union SDL_Event;
  struct SDL_MouseMotionEvent;
  struct SDL_UserEvent;
}

namespace grog { namespace ui {

class SDLApplicationLoop : public AbstractApplicationLoop {
public:

  static Ptr<ApplicationLoop> instance();

  ~SDLApplicationLoop();

  virtual void AddWorkUnit(const WorkUnit& wu);

  virtual void Run();

  virtual void Stop();

private:

  bool running_;
  std::queue<WorkUnit> work_units_;
  SDL_Event* consolidation_pool_;
  unsigned int consolidation_pool_len_;

  SDLApplicationLoop();

  void ConsolidateMouseMotion(SDL_MouseMotionEvent& ev);

  void OnUserEvent(SDL_UserEvent& ev);

  void ProcessWorkUnit();
};

class SDLApplicationContextFactory : public ApplicationContextFactory {
public:

  virtual Ptr<Screen> CreateScreen(
      const Application::Properties& props);

  virtual Ptr<ApplicationLoop> CreateLoop(
      const Application::Properties& props);
};

}} // namespace grog::ui

#endif
