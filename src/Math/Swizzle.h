#ifndef Magnum_Math_Swizzle_h
#define Magnum_Math_Swizzle_h
/*
    Copyright © 2010, 2011, 2012 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/** @file
 * @brief Function Magnum::Math::swizzle()
 */

#include "Vector.h"

namespace Magnum { namespace Math {

#ifndef DOXYGEN_GENERATING_OUTPUT
namespace Implementation {
    template<std::size_t size, std::size_t position> struct ComponentAtPosition {
        static_assert(size > position, "Swizzle parameter out of range of base vector");

        template<class T> inline constexpr static T value(const Math::Vector<size, T>& vector) { return vector[position]; }
    };

    template<std::size_t size, char component> struct Component {};
    template<std::size_t size> struct Component<size, 'x'>: public ComponentAtPosition<size, 0> {};
    template<std::size_t size> struct Component<size, 'y'>: public ComponentAtPosition<size, 1> {};
    template<std::size_t size> struct Component<size, 'z'>: public ComponentAtPosition<size, 2> {};
    template<std::size_t size> struct Component<size, 'w'>: public ComponentAtPosition<size, 3> {};
    template<std::size_t size> struct Component<size, '0'> {
        template<class T> inline constexpr static T value(const Math::Vector<size, T>&) { return T(0); }
    };
    template<std::size_t size> struct Component<size, '1'> {
        template<class T> inline constexpr static T value(const Math::Vector<size, T>&) { return T(1); }
    };
}
#endif

/**
@brief Swizzle Vector components

Creates new vector from given components. Example:
@code
Vector4<std::int32_t> original(-1, 2, 3, 4);

auto vec = swizzle<'w', '1', '0', 'x', 'y', 'z'>(original);
// vec == { 4, 1, 0, -1, 2, 3 }
@endcode
You can use letters `x`, `y`, `z`, `w` for addressing components or letters
`0` and `1` for zero and one. Count of elements is unlimited, but must be at
least one.

See also Magnum::swizzle() which has some added convenience features not
present in this lightweight implementation for Math namespace.

@see @ref matrix-vector-component-access, Vector4::xyz(),
    Vector4::xy(), Vector3::xy()
*/
template<char ...components, std::size_t size, class T> inline constexpr Vector<sizeof...(components), T> swizzle(const Vector<size, T>& vector) {
    return {Implementation::Component<size, components>::value(vector)...};
}

}}

#endif