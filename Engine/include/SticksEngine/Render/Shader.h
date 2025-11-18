//
// Created by beast on 11/16/2025.
//

#ifndef STICKSENGINE_SHADER_H
#define STICKSENGINE_SHADER_H
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "imgui_impl_opengl3_loader.h"
#include "imgui_impl_sdl3.h"

class Shader {
public:
    GLuint program_id;

    Shader(const GLchar *vertex_path, const GLchar *frag_path);

    void use();

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setMatrix4fv(const std::string &name, glm::mat4 value) const;
};

#endif //STICKSENGINE_SHADER_H
