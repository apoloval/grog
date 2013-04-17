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

#include "grog/util/platform.h"

#ifdef GROG_PLATFORM_IS_WINDOWS
  #include <Windows.h>
#endif

#include "grog/ui/app.h"

namespace {

int RunMain(const GrogMainArgs& args) {
  try {
    GrogMain(args);
    return 0;
  } catch (::grog::util::Error& e) {
    std::cerr << "Grog application execution failed." << std::endl;
    std::cerr << e.what()<< std::endl;
    return -1;
  }
}

} // anonymous namespace

#ifdef GROG_PLATFORM_IS_WINDOWS
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw) {
  int argc;
  LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

  GrogMainArgs args;
  for (int i = 0; i < argc; i++) {
    args.push_back(argv[i]);
  }

  return RunMain(args);
}
#else
int main(int argc, char* argv[]) {
  GrogMainArgs args;
  for (int i = 0; i < argc; i++) {
    std::string arg(argv[i]);
    std::wstring warg(arg.begin(), arg.end());
    args.push_back(warg);
  }
  return RunMain(args);
}
#endif
