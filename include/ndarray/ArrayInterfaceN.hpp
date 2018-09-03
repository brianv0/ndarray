// -*- c++ -*-
/*
 * Copyright (c) 2010-2018, Jim Bosch
 * All rights reserved.
 *
 * ndarray is distributed under a simple BSD-like license;
 * see the LICENSE file that should be present in the root
 * of the source distribution, or alternately available at:
 * https://github.com/ndarray/ndarray
 */
#ifndef NDARRAY_ArrayInterfaceN_hpp_INCLUDED
#define NDARRAY_ArrayInterfaceN_hpp_INCLUDED

#include "ndarray/common.hpp"
#include "ndarray/errors.hpp"
#include "ndarray/detail/ArrayImpl.hpp"

namespace ndarray {

namespace detail {

constexpr Offset nested_contiguousness(Size n, Offset c) {
    return (c > 0 && c == n) ? c - 1 : (c < 0 && -c == n) ? c + 1 : c;
}

} // namespace detail

template <typename Derived, typename Element, Offset C>
class ArrayInterfaceN<Derived, Element, 1, C> {
public:

    using Reference = Element &;

    Reference operator[](Size n) const {
        NDARRAY_ASSERT_AUDIT(impl().layout != nullptr, Error::UNINITIALIZED, "layout is null");
        NDARRAY_ASSERT_AUDIT(impl().buffer != nullptr, Error::UNINITIALIZED, "buffer is null");
        NDARRAY_ASSERT_AUDIT(n < impl().layout->size(), Error::OUT_OF_BOUNDS, "array index out of bounds");
        return *reinterpret_cast<Element*>(impl().buffer.get() + n*impl().layout->stride());
    }

private:

    detail::ArrayImpl<1> const & impl() const {
        return static_cast<Derived const &>(*this)._impl;
    }

};


template <typename Derived, typename Element, Size N, Offset C>
class ArrayInterfaceN {
public:

    using Reference = Array<Element, N-1, detail::nested_contiguousness(N, C)>;

    Reference operator[](Size n) const {
        NDARRAY_ASSERT_AUDIT(impl().layout != nullptr, Error::UNINITIALIZED, "layout is null");
        NDARRAY_ASSERT_AUDIT(impl().buffer != nullptr, Error::UNINITIALIZED, "buffer is null");
        NDARRAY_ASSERT_AUDIT(n < impl().layout->size(), Error::OUT_OF_BOUNDS, "array index out of bounds");
        return Reference(
            std::shared_ptr<Byte>(
                impl().buffer,
                impl().buffer.get() + n*impl().layout->stride()
            ),
            impl().layout
        );
    }

private:

    detail::ArrayImpl<N> const & impl() const {
        return static_cast<Derived const &>(*this)._impl;
    }

};

} // namespace ndarray

#endif // !NDARRAY_ArrayInterfaceN_hpp_INCLUDED
