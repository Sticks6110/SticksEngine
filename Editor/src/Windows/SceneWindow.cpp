//
// Created by beast on 11/20/2025.
//

#include "SceneWindow.h"

#include <stb_image.h>
#include <SDL3/SDL.h>

SceneWindow::SceneWindow(Scene *scene) : scene(scene), renderer(Renderer::RenderTarget(800, 600), Renderer::Camera(EDITOR_CAMERA_POS, glm::vec3(EDITOR_CAMERA_PITCH, EDITOR_CAMERA_YAW, 0), 800, 600, 45.0)), shader1("assets/vertex.glsl", "assets/fragment.glsl") {
    //Load Image
    int imgWidth, imgHeight, imgChannels;
    unsigned char *data = stbi_load("assets/img.jpg", &imgWidth, &imgHeight, &imgChannels, 0);
    if (!data) {
        std::cout << "Failed to load image" << std::endl;
    }

    //Generate Texture
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
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
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth2, imgHeight2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);

    //Free the data
    stbi_image_free(data2);

    //Shader
    shader1.use();
    shader1.setInt("uTexture1", 0);
    shader1.setInt("uTexture2", 1);
}

void SceneWindow::Render(float deltaTime) {
    ImGui::SetNextWindowSize({800, 600}, ImGuiCond_FirstUseEver);
    ImGui::Begin("Scene");

    //Check if user is right clicking frame
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);

        EDITOR_CAMERA_YAW += delta.x * 45.0f * deltaTime;
        EDITOR_CAMERA_PITCH += delta.y * -45.0f * deltaTime;

        ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);

        if (ImGui::IsKeyDown(ImGuiKey_W)) {
            EDITOR_CAMERA_POS += 10.0f * renderer.camera._camDir * deltaTime;
        }
        if (ImGui::IsKeyDown(ImGuiKey_S)) {
            EDITOR_CAMERA_POS -= 10.0f * renderer.camera._camDir * deltaTime;
        }
        if (ImGui::IsKeyDown(ImGuiKey_A)) {
            EDITOR_CAMERA_POS -= 10.0f * renderer.camera._camRight * deltaTime;
        }
        if (ImGui::IsKeyDown(ImGuiKey_D)) {
            EDITOR_CAMERA_POS += 10.0f * renderer.camera._camRight * deltaTime;
        }

        renderer.camera.position = EDITOR_CAMERA_POS;
        renderer.camera.direction = glm::vec3(EDITOR_CAMERA_PITCH, EDITOR_CAMERA_YAW, 0);
        renderer.camera.UpdateTransform();
    }

    //Render the scene to the FBO
    ImVec2 size = ImGui::GetContentRegionAvail();
    renderer.Render((int) size.x, (int) size.y, &shader1, &texture1, &texture2);

    //Put the FBO texture onto a image
    ImGui::Image((ImTextureID) (intptr_t) renderer.target.colorTex, size, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}
