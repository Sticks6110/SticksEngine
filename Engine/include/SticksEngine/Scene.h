//
// Created by beast on 11/18/2025.
//

#ifndef STICKSENGINE_SCENE_H
#define STICKSENGINE_SCENE_H
#include <iostream>
#include <string>
#include <map>
#include "GameObject.h"
#include "boost/uuid.hpp"


class Scene {
public:
    Scene(std::string name);

    std::map<boost::uuids::uuid, GameObject *> GameObjects;
    std::vector<GameObject *> Hierarchy;

    std::string name;

    void Update();

    void FixedUpdate();

    void PreRender();

    void Render();

    void PostRender();

    void Clean();

    void DebugPrint();

private:
    boost::uuids::uuid uuid;
};


#endif //STICKSENGINE_SCENE_H
