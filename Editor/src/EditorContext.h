//
// Created by beast on 11/21/2025.
//

#ifndef STICKSENGINE_EDITORCONTEXT_H
#define STICKSENGINE_EDITORCONTEXT_H
#include "SticksEngine/GameObject.h"


class EditorContext {
public:
    Scene *scene = nullptr;
    GameObject *selected = nullptr;
};


#endif //STICKSENGINE_EDITORCONTEXT_H
