//
// Created by beast on 11/17/2025.
//

#ifndef STICKSENGINE_RENDERER_H
#define STICKSENGINE_RENDERER_H
#include <glad/glad.h>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Shader.h"


class Renderer {
public:
    struct RenderTarget {
    public:
        GLuint fbo = 0; //Frame Buffer Object
        GLuint colorTex = 0;
        GLuint depthRb = 0;
        int width = 800;
        int height = 600;

        RenderTarget(int w, int h);

        RenderTarget();

        void Create(int w, int h);

        void Resize(int w, int h);
    };

    struct Camera {
    public:
        glm::vec3 position;
        glm::vec3 direction; //EULER DIRECTION
        glm::vec3 up;
        glm::vec3 forward;

        glm::vec3 _camDir;
        glm::vec3 _camUp;
        glm::vec3 _camRight;

        float fov = 45.0f;
        float near = 0.1f;
        float far = 100.0f;
        bool perspective = true;

        int w;
        int h;

        Camera(glm::vec3 pos, glm::vec3 euler, int _w, int _h, float _fov = 45.0f, float _near = 0.1f, float _far = 100.0f, bool _perspective = true, glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f));

        Camera();

        void Create(glm::vec3 pos, glm::vec3 euler, int _w, int _h, float _fov = 45.0f, float _near = 0.1f, float _far = 100.0f, bool _perspective = true, glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f));

        void UpdateTransform();

        void UpdateProjection();

        void Resize(int w, int h);

        glm::mat4 GetView();

        glm::mat4 GetProjection();

    private:
        glm::mat4 view;
        glm::mat4 projection;
    };

    Renderer(RenderTarget target, Camera camera);

    RenderTarget target;
    Camera camera;

    void Render(int w, int h, Shader *shader, GLuint *texture1, GLuint *texture2);

    void Resize(int w, int h);

private:
    GLuint vbo;
    GLuint vao;
    GLuint ebo;

    glm::mat4 transform;
};


#endif //STICKSENGINE_RENDERER_H
