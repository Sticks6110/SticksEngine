//
// Created by beast on 11/21/2025.
//

#include "InspectorWindow.h"

void InspectorWindow::Render(float deltaTime, EditorContext *context) {
    ImGui::SetNextWindowSize({400, 800}, ImGuiCond_FirstUseEver);
    ImGui::Begin("Inspector");
    if (context->selected != nullptr) {
        float pos[3] = {context->selected->position.x, context->selected->position.y, context->selected->position.z};
        float rot[3] = {context->selected->rotation.x, context->selected->rotation.y, context->selected->rotation.z};
        float sca[3] = {context->selected->scale.x, context->selected->scale.y, context->selected->scale.z};

        bool ena = context->selected->IsEnabled();
        ImGui::SeparatorText(context->selected->name.c_str());
        if (ImGui::Checkbox(" ", &ena)) {
            context->selected->Toggle(ena);
        }
        ImGui::SameLine();
        ImGui::InputText("Name", &context->selected->name);

        ImGui::InputFloat3("Position", pos);
        ImGui::InputFloat3("Rotation", rot);
        ImGui::InputFloat3("Scale", sca);
    }
    //g1.RenderImGui();
    //ImReflect::Input(g.name.c_str(), &g);
    ImGui::End();
}
