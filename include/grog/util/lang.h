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

#include <functional>
#include <list>
#include <memory>

#include "grog/util/error.h"

#define GROG_BEGIN_BUILDER(type) \
  class Builder { \
  private: \
    Ptr<type> obj_; \
  public: \
    inline Builder() : obj_(new type()) {} \
    inline Ptr<type> Build() { return obj_; }

#define GROG_BUILDER_WITH(fname, type, setter) \
    inline Builder& fname(const type& t) { obj_->setter(t); return *this; }

#define GROG_END_BUILDER() };

namespace grog { namespace util {

template <typename T>
class Ptr : public std::shared_ptr<T> {
public:

  inline Ptr() : std::shared_ptr<T>() {}

  inline Ptr(const Ptr& ptr) : std::shared_ptr<T>(ptr) {}

  inline Ptr(const std::shared_ptr<T>& ptr) : std::shared_ptr<T>(ptr) {}

  inline Ptr(T* t) : std::shared_ptr<T>(t) {}

  template <typename U>
  inline Ptr(const Ptr<U>& ptr)
    : std::shared_ptr<T>(std::static_pointer_cast<T>(ptr)) {}

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

class NotificationChannel {
public:

  class Sender {
  public:
    inline virtual ~Sender() {}
  };

  class Message {
  public:
    inline virtual ~Message() {}
  };

  struct Callback {
    typedef std::function<void(void)> Type;
  };

  template <typename T>
  struct CallbackByMessage {
    typedef std::function<void(const T&)> Type;
  };

  template <typename S, typename T>
  struct CallbackBySenderAndMessage {
    typedef std::function<void(const S& s, const T&)> Type;
  };

  template <typename S, typename T>
  inline void Notify(const S& s, const T& t) {
    for (auto& subscriber : subscribers_)
      subscriber->Receive(s, t);
  }

  inline void Listen(const Callback::Type& callback) {
    subscribers_.push_back(new UntypedSubscriber(callback));
  }

  template <typename T>
  inline void Listen(const typename CallbackByMessage<T>::Type& callback) {
    subscribers_.push_back(new TypedMessageSubscriber<T>(callback));
  }

  template <typename S, typename T>
  inline void Listen(
      const typename CallbackBySenderAndMessage<S, T>::Type& callback) {
    subscribers_.push_back(new TypedSenderMessageSubscriber<S, T>(callback));
  }

private:

  class Subscriber {
  public:

    inline virtual ~Subscriber() {}

    virtual void Receive(const Sender& sender, const Message& msg) = 0;
    virtual void Receive(const Message& msg) = 0;
  };

  class UntypedSubscriber : public Subscriber {
  public:

    inline UntypedSubscriber(
        const Callback::Type& callback) : callback_(callback) {}

    inline virtual void Receive(const Sender& sender, const Message& msg) {
      callback_();
    }

    inline virtual void Receive(const Message& msg) {
      callback_();
    }

  private:
    Callback::Type callback_;
  };

  template <typename T>
  class TypedMessageSubscriber : public Subscriber {
  public:

    inline TypedMessageSubscriber(
        const typename CallbackByMessage<T>::Type& callback)
      : callback_(callback) {}

    inline virtual void Receive(const Sender& sender, const Message& msg) {
      Receive(msg);
    }

    inline virtual void Receive(const Message& msg) {
      T* typed_msg = dynamic_cast<T*>(&msg);
      if (typed_msg)
        callback_(*typed_msg);
    }

  private:
    typename CallbackByMessage<T>::Type callback_;
  };

  template <typename S, typename T>
  class TypedSenderMessageSubscriber : Subscriber {
  public:

    inline TypedSenderMessageSubscriber(
        const typename CallbackBySenderAndMessage<S, T>::Type& callback)
      : callback_(callback) {}

    inline virtual void Receive(const Sender& sender, const Message& msg) {
      S* typed_sender = dynamic_cast<S*>(&sender);
      T* typed_msg = dynamic_cast<T*>(&msg);
      if (typed_sender && typed_msg)
        callback_(*typed_sender, *typed_msg);
    }

    inline virtual void Receive(const Message& msg) {
      T* typed_msg = dynamic_cast<T*>(&msg);
      if (typed_msg)
        callback_(*typed_msg);
    }

  private:
    typename CallbackBySenderAndMessage<S, T>::Type callback_;
  };

  std::list<Ptr<Subscriber> > subscribers_;
};

template <typename T>
class Option {
public:

  GROG_DECL_ERROR(NoneError, IllegalStateError);

  static Option Some(const Ptr<T>& obj) { return Option(obj); }
  static Option None() { return Option(); }

  inline operator bool() const { return valid(); }

  inline bool valid() const { return bool(obj_); }

  inline const T& value() const throw (NoneError) {
    if (!obj_)
      GROG_THROW_ERROR(NoneError());
    return *obj_;
  }

  inline T& value() throw (NoneError) {
    if (!obj_)
      GROG_THROW_ERROR(NoneError());
    return *obj_;
  }

private:

  Ptr<T> obj_;

  inline Option(const Ptr<T>& obj = nullptr) : obj_(obj) {}
};

/**
 * A tuple of two elements.
 */
template <typename T1, typename T2>
class Tuple {
public:

  inline Tuple(const T1& v1, const T2& v2) : v1_(v1), v2_(v2) {}

  inline const T1& first() const { return v1_; }
  inline const T1& left() const { return v1_; }
  inline T1& first() { return v1_; }
  inline T1& left(){ return v1_; }
  inline const T2& second() const { return v2_; }
  inline const T2& right() const { return v2_; }
  inline T2& second() { return v2_; }
  inline T2& right() { return v2_; }

private:

  T1 v1_;
  T2 v2_;
};

}} // namespace grog::util

using grog::util::Option;
using grog::util::Ptr;
using grog::util::Tuple;

#endif // GROG_UTIL_LANG_H
