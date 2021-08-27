//
// Created by ozzadar on 2021-08-26.
//

#pragma once

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

PFNGLBINDVERTEXARRAYPROC bindVertexArrayOES;
PFNGLDELETEVERTEXARRAYSPROC deleteVertexArraysOES;
PFNGLGENVERTEXARRAYSPROC genVertexArraysOES;
PFNGLISVERTEXARRAYPROC isVertexArrayOES;

#ifdef __ANDROID__
#include <dlfcn.h>
#define BindVAO bindVertexArrayOES
#define GenVAO genVertexArraysOES
#else
#define BindVAO glBindVertexArray
#define GenVAO glGenVertexArrays
#endif

void getGLFunctions() {
#ifdef __ANDROID__
    void *libhandle = dlopen("libGLESv3.so", RTLD_LAZY);

    bindVertexArrayOES = (PFNGLBINDVERTEXARRAYPROC) dlsym(libhandle,
                                                             "glBindVertexArray");
    deleteVertexArraysOES = (PFNGLDELETEVERTEXARRAYSPROC) dlsym(libhandle,
                                                                   "glDeleteVertexArrays");
    genVertexArraysOES = (PFNGLGENVERTEXARRAYSPROC)dlsym(libhandle,
                                                            "glGenVertexArrays");
    isVertexArrayOES = (PFNGLISVERTEXARRAYPROC)dlsym(libhandle,
                                                        "glIsVertexArray");
#endif
#ifdef GLAD
    gladLoadGLLoader(SDL_GL_GetProcAddress);
#endif
}
