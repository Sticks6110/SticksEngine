#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
out vec2 texCoord;
out vec3 color;
void main() {
   gl_Position = projection * view * transform * vec4(aPos, 1.0);
   color = aColor;
   texCoord = aTex;
}