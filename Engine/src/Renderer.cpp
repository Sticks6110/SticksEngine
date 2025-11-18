//
// Created by beast on 11/17/2025.
//

#include <SDL3/SDL_timer.h>
#include <SticksEngine/Render/Renderer.h>

Renderer::RenderTarget::RenderTarget(int w, int h) {
    Create(w, h);
}

Renderer::RenderTarget::RenderTarget() {
}

void Renderer::RenderTarget::Create(int w, int h) {
    width = w;
    height = h;

    //Enable Depth Testing to remove overlapping
    glEnable(GL_DEPTH_TEST);

    //Create the frame buffer and assign it to the frame buffer index
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //Create the color texture that is rendered to
    glGenTextures(1, &colorTex);
    glBindTexture(GL_TEXTURE_2D, colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Scaling parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Scaling parameters
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);

    //Now the depth render buffer
    glGenRenderbuffers(1, &depthRb);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRb);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRb);

    //Check if the frame buffer was created succesfully
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Frame Buffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::RenderTarget::Resize(int w, int h) {
    width = w;
    height = h;

    glBindTexture(GL_TEXTURE_2D, colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, depthRb);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    //TODO: Create setting to change projection type and FOV
    //projection = glm::perspective(glm::radians(45.0f), (float) w / (float) h, 0.1f, 100.0f);
}

Renderer::Camera::Camera(glm::vec3 pos, glm::vec3 euler, int _w, int _h, float _fov, float _near, float _far, bool _perspective, glm::vec3 _up) {
    Create(pos, euler, w, h, fov, near, far, perspective, _up);
}

Renderer::Camera::Camera() {
}

void Renderer::Camera::Create(glm::vec3 pos, glm::vec3 euler, int _w, int _h, float _fov, float _near, float _far, bool _perspective, glm::vec3 _up) {
    position = pos;
    direction = euler;
    fov = _fov;
    near = _near;
    far = _far;
    perspective = _perspective;
    up = _up;
    w = _w;
    h = _h;

    UpdateTransform();
    UpdateProjection();
}

void Renderer::Camera::UpdateTransform() {
    //Convert the euler angle direction to a direction vector (roll not used because direction does not take into account roll)
    _camDir.x = cos(glm::radians(direction.y)) * cos(glm::radians(direction.x));
    _camDir.y = sin(glm::radians(direction.x));
    _camDir.z = sin(glm::radians(direction.y)) * cos(glm::radians(direction.x));
    _camDir = glm::normalize(_camDir);

    //Find direction vectors of the camera
    _camRight = glm::normalize(glm::cross(_camDir, up));
    _camUp = glm::normalize(glm::cross(_camRight, _camDir));

    //Set the view matrix
    view = glm::lookAt(position, position + _camDir, _camUp);
}

void Renderer::Camera::UpdateProjection() {
    //Set the projection matrix with new values
    projection = glm::perspective(glm::radians(fov), (float) w / (float) h, near, far);

    std::cout << w << " " << h << " " << near << " " << far << std::endl;
    std::cout << _camDir.x << " " << _camDir.y << " " << _camDir.z << std::endl;
    std::cout << _camRight.x << " " << _camRight.y << " " << _camRight.z << std::endl;
    std::cout << _camUp.x << " " << _camUp.y << " " << _camUp.z << std::endl;
}

void Renderer::Camera::Resize(int _w, int _h) {
    w = _w;
    h = _h;
    UpdateProjection();
}

glm::mat4 Renderer::Camera::GetView() {
    return view;
}

glm::mat4 Renderer::Camera::GetProjection() {
    return projection;
}

