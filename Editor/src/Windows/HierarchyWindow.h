//
// Created by beast on 11/21/2025.
//

#ifndef STICKSENGINE_HIERARCHYWINDOW_H
#define STICKSENGINE_HIERARCHYWINDOW_H
#include "../EditorWindow.h"
#include "SticksEngine/Scene.h"


class HierarchyWindow : public EditorWindow {
public:
    void Render(float deltaTime, EditorContext *context) override;

private:
    void RenderGameObjectHierachy(GameObject *gobj, EditorContext *context);
};


#endif //STICKSENGINE_HIERARCHYWINDOW_H
