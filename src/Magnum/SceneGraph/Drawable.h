#ifndef Magnum_SceneGraph_Drawable_h
#define Magnum_SceneGraph_Drawable_h
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
 * @brief Class @ref Magnum::SceneGraph::Drawable, @ref Magnum::SceneGraph::DrawableGroup, alias @ref Magnum::SceneGraph::BasicDrawable2D, @ref Magnum::SceneGraph::BasicDrawable3D, @ref Magnum::SceneGraph::BasicDrawableGroup2D, @ref Magnum::SceneGraph::BasicDrawableGroup3D, typedef @ref Magnum::SceneGraph::Drawable2D, @ref Magnum::SceneGraph::Drawable3D, @ref Magnum::SceneGraph::DrawableGroup2D, @ref Magnum::SceneGraph::DrawableGroup3D
 */

#include "Magnum/SceneGraph/AbstractGroupedFeature.h"

namespace Magnum { namespace SceneGraph {

/**
@brief %Drawable

Adds drawing function to the object. Each %Drawable is part of some
@ref DrawableGroup and the whole group is drawn with particular camera using
@ref AbstractCamera::draw().

@section Drawable-usage Usage

First thing is to add @ref Drawable feature to some object and implement
@ref draw(). You can do it conveniently using multiple inheritance (see
@ref scenegraph-features for introduction). Example:
@code
typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;

class DrawableObject: public Object3D, SceneGraph::Drawable3D {
    public:
        DrawableObject(Object* parent = nullptr, SceneGraph::DrawableGroup3D* group = nullptr): Object3D(parent), SceneGraph::Drawable3D(*this, group) {
            // ...
        }

        void draw(const Matrix4& transformationMatrix, AbstractCamera3D& camera) override {
            // ...
        }
}
@endcode

Then you add these objects to your scene and some drawable group and transform
them as you like. You can also use @ref DrawableGroup::add() and
@ref DrawableGroup::remove().
@code
Scene3D scene;
SceneGraph::DrawableGroup3D drawables;

(new DrawableObject(&scene, &drawables))
    ->translate(Vector3::yAxis(-0.3f))
    .rotateX(30.0_degf);
(new AnotherDrawableObject(&scene, &drawables))
    ->translate(Vector3::zAxis(0.5f));
// ...
@endcode

The last thing you need is camera attached to some object (thus using its
transformation) and with it you can perform drawing in your draw event
implementation. See @ref Camera2D and @ref Camera3D documentation for more
information.
@code
Camera3D camera(&cameraObject);

void MyApplication::drawEvent() {
    camera.draw(drawables);

    swapBuffers();
    // ...
}
@endcode

@section Drawable-performance Using drawable groups to improve performance

You can organize your drawables to multiple groups to minimize OpenGL state
changes -- for example put all objects using the same shader, the same light
setup etc into one group, then put all transparent into another and set common
parameters once for whole group instead of setting them again in each
@ref draw() implementation. Example:
@code
Shaders::PhongShader shader;
SceneGraph::DrawableGroup3D phongObjects, transparentObjects;

void MyApplication::drawEvent() {
    shader.setProjectionMatrix(camera->projectionMatrix())
          .setLightPosition(lightPosition)
          .setLightColor(lightColor)
          .setAmbientColor(ambientColor);
    camera.draw(phongObjects);

    Renderer::setFeature(Renderer::Feature::Blending, true);
    camera.draw(transparentObjects);
    Renderer::setFeature(Renderer::Feature::Blending, false);

    // ...
}
@endcode

@section Drawable-explicit-specializations Explicit template specializations

The following specializations are explicitly compiled into @ref SceneGraph
library. For other specializations (e.g. using @ref Double type) you have to
use @ref Drawable.hpp implementation file to avoid linker errors. See also
@ref compilation-speedup-hpp for more information.

-   @ref Drawable2D
-   @ref Drawable3D

@see @ref scenegraph, @ref BasicDrawable2D, @ref BasicDrawable3D,
    @ref Drawable2D, @ref Drawable3D, @ref DrawableGroup
*/
template<UnsignedInt dimensions, class T> class Drawable: public AbstractGroupedFeature<dimensions, Drawable<dimensions, T>, T> {
    public:
        /**
         * @brief Constructor
         * @param object    %Object this drawable belongs to
         * @param drawables Group this drawable belongs to
         *
         * Adds the feature to the object and also to the group, if specified.
         * Otherwise you can use @ref DrawableGroup::add().
         */
        explicit Drawable(AbstractObject<dimensions, T>& object, DrawableGroup<dimensions, T>* drawables = nullptr);

        /**
         * @brief Group containing this drawable
         *
         * If the drawable doesn't belong to any group, returns `nullptr`.
         */
        DrawableGroup<dimensions, T>* drawables() {
            #ifndef CORRADE_GCC46_COMPATIBILITY
            return AbstractGroupedFeature<dimensions, Drawable<dimensions, T>, T>::group();
            #else
            return static_cast<DrawableGroup<dimensions, T>*>(AbstractGroupedFeature<dimensions, Drawable<dimensions, T>, T>::group());
            #endif
        }

        /** @overload */
        const DrawableGroup<dimensions, T>* drawables() const {
            #ifndef CORRADE_GCC46_COMPATIBILITY
            return AbstractGroupedFeature<dimensions, Drawable<dimensions, T>, T>::group();
            #else
            return static_cast<const DrawableGroup<dimensions, T>*>(AbstractGroupedFeature<dimensions, Drawable<dimensions, T>, T>::group());
            #endif
        }

        /**
         * @brief Draw the object using given camera
         * @param transformationMatrix      %Object transformation relative
         *      to camera
         * @param camera                    Camera
         *
         * Projection matrix can be retrieved from
         * @ref SceneGraph::AbstractCamera::projectionMatrix() "AbstractCamera::projectionMatrix()".
         */
        virtual void draw(const typename DimensionTraits<dimensions, T>::MatrixType& transformationMatrix, AbstractCamera<dimensions, T>& camera) = 0;
};

#ifndef CORRADE_GCC46_COMPATIBILITY
/**
@brief %Drawable for two-dimensional scenes

Convenience alternative to <tt>%Drawable<2, T></tt>. See @ref Drawable for more
information.
@note Not available on GCC < 4.7. Use <tt>%Drawable<2, T></tt> instead.
@see @ref Drawable2D, @ref BasicDrawable3D
*/
template<class T> using BasicDrawable2D = Drawable<2, T>;
#endif

/**
@brief %Drawable for two-dimensional float scenes

@see @ref Drawable3D
*/
#ifndef CORRADE_GCC46_COMPATIBILITY
typedef BasicDrawable2D<Float> Drawable2D;
#else
typedef Drawable<2, Float> Drawable2D;
#endif

#ifndef CORRADE_GCC46_COMPATIBILITY
/**
@brief %Drawable for three-dimensional scenes

Convenience alternative to <tt>%Drawable<3, T></tt>. See @ref Drawable for more
information.
@note Not available on GCC < 4.7. Use <tt>%Drawable<3, T></tt> instead.
@see @ref Drawable3D, @ref BasicDrawable3D
*/
template<class T> using BasicDrawable3D = Drawable<3, T>;
#endif

/**
@brief %Drawable for three-dimensional float scenes

@see @ref Drawable2D
*/
#ifndef CORRADE_GCC46_COMPATIBILITY
typedef BasicDrawable3D<Float> Drawable3D;
#else
typedef Drawable<3, Float> Drawable3D;
#endif

/**
@brief Group of drawables

See @ref Drawable for more information.
@see @ref scenegraph, @ref BasicDrawableGroup2D, @ref BasicDrawableGroup3D,
    @ref DrawableGroup2D, @ref DrawableGroup3D
*/
#ifndef CORRADE_GCC46_COMPATIBILITY
template<UnsignedInt dimensions, class T> using DrawableGroup = FeatureGroup<dimensions, Drawable<dimensions, T>, T>;
#else
template<UnsignedInt dimensions, class T> class DrawableGroup: public FeatureGroup<dimensions, Drawable<dimensions, T>, T> {};
#endif

#ifndef CORRADE_GCC46_COMPATIBILITY
/**
@brief Group of drawables for two-dimensional scenes

Convenience alternative to <tt>%DrawableGroup<2, T></tt>. See @ref Drawable for
more information.
@note Not available on GCC < 4.7. Use <tt>%DrawableGroup<2, T></tt> instead.
@see @ref DrawableGroup2D, @ref BasicDrawableGroup3D
*/
template<class T> using BasicDrawableGroup2D = DrawableGroup<2, T>;
#endif

/**
@brief Group of drawables for two-dimensional float scenes

@see @ref DrawableGroup3D
*/
#ifndef CORRADE_GCC46_COMPATIBILITY
typedef BasicDrawableGroup2D<Float> DrawableGroup2D;
#else
typedef DrawableGroup<2, Float> DrawableGroup2D;
#endif

#ifndef CORRADE_GCC46_COMPATIBILITY
/**
@brief Group of drawables for three-dimensional scenes

Convenience alternative to <tt>%DrawableGroup<3, T></tt>. See @ref Drawable for
more information.
@note Not available on GCC < 4.7. Use <tt>%DrawableGroup<3, T></tt> instead.
@see @ref DrawableGroup3D, @ref BasicDrawableGroup2D
*/
template<class T> using BasicDrawableGroup3D = DrawableGroup<3, T>;
#endif

/**
@brief Group of drawables for three-dimensional float scenes

@see @ref DrawableGroup2D
*/
#ifndef CORRADE_GCC46_COMPATIBILITY
typedef BasicDrawableGroup3D<Float> DrawableGroup3D;
#else
typedef DrawableGroup<3, Float> DrawableGroup3D;
#endif

#ifdef CORRADE_TARGET_WINDOWS
extern template class MAGNUM_SCENEGRAPH_EXPORT Drawable<2, Float>;
extern template class MAGNUM_SCENEGRAPH_EXPORT Drawable<3, Float>;
#endif
}}

#endif
