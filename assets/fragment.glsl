#version 460 core
out vec4 FragColor;
in vec3 color;
in vec2 texCoord;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

void main() {
   FragColor = mix(texture(uTexture1, texCoord), texture(uTexture2, texCoord) * vec4(color, 1.0f), 0.5);
}