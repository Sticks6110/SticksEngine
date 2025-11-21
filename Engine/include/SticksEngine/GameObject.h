//
// Created by beast on 11/18/2025.
//

#ifndef STICKSENGINE_GAMEOBJECT_H
#define STICKSENGINE_GAMEOBJECT_H

#include "Behaviour.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "string"
#include "glm/glm.hpp"
#include "boost/uuid.hpp"


class Scene;

class GameObject {
public:
    GameObject(Scene *scene, std::string name = "New Gameobject", GameObject *parent = nullptr, glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));

    GameObject *parent;
    std::string name = "New GameObject";
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    std::vector<GameObject *> children;
    std::vector<Behaviour *> behaviour;

    void Update();

    void FixedUpdate();

    void PreRender();

    void Render();

    void PostRender();

    void Clean();

    void Toggle(bool isEnabled);

    bool IsEnabled();

    boost::uuids::uuid GetUUID();

    void RenderImGui();

    void DebugPrint();

private:
    bool enabled = true;
    Scene *scene;

    boost::uuids::uuid uuid;
    std::string directory;
};

//IMGUI_REFLECT(GameObject, name, position, rotation, scale)

#endif //STICKSENGINE_GAMEOBJECT_H
