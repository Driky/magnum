#ifndef Magnum_MeshTools_Transform_h
#define Magnum_MeshTools_Transform_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

/** @file
 * @brief Function Magnum::MeshTools::transformVectorsInPlace(), Magnum::MeshTools::transformVectors(), Magnum::MeshTools::transformPointsInPlace(), Magnum::MeshTools::transformPoints()
 */

#include "Magnum/Math/DualQuaternion.h"
#include "Magnum/Math/DualComplex.h"

namespace Magnum { namespace MeshTools {

/**
@brief Transform vectors in-place using given transformation

Usable for one-time mesh transformations that would otherwise negatively affect
dependent objects, such as (uneven) scaling. Accepts any forward-iterable type
with compatible vector type as @p vectors. Expects that @ref Math::Quaternion "Quaternion"
is normalized, no further requirements are for other transformation
representations.

Unlike in transformPointsInPlace(), the transformation does not involve
translation.

Example usage:
@code
std::vector<Vector3> vectors;
auto transformation = Quaternion::rotation(35.0_degf, Vector3::yAxis());
MeshTools::transformVectorsInPlace(rotation, vectors);
@endcode

@see transformVectors(), Matrix3::transformVector(), Matrix4::transformVector(),
    Complex::transformVectorNormalized(), Quaternion::transformVectorNormalized()
@todo GPU transform feedback implementation (otherwise this is only bad joke)
*/
template<class T, class U> void transformVectorsInPlace(const Math::Quaternion<T>& normalizedQuaternion, U& vectors) {
    for(auto& vector: vectors) vector = normalizedQuaternion.transformVectorNormalized(vector);
}

/** @overload */
template<class T, class U> void transformVectorsInPlace(const Math::Complex<T>& complex, U& vectors) {
    for(auto& vector: vectors) vector = complex.transformVector(vector);
}

/** @overload */
template<class T, class U> void transformVectorsInPlace(const Math::Matrix3<T>& matrix, U& vectors) {
    for(auto& vector: vectors) vector = matrix.transformVector(vector);
}

/** @overload */
template<class T, class U> void transformVectorsInPlace(const Math::Matrix4<T>& matrix, U& vectors) {
    for(auto& vector: vectors) vector = matrix.transformVector(vector);
}

/**
@brief Transform vectors using given transformation

Returns transformed vectors instead of modifying them in-place. See
transformVectorsInPlace() for more information.
*/
template<class T, class U> U transformVectors(const T& transformation, U vectors) {
    U result(std::move(vectors));
    transformVectorsInPlace(transformation, result);
    return result;
}

/**
@brief Transform points in-place using given transformation

Usable for one-time mesh transformations that would otherwise negatively affect
dependent objects, such as (uneven) scaling. Accepts any forward-iterable type
with compatible vector type as @p vectors. Expects that
@ref Math::DualQuaternion "DualQuaternion" is normalized, no further
requirements are for other transformation representations.

Unlike in transformVectorsInPlace(), the transformation also involves
translation.

Example usage:
@code
std::vector<Vector3> points;
auto transformation = DualQuaternion::rotation(35.0_degf, Vector3::yAxis())*
    DualQuaternion::translation({0.5f, -1.0f, 3.0f});
MeshTools::transformPointsInPlace(rotation, points);
@endcode

@see transformPoints(), Matrix3::transformPoint(), Matrix4::transformPoint(),
    DualQuaternion::transformPointNormalized()
*/
template<class T, class U> void transformPointsInPlace(const Math::DualQuaternion<T>& normalizedDualQuaternion, U& points) {
    for(auto& point: points) point = normalizedDualQuaternion.transformPointNormalized(point);
}

/** @overload */
template<class T, class U> void transformPointsInPlace(const Math::DualComplex<T>& dualComplex, U& points) {
    for(auto& point: points) point = dualComplex.transformPoint(point);
}

/** @overload */
template<class T, class U> void transformPointsInPlace(const Math::Matrix3<T>& matrix, U& points) {
    for(auto& point: points) point = matrix.transformPoint(point);
}

/** @overload */
template<class T, class U> void transformPointsInPlace(const Math::Matrix4<T>& matrix, U& points) {
    for(auto& point: points) point = matrix.transformPoint(point);
}

/**
@brief Transform points using given transformation

Returns transformed points instead of modifying them in-place. See
transformPointsInPlace() for more information.
*/
template<class T, class U> U transformPoints(const T& transformation, U vectors) {
    U result(std::move(vectors));
    transformPointsInPlace(transformation, result);
    return result;
}

}}

#endif
