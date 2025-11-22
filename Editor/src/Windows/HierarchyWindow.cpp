//
// Created by beast on 11/21/2025.
//

#include "HierarchyWindow.h"

void HierarchyWindow::Render(float deltaTime, EditorContext *context) {
    ImGui::SetNextWindowSize({400, 800}, ImGuiCond_FirstUseEver);
    ImGui::Begin("Hierarchy");
    ImGui::SeparatorText(context->scene->name.c_str());
    ImGui::InputText("Name", &context->scene->name);
    ImGui::Separator();

    for (auto gobj: context->scene->Hierarchy) {
        RenderGameObjectHierachy(gobj, context);
    }
    ImGui::End();
}

void HierarchyWindow::RenderGameObjectHierachy(GameObject *gobj, EditorContext *context) {
    ImGuiTreeNodeFlags baseFlags =
            ImGuiTreeNodeFlags_SpanFullWidth |
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_OpenOnDoubleClick;

    bool hasChildren = !gobj->children.empty();

    ImGuiTreeNodeFlags nodeFlags = baseFlags;
    if (!hasChildren)
        nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    if (context->selected == gobj)
        nodeFlags |= ImGuiTreeNodeFlags_Selected;

    bool open = ImGui::TreeNodeEx(gobj, nodeFlags, "%s", gobj->name.c_str());

    if (ImGui::IsItemClicked())
        context->selected = gobj;

    if (open && hasChildren) {
        for (auto *child: gobj->children)
            RenderGameObjectHierachy(child, context);

        ImGui::TreePop();
    }
}
