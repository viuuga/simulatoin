#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec2 vTexCoord;

layout(std140, binding = 0) uniform buf {
    mat4 mvp;
};

void main()
{
    vTexCoord = texCoord;
    gl_Position = mvp * vec4(position, 1.0);
}
