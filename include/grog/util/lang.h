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

#ifndef GROG_UTIL_LANG_H
#define GROG_UTIL_LANG_H

#include <memory>

namespace grog { namespace util {

template <typename T>
class Ptr : public std::shared_ptr<T> {
public:

  inline Ptr() : std::shared_ptr<T>() {}
  inline Ptr(const Ptr& ptr) : std::shared_ptr<T>(ptr) {}
  inline Ptr(const std::shared_ptr<T>& ptr) : std::shared_ptr<T>(ptr) {}
  inline Ptr(T* t) : std::shared_ptr<T>(t) {}

  inline Ptr& operator = (T* t) { this->reset(t); return *this; }
};

/**
 * An abstract class that may be used to declare other non-copyable classes by
 * extending it.
 */
class NonCopyable {
protected:

  inline NonCopyable() {}

private:

  /* Note: private copy constructor and assignment operator prevents the
   * compiler to generate synthesized ones for children classes.
   */
  inline NonCopyable(const NonCopyable& obj);

  inline NonCopyable& operator = (const NonCopyable& obj);
};

}} // namespace grog::util

using grog::util::Ptr;

#endif // GROG_UTIL_LANG_H
