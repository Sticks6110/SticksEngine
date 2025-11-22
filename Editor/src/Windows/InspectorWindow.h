//
// Created by beast on 11/21/2025.
//

#ifndef STICKSENGINE_INSPECTORWINDOW_H
#define STICKSENGINE_INSPECTORWINDOW_H
#include "../EditorWindow.h"
#include "SticksEngine/GameObject.h"


class InspectorWindow : public EditorWindow {
public:
    void Render(float deltaTime, EditorContext *context) override;
};


#endif //STICKSENGINE_INSPECTORWINDOW_H
