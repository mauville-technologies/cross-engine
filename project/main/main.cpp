#include <SDL.h>

#include "core/graphics.h"
#include "rendering/vertex.h"
#include "rendering/shader.h"

#include <iostream>

SDL_Window* window;
SDL_GLContext context;
bool running { true };
bool initialized { false };


/** GL OBJECTS */
unsigned int VAO;
unsigned int VBO;
unsigned int EBO;

OZZ::Shader* shader = nullptr;

void render(SDL_Window* window, const SDL_GLContext& context)
{
    SDL_GL_MakeCurrent(window, context);

    glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    shader->Bind();

    BindVAO(VAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    SDL_GL_SwapWindow(window);
}

void setupScene() {
    printf("OpenGL version is (%s)\n", glGetString(GL_VERSION));
    OZZ::Vertex vertices[] = {
            {
                .position = {0.5f,  0.5f, 0.0f},
                .colour = {1.0f, 0.f, 0.f}
            },  // top right
            {
                .position = {0.5f, -0.5f, 0.0f},
                .colour = {0.0f, 1.0f, 0.0f}
            },  // bottom right
            {
                .position = {-0.5f, -0.5f, 0.0f},
                .colour = {0.0f, 0.0f, 1.0f}
            },  // bottom left
            {
                .position = {-0.5f,  0.5f, 0.0f},
                .colour = {0.5f, 0.5f, 0.5f}
            }   // top left
    };

    uint32_t indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };

    GenVAO(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    shader = new OZZ::Shader("assets/shaders/simpleShader.vert", "assets/shaders/simpleShader.frag");

    /** This is the bind functionality */
    BindVAO(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OZZ::Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(OZZ::Vertex), (void*)(sizeof(OZZ::Vertex::position)));
    glEnableVertexAttribArray(1);
}

void runMainLoop()
{
    if (!initialized) {
        setupScene();
        initialized = true;
    }
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
#else
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
#include <vector>
int main(int argc, char *argv[])
{
    uint32_t width{800};
    uint32_t height{600};

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
    context = {SDL_GL_CreateContext(window)};
    getGLFunctions();
    std::cout << "Successfully initialised SDL!" << std::endl;
    runApplication();

    SDL_Quit();


    return 0;
}