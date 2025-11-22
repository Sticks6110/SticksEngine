//
// Created by beast on 11/20/2025.
//

#ifndef STICKSENGINE_EDITORWINDOW_H
#define STICKSENGINE_EDITORWINDOW_H
#include "EditorContext.h"


class EditorWindow {
public:
    virtual void Render(float deltaTime, EditorContext *context) {
    };
};


#endif //STICKSENGINE_EDITORWINDOW_H
