//
// Created by beast on 11/18/2025.
//

#ifndef STICKSENGINE_BEHAVIOUR_H
#define STICKSENGINE_BEHAVIOUR_H
#include <SticksEngine/GameObject.h>

class Behaviour {
public:
    explicit Behaviour() {
    }

    virtual ~Behaviour() = default;

    virtual void OnEnable() {
    }

    virtual void OnUpdate() {
    }

    virtual void OnFixedUpdate() {
    }

    virtual void OnPreRender() {
    }

    virtual void OnRender() {
    }

    virtual void OnPostRender() {
    }

    virtual void OnDisable() {
    }

    virtual void Clean() {
    }

    void Toggle(bool toggle) {
        enabled = toggle;
        if (enabled == false) {
            OnDisable();
        } else {
            OnEnable();
        }
    }

    bool IsEnabled() {
        return enabled;
    }

private:
    bool enabled = true;
};

#endif //STICKSENGINE_BEHAVIOUR_H
