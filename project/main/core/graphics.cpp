//
// Created by ozzadar on 2021-09-13.
//

#include "graphics.h"

PFNGLBINDVERTEXARRAYPROC bindVertexArrayOES;
PFNGLDELETEVERTEXARRAYSPROC deleteVertexArraysOES;
PFNGLGENVERTEXARRAYSPROC genVertexArraysOES;
PFNGLISVERTEXARRAYPROC isVertexArrayOES;

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
