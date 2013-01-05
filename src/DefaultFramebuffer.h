#ifndef Magnum_DefaultFramebuffer_h
#define Magnum_DefaultFramebuffer_h
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
 * @brief Class Magnum::DefaultFramebuffer
 */

#include "AbstractFramebuffer.h"

namespace Magnum {

/**
@brief Default framebuffer

Default framebuffer, i.e. the actual screen surface. It is automatically
created when Context is created and it is available through global variable
@ref defaultFramebuffer. It is by default mapped to whole screen surface.

@section DefaultFramebuffer-usage Usage

When you are using only the default framebuffer, the usage is simple. You
must ensure that it is properly resized when application surface is resized,
i.e. you must pass the new size in your @ref Platform::GlutApplication::viewportEvent() "viewportEvent()"
implementation, for example:
@code
void viewportEvent(const Vector2i& size) {
    defaultFramebuffer.setViewport({}, size);

    // ...
}
@endcode

Next thing you probably want is to clear all used buffers before performing
any drawing in your @ref Platform::GlutApplication::drawEvent() "drawEvent()"
implementation, for example:
@code
void drawEvent() {
    defaultFramebuffer.clear(AbstractFramebuffer::Clear::Color|AbstractFramebuffer::Clear::Depth);

    // ...
}
@endcode

See Framebuffer documentation for more involved usage, usage of non-default or
multiple framebuffers.

@section DefaultFramebuffer-performance-optimization Performance optimizations

See also @ref AbstractFramebuffer-performance-optimization "relevant section in AbstractFramebuffer".

If extension @extension{EXT,direct_state_access} is available, functions
mapForDraw() and mapForRead() use DSA to avoid unnecessary calls to
@fn_gl{BindFramebuffer}. See their respective documentation for more
information.
*/
class MAGNUM_EXPORT DefaultFramebuffer: public AbstractFramebuffer {
    friend class Context;

    public:
        #ifndef MAGNUM_TARGET_GLES2
        /**
         * @brief Draw attachment
         *
         * @see mapForDraw()
         * @requires_gles30 Draw attachments for default framebuffer are
         *      available only in OpenGL ES 3.0.
         */
        enum class DrawAttachment: GLenum {
            /** Don't use the output. */
            None = GL_NONE,

            #ifndef MAGNUM_TARGET_GLES
            /**
             * Write output to back left framebuffer.
             * @requires_gl Stereo rendering is not available in OpenGL ES.
             */
            BackLeft = GL_BACK_LEFT,

            /**
             * Write output to back right framebuffer.
             * @requires_gl Stereo rendering is not available in OpenGL ES.
             */
            BackRight = GL_BACK_RIGHT,

            /**
             * Write output to front left framebuffer.
             * @requires_gl Stereo rendering is not available in OpenGL ES.
             */
            FrontLeft = GL_FRONT_LEFT,

            /**
             * Write output to front right framebuffer.
             * @requires_gl Stereo rendering is not available in OpenGL ES.
             */
            FrontRight = GL_FRONT_RIGHT,
            #endif

            /**
             * Write output to back framebuffer.
             *
             * On desktop OpenGL, this is equal to @ref DrawAttachment "DrawAttachment::BackLeft".
             */
            #ifdef MAGNUM_TARGET_GLES
            Back = GL_BACK,
            #else
            Back = GL_BACK_LEFT,
            #endif

            /**
             * Write output to front framebuffer.
             *
             * On desktop OpenGL, this is equal to @ref DrawAttachment "DrawAttachment::FrontLeft".
             */
            #ifdef MAGNUM_TARGET_GLES
            Front = GL_FRONT
            #else
            Front = GL_FRONT_LEFT
            #endif
        };
        #endif

        /**
         * @brief Read attachment
         *
         * @see mapForRead()
         * @requires_gles30 %Extension @es_extension2{NV,read_buffer,GL_NV_read_buffer}
         */
        enum class ReadAttachment: GLenum {
            /** Don't read from any framebuffer */
            None = GL_NONE,

            #ifndef MAGNUM_TARGET_GLES
            /**
             * Read from back left framebuffer.
             * @requires_gl Stereo rendering is not available in OpenGL ES.
             */
            BackLeft = GL_BACK_LEFT,

            /**
             * Read from back right framebuffer.
             * @requires_gl Stereo rendering is not available in OpenGL ES.
             */
            BackRight = GL_BACK_RIGHT,

