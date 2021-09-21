#include <SDL.h>

#include "core/graphics.h"
#include "rendering/vertex.h"
#include "rendering/shader.h"
#include <cstddef>
#include <iostream>
#include <stb_image.h>

SDL_Window* window;
SDL_GLContext context;
bool running { true };
bool initialized { false };


/** GL OBJECTS */
unsigned int VAO;
unsigned int VBO;
unsigned int EBO;

unsigned int texture;
unsigned int texture2;

OZZ::Shader* shader = nullptr;

unsigned char* loadTexture(const std::string& filename) {

}

void render(SDL_Window* window, const SDL_GLContext& context)
{
    SDL_GL_MakeCurrent(window, context);

    glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    shader->Bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    BindVAO(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    SDL_GL_SwapWindow(window);
}

void setupScene() {
    printf("OpenGL version is (%s)\n", glGetString(GL_VERSION));
    OZZ::Vertex vertices[] = {
            {
                .position = {0.5f,  0.5f, 0.0f},
                .colour = {1.0f, 0.f, 0.f},
                .uv = {1.0f, 1.0f}
            },  // top right
            {
                .position = {0.5f, -0.5f, 0.0f},
                .colour = {0.0f, 1.0f, 0.0f},
                .uv = {1.0f, 0.0f}
            },  // bottom right
            {
                .position = {-0.5f, -0.5f, 0.0f},
                .colour = {0.0f, 0.0f, 1.0f},
                .uv = {0.0f, 0.0f}
            },  // bottom left
            {
                .position = {-0.5f,  0.5f, 0.0f},
                .colour = {0.5f, 0.5f, 0.5f},
                .uv = {0.0f, 1.0f}
            }   // top left
    };

    uint32_t indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };

    GenVAO(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenTextures(1, &texture);
    glGenTextures(1, &texture2);

    shader = new OZZ::Shader("assets/shaders/simpleShader.vert", "assets/shaders/simpleShader.frag");
    shader->Bind();
    shader->SetInt("ourTexture", 0);
    shader->SetInt("texture2",  1);

    /** This is the bind functionality */
    BindVAO(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;

    // Load with SDL
    stbi_set_flip_vertically_on_load(true);
    SDL_RWops *file = SDL_RWFromFile("assets/textures/container.jpg", "rb");
    size_t imageDataLength {static_cast<size_t>(SDL_RWsize(file))};
    void* imageData{SDL_LoadFile_RW(file, nullptr, 1)};

    // convert to stbi thing
    auto* data = stbi_load_from_memory(static_cast<stbi_uc*>(imageData), imageDataLength, &width, &height, &nrChannels, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load with SDL
    file = SDL_RWFromFile("assets/textures/awesomeface.png", "rb");
    imageDataLength = static_cast<size_t>(SDL_RWsize(file));
    imageData = SDL_LoadFile_RW(file, nullptr, 1);

    // convert to stbi thing
    data = stbi_load_from_memory(static_cast<stbi_uc*>(imageData), imageDataLength, &width, &height, &nrChannels, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OZZ::Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(OZZ::Vertex), (void*)(offsetof(struct OZZ::Vertex, colour)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OZZ::Vertex), (void*)(offsetof(struct OZZ::Vertex, uv)));
    glEnableVertexAttribArray(2);

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