#include <SDL.h>

#if __ANDROID__
#define GLES
#include <GLES3/gl3.h>
#elif __EMSCRIPTEN__
#define GLES
#include <emscripten.h>
#include <GLES2/gl2.h>
#else
#define GLAD
#include <glad/glad.h>
#endif

#include <iostream>

bool running { true };
void render(SDL_Window* window, const SDL_GLContext& context)
{
    SDL_GL_MakeCurrent(window, context);

    glClearColor( 1.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SDL_GL_SwapWindow(window);
}

SDL_Window* window;
SDL_GLContext context;

void runMainLoop()
{
    SDL_Event event;

    // Each loop we will process any events that are waiting for us.
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                running = false;
                return;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                    return;
                }
                break;
                default:
                    break;
        }
    }

    // Perform our rendering for this frame.
    render(window, context);
}

void runApplication()
{


    // Setup some basic global OpenGL state.
#ifdef GLES
    glClearDepthf(1.0f);
#endif

#ifdef GLAD
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    glClearDepth(1.f);
#endif
    glEnable(GL_DEPTH_TEST);


#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(runMainLoop, 0, 1);
#else
    while(running) {
        runMainLoop();
    }
#endif

    // Clean up after ourselves.
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
}

int main(int argc, char *argv[])
{
    uint32_t width{1000};
    uint32_t height{1000};

#ifdef __EMSCRIPTEN__
    SDL_Renderer *renderer = nullptr;
    SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &window, &renderer);
#else
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Create a new SDL window based on OpenGL.
    window = {SDL_CreateWindow(
            "A Simple Triangle",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI)};

    // Obtain an OpenGL context based on our window.
#endif
    context = {SDL_GL_CreateContext(window)};

//
    std::cout << "Successfully initialised SDL!" << std::endl;

    runApplication();

    SDL_Quit();


    return 0;
}