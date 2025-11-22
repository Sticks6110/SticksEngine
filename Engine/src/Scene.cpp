//
// Created by beast on 11/18/2025.
//

#include <SticksEngine/Scene.h>
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ImReflect.hpp"

Scene::Scene(std::string name) : name(name) {
    uuid = boost::uuids::random_generator()();
}

void Scene::Update() {
    for (auto gameobject: Hierarchy) {
        if (gameobject->IsEnabled()) gameobject->Update();
    }
}

void Scene::FixedUpdate() {
    for (auto gameobject: Hierarchy) {
        if (gameobject->IsEnabled()) gameobject->FixedUpdate();
    }
}

void Scene::PreRender() {
    for (auto gameobject: Hierarchy) {
        if (gameobject->IsEnabled()) gameobject->PreRender();
    }
}

void Scene::Render() {
    for (auto gameobject: Hierarchy) {
        if (gameobject->IsEnabled()) gameobject->Render();
    }
}

void Scene::PostRender() {
    for (auto gameobject: Hierarchy) {
        if (gameobject->IsEnabled()) gameobject->PostRender();
    }
}

void Scene::Clean() {
    for (auto gameobject: Hierarchy) {
        if (gameobject->IsEnabled()) gameobject->Clean();
    }
}

void Scene::DebugPrint() {
    std::cout << uuid << std::endl;
    std::cout << "Hierarchy: " << Hierarchy.size() << std::endl;
    for (auto gameobject: Hierarchy) {
        if (gameobject->IsEnabled()) gameobject->DebugPrint();
    }
}
