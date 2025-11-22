//
// Created by beast on 11/18/2025.
//

#include <SticksEngine/GameObject.h>

#include "SticksEngine/Scene.h"

GameObject::GameObject(Scene *scene, std::string name, GameObject *parent, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : name(name), parent(parent), position(position), rotation(rotation), scale(scale) {
    this->scene = scene;
    this->uuid = boost::uuids::random_generator()();

    scene->GameObjects[uuid] = this;
    if (parent == nullptr) {
        scene->Hierarchy.push_back(this);
    } else {
        parent->children.push_back(this);
    }
}

void GameObject::Update() {
    for (auto &child: children) {
        if (child->enabled) child->Update();
    }
    for (auto &behaviour: behaviour) {
        if (behaviour->IsEnabled()) behaviour->OnUpdate();
    }
}

void GameObject::FixedUpdate() {
    for (auto &child: children) {
        if (child->enabled) child->FixedUpdate();
    }
    for (auto &behaviour: behaviour) {
        if (behaviour->IsEnabled()) behaviour->OnFixedUpdate();
    }
}

void GameObject::PreRender() {
    for (auto &child: children) {
        if (child->enabled) child->PreRender();
    }
    for (auto &behaviour: behaviour) {
        if (behaviour->IsEnabled()) behaviour->OnPreRender();
    }
}

void GameObject::Render() {
    for (auto &child: children) {
        if (child->enabled) child->Render();
    }
    for (auto &behaviour: behaviour) {
        if (behaviour->IsEnabled()) behaviour->OnRender();
    }
}

void GameObject::PostRender() {
    for (auto &child: children) {
        if (child->enabled) child->PostRender();
    }
    for (auto &behaviour: behaviour) {
        if (behaviour->IsEnabled()) behaviour->OnPostRender();
    }
}

void GameObject::Clean() {
    for (auto &child: children) {
        if (child->enabled) child->Clean();
    }
    for (auto &behaviour: behaviour) {
        if (behaviour->IsEnabled()) behaviour->Clean();
    }
}

void GameObject::Toggle(bool isEnabled) {
    enabled = isEnabled;
    for (auto &child: children) {
        child->Toggle(isEnabled);
    }
    for (auto &behaviour: behaviour) {
        behaviour->Toggle(isEnabled);
    }
}

bool GameObject::IsEnabled() {
    return enabled;
}

boost::uuids::uuid GameObject::GetUUID() {
    return uuid;
}

void GameObject::DebugPrint() {
    std::cout << uuid << "  |  " << name << std::endl;
    for (auto &child: children) {
        if (child->enabled) child->DebugPrint();
    }
}