Renderer::Renderer(RenderTarget target, Camera camera) : target(target), camera(camera) {
    float vertices[] = {
        -0.5f, -0.5f, 0.5f, 1, 0, 0, 0, 0,
        0.5f, -0.5f, 0.5f, 1, 0, 0, 1, 0,
        0.5f, 0.5f, 0.5f, 1, 0, 0, 1, 1,

        0.5f, 0.5f, 0.5f, 1, 0, 0, 1, 1,
        -0.5f, 0.5f, 0.5f, 1, 0, 0, 0, 1,
        -0.5f, -0.5f, 0.5f, 1, 0, 0, 0, 0,

        // ---- BACK FACE ----
        -0.5f, -0.5f, -0.5f, 0, 1, 0, 1, 0,
        0.5f, -0.5f, -0.5f, 0, 1, 0, 0, 0,
        0.5f, 0.5f, -0.5f, 0, 1, 0, 0, 1,

        0.5f, 0.5f, -0.5f, 0, 1, 0, 0, 1,
        -0.5f, 0.5f, -0.5f, 0, 1, 0, 1, 1,
        -0.5f, -0.5f, -0.5f, 0, 1, 0, 1, 0,

        // ---- LEFT FACE ----
        -0.5f, -0.5f, -0.5f, 0, 0, 1, 0, 0,
        -0.5f, -0.5f, 0.5f, 0, 0, 1, 1, 0,
        -0.5f, 0.5f, 0.5f, 0, 0, 1, 1, 1,

        -0.5f, 0.5f, 0.5f, 0, 0, 1, 1, 1,
        -0.5f, 0.5f, -0.5f, 0, 0, 1, 0, 1,
        -0.5f, -0.5f, -0.5f, 0, 0, 1, 0, 0,

        // ---- RIGHT FACE ----
        0.5f, -0.5f, -0.5f, 1, 1, 0, 1, 0,
        0.5f, -0.5f, 0.5f, 1, 1, 0, 0, 0,
        0.5f, 0.5f, 0.5f, 1, 1, 0, 0, 1,

        0.5f, 0.5f, 0.5f, 1, 1, 0, 0, 1,
        0.5f, 0.5f, -0.5f, 1, 1, 0, 1, 1,
        0.5f, -0.5f, -0.5f, 1, 1, 0, 1, 0,

        // ---- TOP FACE ----
        -0.5f, 0.5f, 0.5f, 1, 0, 1, 0, 0,
        0.5f, 0.5f, 0.5f, 1, 0, 1, 1, 0,
        0.5f, 0.5f, -0.5f, 1, 0, 1, 1, 1,

        0.5f, 0.5f, -0.5f, 1, 0, 1, 1, 1,
        -0.5f, 0.5f, -0.5f, 1, 0, 1, 0, 1,
        -0.5f, 0.5f, 0.5f, 1, 0, 1, 0, 0,

        // ---- BOTTOM FACE ----
        -0.5f, -0.5f, 0.5f, 0, 1, 1, 0, 0,
        0.5f, -0.5f, 0.5f, 0, 1, 1, 1, 0,
        0.5f, -0.5f, -0.5f, 0, 1, 1, 1, 1,

        0.5f, -0.5f, -0.5f, 0, 1, 1, 1, 1,
        -0.5f, -0.5f, -0.5f, 0, 1, 1, 0, 1,
        -0.5f, -0.5f, 0.5f, 0, 1, 1, 0, 0
    };

    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5,

        6, 7, 8,
        9, 10, 11,

        12, 13, 14,
        15, 16, 17,

        18, 19, 20,
        21, 22, 23,

        24, 25, 26,
        27, 28, 29,

        30, 31, 32,
        33, 34, 35
    };

    //Generate the Vertex Buffer Object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //TODO: Make GL_STATIC_DRAW changeable in the future, do same for VAO (DYNAMIC AND STREAM are faster to access so are better for constantly changing vertices_
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Generate the Vertex Array Object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Position Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    //Color Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Texture Coord Attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Now we create the EBO
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Translation
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -3.0f));
    transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f));
}

void Renderer::Render(int w, int h, Shader *shader, GLuint *texture1, GLuint *texture2) {
    //Resize if needed
    if (w != target.width || h != target.height) {
        Resize(w, h);
    }

    glm::vec3 positions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    //Change to this FBO
    glBindFramebuffer(GL_FRAMEBUFFER, target.fbo);

    //Change the viewport to new dimensions if changed
    glViewport(0, 0, w, h);

    //Fill screen with solid color
    glClearColor(144.0f / 255.0f, 213.0f / 255.0f, 1.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Start using shader
    shader->use();

    //Set transform, view, and projection matrices
    shader->setMatrix4fv("view", camera.GetView());
    shader->setMatrix4fv("projection", camera.GetProjection());

    //Bind texture 1
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *texture1);

    //Bind texture 2
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, *texture2);

    //Bind vertex array
    glBindVertexArray(vao);

    float time = SDL_GetTicks() / 1000.0f;
    for (unsigned int i = 0; i < 10; i++) {
        //Rotate Mesh
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, positions[i]);
        transform = glm::rotate(transform, i * 20.0f, glm::vec3(0.2f, 0.5f, 0.7f));
        transform = glm::rotate(transform, time + (i * 20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f));

        //Update transform matrix uniform
        shader->setMatrix4fv("transform", transform);

        //Draw
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    //Set back to normal
    glBindVertexArray(0);

    //Set FBO back to default
    glUseProgram(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Resize(int w, int h) {
    target.Resize(w, h);
    camera.Resize(w, h);
}
