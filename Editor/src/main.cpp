#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <iostream>
#include <fstream>
#include <glad/glad.h>

#include <SDL3/SDL.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include "ImReflect.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include <SticksEngine/Render/Shader.h>
#include <SticksEngine/Render/Renderer.h>

#include "SticksEngine/GameObject.h"
#include "SticksEngine/Scene.h"
#include "Windows/SceneWindow.h"

bool FPS_COUNTER = false;
bool WIREFRAME = false;

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

    //Test Scene
    Scene *scene = new Scene("Scene Name");
    GameObject g1(scene, "Game Object 1");
    GameObject g2(scene, "Game Object 2", &g1);
    GameObject g3(scene, "Game Object 3", &g1);
    GameObject g4(scene, "Game Object 4", &g3);
    GameObject g5(scene, "Game Object 5");
    scene->DebugPrint();

    //Create Scene Editor Window
    SceneWindow scene_window(scene);

    ///
    /// MAIN LOOP
    ///

    //FPS Tracking
    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 last = SDL_GetPerformanceCounter();
    bool running = true;
    while (running) {
        ///
        /// FPS LIMITING
        ///
        Uint64 now = SDL_GetPerformanceCounter();
        double delta = (double) (now - last) * 1000.0 / freq;
        float deltaTime = delta / 1000.0;

        if (delta < 1000.0 / 60.0) {
            SDL_Delay(0);
            continue;
        }

        last = now;

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
                ImGui::Text("FPS: %f", 1000.0 / delta);
            }

            ImGui::EndMainMenuBar();
        }

        //Create a docking space for the editor windows
        ImGui::DockSpaceOverViewport();

        //Create Scene View
        scene_window.Render(deltaTime);

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
        ImGui::End();*/

        //Create the Hierarchy
        ImGui::SetNextWindowSize({400, 800}, ImGuiCond_FirstUseEver);
        ImGui::Begin("Hierarchy");
        scene->RenderImGui();
        ImGui::End();

        //Create the Inspector
        ImGui::SetNextWindowSize({400, 800}, ImGuiCond_FirstUseEver);
        ImGui::Begin("Inspector");
        g1.RenderImGui();
        //ImReflect::Input(g.name.c_str(), &g);
        ImGui::End();

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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
