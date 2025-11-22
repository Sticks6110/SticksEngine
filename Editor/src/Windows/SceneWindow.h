//
// Created by beast on 11/20/2025.
//

#ifndef STICKSENGINE_SCENEWINDOW_H
#define STICKSENGINE_SCENEWINDOW_H
#include <glm/glm.hpp>

#include "../EditorWindow.h"
#include "imgui.h"
#include "SticksEngine/Scene.h"
#include "SticksEngine/Render/Renderer.h"


class SceneWindow : public EditorWindow {
public:
    SceneWindow(Scene *scene);

    void Render(float deltaTime, EditorContext *context) override;

private:
    Renderer renderer;
    Scene *scene;
    glm::vec3 EDITOR_CAMERA_POS = glm::vec3(0, 0, 10);
    float EDITOR_CAMERA_YAW = -90.0f;
    float EDITOR_CAMERA_PITCH = 0.0f;

    //TEMP
    GLuint texture1;
    GLuint texture2;
    Shader shader1;
};


#endif //STICKSENGINE_SCENEWINDOW_H
