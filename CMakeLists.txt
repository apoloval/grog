# This file is part of Grog.
#
# Grog is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Grog is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)

project(Grog)

find_package(SDL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread date_time chrono regex)
find_package(OpenGL REQUIRED)

set(libgrog_HEADERS
  include/grog/ui/app.h
  include/grog/ui/app-sdl.h
  include/grog/ui/color.h
  include/grog/ui/draw.h
  include/grog/ui/draw-gl.h
  include/grog/ui/draw-sdl.h
  include/grog/ui/euclidean.h
  include/grog/ui/event.h
  include/grog/ui/layout.h
  include/grog/ui/mouse.h
  include/grog/ui/widget.h
  include/grog/util/error.h
  include/grog/util/lang.h
  include/grog/util/platform.h
)

set(libgrog_SOURCES
  src/ui/app.cc
  src/ui/app-sdl.cc
  src/ui/color.cc
  src/ui/draw.cc
  src/ui/draw-gl.cc
  src/ui/draw-sdl.cc
  src/ui/event.cc
  src/ui/layout.cc
  src/ui/main.cc
  src/ui/mouse.cc
  src/ui/widget.cc
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  list(APPEND libgrog_HEADERS include/grog/ui/app-cocoa.h)
  list(APPEND libgrog_SOURCES src/ui/app-cocoa.mm)
endif()

include_directories(include
  ${Boost_INCLUDE_DIR}
  ${SDL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "-std=c++11")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "/EHsc")
endif()

add_library(libgrog ${libgrog_SOURCES} ${libgrog_HEADERS})

add_executable(test-draw WIN32 MACOSX_BUNDLE test/ui/draw.cc)
target_link_libraries(test-draw libgrog
  ${SDL_LIBRARY}
  # ${SDLMAIN_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${Boost_LIBRARIES})
