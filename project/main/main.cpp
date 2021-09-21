#include <SDL.h>

#include "core/graphics.h"
#include "rendering/vertex.h"
#include "rendering/shader.h"
#include <cstddef>
#include <iostream>
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <rendering/objectExamples.h>

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

constexpr glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
};

void render()
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

    for (unsigned int i = 0; i < 10; i++) {
        glm::mat4 model {1.f};
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * static_cast<float>(i);
        model = glm::rotate(model, (static_cast<float>(SDL_GetTicks()) / 1000.f) * glm::radians(angle), glm::vec3{0.5f, 1.f, 0.f});
        shader->SetMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / sizeof(OZZ::Vertex));
    }
    SDL_GL_SwapWindow(window);
}

void setupScene() {
    printf("OpenGL version is (%s)\n", glGetString(GL_VERSION));


    uint32_t indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };

    glm::mat4 model = glm::mat4{1.0f};
    model = glm::rotate(model, glm::radians(-55.f), glm::vec3{1.f, 0.f, 0.0f});

    glm::mat4 view = glm::mat4 {1.f};
    view = glm::translate(view, glm::vec3{0.f,0.f,-3.f});

    int windowWidth, windowHeight;
    SDL_GL_GetDrawableSize(window, &windowWidth, &windowHeight);

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)windowWidth/(float)windowHeight, 0.1f, 100.0f);

    GenVAO(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenTextures(1, &texture);
    glGenTextures(1, &texture2);

    shader = new OZZ::Shader("assets/shaders/simpleShader.vert", "assets/shaders/simpleShader.frag");
    shader->Bind();
    shader->SetInt("ourTexture", 0);
    shader->SetInt("texture2",  1);
    shader->SetMat4("model", model);
    shader->SetMat4("view", view);
    shader->SetMat4("projection", proj);

    /** This is the bind functionality */
    BindVAO(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

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
    int imageDataLength {static_cast<int>(SDL_RWsize(file))};
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
    imageDataLength = static_cast<int>(SDL_RWsize(file));
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OZZ::Vertex), nullptr);
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
    render();
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
int main([[maybe_unused]] int argc, char *argv[])
{
    int width{800};
    int height{600};

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