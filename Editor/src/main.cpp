#include <iostream>
#include <fstream>
#include <glad/glad.h>

#include <SDL3/SDL.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"

#include "stb_image.h"

#include <SticksEngine/Render/Shader.h>

bool FPS_COUNTER = false;
bool WIREFRAME = false;

struct RenderTarget {
    GLuint fbo = 0; //Frame Buffer Object
    GLuint colorTex = 0;
    GLuint depthRb = 0;
    int width = 800;
    int height = 600;

    //TEMPORARY
    GLuint vbo;
    GLuint vao;
    GLuint ebo;

    void Create(int w, int h) {
        width = w;
        height = h;

        //TEMPORARY
        float vertices[] = {
            // positions        // colors       // texture coords
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3 // second triangle
        };

        //Create the frame buffer and assign it to the frame buffer index
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        //Create the color texture that is rendered to
        glGenTextures(1, &colorTex);
        glBindTexture(GL_TEXTURE_2D, colorTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Scaling parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Scaling parameters
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);

        //Now the depth render buffer
        glGenRenderbuffers(1, &depthRb);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRb);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRb);

        //Check if the frame buffer was created succesfully
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Frame Buffer is not complete!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        ///
        ///TODO: MOVE THIS TO SEPERATE OBJECT
        ///
        //Generate the Vertex Buffer Object
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //TODO: Make GL_STATIC_DRAW changeable in the future, do same for VAO (DYNAMIC AND STREAM are faster to access so are better for constantly changing vertices_
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //Generate the Vertex Array Object
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        //Position Attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        //Color Attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        //Texture Coord Attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        //Now we create the EBO
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    void Resize(int w, int h) {
        width = w;
        height = h;

        glBindTexture(GL_TEXTURE_2D, colorTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glBindRenderbuffer(GL_RENDERBUFFER, depthRb);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    }

    //TODO: Take in scene object that contains all objects
    void RenderSceneToSelf(int w, int h, Shader *shader, GLuint *texture1, GLuint *texture2) {
        //Resize if needed
        if (w != width || h != height) {
            Resize(w, h);
        }

        //Change to this FBO
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        //Change the viewport to new dimensions if changed
        glViewport(0, 0, w, h);

        //Fill screen with solid color
        glClearColor(144.0f / 255.0f, 213.0f / 255.0f, 1.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Draw
        //glUseProgram(program_id);
        shader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, *texture2);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //Set FBO back to default
        glUseProgram(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Clean() {
    }
};

int main() {
    std::cout << "Hello, World!" << std::endl;

    ///
    /// SDL and GL Setup
    ///

    //Initialize SDL with Video flag
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL_Init failed!" << std::endl;
        return -1;
    }

    //Set GL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); //RGB8

    //Create a resizable Window
    SDL_Window *window = SDL_CreateWindow("Stick's Engine", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cout << "SDL_CreateWindow failed!" << std::endl;
        return -1;
    }

    //Create the GL Context and get them ready for rendering
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

    ///
    /// Dear IMGUI Setup
    ///

    //Ensures all versions matchup
    IMGUI_CHECKVERSION();

    //Create context and get IO (keyboard, mouse, etc)
    ImGuiContext *imgui_context = ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    //ImGui settings
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; //Enable Docking

    //Initialize IMGUI for SDL3 and OpenGL
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 460");

    //Flip STBI images vertically so they render the right way
    stbi_set_flip_vertically_on_load(true);

    //Load Image
    int imgWidth, imgHeight, imgChannels;
    unsigned char *data = stbi_load("assets/wall.jpg", &imgWidth, &imgHeight, &imgChannels, 0);
    if (!data) {
        std::cout << "Failed to load image" << std::endl;
    }

    //Generate Texture
    GLuint textureID1;
    glGenTextures(1, &textureID1);
    glBindTexture(GL_TEXTURE_2D, textureID1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    //Load Image
    int imgWidth2, imgHeight2, imgChannels2;
    unsigned char *data2 = stbi_load("assets/container.jpg", &imgWidth2, &imgHeight2, &imgChannels2, 0);
    if (!data2) {
        std::cout << "Failed to load image" << std::endl;
    }

    //Generate Texture
    GLuint textureID2;
    glGenTextures(1, &textureID2);
    glBindTexture(GL_TEXTURE_2D, textureID2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth2, imgHeight2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);

    //Free the data
    stbi_image_free(data2);

    //Shader
    Shader shader("assets/vertex.glsl", "assets/fragment.glsl");
    shader.use();
    shader.setInt("uTexture1", 0);
    shader.setInt("uTexture2", 1);

    //FBO for editor renderer
    RenderTarget viewport;
    viewport.Create(800, 600);

    ///
    /// MAIN LOOP
    ///
    bool running = true;
    while (running) {
        ///
        /// Event Handling
        ///
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT)
                running = false;
        }

        ///
        /// ImGui Rendering
        ///

        //Start a new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        //Main Menu Bar
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                }
                if (ImGui::MenuItem("Play", "F5")) {
                }
                if (ImGui::MenuItem("Build")) {
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Preferences", "Ctrl+Shift+P")) {
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
                }
                if (ImGui::MenuItem("Redo", "Ctrl+Shift+Z")) {
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Colab")) {
                if (ImGui::MenuItem("Create Repo")) {
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                ImGui::Checkbox("FPS", &FPS_COUNTER);
                if (ImGui::Checkbox("Wireframe", &WIREFRAME)) {
                    if (WIREFRAME) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    } else if (!WIREFRAME) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    }
                }
                ImGui::EndMenu();
            }

            //Debug Info
            ImGui::Separator();
            if (FPS_COUNTER) {
                ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
            }

            ImGui::EndMainMenuBar();
        }

        //Create a docking space for the editor windows
        ImGui::DockSpaceOverViewport();

        //Create Scene View
        ImGui::SetNextWindowSize({800, 600}, ImGuiCond_FirstUseEver);
        ImGui::Begin("Scene");

        //Render the scene to the FBO
        ImVec2 size = ImGui::GetContentRegionAvail();
        viewport.RenderSceneToSelf((int) size.x, (int) size.y, &shader, &textureID1, &textureID2);

        //Put the FBO texture onto a image
        ImGui::Image((ImTextureID) (intptr_t) viewport.colorTex, size, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();

        //Create the File Views
        //SRC
        /*ImGui::SetNextWindowSize({800, 400}, ImGuiCond_FirstUseEver);
        ImGui::Begin("Source Browser");
        ImGui::End();

        //ASSETS
        ImGui::SetNextWindowSize({800, 400}, ImGuiCond_FirstUseEver);
        ImGui::Begin("Assets Browser");
        ImGui::End();

        //Create the Console View
        ImGui::SetNextWindowSize({800, 400}, ImGuiCond_FirstUseEver);
        ImGui::Begin("Console");
        ImGui::End();

        //Create the Hierarchy
        ImGui::SetNextWindowSize({400, 800}, ImGuiCond_FirstUseEver);
        ImGui::Begin("Hierarchy");
        ImGui::End();

        //Create the Inspector
        ImGui::SetNextWindowSize({400, 800}, ImGuiCond_FirstUseEver);
        ImGui::Begin("Inspector");
        ImGui::End();*/

        //Render the UI
        ImGui::Render();

        ///
        /// Rendering
        ///
        int w;
        int h;
        SDL_GetWindowSize(window, &w, &h);

        glViewport(0, 0, w, h);
        glClearColor(144.0f / 255.0f, 213.0f / 255.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    // cleanup
    viewport.Clean();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
