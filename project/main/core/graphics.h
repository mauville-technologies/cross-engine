//
// Created by ozzadar on 2021-08-26.
//

#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <SDL.h>

#if __ANDROID__
#define GLES
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#elif __EMSCRIPTEN__
#define GLES
#include <emscripten.h>
#include <GLES3/gl3.h>
#else
#define GLAD
#include <glad/glad.h>
#endif

extern PFNGLBINDVERTEXARRAYPROC bindVertexArrayOES;
extern PFNGLDELETEVERTEXARRAYSPROC deleteVertexArraysOES;
extern PFNGLGENVERTEXARRAYSPROC genVertexArraysOES;
extern PFNGLISVERTEXARRAYPROC isVertexArrayOES;

#ifdef __ANDROID__
#include <dlfcn.h>
#define BindVAO bindVertexArrayOES
#define GenVAO genVertexArraysOES
#else
#define BindVAO glBindVertexArray
#define GenVAO glGenVertexArrays
#endif

void getGLFunctions();

#endif
