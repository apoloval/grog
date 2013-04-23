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

#ifndef GROG_UTIL_ERROR_H
#define GROG_UTIL_ERROR_H

#include <exception>
#include <string>
#include <utility>

#include <boost/exception/all.hpp>

namespace grog { namespace util {

struct Error : virtual std::exception, virtual boost::exception
{
   virtual const char* what() const throw ()
   {
      return boost::diagnostic_information_what(*this);
   }

   operator boost::exception_ptr() const {
      return boost::copy_exception(*this);
   }
};

struct LogicErrorBase : virtual Error {};
struct RuntimeErrorBase : virtual Error {};

template <typename ErrorInfo, typename E>
const typename ErrorInfo::error_info::value_type* GetErrorInfo(const E& e)
{ return boost::get_error_info<ErrorInfo>(e); }

}} // namespace grog::util

#define GROG_DECL_LOGIC_ERROR(name) \
    struct name : virtual ::grog::util::LogicErrorBase {}
#define GROG_DECL_RUNTIME_ERROR(name) \
    struct name : virtual ::grog::util::RuntimeErrorBase {}
#define GROG_DECL_ERROR(name, parent) \
    struct name : virtual parent {}

#define GROG_DECL_ERROR_INFO(name, type) \
   typedef boost::error_info<struct name##Tag, type> name

#define GROG_THROW_ERROR(error) BOOST_THROW_EXCEPTION(error)

namespace grog { namespace util {

GROG_DECL_LOGIC_ERROR(ConnectionError);
GROG_DECL_LOGIC_ERROR(InvalidInputError);
GROG_DECL_LOGIC_ERROR(NotFoundError);
GROG_DECL_LOGIC_ERROR(NullPointerError);

GROG_DECL_RUNTIME_ERROR(IllegalStateError);
GROG_DECL_RUNTIME_ERROR(IOError);

GROG_DECL_ERROR_INFO(CodeInfo, int);
GROG_DECL_ERROR_INFO(FileNameInfo, std::wstring);
GROG_DECL_ERROR_INFO(FunctionNameInfo, std::string);
GROG_DECL_ERROR_INFO(IndexInfo, int);
GROG_DECL_ERROR_INFO(LowerBoundInfo, int);
GROG_DECL_ERROR_INFO(MessageInfo, std::string);
GROG_DECL_ERROR_INFO(NestedErrorInfo, boost::exception_ptr);
GROG_DECL_ERROR_INFO(UpperBoundInfo, int);

}} // namespace grog::util

#endif