            /**
             * Read from front left framebuffer.
             * @requires_gl Stereo rendering is not available in OpenGL ES.
             */
            FrontLeft = GL_FRONT_LEFT,

            /**
             * Read from front right framebuffer.
             * @requires_gl Stereo rendering is not available in OpenGL ES.
             */
            FrontRight = GL_FRONT_RIGHT,

            /**
             * Read from left framebuffer.
             * @requires_gl Stereo rendering is not available in OpenGL ES.
             */
            Left = GL_LEFT,

            /**
             * Read from right framebuffer.
             * @requires_gl Stereo rendering is not available in OpenGL ES.
             */
            Right = GL_RIGHT,
            #endif

            /** Read from back framebuffer. */
            Back = GL_BACK,

            /**
             * Read from front framebuffer.
             * @requires_es_extension %Extension @es_extension2{NV,read_buffer,GL_NV_read_buffer}
             */
            Front = GL_FRONT

            #ifndef MAGNUM_TARGET_GLES
            ,

            /**
             * Read from front and back framebuffer.
             * @requires_gl In OpenGL ES you must specify either
             *      @ref Magnum::DefaultFramebuffer::ReadAttachment "ReadAttachment::Front"
             *      or @ref Magnum::DefaultFramebuffer::ReadAttachment "ReadAttachment::Back".
             */
            FrontAndBack = GL_FRONT_AND_BACK
            #endif
        };

        explicit MAGNUM_LOCAL DefaultFramebuffer();

        #ifndef MAGNUM_TARGET_GLES2
        /**
         * @brief Map shader outputs to buffer attachment
         *
         * @p attachments is list of shader outputs mapped to framebuffer
         * buffer attachments. Shader outputs which are not listed are not
         * used, you can achieve the same by passing @ref DrawAttachment "DrawAttachment::None"
         * as attachment. Example usage:
         * @code
         * framebuffer.mapForDraw({{MyShader::ColorOutput, DefaultFramebuffer::DrawAttachment::BackLeft},
         *                         {MyShader::NormalOutput, DefaultFramebuffer::DrawAttachment::None}});
         * @endcode
         *
         * If @extension{EXT,direct_state_access} is not available and the
         * framebufferbuffer is not currently bound, it is bound before the
         * operation.
         * @see mapForRead(), @fn_gl{BindFramebuffer}, @fn_gl{DrawBuffers} or
         *      @fn_gl_extension{FramebufferDrawBuffers,EXT,direct_state_access}
         * @requires_gles30 Draw attachments for default framebuffer are
         *      available only in OpenGL ES 3.0.
         */
        void mapForDraw(std::initializer_list<std::pair<GLuint, DrawAttachment>> attachments);

        /**
         * @brief Map shader output to buffer attachment
         * @param attachment        Buffer attachment
         *
         * Similar to above function, can be used in cases when shader has
         * only one (unnamed) output.
         *
         * If @extension{EXT,direct_state_access} is not available and the
         * framebufferbuffer is not currently bound, it is bound before the
         * operation.
         * @see mapForRead(), @fn_gl{BindFramebuffer}, @fn_gl{DrawBuffer} or
         *      @fn_gl_extension{FramebufferDrawBuffer,EXT,direct_state_access}
         * @requires_gles30 Draw attachments for default framebuffer are
         *      available only in OpenGL ES 3.0.
         */
        inline void mapForDraw(DrawAttachment attachment) {
            (this->*drawBufferImplementation)(static_cast<GLenum>(attachment));
        }
        #endif

        /**
         * @brief Map given attachment for reading
         * @param attachment        Buffer attachment
         *
         * If @extension{EXT,direct_state_access} is not available and the
         * framebufferbuffer is not currently bound, it is bound before the
         * operation.
         * @see mapForDraw(), @fn_gl{BindFramebuffer}, @fn_gl{ReadBuffer} or
         *      @fn_gl_extension{FramebufferReadBuffer,EXT,direct_state_access}
         * @requires_gles30 %Extension @es_extension2{NV,read_buffer,GL_NV_read_buffer}
         */
        inline void mapForRead(ReadAttachment attachment) {
            (this->*readBufferImplementation)(static_cast<GLenum>(attachment));
        }

    private:
        static void MAGNUM_LOCAL initializeContextBasedFunctionality(Context* context);
};

/** @brief Default framebuffer instance */
extern DefaultFramebuffer MAGNUM_EXPORT defaultFramebuffer;

}

#endif